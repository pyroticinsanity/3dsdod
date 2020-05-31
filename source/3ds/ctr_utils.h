#ifndef DOD_CTR_UTILS_HEADER
#define DOD_CTR_UTILS_HEADER

#include <citro3d.h>
#include <citro2d.h>

namespace CtrUtils
{
	/**
	 * Loads a PNG file from the specified path.
	 * @param filename The absolute path to the file
	 * @param imgWidth The width to set it to
	 * @param imgHeight The height to set it to
	 * @param tex - The texture to load it into
	 * @param image - The corresponding image to generate
	 * @return true if successful otherwise false
	 */
	bool LoadImageFromFile(const char *filename, float imgWidth, float imgHeight, C3D_Tex *tex, C2D_Image *image);

	/**
	 * Wrapper function for scanning the input on the 3DS and management for the home button
	 */
	void ScanInput();
}

#endif