#include "renderer_factory.h"

#include "citro_renderer.h"

RendererFactory::RendererFactory()
: _renderer(NULL)
{}

Renderer* RendererFactory::GetRenderer()
{
    if(_renderer == NULL)
    {
        _renderer = new CitroRenderer();
    }

    return _renderer;
}