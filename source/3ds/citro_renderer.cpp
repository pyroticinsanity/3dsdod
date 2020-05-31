#include <3ds.h>

#include "../dod.h"
#include "../enhanced.h"
#include "../viewer.h" // TODO Figure out how to decouple this
#include "../oslink.h" // TODO Figure out how to decouple this
#include "../version.h"

#include "keyboard_t3x.h"

#include "ctr_utils.h"

#include "citro_renderer.h"

extern OS_Link oslink;
extern Viewer viewer;
extern Coordinate crd;

const struct kbdKey keyboardKeys[40] = {{87, 105, 15, 15, 'Q'},
										{107, 105, 15, 15, 'W'},
										{127, 105, 15, 15, 'E'},
										{144, 105, 15, 15, 'R'},
										{163, 105, 15, 15, 'T'},
										{183, 105, 15, 15, 'Y'},
										{201, 105, 15, 15, 'U'},
										{221, 105, 15, 15, 'I'},
										{240, 105, 15, 15, 'O'},
										{259, 105, 15, 15, 'P'},

										{92, 128, 15, 15, 'A'},
										{110, 128, 15, 15, 'S'},
										{130, 128, 15, 15, 'D'},
										{150, 128, 15, 15, 'F'},
										{169, 128, 15, 15, 'G'},
										{188, 128, 15, 15, 'H'},
										{207, 128, 15, 15, 'J'},
										{225, 128, 15, 15, 'K'},
										{246, 128, 15, 15, 'L'},
										{285, 128, 25, 15, 0x0D},

										{99, 150, 15, 15, 'Z'},
										{119, 150, 15, 15, 'X'},
										{139, 150, 15, 15, 'C'},
										{159, 150, 15, 15, 'V'},
										{178, 150, 15, 15, 'B'},
										{197, 150, 15, 15, 'N'},
										{217, 150, 15, 15, 'M'},
										{237, 150, 15, 15, ','},
										{257, 150, 15, 15, '.'},
										{125, 173, 150, 15, ' '}

};

const int CitroRenderer::ScreenHeight = 240;
const int CitroRenderer::ScreenWidth = 400;

CitroRenderer::CitroRenderer()
	: Renderer(), _currentImage(NULL), _xOffset(0), _yOffset(0)
{
	// Init libs
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);

	// Need to increase this from the default to handle so many pixel based triangles.
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS * 2);
	C2D_Prepare();

	gfxSet3D(true);

	// Create screens
	_left = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	_right = C2D_CreateScreenTarget(GFX_TOP, GFX_RIGHT);
	_bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	_keyboardImg.tex = new C3D_Tex();
	Tex3DS_SubTexture *subtex = new Tex3DS_SubTexture();

	subtex->left = 0;
	subtex->top = 0;
	subtex->width = 400;
	subtex->height = 240;
	subtex->right = 1;
	subtex->bottom = 1;

	_keyboardImg.subtex = subtex;

	Tex3DS_Texture t3x = Tex3DS_TextureImport(keyboard_t3x, keyboard_t3x_size,
											  _keyboardImg.tex, NULL, false);

	// Delete the t3x object since we don't need it
	Tex3DS_TextureFree(t3x);
}

CitroRenderer::~CitroRenderer()
{
	C3D_TexDelete(_keyboardImg.tex);
	delete _keyboardImg.subtex;

	C3D_TexDelete(_controlsImg.tex);
	delete _controlsImg.subtex;
}

void CitroRenderer::clearBuffer(bool includeDepthBuffer)
{
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(_left, _clearColor);
	C2D_TargetClear(_right, _clearColor);
	C2D_SceneBegin(_left);
}

void CitroRenderer::deinitialize()
{
	C2D_TextBufDelete(_versionBuf);
	C2D_Fini();
	C3D_Fini();
	gfxExit();
}

