#include "MainWindow.h"

#include <QToolBar>
#include <QAction>
#include <QVBoxLayout>

#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <AIS_Shape.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <gp_Trsf.hxx>
#include <gp_Vec.hxx>
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <TopoDS_Compound.hxx>
#include <QFileDialog>
#include <QMessageBox>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>
#include <gp_Circ.hxx>
#include <gp_Ax2.hxx>
#include <gp_Pln.hxx>
#include <gp_Dir.hxx>
#include <gp_Elips.hxx>
#include <Geom_Plane.hxx>
#include <Geom_Circle.hxx>
#include <Geom_Ellipse.hxx>
#include <Geom_Line.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Geom_BSplineCurve.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColStd_Array1OfInteger.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <random>
#include <QDebug>
#include <fstream>
//https://gitee.com/zhangly1403/OCCT-Examples/blob/master/Sample01/MainWindow.cpp
MainWindow::MainWindow(QWidget* parent /*= nullptr*/):QMainWindow(parent)
{
	this->setWindowTitle("OCCT Qt Example");
	resize(800, 600);

	m_centralWidget = new QWidget(this);
	QVBoxLayout* layout = new QVBoxLayout(m_centralWidget);
	layout->SetDefaultConstraint(0, 0, 0, 0);
	
	m_viewport = new Viewport(m_centralWidget);
	layout->addWidget(m_viewport);

	setCentralWidget(m_centralWidget);

	initToolBar();
	m_viewport->fitAll();
}

MainWindow::~MainWindow()
{

}

void MainWindow::setWireframeMode()
{
	if (m_viewport)
	{
		m_viewport->setDisplayMode(AIS_WireFrame);
	}
}

void MainWindow::setShadedMode()
{
	if (m_viewport)
	{
		m_viewport->setDisplayMode(AIS_Shaded);
	}
}

void MainWindow::createCube()
{
	if (m_viewport)
	{
		gp_Trsf transform;
		transform.SetTranslation(gp_Vec(200.0, 0.0, 0.0));

		TopoDS_Shape cube = BRepPrimAPI_MakeBox(100.0, 100.0, 100.0).Shape();
		BRepBuilderAPI_Transform shapeTransform(cube, transform);
		cube = shapeTransform.Shape();

		Handle(AIS_Shape) aisCube = new AIS_Shape(cube);
		m_viewport->displayEntity(aisCube);
	}
}

void MainWindow::createCylinder()
{
	if (m_viewport)
	{
		gp_Trsf transform;
		transform.SetTranslation(gp_Vec(0.0, 200.0, 0.0));

		TopoDS_Shape cylinder = BRepPrimAPI_MakeCylinder(50.0, 100.0).Shape();
		BRepBuilderAPI_Transform shapTransform(cylinder, transform);
		cylinder = shapTransform.Shape();
		Handle(AIS_Shape) aisCylinder = new AIS_Shape(cylinder);
		m_viewport->displayEntity(aisCylinder);
	}
}

void MainWindow::createCone()
{
	if (m_viewport)
	{
		gp_Trsf transform;
		transform.SetTranslation(gp_Vec(-200.0, 0.0, 0.0));

		TopoDS_Shape cone = BRepPrimAPI_MakeCone(50.0, 25.0, 100.0).Shape();
		BRepBuilderAPI_Transform shapeTransform(cone, transform);
		cone = shapeTransform.Shape();

		Handle(AIS_Shape) aisCone = new AIS_Shape(cone);
		m_viewport->displayEntity(aisCone);
	}
}

void MainWindow::createTorus()
{
	if (m_viewport)
	{
	}
}

void MainWindow::createSphere()
{
	if (m_viewport)
	{
	}
}

void MainWindow::randomTest()
{
	if (m_viewport)
	{
	}
}

void MainWindow::initToolBar()
{
	// 创建工具栏
	QToolBar* toolBar = addToolBar("Tools");

	// 添加线框模式按钮
	QAction* wireframeAction = toolBar->addAction("线框");
	connect(wireframeAction, &QAction::triggered, this, &MainWindow::setWireframeMode);

	// 添加填充模式按钮
	QAction* shadedAction = toolBar->addAction("填充");
	connect(shadedAction, &QAction::triggered, this, &MainWindow::setShadedMode);

	// 添加创建立方体按钮
	QAction* cubeAction = toolBar->addAction("立方体");
	connect(cubeAction, &QAction::triggered, this, &MainWindow::createCube);

	// 添加创建圆柱按钮
	QAction* cylinderAction = toolBar->addAction("圆柱");
	connect(cylinderAction, &QAction::triggered, this, &MainWindow::createCylinder);

	// 添加创建圆台按钮
	QAction* coneAction = toolBar->addAction("圆台");
	connect(coneAction, &QAction::triggered, this, &MainWindow::createCone);

	// 添加创建圆环按钮
	QAction* torusAction = toolBar->addAction("圆环");
	connect(torusAction, &QAction::triggered, this, &MainWindow::createTorus);

	// 添加创建球体按钮
	QAction* sphereAction = toolBar->addAction("球体");
	connect(sphereAction, &QAction::triggered, this, &MainWindow::createSphere);
	// 添加随机测试按钮
	QAction* randomTestAction = toolBar->addAction("随机测试");
	connect(randomTestAction, &QAction::triggered, this, &MainWindow::randomTest);
}
