#ifndef LR_SUPERFLAPPYBIRDS_RENDERERER_HPP
#define LR_SUPERFLAPPYBIRDS_RENDERERER_HPP

#include <iostream>
#include <cstdint>
#include <cassert>
#include <cmath>
#include <cstring>
#include "rect.hpp"
#include "omp.h"
#include "texture.hpp"

class Renderer {
public:
    int width;
    int height;
    uint32_t* framebuffer;

    Renderer(int width, int height): width(width), height(height) {
        framebuffer = new uint32_t[width * height];
    }

    ~Renderer() {
        delete [] framebuffer;
    }

    static uint32_t rgb(uint8_t r, uint8_t g, uint8_t b) {
        uint8_t a = 255;
        return a << 24 | r << 16 | g << 8 | b;
    }

    /// Fill the screen with the specified colour.
    /// \param colour The colour to fill the screen with.
    void Clear(uint32_t colour) {
        int threads = omp_get_max_threads();
        int len = width * height;
#pragma omp parallel for
        for (int i = 0; i < threads; ++i) {
            uint32_t *start = framebuffer + len / threads * i;
            uint32_t *end = framebuffer + len / threads * (i + 1);
            std::fill(start, end, colour);
        }
    }

    void RenderSquare(uint32_t colour, Rect* dest) {
#pragma omp parallel for
        for (int y = 0; y < dest->h; ++y) {
            uint32_t *start = framebuffer + (dest->y + y) * width + dest->x;
            std::fill(start, start + dest->w, colour);
        }
    }

    /// Renders the whole image to the screen at the position and size defined in the rect.
    /// \param image The image to draw to the screen.
    /// \param dest The position and size to draw the image at.
    void Render(const Texture* image, Rect* dest);

    /// Renders the whole image to the screen at the position and size defined in the rect.
    /// \param image The image to draw to the screen.
    /// \param dest The position and size to draw the image at.
    /// \param angle The angle in degrees to rotate the image.
    void Render(const Texture* image, Rect* dest, int angle);

    /// Draws a part of an image to the screen.
    /// \param image The image to draw to the screen.
    /// \param src The subsection of the image to draw.
    /// \param dest The position and size to draw the image at.
    void Render(const Texture* image, Rect* src, Rect* dest);

    /// Draws an image to the screen.
    /// \param image The image to draw to the screen.
    /// \param src The subsection of the image to draw.
    /// \param dest The position and size to draw the image at.
    /// \param angle The angle in degrees to rotate the image.
    void Render(const Texture* image, Rect* src, Rect* dest, double angle);

    /// Renders the whole image to the screen at the position and size defined in the rect.
    /// The image's alpha is ignored and the specified opacity is used instead.
    /// \param image The image to draw to the screen.
    /// \param dest The position and size to draw the image at.
    /// \param opacity The opacity to draw the image at, should be between 0 and 1.
    void RenderForceAlpha(const Texture* image, Rect* dest, double opacity);

private:
    /// Draws the addend pixel over the augend pixel with the specified transparency.
    /// \param augend The base pixel.
    /// \param addend The top pixel.
    /// \param opacity The opacity to draw the top pixel with.
    /// \return The result of the pixel addition.
    uint32_t OverlayPixels(uint32_t augend, uint32_t addend, double opacity);

    void Render90(uint32_t *framebuffer, int width, int height, const Texture* image, Rect *src, Rect *dest);
    void Render180(uint32_t *framebuffer, int width, int height, const Texture* image, Rect *src, Rect *dest);
    void Render270(uint32_t *framebuffer, int width, int height, const Texture* image, Rect *src, Rect *dest);
};


#endif //LR_SUPERFLAPPYBIRDS_RENDERERER_HPP
