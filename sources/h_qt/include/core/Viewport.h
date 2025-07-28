#pragma once

#include <QWidget>
#include <QWidget>
#include <AIS_InteractiveContext.hxx>
#include <V3d_Viewer.hxx>
#include <V3d_View.hxx>
#include <AIS_RubberBand.hxx>
#include <AIS_Shape.hxx>
#include <QMouseEvent>
#include <QWheelEvent>

class Viewport :public QWidget {
	Q_OBJECT;

public:
	explicit Viewport(QWidget* parent = nullptr);
	~Viewport();

	void fitAll();
	void zoom(float delta);
	void rotate(float x, float y);
	void pan(float x, float y);
	void setDisplayMode(int mode);
	void displayEntity(const Handle(AIS_Shape)& entity);//显示几何体
	void displayEntities(const QList<Handle(AIS_Shape)>& entities);

	//坐标转换
	gp_Pnt convertWithProj(const QPoint& clickPos)const;
	QList<Handle(AIS_Shape)> getAllAISShapes() const;

protected:
	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;
	QPaintEngine* paintEngine() const override { return nullptr; }

private:
	void initViewer();
	void initSelectionRect();
	//选择处理
	void handleSelection(const QPoint& pos);
	void handleSingleSelection(const QPoint& pos);
	void handleAreaSelection(const QPoint& p1, const QPoint& p2);
	void finishSelection(const QPoint& endPos);
	void previewSelectionRect(const QPoint& pos);


private:
	Handle(AIS_RubberBand) m_selectionRect;

	Handle(V3d_Viewer)m_viewer;
	Handle(V3d_View) m_view;
	Handle(AIS_InteractiveContext) m_context;
	Handle(Aspect_NeutralWindow) m_window;

	QPoint m_lastMousePos;
	bool m_isPanning = false;
	bool m_isRotatiing = false;
	bool m_isSelecting = false;
	bool m_isMultiSelecting = false;
	QPoint m_selectStartPos;
	QPoint m_selectEndPos;
};