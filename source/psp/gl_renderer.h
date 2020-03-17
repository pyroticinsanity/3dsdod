#ifndef DOD_GL_RENDERER_HEADER
#define DOD_GL_RENDERER_HEADER

#ifdef PSP
#include "renderer.h"

class GlRenderer: public Renderer
{
public:
    static const int ScreenHeight;
    static const int ScreenWidth;
    
    GlRenderer() : Renderer() {}

    virtual void beginRendering() {}
    virtual void clearBuffer(bool includeDepthBuffer = false);

    virtual void deinitialize();
    virtual void drawLine(float x0, float y0, float x1, float y1);
    virtual void drawQuad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);
    virtual void drawVector(float X0, float Y0, float X1, float Y1);

    virtual void endRendering() {}

    virtual int void getScreenHeight()
    {
        return ScreenHeight;
    }

    virtual int void getScreenWidth()
    {
        return ScreenWidth;
    }

    virtual unsigned int getVideoModeFlags()
    {
        return SDL_OPENGL;
    }
    
    virtual void initialize();
    
    virtual void plotPoint(double X, double Y);

    virtual void resetMatrix();

    virtual void setClearColor(float red, float green, float blue, float alpha = 1.0f);

    virtual void setColor(float color[3]);
    virtual void setColor(float red, float green, float blue, float alpha = 1.0f);

    virtual void setViewport(int x, int y, int width, int height);

    virtual void swapBuffers();

    virtual void translateMatrix(float xOffset, float yOffset);
};

#endif //PSP
#endif //DOD_GL_RENDERER_HEADER