// Exercise includes
#include "Viewer.h"

// OpenCascade includes
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <Geom_Plane.hxx>
#include <Intf_InterferencePolygon2d.hxx>
#include <Intf_Polygon2d.hxx>

//-----------------------------------------------------------------------------

//! The derived polygon class.

class SimplePolygon :public Intf_Polygon2d {
public:
	static Handle(Geom_Plane) PLANE;
public:
	SimplePolygon(const std::initializer_list < std::pair<double, double>>& poles) {
		for (const auto& pole : poles)
		{
			gp_Pnt2d p(pole.first, pole.second);
			m_poles.push_back(p);

			myBox.Add(p);
		}
	}
public:
	// 通过 Intf_Polygon2d 继承
	virtual	 double DeflectionOverEstimation() const override
	{
		return  Precision::Confusion();
	}
	virtual int NbSegments() const override
	{
		return m_poles.size() - 1;
	}
	virtual	 void Segment(const int index, gp_Pnt2d& theBegin, gp_Pnt2d& theEnd) const override
	{
		theBegin = m_poles[index - 1];
		theEnd = m_poles[index];
	}
public:
	operator TopoDS_Shape() {
		BRepBuilderAPI_MakePolygon mkPoly;
		for (const auto& p : m_poles) {
			mkPoly.Add(PLANE->Value(p.X(), p.Y()));
		}
		return mkPoly.Shape();
	}
protected:
	std::vector<gp_Pnt2d> m_poles;
};

Handle(Geom_Plane) SimplePolygon::PLANE;
//-----------------------------------------------------------------------------

int main(int argc, char** argv)
{
	Viewer vout(50, 50, 500, 500);

	SimplePolygon::PLANE = new Geom_Plane(gp::XOY());

	SimplePolygon poly1 = { {0.0, 0.0}, {1.0,  0.0}, {0.25, 0.75} };
	SimplePolygon poly2 = { {0.5, 1.2}, {0.5, -1.0}, {1.0, 3.0} };

	vout << poly1 << poly2;

	Intf_InterferencePolygon2d algo(poly1, poly2);
	const int numPts = algo.NbSectionPoints();

	for (int isol = 1; isol <= numPts; ++isol)
	{
		gp_Pnt2d pnt2D = algo.Pnt2dValue(isol);
		gp_Pnt pnt3D = SimplePolygon::PLANE->Value(pnt2D.X(), pnt2D.Y());

		vout << BRepBuilderAPI_MakeVertex(pnt3D);
	}

	std::cout << "Num. of intersection points: " << numPts << std::endl;

	vout.StartMessageLoop();

	return 0;
}
