#include <iostream>
#include <BRepPrimAPI_MakeBox.hxx>

#include <vtkAutoInit.h>
#include <vtkrenderWindow.h>
#include <vtkRenderer.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyDatamapper.h>
#include <IVtkTools_ShapeDataSource.hxx>


VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)


int main() {
	BRepPrimAPI_MakeBox mkBox(1., 2., 3);
	const TopoDS_Shape& shape = mkBox.Shape();

	vtkNew<	vtkRenderWindow> renwin;
	vtkNew<vtkRenderer> ren;

	// 将渲染器添加到渲染窗口
	renwin->AddRenderer(ren);
	renwin->SetSize(640, 480);
	ren->SetBackground(0.1, 0.2, 0.4);



	vtkNew<vtkInteractorStyleTrackballCamera> istype;
	vtkNew< vtkRenderWindowInteractor> iren;

	iren->SetRenderWindow(renwin);
	iren->SetInteractorStyle(istype);

	vtkNew<IVtkTools_ShapeDataSource> occSource;
	occSource->SetShape(new IVtkOCC_Shape(shape));

	vtkNew<vtkPolyDataMapper> mapper;
	mapper->SetInputConnection(occSource->GetOutputPort());
	vtkNew<vtkActor> actor;
	actor->SetMapper(mapper);
	ren->AddActor(actor);

	renwin->Render();
	iren->Start();
}