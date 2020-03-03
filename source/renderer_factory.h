#ifndef DOD_RENDERER_FACTORY_HEADER
#define DOD_RENDERER_FACTORY_HEADER

#include "renderer.h"
class RendererFactory
{
public:
    static Renderer* GetRenderer();

private:
    RendererFactory();
    
    static Renderer* _renderer;
};

#endif // DOD_RENDERER_FACTORY_HEADER