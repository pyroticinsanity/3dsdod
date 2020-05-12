#ifndef MANUAL_HEADER
#define MANUAL_HEADER

#include <3ds.h>

#include <citro3d.h>
#include <citro2d.h>

/**
 * This class abstracts management of reading a manual from a directory of PNG images.
 */
class Manual
{
public:
    /**
     * @param path The location to look for PNG files
     */
    Manual(std::string path);

    virtual ~Manual()
    {
    }

    void close();

    void drawPage(C3D_RenderTarget *target);

    void nextPage();

    void open();

    void previousPage();

    void scrollDown();

    void scrollUp();

private:

    /**
     * Reads in the list of pages from the specified directory.
     * @return true if it had permission to read the files otherwise false
     */
    bool getManualPages();

    /**
     * The current page that it is on
     */
    int _currentPage;

    C3D_Tex _tex;
    C2D_Image _img;

    /**
     * The list of pages
     */
    std::vector<std::string> _pages;
    
    /**
     * The location of the images to render.
     */
    std::string _path;

    int _scale;
    int _scroll;
};

#endif // MANUAL_HEADER