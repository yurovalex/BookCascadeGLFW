#include "modelingOCCT.h"
#include "GlfwOcctView.h"


#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>

#include <Prs3d_DatumAspect.hxx>

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

#include <gp_Elips2d.hxx>
#include <GCE2d_MakeArcOfEllipse.hxx>
#include <BRepBuilderAPI_MakeEdge2d.hxx>
#include <gp_Parab.hxx>
#include <gp_Hypr.hxx>
void ModelingOCCT::initExample4()
{
    gp_Elips2d newElip2D(gp_Ax2d(gp_Pnt2d(0,0),gp_Dir2d(1,0)),20,10,true);
    Handle(Geom2d_TrimmedCurve) myObject = GCE2d_MakeArcOfEllipse(newElip2D,0,2*M_PI,Standard_True);
    TopoDS_Edge edgeMyObj = BRepBuilderAPI_MakeEdge2d(myObject);
    Handle(AIS_Shape) AISnewElip2D = new AIS_Shape(edgeMyObj);
    objView->GetContext()->Display (AISnewElip2D, AIS_Shaded, 0, true);

    // gp_Hypr myHypr;
    // myHypr.SetLocation(gp_Pnt(10,10,0));
    // myHypr.SetMajorRadius(50);
    // myHypr.SetMinorRadius(25);
    // TopoDS_Edge myEdge = BRepBuilderAPI_MakeEdge(myHypr);
    // Handle(AIS_Shape) aisEdge=new AIS_Shape(myEdge);
    // aisEdge->SetColor(Quantity_NOC_ROSYBROWN);
    // //myOccView->getContext()->Display(aisEdge, true );
    // objView->GetContext()->Display (aisEdge, AIS_Shaded, 0, true);

    // gp_Parab myParab(gp_Ax1(gp_Pnt(0,0,0),gp_Dir(1,0,0)),gp_Pnt(0,10,0));
    // TopoDS_Edge myEdge1 = BRepBuilderAPI_MakeEdge(myParab);
    // Handle(AIS_Shape) aisEdge1=new AIS_Shape(myEdge1);
    // aisEdge1->SetColor(Quantity_NOC_WHITESMOKE);
    // //myOccView->getContext()->Display(aisEdge1, true );
    // objView->GetContext()->Display (aisEdge1, AIS_Shaded, 0, true);
}

#include <GeomConvert.hxx>
#include <Geom_BezierCurve.hxx>

void ModelingOCCT::initLession5()
{
    TColgp_Array1OfPnt array(1,6);
    gp_Pnt myPoint1(0,10,0);
    gp_Pnt myPoint2(10,0,0);
    gp_Pnt myPoint3(0,-10,0);
    gp_Pnt myPoint4(-5,-5,0);
    gp_Pnt myPoint5(-10,0,0);

    array.SetValue(1, gp_Pnt (0,10,0));
    array.SetValue(2,myPoint2);
    array.SetValue(3,myPoint3);
    array.SetValue(4,myPoint4);
    array.SetValue(5,myPoint5);

    Handle(Geom_BezierCurve) curve_bz = new Geom_BezierCurve(array);
    curve_bz->InsertPoleAfter(6,  gp_Pnt (0,10,0));
    //Standard_Boolean flag = curve_bz->IsClosed();
    TopoDS_Edge myEdge = BRepBuilderAPI_MakeEdge(curve_bz);
    Handle(AIS_Shape) aisEdge = new AIS_Shape(myEdge);
    aisEdge->SetWidth(5.0);
    aisEdge->SetColor(Quantity_NOC_ORANGE);
    objView->GetContext()->Display (aisEdge, AIS_Shaded, 0, true);
}


#include <TopoDS_Face.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <Geom_Surface.hxx>
#include <Geom_BezierSurface.hxx>

#include <random>

