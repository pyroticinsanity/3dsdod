#ifndef DOD_GL_RENDERER_HEADER
#define DOD_GL_RENDERER_HEADER

#include "renderer.h"

class GlRenderer: public Renderer
{
public:
    GlRenderer() : Renderer() {}

    virtual void clearBuffer(bool includeDepthBuffer = false);
    virtual void drawVector(float X0, float Y0, float X1, float Y1);
    
    virtual void plotPoint(double X, double Y);

    virtual void setClearColor(float red, float green, float blue, float alpha);

    virtual void setColor(float color[3]);
    virtual void setColor(float red, float green, float blue, float alpha = 0.0f);
};
#endif //DOD_GL_RENDERER_HEADER