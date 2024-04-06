#pragma once
#include <AIS_Shape.hxx>
#include <AIS_ViewCube.hxx>

class GlfwOcctView;
class ModelingOCCT
{
public:
    ModelingOCCT(GlfwOcctView *obj);

    void initDemoScene();
    void initExample1();
    void initExample2();
    void initExample4();
    void initLession5();
    void initLession6();
    void SearchingGP_Points();
    void HelixExample();
    void initLession7();
    void initLession8();
    void initLession9();
    void initLession10();
    void initLession11();

    void review();

    //Using gp_Point and Cartesian
    void Lession2();

private:


private:
    GlfwOcctView *objView;

};


