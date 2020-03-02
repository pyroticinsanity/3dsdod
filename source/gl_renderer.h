#ifndef DOD_GL_RENDERER_HEADER
#define DOD_GL_RENDERER_HEADER

#include "renderer.h"

class GlRenderer: public Renderer
{
public:
    GlRenderer() : Renderer() {}

    void drawVector(float X0, float Y0, float X1, float Y1);
    
    void plotPoint(double X, double Y);

    void setClearColor(float red, float green, float blue, float alpha);
};
#endif //DOD_GL_RENDERER_HEADER