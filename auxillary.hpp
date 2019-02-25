#ifndef SUPERSNAKES_LIBRETRO_AUXILLARY_HPP
#define SUPERSNAKES_LIBRETRO_AUXILLARY_HPP

#include <vector>
#include "engine/rect.hpp"

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Auxiliary {
public:
    /// Gets the frames for each character to be drawn.
    /// \param src_rects
    /// \param x
    /// \param y
    /// \param pixels_between_characters
    /// \param scale
    /// \return
    static std::vector<std::pair<Rect, Rect>> getFontRects(const std::vector<Rect> &src_rects, int x, int y, int pixels_between_characters, float scale = 1) {
        int x_offset = 0;
        std::vector<std::pair<Rect, Rect>> positions;
        for (auto &src_rect : src_rects) {
            Rect dest_rect;
            dest_rect.x = x + x_offset;
            dest_rect.y = y;
            dest_rect.w = (int)(src_rect.w * scale);
            dest_rect.h = (int)(src_rect.h * scale);
            positions.emplace_back(src_rect, dest_rect);
            x_offset += dest_rect.w + pixels_between_characters;
        }
        return positions;
    }

    /// Moves a position in a direction.
    /// \param pos The position to move.
    /// \param move The direction to move it in.
    /// \return The translated position.
    static Pos ApplyDirection(Pos pos, Direction move) {
        Pos next = pos;
        switch (move) {
            case UP:
                --next.y;
                break;
            case DOWN:
                ++next.y;
                break;
            case LEFT:
                --next.x;
                break;
            case RIGHT:
                ++next.x;
                break;
        }
        return next;
    }

    /// Returns the direction that is opposite to the one provided.
    /// \param direction The direction to find the opposite of.
    /// \return The opposite direction.
    static Direction OppositionDirection(Direction direction) {
        switch (direction) {
            case UP:
                return DOWN;
            case DOWN:
                return LEFT;
            case LEFT:
                return RIGHT;
            case RIGHT:
                return LEFT;
        }
        throw std::runtime_error("Invalid direction provided.");
    }
};

#endif //SUPERSNAKES_LIBRETRO_AUXILLARY_HPP