void ModelingOCCT::initLession6()
{
    //Build surface 3D -change z koordinate
    int arraySize = 8;
    TColgp_Array2OfPnt myStucture(0,arraySize,0,arraySize);

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> z(-3.0, 3.0);
    for(int i=0; i<=arraySize; i++)
    {
        for(int j=0; j<=arraySize; j++)
        {
            //int z=-3 + rand(unsigned(time(0)))%6;


            myStucture.SetValue(j,i,gp_Pnt(i,j,z(mt)));
        }
    }


    Handle (Geom_Surface) myGSurface = new Geom_BezierSurface(myStucture);
    TopoDS_Face myFace = BRepBuilderAPI_MakeFace(myGSurface,0.01);
    Handle(AIS_Shape) aisFace=new AIS_Shape(myFace);
    aisFace->SetWidth(3.0);
    aisFace->SetColor(Quantity_NOC_ORANGE1);
    objView->GetContext()->Display (aisFace, AIS_Shaded, 0, true);
}

#include <gp_Pnt.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
void ModelingOCCT::SearchingGP_Points()
{
    gp_Pnt somePoint(10.0f,10.0f,10.);

    somePoint.SetCoord (2,500.f);
    somePoint.SetX (0);
    somePoint.SetXYZ(gp_XYZ(-1,-1,-1));
    //void Coord (Standard_Real& theXp, Standard_Real& theYp, Standard_Real& theZp)
    Standard_Real theXp, theYp, theZp;
    somePoint.Coord(theXp, theYp, theZp);

    if (somePoint.IsEqual(gp_Pnt(-0.1,-1,-1),0.2))
    {

        //Ура, условие выполнено, точки совпали
    }

    //! Rotates a point. theA1 is the axis of the rotation.
    //! theAng is the angular value of the rotation in radians.
    //Standard_NODISCARD Standard_EXPORT gp_Pnt Mirrored (const gp_Ax2& theA2) const;
    gp_Pnt  somePoint2 =  somePoint.Mirrored(gp_Ax2(gp_Pnt(0,0,0),gp_Dir (1,0,0), gp_Dir (0,1,0)));


    TopoDS_Vertex vertex = BRepBuilderAPI_MakeVertex(somePoint);
    Handle(AIS_Shape) aisVertex = new AIS_Shape(vertex);
    aisVertex->SetColor(Quantity_NOC_ORANGE);
    objView->GetContext()->Display (aisVertex, AIS_Shaded, 0, true);
}

#include <gp_Lin2d.hxx>
#include <GCE2d_MakeSegment.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <TopoDS_Edge.hxx>
#include <BRepTools.hxx>
void ModelingOCCT::HelixExample()
{
    Handle_Geom_CylindricalSurface aCylinder = new Geom_CylindricalSurface(gp::XOY(), 6.0);
    gp_Lin2d aLine2d(gp_Pnt2d(0.0, 0.0), gp_Dir2d(1.0, 1.0));
    Handle_Geom2d_TrimmedCurve aSegment = GCE2d_MakeSegment(aLine2d, 0.0, M_PI * 2.0);
    TopoDS_Edge aHelixEdge = BRepBuilderAPI_MakeEdge(aSegment, aCylinder, 0.0, 12.0 * M_PI).Edge();
    BRepTools::Dump(aHelixEdge, std::cout);
    BRepTools::Write(aHelixEdge, "/run/media/alexey/LinuxData/helix.brep");
    Handle(AIS_Shape) theLineToDisplay = new AIS_Shape(aHelixEdge);
    Quantity_ColorRGBA myCol(1,1,0,1);
    theLineToDisplay->SetColor(myCol.GetRGB());
    objView->GetContext()->Display (theLineToDisplay, AIS_Shaded, 0, true);
//https://stackoverflow.com/questions/41592413/how-can-i-change-the-underlying-geometry-of-a-topods-shape-in-opencascade
}

