#ifndef SUPERSNAKES_LIBRETRO_ASSETS_HPP
#define SUPERSNAKES_LIBRETRO_ASSETS_HPP

#include <string>
#include "engine/image.hpp"
#include "engine/rect.hpp"
#include "engine/audio.hpp"

typedef std::vector<std::pair<Rect, Rect>> text_t;

struct SnakeAssets {
    Texture *portrait;
    Texture *head;
    Texture *body_straight;
    Texture *body_turn;
    Texture *tail_straight;
    Texture *tail_turn_left;
    Texture *tail_turn_right;

    SnakeAssets(const std::string &core_folder_path, const std::string &path_portrait, const std::string &path_head, const std::string &path_body_straight,
                const std::string &path_body_turn, const std::string &path_tail_straight,
                const std::string &path_tail_turn_left) {
        portrait = Image::LoadBMP(core_folder_path + path_portrait);
        head = Image::LoadBMP(core_folder_path + path_head);
        body_straight = Image::LoadBMP(core_folder_path + path_body_straight);
        body_turn = Image::LoadBMP(core_folder_path + path_body_turn);
        tail_straight = Image::LoadBMP(core_folder_path + path_tail_straight);
        tail_turn_left = Image::LoadBMP(core_folder_path + path_tail_turn_left);
        tail_turn_right = tail_turn_left->FlipHorizontally();
    }
};

struct Assets {
public:
    std::vector<SnakeAssets*> snakes;
    Texture *raspberry;
    Texture *wall_top;
    Texture *wall_bottom;
    Texture *wall_left;
    Texture *wall_right;
    Texture *wall_top_left;
    Texture *wall_top_right;
    Texture *wall_bottom_left;
    Texture *wall_bottom_right;
    Texture *font;
    Texture *ui_title;
    Texture *ui_start_button;
    Texture *ui_arrow_left;
    Texture *ui_arrow_right;
    Sound *snd_chomp;
    Sound *snd_crash;
    std::vector<Sound*> snd_background;

    explicit Assets(const std::string &core_folder_path) {
        for (int i = 1; i <=8; ++i)
            snakes.push_back(new SnakeAssets(core_folder_path,
                                             "resources/snake_" + std::to_string(i) + "_portrait.bmp",
                                             "resources/snake_" + std::to_string(i) + "_head.bmp",
                                             "resources/snake_" + std::to_string(i) + "_body_straight.bmp",
                                             "resources/snake_" + std::to_string(i) + "_body_turn.bmp",
                                             "resources/snake_" + std::to_string(i) + "_tail_straight.bmp",
                                             "resources/snake_" + std::to_string(i) + "_tail_turn_left.bmp"
            ));
        raspberry = Image::LoadBMP(core_folder_path + "resources/raspberry.bmp");
        wall_top = Image::LoadBMP(core_folder_path + "resources/wall_top.bmp");
        wall_bottom = wall_top->Rotate180();
        wall_left = wall_top->Rotate90CCW();
        wall_right = wall_top->Rotate90CW();
        wall_top_right = Image::LoadBMP(core_folder_path + "resources/wall_top_right.bmp");
        wall_top_left = wall_top_right->Rotate90CCW();
        wall_bottom_left = wall_top_right->Rotate180();
        wall_bottom_right = wall_top_right->Rotate90CW();
        font = Image::LoadBMP(core_folder_path + "resources/font.bmp");
        ui_title = Image::LoadBMP(core_folder_path + "resources/title.bmp");
        ui_start_button = Image::LoadBMP(core_folder_path + "resources/ui_start_button.bmp");
        ui_arrow_left = Image::LoadBMP(core_folder_path + "resources/ui_arrow_left.bmp");
        ui_arrow_right = Image::LoadBMP(core_folder_path + "resources/ui_arrow_right.bmp");

        snd_chomp = Audio::LoadWav(core_folder_path + "resources/audio/chomp.wav");
        snd_crash = Audio::LoadWav(core_folder_path + "resources/audio/crash.wav");
        snd_background.push_back(Audio::LoadWav(core_folder_path + "resources/audio/background_8-Bit-Mayhem.wav"));
        snd_background.push_back(Audio::LoadWav(core_folder_path + "resources/audio/background_Arcade-Fantasy.wav"));
        snd_background.push_back(Audio::LoadWav(core_folder_path + "resources/audio/background_Automation.wav"));
        snd_background.push_back(Audio::LoadWav(core_folder_path + "resources/audio/background_Blob-Monsters-on-the-Loose.wav"));
        snd_background.push_back(Audio::LoadWav(core_folder_path + "resources/audio/background_Hypnotic-Puzzle3.wav"));
        snd_background.push_back(Audio::LoadWav(core_folder_path + "resources/audio/background_Pixel-Drama.wav"));
    }

    Rect GetFontSrcRect(char letter) {
        std::locale locale;
        letter = std::toupper(letter, locale);
        switch (letter) {
            case 'A': return {0, 0, 4, 6};
            case 'B': return {5, 0, 4, 6};
            case 'C': return {10, 0, 3, 6};
            case 'D': return {14, 0, 4, 6};
            case 'E': return {19, 0, 3, 6};
            case 'F': return {23, 0, 3, 6};
            case 'G': return {27, 0, 4, 6};
            case 'H': return {32, 0, 4, 6};
            case 'I': return {37, 0, 3, 6};
            case 'J': return {41, 0, 3, 6};
            case 'K': return {45, 0, 4, 6};
            case 'L': return {50, 0, 3, 6};
            case 'M': return {54, 0, 5, 6};
            case 'N': return {60, 0, 4, 6};
            case 'O': return {65, 0, 4, 6};
            case 'P': return {70, 0, 4, 6};
            case 'Q': return {75, 0, 4, 6};
            case 'R': return {80, 0, 4, 6};
            case 'S': return {85, 0, 4, 6};
            case 'T': return {90, 0, 3, 6};
            case 'U': return {94, 0, 4, 6};
            case 'V': return {99, 0, 3, 6};
            case 'W': return {103, 0, 5, 6};
            case 'X': return {109, 0, 3, 6};
            case 'Y': return {113, 0, 3, 6};
            case 'Z': return {117, 0, 4, 6};
            case '0': return {122, 0, 4, 6};
            case '1': return {127, 0, 3, 6};
            case '2': return {131, 0, 4, 6};
            case '3': return {136, 0, 4, 6};
            case '4': return {141, 0, 4, 6};
            case '5': return {146, 0, 4, 6};
            case '6': return {151, 0, 4, 6};
            case '7': return {156, 0, 4, 6};
            case '8': return {161, 0, 4, 6};
            case '9': return {166, 0, 4, 6};
            case ' ': return {171, 0, 4, 6};
            case '_': return {176, 0, 4, 6};
            case ':': return {181, 0, 1, 6};
            case '.': return {183, 0, 1, 6};
            case '\b': return {185, 0, 11, 12};
            case '\n': return {197, 0, 12, 12};
            case '"': return {0, 6, 3, 6};
            case '\'': return {4, 6, 1, 6};
            case ',': return {6, 6, 1, 6};
            case '/': return {8, 6, 3, 6};
            case '(': return {12, 6, 2, 6};
            case ')': return {15, 6, 2, 6};
            default:
                throw std::runtime_error("Unknown letter '" + std::to_string(letter) + "'");
        }
    }

    std::vector<Rect> GetFontSrcRect(const std::string &text) {
        std::vector<Rect> rects;
        for (char i : text) {
            rects.push_back(GetFontSrcRect(i));
        }
        return rects;
    }
};

#endif //SUPERSNAKES_LIBRETRO_ASSETS_HPP
