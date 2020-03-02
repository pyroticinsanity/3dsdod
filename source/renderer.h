#ifndef DOD_RENDERER_HEADER
#define DOD_RENDERER_HEADER

class Renderer
{
public:
    Renderer() {}

    virtual ~Renderer() {}
    void drawVector(float X0, float Y0, float X1, float Y1) = 0;

    void plotPoint(double X, double Y) = 0;

    void setClearColor(float red, float green, float blue, float alpha) = 0;
};

#endif // DOD_RENDERER_HEADER