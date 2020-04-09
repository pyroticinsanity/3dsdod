#include "renderer_factory.h"

#ifdef _3DS
#include "3ds/citro_renderer.h"
#endif

Renderer* RendererFactory::_renderer = NULL;
RendererFactory::RendererFactory()
{}

Renderer* RendererFactory::GetRenderer()
{
    if(_renderer == NULL)
    {
#ifdef _3DS
        _renderer = new CitroRenderer();
#endif
    }

    return _renderer;
}