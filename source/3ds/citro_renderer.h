#ifndef DOD_CITRO_RENDERER_HEADER
#define DOD_CITRO_RENDERER_HEADER

#include <citro2d.h>
#include "../renderer.h"

class CitroRenderer: public Renderer
{
public:
   CitroRenderer();

   virtual ~CitroRenderer() {}

    virtual void beginRendering();
    virtual void clearBuffer(bool includeDepthBuffer = false);

    virtual void deinitialize();
    
    virtual void drawLine(float x0, float y0, float x1, float y1);

    virtual void drawQuad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);

    virtual void drawVector(float X0, float Y0, float X1, float Y1);

    virtual void endRendering();
    virtual void initialize();

    virtual void plotPoint(double X, double Y);

    virtual void resetMatrix();

    virtual void setClearColor(float red, float green, float blue, float alpha = 1.0f);

    virtual void setColor(float color[3]);
    virtual void setColor(float red, float green, float blue, float alpha = 1.0f);
    virtual void setTranslation(float xOffset, float yOffset);
    virtual void setViewport(int x, int y, int width, int height);

    virtual void swapBuffers();
private:
    u32 _clearColor;

    u32 _color;

    C3D_RenderTarget* _top;

    float _xOffset;
    float _yOffset;
};
#endif // DOD_CITRO_RENDERER_HEADER