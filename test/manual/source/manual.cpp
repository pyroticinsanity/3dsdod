#include <3ds.h>

#include <citro3d.h>
#include <citro2d.h>
   
   #include "manual.h"

#include "dod01_t3x.h"
#include "dod02_t3x.h"
#include "dod03_t3x.h"
#include "dod04_t3x.h"
#include "dod05_t3x.h"
#include "dod06_t3x.h"
#include "dod07_t3x.h"
#include "dod08_t3x.h"
#include "dod09_t3x.h"
#include "dod10_t3x.h"
#include "dod11_t3x.h"
#include "dod12_t3x.h"
#include "dod13_t3x.h"
#include "dod14_t3x.h"
#include "dod15_t3x.h"
#include "dod16_t3x.h"
#include "dod17_t3x.h"
#include "dod18_t3x.h"
#include "dod19_t3x.h"
#include "dod20_t3x.h"
#include "dod21_t3x.h"
#include "dod22_t3x.h"
#include "dod23_t3x.h"
#include "dod24_t3x.h"
#include "dod25_t3x.h"
#include "dod26_t3x.h"
#include "dod27_t3x.h"
#include "dod28_t3x.h"
#include "dod29_t3x.h"
#include "dod30_t3x.h"
#include "dod31_t3x.h"
#include "dod32_t3x.h"
#include "dod33_t3x.h"
#include "dod34_t3x.h"
#include "dod35_t3x.h"
#include "dod36_t3x.h"
#include "dod37_t3x.h"
#include "dod38_t3x.h"

   struct Page
{
	const u8* t3x;
	const size_t size;
};
/*
void loadImage(const u8* input, size_t insize, C3D_Tex* tex, C2D_Image* image)
{
	Tex3DS_SubTexture *subtex = new Tex3DS_SubTexture();
	subtex->left=subtex->top=0; subtex->width=600; subtex->height=600;
	subtex->right=subtex->bottom=1;
	image->subtex = subtex;
	image->tex = tex;

   printf("Loading Image size: %d\n", insize);

	Tex3DS_Texture t3x = Tex3DS_TextureImport(input, insize, tex, NULL, false);

	C3D_TexSetFilter(tex, GPU_LINEAR, GPU_NEAREST);

	// Delete the t3x object since we don't need it
	Tex3DS_TextureFree(t3x);
}*/

extern void loadImage(const u8* input, size_t insize, C3D_Tex* tex, C2D_Image* image);

extern struct Page Pages[];
/*
struct Page Pages[] =
{
        {dod01_t3x, dod01_t3x_size},
        {dod02_t3x, dod02_t3x_size},
        {dod03_t3x, dod03_t3x_size},
        {dod04_t3x, dod04_t3x_size},
        {dod05_t3x, dod05_t3x_size},
        {dod06_t3x, dod06_t3x_size},
        {dod07_t3x, dod07_t3x_size},
        {dod08_t3x, dod08_t3x_size},
        {dod09_t3x, dod09_t3x_size},
        {dod10_t3x, dod10_t3x_size},
        {dod11_t3x, dod11_t3x_size},
        {dod12_t3x, dod12_t3x_size},
        {dod13_t3x, dod13_t3x_size},
        {dod14_t3x, dod14_t3x_size},
        {dod15_t3x, dod15_t3x_size},
        {dod16_t3x, dod16_t3x_size},
        {dod17_t3x, dod17_t3x_size},
        {dod18_t3x, dod18_t3x_size},
        {dod19_t3x, dod19_t3x_size},
        {dod20_t3x, dod20_t3x_size},
        {dod21_t3x, dod21_t3x_size},
        {dod22_t3x, dod22_t3x_size},
        {dod23_t3x, dod23_t3x_size},
        {dod24_t3x, dod24_t3x_size},
        {dod25_t3x, dod25_t3x_size},
        {dod26_t3x, dod26_t3x_size},
        {dod27_t3x, dod27_t3x_size},
        {dod28_t3x, dod28_t3x_size},
        {dod29_t3x, dod29_t3x_size},
        {dod30_t3x, dod30_t3x_size},
        {dod31_t3x, dod31_t3x_size},
        {dod32_t3x, dod32_t3x_size},
        {dod33_t3x, dod33_t3x_size},
        {dod34_t3x, dod34_t3x_size},
        {dod35_t3x, dod35_t3x_size},
        {dod36_t3x, dod36_t3x_size},
        {dod37_t3x, dod37_t3x_size},
        {dod38_t3x, dod38_t3x_size}
};*/

#define NUMBER_OF_PAGES 38
   
   Manual::Manual()
   : _currentPage(0), _scale(0.87), _scroll(0)
   {
      open();
   }

    void Manual::close()
    {
       C3D_TexDelete(&_tex);
    }

    void Manual::drawPage(C3D_RenderTarget* target)
    {
       C2D_SceneBegin(target);
        C2D_DrawImageAt(_img, -120, _scroll, 0, NULL, _scale, _scale);
    }

    void Manual::nextPage()
    {
       if(_currentPage < NUMBER_OF_PAGES - 1)
       {
         _currentPage++;
			C3D_TexDelete(&_tex);
			loadImage(Pages[_currentPage].t3x, Pages[_currentPage].size, &_tex, &_img);

			_scroll = 0;
       }
    }

    void Manual::open()
    {
       printf("Loading page 0\n");
       _currentPage = 0;
       loadImage(Pages[0].t3x, Pages[0].size, &_tex, &_img);
    }

    void Manual::previousPage()
    {
       if(_currentPage > 0)
       {
       _currentPage--;
			_scroll = 0;
			C3D_TexDelete(&_tex);
			loadImage(Pages[_currentPage].t3x, Pages[_currentPage].size, &_tex, &_img);
       }
    }

    void Manual::scrollDown()
    {
       if(_scroll > -160)
       {
          _scroll-=5;
       }
    }

    void Manual::scrollUp()
    {
       if(_scroll < 0)
       {
          _scroll+=5;
       }
    }