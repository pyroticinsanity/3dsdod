#ifndef DOD_CITRO_RENDERER_HEADER
#define DOD_CITRO_RENDERER_HEADER

#include "renderer.h"

class CitroRenderer: public Renderer
{
   CitroRenderer() : Renderer() {}

   void drawVector(float X0, float Y0, float X1, float Y1);

    void plotPoint(double X, double Y);

    void setClearColor(float red, float green, float blue, float alpha);
};
#endif // DOD_CITRO_RENDERER_HEADER