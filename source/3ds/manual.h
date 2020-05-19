#ifndef MANUAL_HEADER
#define MANUAL_HEADER

#include <string>
#include <vector>

#include <citro3d.h>
#include <citro2d.h>

#include "citro_renderer.h"

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
        close();
    }

    /**
     * Cleans up the current page.
     */
    void close();

    /**
     * Draws the page to the given target.
     * @param renderer The target to render the image on.
     */
    void draw(CitroRenderer* renderer);

    /**
     * Returns the current image to be rendered.
     */
    Renderer::Image* getCurrentImage()
    {
        return &_currentImage;
    }

    /**
     * @return true if the manual was successfully loaded otherwise false.
     */
    bool isInitialized()
    {
        return _initialized;
    }

    /**
     * Go to the next page. If it's at the last page, do nothing.
     */
    void nextPage();

    /**
     * Initializes the manual to the first page.
     */
    void open();

    /**
     * Go to the previous page. If it's at the first page, do nothing.
     */
    void previousPage();

    /**
     * Move the page up in the view until the bottom of the page is at the bottom of the screen.
     */
    void scrollDown();

    /**
     * Move the page down in the view until the top of the page is at the top of the screen.
     */
    void scrollUp();

private:

    /**
     * Reads in the list of pages from the specified directory.
     * @return true if it had permission to read the files otherwise false
     */
    bool getManualPages();

    /**
     * Loads the image into the currentImage.
     * @param filename The name of the file to load.
     * @return true if successfully loaded otherwise false
     */
    bool loadImage(std::string filename);

    /**
     * The current image to draw.
     */
    Renderer::Image _currentImage;

    /**
     * The current page that it is on.
     */
    unsigned int _currentPage;

    /**
     * Indicatea if it is ready to be used.
     */
    bool _initialized;

    /**
     * Stores the current texture.
     */
    C3D_Tex _tex;

    /**
     * Stores the current image to render.
     */
    C2D_Image _img;

    /**
     * The list of pages.
     */
    std::vector<std::string> _pages;

    /**
     * The location of the images to render.
     */
    std::string _path;

    /**
     * The current scale to render the image at.
     */
    int _scale;

    /**
     * The current scroll position.
     */
    int _scroll;
};

#endif // MANUAL_HEADER