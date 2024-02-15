#include "modelingOCCT.h"
#include "GlfwOcctView.h"

#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>

ModelingOCCT::ModelingOCCT(GlfwOcctView *obj):objView(obj)
{

}

void ModelingOCCT::initDemoScene()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation (gp_Pnt (0.0, 0.0, 0.0));
    Handle(AIS_Shape) aBox = new AIS_Shape (BRepPrimAPI_MakeBox (anAxis, 50, 50, 50).Shape());
    objView->GetContext()->Display (aBox, AIS_Shaded, 0, false);
    anAxis.SetLocation (gp_Pnt (25.0, 125.0, 0.0));
    Handle(AIS_Shape) aCone = new AIS_Shape (BRepPrimAPI_MakeCone (anAxis, 25, 0, 50).Shape());
    objView->GetContext()->Display (aCone, AIS_Shaded, 0, false);
    anAxis.SetLocation (gp_Pnt (100, 100.0, 0.0));
    Handle(AIS_Shape) aCylinder = new AIS_Shape (BRepPrimAPI_MakeCylinder(anAxis, 25, 50, 3 * M_PI/2).Shape());
    objView->GetContext()->Display (aCylinder, AIS_Shaded, 0, false);
    anAxis.SetLocation (gp_Pnt (150, 50.0, 0.0));
    Handle(AIS_Shape) aTorus = new AIS_Shape (BRepPrimAPI_MakeTorus(anAxis, 30, 15, 3 * M_PI/2).Shape());
    objView->GetContext()->Display (aTorus, AIS_Shaded, 0, false);
    anAxis.SetLocation (gp_Pnt (150, 150.0, 0.0));
    Handle(AIS_Shape) aSphere= new AIS_Shape (BRepPrimAPI_MakeSphere(anAxis, 30, 3 * M_PI/2).Shape());
    objView->GetContext()->Display (aSphere, AIS_Shaded, 0, false);
}


#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepFilletAPI_MakeChamfer.hxx>
#include <TopoDS.hxx>
#include<TopoDS_Shape.hxx>
#include <TopExp_Explorer.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopExp.hxx>

void ModelingOCCT::initExample1()
{
    TopoDS_Shape box = BRepPrimAPI_MakeBox(gp_Pnt(150, 0, 0), 100, 50, 10).Shape();
    TopTools_IndexedMapOfShape edge; // массив ребер
    TopExp::MapShapes(box, TopAbs_EDGE, edge);
    BRepFilletAPI_MakeChamfer aChamfer(box);
    aChamfer.Add(3, TopoDS::Edge(edge.FindKey(2)));
    Handle (AIS_Shape) aAISShape = new AIS_Shape(aChamfer);
    aAISShape->SetMaterial(Graphic3d_NOM_CHROME);
    aAISShape->SetDisplayMode(AIS_Shaded);
    objView->GetContext()->Display (aAISShape, AIS_Shaded, 0, true);
}

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepPrimAPI_MakePrism.hxx>

void ModelingOCCT::initExample2()
{
    gp_Pnt pnt1(0, 0, 0);
    gp_Pnt pnt2(-10, -10, 0);
    gp_Pnt pnt3(-5, -20, 0);
    gp_Pnt pnt4(10, -20, 0);
    gp_Pnt pnt5(20, -5, 0);

    TopoDS_Edge aEdge1 = BRepBuilderAPI_MakeEdge(pnt1, pnt2);
    TopoDS_Edge aEdge2 = BRepBuilderAPI_MakeEdge(pnt2, pnt3);
    TopoDS_Edge aEdge3 = BRepBuilderAPI_MakeEdge(pnt3, pnt4);
    TopoDS_Edge aEdge4 = BRepBuilderAPI_MakeEdge(pnt4, pnt5);
    TopoDS_Edge aEdge5 = BRepBuilderAPI_MakeEdge(pnt5, pnt1);

    BRepBuilderAPI_MakeWire makeW;
    makeW.Add(aEdge1);
    makeW.Add(aEdge2);
    makeW.Add(aEdge3);
    makeW.Add(aEdge4);
    makeW.Add(aEdge5);
    TopoDS_Wire Wc = makeW.Wire();
    TopoDS_Face F = BRepBuilderAPI_MakeFace(Wc);
    gp_Vec move(gp_Dir(0, 0, 1));
    move *= 50;
    TopoDS_Shape shape = BRepPrimAPI_MakePrism(F, move);
    Handle (AIS_Shape) aAISShape = new AIS_Shape(shape);
    aAISShape->SetMaterial(Graphic3d_NOM_CHROME);
    aAISShape->SetDisplayMode(AIS_Shaded);
    objView->GetContext()->Display (aAISShape, AIS_Shaded, 0, true);
}

#include <iostream>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <TopoDS_Vertex.hxx>
#include <Geom_CartesianPoint.hxx>
#include <AIS_Point.hxx>
#include <AIS_Line.hxx>
#include <Graphic3d_AspectLine3d.hxx>

void ModelingOCCT::Lession2()
{
    gp_Pnt pnt1(10, 0, 0);
    gp_Pnt pnt2 {100, 1, 1};
    std::cout << pnt2.Distance(pnt1) <<std::endl;

    Handle(Prs3d_Drawer) aDrawer = objView->GetContext()->HighlightStyle(Prs3d_TypeOfHighlight_Selected);
    aDrawer->SetColor(Quantity_NOC_RED4);
    aDrawer->SetIsoOnPlane(true);

    TopoDS_Vertex vertex2 = BRepBuilderAPI_MakeVertex(pnt2);
    Handle(AIS_Shape) aisPoint2 = new AIS_Shape(vertex2);
    objView->GetContext()->Display (aisPoint2, AIS_Shaded, 0, true);

    Handle(Geom_CartesianPoint) startPoint = new Geom_CartesianPoint(pnt1);
    Handle(Geom_CartesianPoint) endPoint = new Geom_CartesianPoint(pnt2);
    Handle(AIS_Line) theLineToDisplay = new AIS_Line(startPoint, endPoint);
    Handle(Prs3d_LineAspect) aLineAttrib = new Prs3d_LineAspect(Quantity_NOC_CADETBLUE,Aspect_TOL_SOLID, 5.);
    theLineToDisplay->Attributes()->SetLineAspect(aLineAttrib);

    objView->GetContext()->Display (theLineToDisplay, AIS_Shaded, 0, true);
}

