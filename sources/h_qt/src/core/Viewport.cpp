#include "core/Viewport.h"

#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <Graphic3d_GraphicDriver.hxx>
#include <Aspect_NeutralWindow.hxx>
#include <Quantity_Color.hxx>
#include <QDebug>
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <NCollection_List.hxx>
#include <QApplication>
Viewport::Viewport(QWidget* parent /*= nullptr*/)
{
	//设置QT窗口属性
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);
	setFocusPolicy(Qt::StrongFocus);
	setMouseTracking(true);
	setWindowFlags(Qt::FramelessWindowHint);
	//初始化视图
	initViewer();
	initSelectionRect();
}

Viewport::~Viewport()
{

}

void Viewport::fitAll()
{
	if (!m_view.IsNull()) {
		m_view->FitAll();
		m_view->ZFitAll();
		m_view->Redraw();
	}
}

void Viewport::zoom(float delta)
{

}

void Viewport::rotate(float x, float y)
{

}

void Viewport::pan(float x, float y)
{

}

void Viewport::setDisplayMode(int mode)
{
	if (!m_context.IsNull())
	{
		m_context->SetDisplayMode(mode, Standard_True);
	}
}

void Viewport::displayEntity(const Handle(AIS_Shape)& entity)
{
	if (!m_context.IsNull()) {
		m_context->Display(entity, Standard_True);
		m_view->Redraw();
	}
}

void Viewport::displayEntities(const QList<Handle(AIS_Shape)>& entities)
{
	if (!m_context.IsNull())
	{
		for (const Handle(AIS_Shape)& entity:entities)
		{
			if (!entity.IsNull())
			{
				m_context->Display(entity, Standard_False);
			}
		}
		m_view->Redraw();
	}
}

gp_Pnt Viewport::convertWithProj(const QPoint& clickPos) const
{
	if (m_view.IsNull()) {
		return gp_Pnt();
	}

	Standard_Real x, y, z;//投影坐标点
	Standard_Real Vx, Vy, Vz;//射线方向向量

	m_view->ConvertWithProj(clickPos.x(), clickPos.y(), x, y, z, Vx, Vy, Vz);

	return gp_Pnt(x,y,z);
}

QList<Handle(AIS_Shape)> Viewport::getAllAISShapes() const
{
	QList < Handle(AIS_Shape)> shapes;

	if (!m_context.IsNull()) {
		AIS_ListOfInteractive dispalyedObjects;
		m_context->DisplayedObjects(dispalyedObjects);

		for (AIS_ListOfInteractive::Iterator it(dispalyedObjects); it.More(); it.Next())
		{
			if (Handle(AIS_Shape) shape = Handle(AIS_Shape)::DownCast(it.Value()))
			{
				shapes.append(shape);
			}
		}
	}
	else
	{
		qDebug() << "Context is null";
	}
	qDebug() << "Total shapes found:"<<shapes.size();
	return shapes;
}

void Viewport::paintEvent(QPaintEvent* event)
{
	if (!m_view.IsNull()) {
		m_view->Redraw();
	}
	QWidget::paintEvent(event);
}

void Viewport::resizeEvent(QResizeEvent* event)
{
	if (!m_view.IsNull()&&m_window.IsNull())
	{
		m_window->SetSize(event->size().width(), event->size().height());
		m_view->MustBeResized();
	}
	update();
	QWidget::resizeEvent(event);
}

void Viewport::mousePressEvent(QMouseEvent* event)
{
	setFocus();

	if (event->button()==Qt::MiddleButton)
	{
		if (event->modifiers()&Qt::ShiftModifier)
		{
			m_view->StartRotation(event->pos().x(), event->pos().y());
			m_isRotatiing = true;
			setCursor(Qt::ClosedHandCursor);//设置旋转光标
			return;
		}
		else
		{
			m_isPanning = true;
			setCursor(Qt::SizeAllCursor);//设置移动光标
			m_lastMousePos = event->pos();
			return;
		}
	}
	if (event->button()==Qt::LeftButton)
	{
		if (!(event->modifiers() & Qt::ShiftModifier)) {
			m_selectStartPos = event->pos();
			m_isMultiSelecting = true;
			return;
		}
	}
	m_lastMousePos = event->pos();
}

void Viewport::mouseMoveEvent(QMouseEvent* event)
{
	if (!event||m_view.IsNull()||m_context.IsNull())
	{
		return;
	}
	setFocus();

	const QPoint pos = event->pos();
	gp_Pnt worldPoint = convertWithProj(pos);

	if (m_isPanning) {
		const int dx = (event->position().x() - m_lastMousePos.x());
		const int dy = -(event->position().y() - m_lastMousePos.y());

		m_view->Pan(dx, dy);
		m_lastMousePos = event->pos();
		return;
	}

	if (m_isRotatiing)
	{
		m_view->Rotation(event->x(), event->y());
		m_lastMousePos = event->pos();
		return;
	}

	//移动触发高亮
	m_context->MoveTo(pos.x(), pos.y(), m_view, Standard_True);

	//框选预览
	if (m_isMultiSelecting)
	{
		previewSelectionRect(event->pos());
		return;
	}
}

