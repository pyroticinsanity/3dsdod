#ifndef DOD_RENDERER_HEADER
#define DOD_RENDERER_HEADER

class Renderer
{
public:
    Renderer() {}

    virtual ~Renderer() {}

    virtual void clearBuffer(bool includeDepthBuffer = false) = 0;
    virtual void drawLine(float x0, float y0, float x1, float y1) = 0;
    virtual void drawVector(float X0, float Y0, float X1, float Y1) = 0;

    virtual void plotPoint(double X, double Y) = 0;

    virtual void setClearColor(float red, float green, float blue, float alpha) = 0;

    virtual void setColor(float color[3]) = 0;
    virtual void setColor(float red, float green, float blue, float alpha = 0.0f) = 0;
};

#endif // DOD_RENDERER_HEADER