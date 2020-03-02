#include <SDL/SDL_opengl.h>

#include "dod.h"
#include "enhanced.h"

#include "gl_renderer.h"

void GlRenderer::clearBuffer(bool includeDepthBuffer)
{
	glClear(GL_COLOR_BUFFER_BIT | (includeDepthBuffer ? GL_DEPTH_BUFFER_BIT : 0));
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

		// draw the vector
		glBegin(GL_LINES);
		glColor3fv(clrLine);
		glVertex2f(crd.newX(X0),crd.newY(Y0));
		glVertex2f(crd.newX(X1),crd.newY(Y1));
		glColor3fv(fgColor);
		glEnd();
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
		glBegin(GL_QUADS);
		glVertex2f(crd.newX(X),	crd.newY(Y));
		glVertex2f(crd.newX(X+1),	crd.newY(Y));
		glVertex2f(crd.newX(X+1),	crd.newY(Y+1));
		glVertex2f(crd.newX(X),		crd.newY(Y+1));
		glEnd();
	}
}

void GlRenderer::setClearColor(float red, float green, float blue, float alpha)
{
    glClearColor(red, green, blue, alpha);
}