#include <3ds.h>

#include "../dod.h"
#include "../enhanced.h"
#include "../viewer.h" // TODO Figure out how to decouple this
#include "../oslink.h" // TODO Figure out how to decouple this

#include "citro_renderer.h"

extern OS_Link                oslink;
extern Viewer		viewer;
extern Coordinate     crd;

void CitroRenderer::clearBuffer(bool includeDepthBuffer)
{
	
}

void CitroRenderer::drawLine(float x0, float y0, float x1, float y1)
{
	double width = 1;

	double slope = (x1 - x0) / (y1 - y0);
	double yMod = width * sin(atan(slope));
	double xMod = width * cos(atan(slope));

	C2D_DrawTriangle(_xOffset + x0 - xMod, _yOffset + y0 + yMod, _color, 
		_xOffset + x0 + xMod, _yOffset + y0 - yMod, _color,
		_xOffset + x1 - xMod, _yOffset + y1 + yMod, _color, 0);

	C2D_DrawTriangle(_xOffset + x1 - xMod, _yOffset + y1 + yMod, _color, 
		_xOffset + x0 + xMod, _yOffset + y0 - yMod, _color,
		_xOffset + x1 + xMod, _yOffset + y1 - yMod, _color, 0);
}

void CitroRenderer::drawQuad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3)
{
	//TODO Probably a better way to do this with less triangles.
	drawLine(x0, y0, x1, y1);
	drawLine(x1, y1, x2, y2);
	drawLine(x2, y2, x3, y3);
	drawLine(x3, y3, x0, y0);
}

void CitroRenderer::drawVector(float X0, float Y0, float X1, float Y1)
{
    	if (g_options&OPT_VECTOR) { // draw using GL vectors
		float		clrLine[3];

		if (viewer.VCTFAD == 0xff) return; // do not draw lines with VCTFAD=255

		// calculate line color from VCTFAD
		float flBirghtness=1.0f/(viewer.VCTFAD/2.0f + 1.0f);
		// calculate color between FG and BG
		clrLine[0]=viewer.fgColor[0]*flBirghtness+viewer.bgColor[0]*(1.0f-flBirghtness);
		clrLine[1]=viewer.fgColor[1]*flBirghtness+viewer.bgColor[1]*(1.0f-flBirghtness);
		clrLine[2]=viewer.fgColor[2]*flBirghtness+viewer.bgColor[2]*(1.0f-flBirghtness);

        setColor(clrLine[0], clrLine[1], clrLine[2], 1.0);
		drawLine(X0, Y0, X1, Y1);
		setColor(viewer.fgColor);
	}
	else {
		float XL, YL, L;
		int FADCNT;
		double DX, DY, XX, YY;

		if (viewer.VCTFAD == 0xFF)
		{
			return;
		}
		FADCNT = viewer.VCTFAD + 1;
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
			// we are scaling up according to the screen width
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
				FADCNT = viewer.VCTFAD + 1;
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

void CitroRenderer::initialize()
{
	// TODO
}

void CitroRenderer::plotPoint(double X, double Y)
{
	drawQuad(
			_xOffset + crd.newX(X),	_yOffset + crd.newY(Y),
			_xOffset + crd.newX(X+1),	_yOffset + crd.newY(Y),
			_xOffset + crd.newX(X+1),	_yOffset + crd.newY(Y+1),
			_xOffset + crd.newX(X),	_yOffset + crd.newY(Y+1));
}

void CitroRenderer::resetMatrix()
{
	_xOffset = 0;
	_yOffset = 0;
}

void CitroRenderer::setClearColor(float red, float green, float blue, float alpha)
{
    _clearColor = C2D_Color32f(red, green, blue, alpha);
}

void CitroRenderer::setColor(float color[3])
{
	setColor(color[0], color[1], color[2]);
}

void CitroRenderer::setColor(float red, float green, float blue, float alpha)
{
	_color = C2D_Color32f(red, green, blue, alpha);
}

void CitroRenderer::setViewport(int x, int y, int width, int height)
{
	C3D_SetViewport(x, y, width, height);
}

void CitroRenderer::setTranslation(float xOffset, float yOffset)
{
	_xOffset = xOffset;
	_yOffset = yOffset;
}

void CitroRenderer::swapBuffers()
{
	gfxSwapBuffers();
}