void CitroRenderer::drawLine(float x0, float y0, float x1, float y1, Layers layer)
{
	float width = 0.5;
	float slider = osGet3DSliderState();

	float slope = (x1 - x0) / (y1 - y0);
	float yWidth = width * sin(atan(slope));
	float xWidth = width * cos(atan(slope));

	float xShift = 0;
	xShift = getStereoscopicOffset(layer, slider);

	_triangles.push(Triangle(x0 - xWidth + xShift, y0 + yWidth,
							 x0 + xWidth + xShift, y0 - yWidth,
							 x1 - xWidth + xShift, y1 + yWidth, _color, layer));

	C2D_DrawTriangle(x0 - xWidth - xShift, y0 + yWidth, _color,
					 x0 + xWidth - xShift, y0 - yWidth, _color,
					 x1 - xWidth - xShift, y1 + yWidth, _color, 0);

	_triangles.push(Triangle(x1 - xWidth + xShift, y1 + yWidth,
							 x0 + xWidth + xShift, y0 - yWidth,
							 x1 + xWidth + xShift, y1 - yWidth,
							 _color, layer));

	C2D_DrawTriangle(x1 - xWidth - xShift, y1 + yWidth, _color,
					 x0 + xWidth - xShift, y0 - yWidth, _color,
					 x1 + xWidth - xShift, y1 - yWidth, _color, 0);
}

void CitroRenderer::drawQuad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3,
							 Layers layer)
{
	// We need to flip the y-axis because OpenGL's coordinate system has 0 at the bottom whereas
	// the 3DS has 0 at the top
	y0 = ScreenHeight - y0;
	y1 = ScreenHeight - y1;
	y2 = ScreenHeight - y2;
	y3 = ScreenHeight - y3;

	float slider = osGet3DSliderState();
	float xShift = 0;
	xShift = getStereoscopicOffset(layer, slider);

	_triangles.push(Triangle(_xOffset + x0 + xShift, _yOffset + y0,
							 _xOffset + x1 + xShift, _yOffset + y1,
							 _xOffset + x2 + xShift, _yOffset + y2, _color, layer));

	C2D_DrawTriangle(_xOffset + x0 - xShift, _yOffset + y0, _color,
					 _xOffset + x1 - xShift, _yOffset + y1, _color,
					 _xOffset + x2 - xShift, _yOffset + y2, _color, 0);

	_triangles.push(Triangle(_xOffset + x2 + xShift, _yOffset + y2,
							 _xOffset + x3 + xShift, _yOffset + y3,
							 _xOffset + x0 + xShift, _yOffset + y0, _color, layer));

	C2D_DrawTriangle(_xOffset + x2 - xShift, _yOffset + y2, _color,
					 _xOffset + x3 - xShift, _yOffset + y3, _color,
					 _xOffset + x0 - xShift, _yOffset + y0, _color, 0);
}

void CitroRenderer::drawImage(Renderer::Image *img)
{
	C2D_Image *data = (C2D_Image *)img->data;
	C2D_DrawImageAt(*data, img->x, img->y, img->depth);

	_currentImage = data;
}

void CitroRenderer::drawKeyboard(struct kbdKey key)
{
	clearBuffer();
	int squareWidth = key.width;
	int squareHeight = key.height;
	int highlightX = key.x;
	int highlightY = key.y;

	C2D_DrawImageAt(_keyboardImg, 0, 0, 0);

	_currentImage = &_keyboardImg;

	resetMatrix();

	setColor(0.0f, 1.0f, 0.0f);
	drawLine(highlightX, highlightY, highlightX + squareWidth, highlightY);
	drawLine(highlightX + squareWidth, highlightY, highlightX + squareWidth, highlightY + squareHeight);
	drawLine(highlightX + squareWidth, highlightY + squareHeight, highlightX, highlightY + squareHeight);
	drawLine(highlightX, highlightY + squareHeight, highlightX, highlightY);
	setColor(viewer.fgColor);
}

