#ifdef PSP
#include <SDL/SDL_opengl.h>

#include "../dod.h"
#include "../enhanced.h"

#include "gl_renderer.h"

#define glColor3fv(x) glColor3f(x[0], x[1], x[2])

const int GlRenderer::ScreenHeight = 272;
const int GlRenderer::ScreenWidth = 480;


const struct kbdKey keyboardKeys[40] = { 

													{103, 130, 23, 23, 'Q'},
													{126, 130, 23, 23, 'W'},
													{149, 130, 23, 23, 'E'},
													{172, 130, 23, 23, 'R'},
													{195, 130, 23, 23, 'T'},
													{218, 130, 23, 23, 'Y'},
													{241, 130, 23, 23, 'U'},
													{264, 130, 23, 23, 'I'},
													{287, 130, 23, 23, 'O'},
													{310, 130, 23, 23, 'P'},

													{107, 106, 23, 23, 'A'},
													{130, 106, 23, 23, 'S'},
													{153, 106, 23, 23, 'D'},
													{176, 106, 23, 23, 'F'},
													{199, 106, 23, 23, 'G'},
													{222, 106, 23, 23, 'H'},
													{245, 106, 23, 23, 'J'},
													{268, 106, 23, 23, 'K'},
													{291, 106, 23, 23, 'L'},
													{339, 106, 35, 23, 0x0D},

													{117,  80, 23, 23, 'Z'},
													{140,  80, 23, 23, 'X'},
													{163,  80, 23, 23, 'C'},
													{186,  80, 23, 23, 'V'},
													{211,  80, 23, 23, 'B'},
													{235,  80, 23, 23, 'N'},
													{258,  80, 23, 23, 'M'},
													{281,  80, 23, 23, ','},
													{304,  80, 23, 23, '.'},
													{146,  57, 188, 23, ' '}

												};

void GlRenderer::clearBuffer(bool includeDepthBuffer)
{
	glClear(GL_COLOR_BUFFER_BIT | (includeDepthBuffer ? GL_DEPTH_BUFFER_BIT : 0));
}

void GlRenderer::deinitialize()
{

}

void GlRenderer::drawLine(float x0, float y0, float x1, float y1)
{
	glBegin(GL_LINES);
		glVertex2f(x0, y0);
		glVertex2f(x1, y1);
	glEnd();
}

void GlRenderer::drawQuad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3)
{
	glBegin(GL_QUADS);
		glVertex2f(x0, y0);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
		glVertex2f(x3, y3);
	glEnd();
}

void GlRenderer::drawKeyboard(struct kbdKey key)
{
	int squareWidth = key.width;
	int squareHeight = key.height;
	int highlightX = key.x;
	int highlightY = key.y;

	glEnable(GL_TEXTURE_2D);
	// Clear screen
	_renderer->setColor(bgColor);
	_renderer->clearBuffer(true);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  	glBindTexture(GL_TEXTURE_2D, keyboardTexture);
  	_renderer->resetMatrix();

 	glBegin(GL_QUADS);
    	glTexCoord2f(0,1);
    	glVertex2f(0, 0);

    	glTexCoord2f(1,1);
    	glVertex2f(480, 0);

    	glTexCoord2f(1,0);
    	glVertex2f(480, 272);

    	glTexCoord2f(0,0);
    	glVertex2f(0, 272);

  	glEnd();
  	glDisable(GL_TEXTURE_2D);

	glBegin(GL_LINE_STRIP);
		_renderer->setColor(0.0f, 1.0f, 0.0f, 1.0f);
		glVertex2f(highlightX, highlightY);

		_renderer->setColor(0.0f, 1.0f, 0.0f, 1.0f);
		glVertex2f(highlightX + squareWidth, highlightY);

		_renderer->setColor(0.0f, 1.0f, 0.0f, 1.0f);
		glVertex2f(highlightX + squareWidth, highlightY + squareHeight);

		_renderer->setColor(0.0f, 1.0f, 0.0f, 1.0f);
		glVertex2f(highlightX, highlightY + squareHeight);

		_renderer->setColor(0.0f, 1.0f, 0.0f, 1.0f);
		glVertex2f(highlightX, highlightY);
	glEnd();

 	// Draw Boxes for menu
 	_renderer->setColor(fgColor);
 	_renderer->resetMatrix();

  	drawString(0, 0, oslink.commandCreatorBuffer, strlen(oslink.commandCreatorBuffer));

  	_renderer->swapBuffers();

}