void ModelingOCCT::initLession7()
{
    TColgp_Array2OfPnt points(1,5,1,3);
    points.SetValue(1,1,gp_Pnt(1.,-1.,0.));
    points.SetValue(1,2,gp_Pnt(1.,-1.,1.));
    points.SetValue(1,3,gp_Pnt(0.,-1.,1.));
    points.SetValue(2,1,gp_Pnt(1.,0.,0.));
    points.SetValue(2,2,gp_Pnt(1.,0.,1.));
    points.SetValue(2,3,gp_Pnt(0.,0.,1.));
    points.SetValue(3,1,gp_Pnt(1.,1.,0.));
    points.SetValue(3,2,gp_Pnt(1.,1.,1.));
    points.SetValue(3,3,gp_Pnt(0.,0.,1.));
    points.SetValue(3,3,gp_Pnt(0.,0.,1.));
    points.SetValue(4,1,gp_Pnt(0.,1.,0.));
    points.SetValue(4,2,gp_Pnt(0.,1.,1.));
    points.SetValue(4,3,gp_Pnt(0.,0.,1.));
    points.SetValue(5,1,gp_Pnt(-1.,1.,0.));
    points.SetValue(5,2,gp_Pnt(-1.,1.,1.));
    points.SetValue(5,3,gp_Pnt(-1.,0.,1.));
    Handle(Geom_BezierSurface) surface =new Geom_BezierSurface(points);

    TopoDS_Face myFace = BRepBuilderAPI_MakeFace(surface,0.1);
    Handle(AIS_Shape) aisFace=new AIS_Shape(myFace);
    //aisFace->SetWidth(3.0);
    //aisFace->SetColor(Quantity_NOC_GREEN1);
    //myOccView->getContext()->Display(aisFace, true );
    Quantity_ColorRGBA myCol(0.903f, 0.56, 0.03, .9f);
    aisFace->SetColor(myCol.GetRGB());
    objView->GetContext()->Display (aisFace, AIS_Shaded, 0, true);
}


#include <BRepBuilderAPI_MakePolygon.hxx>
#include <GeomPlate_BuildPlateSurface.hxx>
#include <BRepTools_WireExplorer.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <GeomPlate_MakeApprox.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRepAdaptor_HArray1OfCurve.hxx>

#include <BRepFill_CurveConstraint.hxx>

void ModelingOCCT::initLession8()
{
    Standard_Integer NbCurFront=4,
        NbPointConstraint=1;
    gp_Pnt P1(0.,0.,0.);
    gp_Pnt P2(0.,10.,0.);
    gp_Pnt P3(0.,10.,10.);
    gp_Pnt P4(0.,0.,10.);
    gp_Pnt P5(5.,5.,5.);
    BRepBuilderAPI_MakePolygon W;
    W.Add(P1);
    W.Add(P2);
    W.Add(P3);
    W.Add(P4);
    W.Add(P1);
    // Initialize a BuildPlateSurface
    GeomPlate_BuildPlateSurface BPSurf(3,15,2);

    // Create the curve constraints
    BRepTools_WireExplorer anExp;
    for(anExp.Init(W); anExp.More(); anExp.Next())
    {
        TopoDS_Edge E = anExp.Current();
        Handle(BRepAdaptor_Curve) C = new
            BRepAdaptor_Curve();
       // C->ChangeCurve().Initialize(E);
        Handle(BRepFill_CurveConstraint) Cont= new
            BRepFill_CurveConstraint(C,0);
        BPSurf.Add(Cont);
    }
    // Point constraint
    Handle(GeomPlate_PointConstraint) PCont= new
        GeomPlate_PointConstraint(P5,0);
    BPSurf.Add(PCont);
    // Compute the Plate surface
    BPSurf.Perform();
    // Approximation of the Plate surface
    Standard_Integer MaxSeg=9;
    Standard_Integer MaxDegree=8;
    Standard_Integer CritOrder=0;
    Standard_Real dmax,Tol;
    Handle(GeomPlate_Surface) PSurf = BPSurf.Surface();
    dmax = Max(0.0001,10*BPSurf.G0Error());
    Tol=0.0001;
    // GeomPlate_MakeApprox Mapp(PSurf,Tol,MaxSeg,MaxDegree,dmax,CritOrder);
    // Handle (Geom_Surface) Surf (Mapp.Surface());
    // // create a face corresponding to the approximated Plate Surface
    //     Standard_Real Umin, Umax, Vmin, Vmax;
    // PSurf->Bounds( Umin, Umax, Vmin, Vmax);
    // BRepBuilderAPI_MakeFace MF(Surf,Umin, Umax, Vmin, Vmax);
}


