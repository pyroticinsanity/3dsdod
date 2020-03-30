#ifndef MANUAL_HEADER
#define MANUAL_HEADER

#include <3ds.h>

#include <citro3d.h>
#include <citro2d.h>

class Manual
{
public:
    Manual();
    
    virtual ~Manual()
    {}

    void close();

    void drawPage(C3D_RenderTarget* target);

    void nextPage();

    void open();

    void previousPage();

    void scrollDown();

    void scrollUp();

//private:
    int _currentPage;

    C3D_Tex _tex;
    C2D_Image _img;

    int _scale;
    int _scroll;
    
};

#endif // MANUAL_HEADER