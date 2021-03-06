
#include <3ds.h>
#include <png.h>

#include "ctr_utils.h"

extern void quitGame();

#define TEX_MIN_SIZE 64
#define TEX_MAX_SIZE 1024
unsigned int nextPow2(unsigned int v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	return v >= TEX_MIN_SIZE ? v : TEX_MIN_SIZE;
}

// Taken from Anemone
bool CtrUtils::LoadImageFromFile(const char *filename, float imgWidth, float imgHeight, C3D_Tex *tex, C2D_Image *image)
{
	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	png_infop info = png_create_info_struct(png);

	if (setjmp(png_jmpbuf(png)))
	{
		png_destroy_read_struct(&png, &info, NULL);
		return false;
	}

	FILE *fp = fopen(filename, "rb");

	if (fp == NULL)
	{
		printf("Failed to load image from: %s\n", filename);
		return false;
	}

	png_init_io(png, fp);
	png_read_info(png, info);

	int width = png_get_image_width(png, info);
	int height = png_get_image_height(png, info);

	png_byte color_type = png_get_color_type(png, info);
	png_byte bit_depth = png_get_bit_depth(png, info);

	// Read any color_type into 8bit depth, ABGR format.
	// See http://www.libpng.org/pub/png/libpng-manual.txt

	if (bit_depth == 16)
		png_set_strip_16(png);

	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png);

	// PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(png);

	if (png_get_valid(png, info, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png);

	// These color_type don't have an alpha channel then fill it with 0xff.
	if (color_type == PNG_COLOR_TYPE_RGB ||
		color_type == PNG_COLOR_TYPE_GRAY ||
		color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

	if (color_type == PNG_COLOR_TYPE_GRAY ||
		color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png);

	//output ABGR
	// png_set_bgr(png); // Doesn't seem to do anything??
	png_set_swap_alpha(png);

	png_read_update_info(png, info);

	png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);
	for (int y = 0; y < height; y++)
	{
		row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png, info));
	}

	png_read_image(png, row_pointers);

	fclose(fp);
	png_destroy_read_struct(&png, &info, NULL);

	image->tex = tex;

	unsigned int texWidth = nextPow2(width);
	unsigned int texHeight = nextPow2(height);

	if (texWidth > TEX_MAX_SIZE || texHeight > TEX_MAX_SIZE)
	{
		printf("3DS only supports up to 1024x1024 textures. Reduce the image size.\n");
		printf("It does not support %dx%d\n", texWidth, texHeight);

		for (int y = 0; y < height; y++)
		{
			free(row_pointers[y]);
		}
		free(row_pointers);
		return false;
	}

	Tex3DS_SubTexture *subt3x = new Tex3DS_SubTexture();
	subt3x->width = imgWidth;
	subt3x->height = imgHeight;
	subt3x->left = 0.0f;
	subt3x->top = 1.0f;
	subt3x->right = width / (float)texWidth;
	subt3x->bottom = 1.0 - (height / (float)texHeight);
	image->subtex = subt3x;

	C3D_TexInit(image->tex, texWidth, texHeight, GPU_RGBA8);
	C3D_TexSetFilter(image->tex, GPU_LINEAR, GPU_NEAREST);

	memset(image->tex->data, 0, image->tex->size);

	for (int j = 0; j < height; j++)
	{
		png_bytep row = row_pointers[j];
		for (int i = 0; i < width; i++)
		{
			png_bytep px = &(row[i * 4]);

			// Swap the colours since png_set_bgr doesn't seem to work as expected
			// Note: 0 - B, 1 - G, 2 - R, 3 - A
			unsigned argb = (px[3]) + (px[2] << 8) + (px[1] << 16) + (px[0] << 24);

			u32 dst = ((((j >> 3) * (texWidth >> 3) + (i >> 3)) << 6) + ((i & 1) | ((j & 1) << 1) | ((i & 2) << 1) | ((j & 2) << 2) | ((i & 4) << 2) | ((j & 4) << 3))) * 4;

			memcpy((u8*)image->tex->data + dst, &argb, sizeof(u32));
		}
	}

	for (int y = 0; y < height; y++)
	{
		free(row_pointers[y]);
	}
	free(row_pointers);

	return true;
}

void CtrUtils::ScanInput()
{
	bool result = aptMainLoop();
	if(!result)
	{
		quitGame();
	}

	hidScanInput();
	svcSleepThread(1);
}
