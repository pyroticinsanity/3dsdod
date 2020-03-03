#include "renderer_factory.h"

#include "3ds/citro_renderer.h"

Renderer* RendererFactory::_renderer = NULL;
RendererFactory::RendererFactory()
{}

Renderer* RendererFactory::GetRenderer()
{
    if(_renderer == NULL)
    {
        _renderer = new CitroRenderer();
    }

    return _renderer;
}