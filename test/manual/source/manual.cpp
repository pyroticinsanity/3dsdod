#include <3ds.h>

#include <citro3d.h>
#include <citro2d.h>
#include <dirent.h>
#include <string>
#include <vector>

#include "manual.h"

extern bool loadImageFromFile(const char *filename, C3D_Tex *tex, C2D_Image *image);

Manual::Manual(std::string path)
    : _currentPage(0), _path(path), _scale(1.0), _scroll(0)
{
   if(!getManualPages())
   {
      printf("Failed to open directory %s.\n", path.c_str());
   }
   else
   {
      open();
   }
}

void Manual::close()
{
   C3D_TexDelete(&_tex);
}

void Manual::drawPage(C3D_RenderTarget *target)
{
   C2D_SceneBegin(target);
   C2D_DrawImageAt(_img, 0, _scroll, 0, NULL, _scale, _scale);
}

bool Manual::getManualPages()
{
	DIR *dir;
	struct dirent *ent;
	if((dir = opendir(_path.c_str())) != NULL)
	{
		/* print all the files and directories within directory */
		while((ent = readdir(dir)) != NULL)
		{
			if(strstr(ent->d_name, ".png") || strstr(ent->d_name, ".PNG"))
			{
				_pages.push_back(_path + std::string("/") + ent->d_name);
			}
		}
		closedir(dir);
	}
	else
	{
		/* could not open directory */
		return false;
	}

	return true;
}

void Manual::nextPage()
{
   if (_currentPage < _pages.size() - 1)
   {
      _currentPage++;
      C3D_TexDelete(&_tex);
      delete _img.subtex;
      loadImageFromFile(_pages[_currentPage].c_str(), &_tex, &_img);

      _scroll = 0;
   }
}

void Manual::open()
{
   printf("Loading page 0\n");
   _currentPage = 0;
   delete _img.subtex;
   loadImageFromFile(_pages[_currentPage].c_str(), &_tex, &_img);
}

void Manual::previousPage()
{
   if (_currentPage > 0)
   {
      _currentPage--;
      _scroll = 0;
      C3D_TexDelete(&_tex);
      delete _img.subtex;
      loadImageFromFile(_pages[_currentPage].c_str(), &_tex, &_img);
   }
}

void Manual::scrollDown()
{
   if (_scroll > -160)
   {
      _scroll -= 5;
   }
}

void Manual::scrollUp()
{
   if (_scroll < 0)
   {
      _scroll += 5;
   }
}