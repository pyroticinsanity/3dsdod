#ifndef DOD_CITRO_RENDERER_HEADER
#define DOD_CITRO_RENDERER_HEADER

#include <queue>

#include <citro2d.h>
#include "../renderer.h"

/**
 * Encapsulates all of the 3DS code for rendering graphics.
 */
class CitroRenderer : public Renderer
{
public:
    static const int ScreenHeight;
    static const int ScreenWidth;

    // Contains the information for rendering a triangle
    struct Triangle
    {
        Triangle(float X0, float Y0, float X1, float Y1, float X2, float Y2, u32 Color, Layers Layer)
            : x0(X0), y0(Y0), x1(X1), y1(Y1), x2(X2), y2(Y2), color(Color), layer(Layer){};
        float x0;
        float y0;
        float x1;
        float y1;
        float x2;
        float y2;
        u32 color;
        Layers layer;
    };

    CitroRenderer();

    virtual ~CitroRenderer() {}

    /**
     * Clears the screen with the clear color that has been set by setClearColor.
     * @param includeDepthBuffer - Unused for the 3DS
     */
    virtual void clearBuffer(bool includeDepthBuffer = false);

    /**
     * Must be called before exiting to clean up the graphics libraries
     */
    virtual void deinitialize();

    /**
     * Draws an image on screen.
     * @param img The image to draw
     */
    virtual void drawImage(Renderer::Image* img);
    
    /**
     * Draws the keyboard image onto the screen and highlights the specified key.
     * @param key - The key to highlight
     */
    virtual void drawKeyboard(struct kbdKey key);

    /**
     * Draws a line on the screen with the color that has been set by setColor.
     * @param x0 - The x coordinate for the first point.
     * @param y0 - The y coordinate for the first point.
     * @param x1 - The x coordinate for the second point.
     * @param y1 - The y coordinate for the second point.
     * @param layer - The layer to draw on (Used for stereoscopic 3D)
     */
    virtual void drawLine(float x0, float y0, float x1, float y1, Layers layer = LAYER_DEFAULT);

    /**
     * Draws a quad on the screen filled with the color that has been set by setColor.
     * @param x0 - The x coordinate for the first point.
     * @param y0 - The y coordinate for the first point.
     * @param x1 - The x coordinate for the second point.
     * @param y1 - The y coordinate for the second point.
     * @param x2 - The x coordinate for the third point.
     * @param y2 - The y coordinate for the third point.
     * @param x3 - The x coordinate for the fourth point.
     * @param y3 - The y coordinate for the fourth point.
     * @param layer - The layer to draw on (Used for stereoscopic 3D)
     */
    virtual void drawQuad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3,
                          Layers layer = LAYER_DEFAULT);

    /**
     * Draws a vector on the screen with the color that has been set by setColor based on the mode that it is
     * in. Vector mode draws a line while the other modes draw pixels.
     * @param x0 - The x coordinate for the first point.
     * @param y0 - The y coordinate for the first point.
     * @param x1 - The x coordinate for the second point.
     * @param y1 - The y coordinate for the second point.
     * @param layer - The layer to draw on (Used for stereoscopic 3D)
     */
    virtual void drawVector(float X0, float Y0, float X1, float Y1, Layers layer = LAYER_DEFAULT);

    /**
     * Returns the screen height.
     */
    virtual int getScreenHeight()
    {
        return ScreenHeight;
    }

    /**
     * Returns the screen width.
     */
    virtual int getScreenWidth()
    {
        return ScreenWidth;
    }

    /**
     * Wrapper function to get flags for SDL. Unused for the 3DS.
     */
    virtual unsigned int getVideoModeFlags()
    {
        return 0;
    }

    /**
     * Must be called before use to initialize any libraries.
     */
    virtual void initialize();

    /**
     * Plots a pixel based on the specified x and y.
     * @param X - The x coordinate for the point
     * @param Y - The y coordinate for the point
     * @param layer - The layer to draw on (Used for stereoscopic 3D)
     */
    virtual void plotPoint(double X, double Y, Layers layer = LAYER_DEFAULT);

    /**
     * Resets any translations that have been set for rendering quads.
     */
    virtual void resetMatrix();

    /**
     * Sets the clear color to be used.
     */
    virtual void setClearColor(float red, float green, float blue, float alpha = 1.0f);

    /**
     * Sets the color to be used for drawing. Wrapper function to take in an array.
     */
    virtual void setColor(float color[3]);

    /**
     * Sets the color to be used for drawing.
     */
    virtual void setColor(float red, float green, float blue, float alpha = 1.0f);

    /**
     * Sets the viewport of the camera.
     */
    virtual void setViewport(int x, int y, int width, int height);

    /**
     * Called to draw the graphics on screen.
     */
    virtual void swapBuffers();

    /**
     * Applies a translation for rendering quads.
     */
    virtual void translateMatrix(float xOffset, float yOffset);

private:

    /**
     * Returns the offset to shift the pixels by based on the layer.
     * @param layer The layer to get the offset for.
     * @param slider The stereoscopic level that the 3DS is set to
     */
    float getStereoscopicOffset(Layers layer, float slider);

    /**
     * Helper function to redraw all graphics on the right screen for stereoscopic mode.
     * It draws 
     */
    void renderRightScreen();

    /**
     * The color to clear the screen with.
     */
    u32 _clearColor;

    /**
     * The color to draw with.
     */
    u32 _color;

    /**
     * If not NULL then it tells the right screen to render the image.
     */
    C2D_Image *_currentImage;

    /**
     * The keyboard image used by drawKeyboard.
     */
    C2D_Image _keyboardImg;

    /**
     * The left screen target.
     */
    C3D_RenderTarget *_left;

    /**
     * The right screen target for 3D mode.
     */
    C3D_RenderTarget *_right;

    /**
     * The list of triangles to render on the right screen.
     */
    std::queue<Triangle> _triangles;

    /**
     * x offset for translation.
     */
    float _xOffset;

    /**
     * y offset for translation.
     */
    float _yOffset;
};
#endif // DOD_CITRO_RENDERER_HEADER