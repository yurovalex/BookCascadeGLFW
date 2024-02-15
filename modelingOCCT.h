#pragma once

class GlfwOcctView;

class ModelingOCCT
{
public:
    ModelingOCCT(GlfwOcctView *obj);

    void initDemoScene();
    void initExample1();
    void initExample2();

    //Using gp_Point and Cartesian
    void Lession2();

private:
    GlfwOcctView *objView;
};