// Draws a line
void GlRenderer::drawVector(float X0, float Y0, float X1, float Y1)
{
	if (g_options&OPT_VECTOR) { // draw using GL vectors
		GLfloat		clrLine[3];

		if (VCTFAD == 0xff) return; // do not draw lines with VCTFAD=255

		// calculate line color from VCTFAD
		float flBirghtness=1.0f/(VCTFAD/2.0f + 1.0f);
		// calculate color between FG and BG
		clrLine[0]=fgColor[0]*flBirghtness+bgColor[0]*(1.0f-flBirghtness);
		clrLine[1]=fgColor[1]*flBirghtness+bgColor[1]*(1.0f-flBirghtness);
		clrLine[2]=fgColor[2]*flBirghtness+bgColor[2]*(1.0f-flBirghtness);

		setColor(clrLine);
		drawLine(
			crd.newX(X0),crd.newY(Y0),
			crd.newX(X1),crd.newY(Y1));
		setColor(fgColor);
	}
	else {
		float XL, YL, L;
		int FADCNT;
		double DX, DY, XX, YY;

		if (VCTFAD == 0xFF)
		{
			return;
		}
		FADCNT = VCTFAD + 1;
		XL = (X1 > X0) ? (X1 - X0) : (X0 - X1);
		YL = (Y1 > Y0) ? (Y1 - Y0) : (Y0 - Y1);
		L = (XL > YL) ? XL : YL;
		if (L == 0)
		{
			return;
		}
		DX = ((double) XL / (double) L) * ((X0 < X1) ? 1 : -1);
		DY = ((double) YL / (double) L) * ((Y0 < Y1) ? 1 : -1);
		if (g_options&OPT_HIRES) { // prepare to draw a HIRES line
			// in hires mode, all we need to do is increase the # pixels per line.
			// we are scaling up accoring to the screen width
			int scale=oslink.width>>8;
			DX/=(double)scale;
			DY/=(double)scale;
			L*=scale;
		}
		XX = X0 + 0.5;
		YY = Y0 + 0.5;
		do
		{
			if (--FADCNT == 0)
			{
				FADCNT = VCTFAD + 1;
				if (XX >= 0.0 && XX < 256.0 &&
					YY >= 0.0 && YY < 152.0)
				{
					if (g_options&OPT_HIRES)
						plotPoint(XX, YY);
					else {
						plotPoint((int)XX, (int)YY);
					}
				}
			}
			XX += DX;
			YY += DY;
			--L;
		} while (L > 0);
	}
}

void GlRenderer::initialize()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D(0, oslink.width, 0, oslink.height);
	glOrtho(0, oslink.width, 0, oslink.height, -1, 1);

	SDL_Surface* surface = IMG_Load("images/keyboard.png");

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &keyboardTexture);
	glBindTexture(GL_TEXTURE_2D, keyboardTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h , 0, GL_RGB,
	             GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);
	glDisable(GL_TEXTURE_2D);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	resetMatrix();
}

void GlRenderer::plotPoint(double X, double Y)
{
	if (g_options&OPT_HIRES)
	{	// draw a single pixel
		glBegin(GL_POINTS);
		float x,y;
		x=crd.newX(X);
		y=crd.newY(Y);
		glVertex2f(x,y);
		glEnd();
	}
	else { // draw a COCO pixel (square)
		drawQuad(
			crd.newX(X),	crd.newY(Y),
			crd.newX(X+1),	crd.newY(Y),
			crd.newX(X+1),	crd.newY(Y+1),
			crd.newX(X),	crd.newY(Y+1));
	}
}

void GlRenderer::resetMatrix()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GlRenderer::setClearColor(float red, float green, float blue, float alpha)
{
    glClearColor(red, green, blue, alpha);
}

void GlRenderer::setColor(float color[3])
{
	glColor3fv(color);
}

void GlRenderer::setColor(float red, float green, float blue, float alpha)
{
	glColor4f(red, green, blue, alpha);
}

void GlRenderer::setViewport(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
}

void GlRenderer::swapBuffers()
{
	SDL_GL_SwapBuffers();
}

void GlRenderer::translateMatrix(float xOffset, float yOffset)
{
	glTranslatef(xOffset, yOffset, 0.0);
}

#endif //PSP