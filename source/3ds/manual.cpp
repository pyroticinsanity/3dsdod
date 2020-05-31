#include <3ds.h>

#include <algorithm>
#include <dirent.h>

#include "ctr_utils.h"
#include "manual.h"

const int SCREEN_HEIGHT = 240;

Manual::Manual(std::string path)
    : _currentImage(NULL, 0, 0, 0), _currentPage(0), _initialized(false), _path(path), _scale(1.0), _scroll(0)
{
   if(getManualPages())
   {
     open();
   }
}

void Manual::close()
{
	if(_initialized)
	{
		C3D_TexDelete(&_tex);
		delete _img.subtex;
		_initialized = false;
	}
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
		std::sort(_pages.begin(), _pages.end());
	}
	else
	{
		/* could not open directory */
		return false;
	}

	return true;
}

bool Manual::loadImage(std::string filename)
{
	if(CtrUtils::LoadImageFromFile(filename.c_str(), 400, 400, &_tex, &_img))
	{
		_currentImage.data = (void*)&_img;
		_currentImage.x = 0;
		_currentImage.y = 0;
		_currentImage.depth = 0;
		return true;
	}
	else
	{
		_currentImage.data = NULL;
		return false;
	}
}

void Manual::nextPage()
{
   if (_currentPage < _pages.size() - 1)
   {
      _currentPage++;
      C3D_TexDelete(&_tex);
      delete _img.subtex;
	  loadImage(_pages[_currentPage]);

      _scroll = 0;
   }
}

void Manual::open()
{
   	_currentPage = 0;
   	if(_pages.size() > 0)
   	{
   		loadImage(_pages[_currentPage]);
		_initialized = true;
	}
}

void Manual::previousPage()
{
   if (_currentPage > 0)
   {
      _currentPage--;
      _scroll = 0;
      C3D_TexDelete(&_tex);
      delete _img.subtex;
      loadImage(_pages[_currentPage]);
   }
}

void Manual::scrollDown()
{
   if (_currentImage.y > (SCREEN_HEIGHT - _img.subtex->height))
   {
      _currentImage.y -= 5;
   }
}

void Manual::scrollUp()
{
   if (_currentImage.y < 0)
   {
      _currentImage.y += 5;
   }
}