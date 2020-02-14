/***************************************************************************
 *   Copyright (C) 2008 by darkweb   *
 *      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <stdlib.h>
#include "SDL.h"

using namespace std;

int main(int argc, char *argv[])
{
 cout <<"Initializing SDL." << endl;
  /* Initializes Audio and the CDROM, add SDL_INIT_VIDEO for Video */
  if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_CDROM)< 0) {
    cout <<"Could not initialize SDL:" << SDL_GetError() << endl;
       SDL_Quit();
  } else {
    cout << "Audio & CDROM initialized correctly" << endl;;
    /* Trying to read number of CD devices on system */
	cout << "Drives available :" << SDL_CDNumDrives() << endl;
    for(int i=0; i < SDL_CDNumDrives(); ++i) {
      cout << "Drive " << i << "\"" << SDL_CDName(i) << "\"";
    }
  }
  SDL_Quit();
}
