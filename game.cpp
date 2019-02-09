#include "game.hpp"

const char *Game::game_name = "Super Snakes";
const char *Game::game_version = "0.9.0";

Game::Game(unsigned int screen_width, unsigned int screen_height, std::string core_folder_path, std::string config_folder_path) {
    renderer = new Renderer(screen_width, screen_height);
}

void Game::GameLoop(double delta_time, std::vector<Input> controller_inputs) {

}

uint32_t *Game::GetFrameBuffer() {
    renderer->Clear(Renderer::rgb(255, 0, 0));
    renderer->Clear(Renderer::rgb(0, 0, 255));
    return renderer->framebuffer;
}