#include <BRepPrimAPI_MakeWedge.hxx>
void ModelingOCCT::initLession9()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation (gp_Pnt (0.0, 0.0, 0.0));
    Handle(AIS_Shape) m_aWedge = new AIS_Shape (BRepPrimAPI_MakeWedge(anAxis,15,15,30,5).Shape());

    objView->GetContext()->Display (m_aWedge, AIS_Shaded, 0, false);


    Handle(AIS_Shape) m_aBox = new AIS_Shape (BRepPrimAPI_MakeBox(gp_Pnt(150, 0, 0), 100, 50, 10).Shape());
    objView->GetContext()->Display (m_aBox, AIS_Shaded, 0, false);
}


#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <AIS_ViewCube.hxx>
#include <Prs3d_DatumAspect.hxx>
#include <Message_ProgressIndicator.hxx>



void ModelingOCCT::initLession10()
{



    TopoDS_Shape box_1 = BRepPrimAPI_MakeBox(gp_Pnt(0, 0, 0), 100, 75, 12).Shape();
    TopoDS_Shape box_2 = BRepPrimAPI_MakeBox(gp_Pnt(2.5, 2.5, 2.5), 95, 70, 9.5).Shape();
    TopoDS_Shape ShapeCut_1 = BRepAlgoAPI_Cut(box_1, box_2);
    TopoDS_Shape cyl_1 = BRepPrimAPI_MakeCylinder(gp_Ax2(gp_Pnt(50, 37.5, 2.5), gp_Dir(0, 0, 1)), 7.5, 3).Shape();
    TopoDS_Shape ShapeFuse_1 = BRepAlgoAPI_Fuse(ShapeCut_1, cyl_1);
    TopoDS_Shape conus = BRepPrimAPI_MakeCone(gp_Ax2(gp_Pnt(50, 37.5, 5.5), gp_Dir(0, 0, 1)), 5, 4, 5.5).Shape();
    TopoDS_Shape ShapeFuse_2 = BRepAlgoAPI_Fuse(ShapeFuse_1, conus);
    TopoDS_Shape cyl_2 = BRepPrimAPI_MakeCylinder(gp_Ax2(gp_Pnt(50, 37.5, 11), gp_Dir(0, 0, -1)), 2.5, 11).Shape();
    TopoDS_Shape ShapeCut_2 = BRepAlgoAPI_Cut(ShapeFuse_2, cyl_2);
    TopoDS_Shape box_3 = BRepPrimAPI_MakeBox(gp_Pnt(97.5, 22.5, 7.5), 2.5, 30, 4.5).Shape();
    TopoDS_Shape ShapeCut_3 = BRepAlgoAPI_Cut(ShapeCut_2, box_3);
    TopoDS_Shape box_4 = BRepPrimAPI_MakeBox(gp_Ax2(gp_Pnt(1, 1, 11), gp_Dir(0, 0, 1)), 98, 73, 1).Shape();
    TopoDS_Shape ShapeCut_4 = BRepAlgoAPI_Cut(ShapeCut_3, box_4);

    Handle (AIS_Shape) aAISShape = new AIS_Shape(ShapeCut_4);
   // aAISShape->SetMaterial(Graphic3d_NOM_CHROME);


    //activate boundary edges at the global level
     //objView->GetContext()->DefaultDrawer()->SetFaceBoundaryDraw(true);
    // objView->GetContext()->DefaultDrawer()->FaceBoundaryAspect()->SetColor(Quantity_NameOfColor::Quantity_NOC_WHITE);


     aAISShape->SetDisplayMode(AIS_Shaded);
     objView->GetContext()->Display (aAISShape,false);
     // objView->GetContext()->Deactivate();
     // objView->GetContext()->Activate(AIS_Shape::SelectionMode(TopAbs_EDGE));
     // objView->GetContext()->HighlightStyle()->SetColor(Quantity_NameOfColor::Quantity_NOC_WHITE);
     // objView->GetContext()->SelectionStyle()->SetColor(Quantity_NameOfColor::Quantity_NOC_RED);

     objView->GetContext()->SetSelected(aAISShape,true);
     objView->GetContext()->UpdateCurrentViewer();
     //ExportStep("CoverCone.step");
     ExportVrml("CoverCone.vrml");
}
#include <STEPControl_StepModelType.hxx>
#include <STEPControl_Writer.hxx>

