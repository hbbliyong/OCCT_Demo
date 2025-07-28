#pragma  once

#include "QMainWindow.h"
#include "core/Viewport.h"
class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
explicit	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private slots:
	void setWireframeMode();  // 线框模式
	void setShadedMode();     // 填充模式
	void createCube();        // 创建立方体
	void createCylinder();    // 创建圆柱
	void createCone();        // 创建圆台
	void createTorus();       // 创建圆环
	void createSphere();      // 创建球体
	void randomTest();        // 随机测试

private:
	void initToolBar();

	Viewport* m_viewport = nullptr;
	QWidget* m_centralWidget = nullptr;
};