void CitroRenderer::drawVector(float X0, float Y0, float X1, float Y1, Layers layer)
{
	if (g_options & OPT_VECTOR)
	{
		float clrLine[3];

		if (viewer.VCTFAD == 0xff)
			return; // do not draw lines with VCTFAD=255

		// calculate line color from VCTFAD
		float flBirghtness = 1.0f / (viewer.VCTFAD / 2.0f + 1.0f);
		// calculate color between FG and BG
		clrLine[0] = viewer.fgColor[0] * flBirghtness + viewer.bgColor[0] * (1.0f - flBirghtness);
		clrLine[1] = viewer.fgColor[1] * flBirghtness + viewer.bgColor[1] * (1.0f - flBirghtness);
		clrLine[2] = viewer.fgColor[2] * flBirghtness + viewer.bgColor[2] * (1.0f - flBirghtness);

		// Apply a bounding box around it
		X0 = (X0 < 0) ? 0 : (X0 > 256) ? 256 : X0;
		X1 = (X1 < 0) ? 0 : (X1 > 256) ? 256 : X1;
		//Y0 = (Y0 < 0) ? 0 : (Y0 > 152) ? 152 : Y0;
		//Y1 = (Y1 < 0) ? 0 : (Y1 > 152) ? 152 : Y1;

		// We need to flip the y-axis because OpenGL's coordinate system has 0 at the bottom whereas
		// the 3DS has 0 at the top
		//Y0 = ScreenHeight - Y0;
		//Y1 = ScreenHeight - Y1;

		setColor(clrLine[0], clrLine[1], clrLine[2]);
		drawLine(crd.newX(X0), Y0, crd.newX(X1), Y1, layer);
		setColor(viewer.fgColor);
	}
	else
	{
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
		DX = ((double)XL / (double)L) * ((X0 < X1) ? 1 : -1);
		DY = ((double)YL / (double)L) * ((Y0 < Y1) ? 1 : -1);
		if (g_options & OPT_HIRES)
		{ // prepare to draw a HIRES line
			// in hires mode, all we need to do is increase the # pixels per line.
			// we are scaling up according to the screen width
			int scale = oslink.width >> 8;
			DX /= (double)scale;
			DY /= (double)scale;
			L *= scale;
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
					if (g_options & OPT_HIRES)
						plotPoint(XX, YY, layer);
					else
					{
						plotPoint((int)XX, (int)YY, layer);
					}
				}
			}
			XX += DX;
			YY += DY;
			--L;
		} while (L > 0);
	}
}

float CitroRenderer::getStereoscopicOffset(Layers layer, float slider)
{
	float xShift = 0;
	if (slider > 0.0f)
	{
		switch (layer)
		{
		case LAYER_DEFAULT:
			xShift = 0;
			break;
		case LAYER_0:
			xShift = 2.0f;
			break;
		case LAYER_FRONT:
			xShift = -1.5f;
			break;
		case LAYER_UI:
			xShift = -2.0f;
			break;
		default:
			xShift = 0;
			break;
		}
		xShift = xShift * slider;
	}

	return xShift;
}

void CitroRenderer::initialize()
{
	CtrUtils::LoadImageFromFile("romfs:/images/controls.png", 320, 240, &_controlsTex, &_controlsImg);

	_versionBuf = C2D_TextBufNew(4096);

	C2D_TextParse(&_versionText, _versionBuf, VersionString);
	C2D_TextOptimize(&_versionText);

	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(_bottom, _clearColor);
	C2D_SceneBegin(_bottom);
	C2D_DrawImageAt(_controlsImg, 0, 0, 0);
	C2D_DrawText(&_versionText, C2D_AtBaseline | C2D_WithColor, 145.0f, 90.0f, 1.0f, 0.5f, 0.5f, C2D_Color32f(1.0f, 1.0f, 0.0f, 1.0f));
	C3D_FrameEnd(0);
}

void CitroRenderer::plotPoint(double X, double Y, Layers layer)
{
	drawQuad(
		_xOffset + crd.newX(X), _yOffset + crd.newY(Y),
		_xOffset + crd.newX(X + 1), _yOffset + crd.newY(Y),
		_xOffset + crd.newX(X + 1), _yOffset + crd.newY(Y + 1),
		_xOffset + crd.newX(X), _yOffset + crd.newY(Y + 1), layer);
}

void CitroRenderer::renderRightScreen()
{
	float slider = osGet3DSliderState();

	C2D_SceneBegin(_right);

	if (_currentImage != NULL)
	{
		C2D_DrawImageAt(*_currentImage, 0, 0, 0);
		_currentImage = NULL;
	}

	while (!_triangles.empty())
	{
		Triangle triangle = _triangles.front();
		_triangles.pop();

		if (slider > 0.0f)
		{
			C2D_DrawTriangle(triangle.x0, triangle.y0, triangle.color,
							 triangle.x1, triangle.y1, triangle.color,
							 triangle.x2, triangle.y2, triangle.color, 0);
		}
	}
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
	C2D_SceneSize(width, height, false);
}

void CitroRenderer::swapBuffers()
{
	renderRightScreen();
	C3D_FrameEnd(0);
}

void CitroRenderer::translateMatrix(float xOffset, float yOffset)
{
	_xOffset += xOffset;

	// Flip the y-axis
	_yOffset -= yOffset;
}