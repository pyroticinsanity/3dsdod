#ifndef DOD_RENDERER_HEADER
#define DOD_RENDERER_HEADER

class Renderer
{
public:
    Renderer() {}

    virtual ~Renderer() {}

    virtual void beginRendering() = 0;

    virtual void clearBuffer(bool includeDepthBuffer = false) = 0;
    virtual void deinitialize() = 0;

    virtual void drawLine(float x0, float y0, float x1, float y1) = 0;
    virtual void drawQuad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3) = 0;
    virtual void drawVector(float X0, float Y0, float X1, float Y1) = 0;

    virtual void endRendering() = 0;
    
    virtual int getScreenHeight() = 0;
    virtual int getScreenWidth() = 0;

    virtual unsigned int getVideoModeFlags() = 0;

    virtual void initialize() = 0;
    virtual void plotPoint(double X, double Y) = 0;

    virtual void resetMatrix() = 0;

    virtual void setClearColor(float red, float green, float blue, float alpha = 1.0f) = 0;

    virtual void setColor(float color[3]) = 0;
    virtual void setColor(float red, float green, float blue, float alpha = 1.0f) = 0;

    virtual void setViewport(int x, int y, int width, int height) = 0;

    virtual void swapBuffers() = 0;

    virtual void translateMatrix(float xOffset, float yOffset) = 0;
};

#endif // DOD_RENDERER_HEADER