void Viewport::mouseReleaseEvent(QMouseEvent* event)
{
	if (m_isPanning||m_isRotatiing)
	{
		setCursor(Qt::ArrowCursor);
	}
	m_isPanning = false;
	m_isRotatiing = false;

	if (m_isMultiSelecting) {
		finishSelection(event->pos());
		m_isMultiSelecting = false;
		return;
	}
}

void Viewport::wheelEvent(QWheelEvent* event)
{
	m_view->StartZoomAtPoint(event->position().x(), event->position().y());
	//缩放灵敏度
	double zoomSensitivity = 0.2;
	zoomSensitivity = Min(Max(0.2, zoomSensitivity), 0.8);
	m_view->ZoomAtPoint(0, 0, event->angleDelta().y() * zoomSensitivity, 0);
}

void Viewport::initViewer()
{
	//创建显示连接和图形驱动
	Handle(Aspect_DisplayConnection) displayConnection = new Aspect_DisplayConnection();
	Handle(Graphic3d_GraphicDriver) graphicDriver = new OpenGl_GraphicDriver(displayConnection);

//创建查看器
	m_viewer = new V3d_Viewer(graphicDriver);
	m_viewer->SetDefaultViewSize(1000.);
	m_viewer->SetDefaultViewProj(V3d_XnegYnegZpos);
	m_viewer->SetDefaultLights();
	m_viewer->SetLightOn();

	//创建视图
	m_view = m_viewer ->CreateView();
	m_view->SetImmediateUpdate(Standard_True);

	//创建窗口并绑定QT窗口句柄
	m_window = new Aspect_NeutralWindow();
	m_window->SetNativeHandles((Aspect_Drawable)winId(), 0,0);
	m_window->SetSize(width(), height());
	m_view->SetWindow(m_window);
	if (!m_window->IsMapped()) {
		m_window->Map();
	}

	//创建交互上下文
	m_context = new AIS_InteractiveContext(m_viewer);
	m_context->SetPixelTolerance(10);
	m_view->SetBackgroundColor(Quantity_NOC_GRAY40);
	m_view->MustBeResized();//确保视图调整大小
	m_view->FitAll();//确保视图显示所有内容
}

void Viewport::initSelectionRect()
{
	m_selectionRect = new AIS_RubberBand(Quantity_NOC_YELLOW1, Aspect_TOL_DASH);
	m_selectionRect->SetTransformPersistence(new Graphic3d_TransformPers(Graphic3d_TMF_2d, Aspect_TOTP_LEFT_LOWER));
	m_selectionRect->SetZLayer(Graphic3d_ZLayerId_TopOSD);
}

void Viewport::handleSelection(const QPoint& pos)
{
	m_context->MoveTo(pos.x(), pos.y(), m_view, Standard_True);

	bool isCtrl = QApplication::keyboardModifiers() & Qt::ControlModifier;
	AIS_SelectionScheme scheme = isCtrl ? AIS_SelectionScheme_Add : AIS_SelectionScheme_Replace;

	if (m_context->HasDetected())
	{
		m_context->SelectDetected(scheme);
	}
	else
	{
		if (!isCtrl) {
			m_context->ClearSelected(Standard_True);
		}
	}
	m_context->UpdateCurrentViewer();
}

void Viewport::handleSingleSelection(const QPoint& pos)
{
	m_context->MoveTo(pos.x(), pos.y(), m_view, Standard_True);

	bool isCtrl = QApplication::keyboardModifiers() & Qt::ControlModifier;
	AIS_SelectionScheme scheme = isCtrl ? AIS_SelectionScheme_Add : AIS_SelectionScheme_Replace;

	if (m_context->HasDetected()) {
		m_context->SelectDetected(scheme);
	}
	else {
		if (!isCtrl)
			m_context->ClearSelected(Standard_True);
	}

	m_context->UpdateCurrentViewer();
}

void Viewport::handleAreaSelection(const QPoint& p1, const QPoint& p2)
{
	Standard_Integer xMin = qMin(p1.x(), p2.x());
	Standard_Integer xMax = qMax(p1.x(), p2.x());
	Standard_Integer yMin = qMin(p1.y(), p2.y());
	Standard_Integer yMax = qMax(p1.y(), p2.y());
	bool isCtrl = QApplication::keyboardModifiers() & Qt::ControlModifier;

	//确保区域有效
	if (xMax-xMin<2||yMax-yMin<2)
	{
		return;
	}

	m_context->SelectRectangle(Graphic3d_Vec2i(xMin, yMin), Graphic3d_Vec2i(xMax, yMax), m_view, isCtrl ?
		AIS_SelectionScheme_Add : AIS_SelectionScheme_Replace);
	m_context->UpdateCurrentViewer();
}

void Viewport::finishSelection(const QPoint& endPos)
{
	//移除矩形框
	if (!m_selectionRect.IsNull()&&m_context->IsDisplayed(m_selectionRect))
	{
		m_context->Remove(m_selectionRect, Standard_False);
	}

	//如果拖动距离太小，当做点处理
	if ((endPos-m_selectStartPos).manhattanLength()<3)
	{
		handleSingleSelection(m_selectStartPos);
	}
	else
	{
		handleAreaSelection(m_selectStartPos, endPos);
	}

	m_view->Redraw();
}

void Viewport::previewSelectionRect(const QPoint& pos)
{
	m_selectEndPos = pos;

	//更新框选矩形

}