bool ModelingOCCT::ExportStep (const TCollection_AsciiString& theFileName)
{
     STEPControl_StepModelType aType = STEPControl_AsIs;
     STEPControl_Writer        aWriter;
     for (objView->GetContext()->InitSelected(); objView->GetContext()->MoreSelected(); objView->GetContext()->NextSelected())
     {
        Handle(AIS_Shape) anIS = Handle(AIS_Shape)::DownCast (objView->GetContext()->SelectedInteractive());
        if (anIS.IsNull())
        {
            return false;
        }

        TopoDS_Shape aShape = anIS->Shape();
        if (aWriter.Transfer (aShape, aType) != IFSelect_RetDone)
        {
            return false;
        }
     }
     return aWriter.Write (theFileName.ToCString()) == IFSelect_RetDone;
}

#include <VrmlAPI_Writer.hxx>
bool ModelingOCCT::ExportVrml (const TCollection_AsciiString& theFileName)
{
     TopoDS_Compound aRes;
     BRep_Builder    aBuilder;
     aBuilder.MakeCompound (aRes);
     for (objView->GetContext()->InitSelected(); objView->GetContext()->MoreSelected(); objView->GetContext()->NextSelected())
     {
        Handle(AIS_Shape) anIS = Handle(AIS_Shape)::DownCast (objView->GetContext()->SelectedInteractive());
        if (anIS.IsNull())
        {
            return false;
        }
        aBuilder.Add (aRes, anIS->Shape());
     }

     VrmlAPI_Writer aWriter;
     aWriter.Write (aRes, theFileName.ToCString());
     return true;
}


#include <AIS_InteractiveObject.hxx>
void ModelingOCCT::review()
{
    AIS_ListOfInteractive aList;
    objView->GetContext()->DisplayedObjects(aList);

    for (AIS_ListIteratorOfListOfInteractive aListIterator (aList); aListIterator.More(); aListIterator.Next())
    {
        Handle(AIS_Shape) aisShape = Handle(AIS_Shape)::DownCast(aListIterator.Value());
        aisShape->SetDisplayMode(AIS_Shaded);
        aisShape->SetMaterial(Graphic3d_NameOfMaterial_Gold);
        aisShape->SetTransparency(0.3);
    }
    if (aList.IsEmpty())
    {
        return;
    }



    objView->GetContext()->Deactivate();
    objView->GetContext()->Activate(AIS_Shape::SelectionMode(TopAbs_FACE));
    objView->GetContext()->UpdateCurrentViewer();
    return;
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

    TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(pnt1,pnt2);
    Handle(AIS_Shape) theLineToDisplay = new AIS_Shape(edge);

    Quantity_ColorRGBA myCol(0,1,0,1);
    theLineToDisplay->SetColor(myCol.GetRGB());
    //??????
    theLineToDisplay->SetWidth(5.);

    objView->GetContext()->Display (theLineToDisplay, AIS_Shaded, 0, true);

}

