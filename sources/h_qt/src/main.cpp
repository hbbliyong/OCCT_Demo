#include <iostream>
#include <QApplication>
#include "MainWindow.h"
#include <QSurfaceFormat>
int main(int argc,char* argv[]) {

	// 关键设置：强制使用桌面版 OpenGL
	QApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
	QApplication::setAttribute(Qt::AA_ShareOpenGLContexts); // 启用上下文共享

	QSurfaceFormat format;
	format.setVersion(4, 3); // OCCT 7.6+要求最低OpenGL 4.3
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setSamples(8);  // 多重采样抗锯齿
	QSurfaceFormat::setDefaultFormat(format);

	QApplication a(argc, argv);

	// 设置全局字体为微软雅黑 10号
	QFont font("Microsoft YaHei", 10);
	a.setFont(font);

	MainWindow w;
	w.show();

	return a.exec();
}