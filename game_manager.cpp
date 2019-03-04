#include <utility>

#include "game_manager.hpp"
#include "contexts/context_menu.hpp"

const char *GameManager::game_name = "Super Snakes";
const char *GameManager::game_version = "0.9.0";

GameManager::GameManager(unsigned int screen_width, unsigned int screen_height, std::string core_folder_path, std::string config_folder_path) {
    assets = new Assets(core_folder_path);
    save_data = new SaveData(std::move(config_folder_path));
    save_data->Deserialise();
    renderer = new Renderer(screen_width, screen_height);
    RefreshLevels(core_folder_path);

    PushNewContext(new ContextMenu(this, &mixer, save_data, screen_width, screen_height, assets, &levels, 8));
}

void GameManager::GameLoop(double delta_time, std::vector<Input> controller_inputs) {
    contexts.front()->Update(delta_time, std::move(controller_inputs));
}

uint32_t *GameManager::GetFrameBuffer() {
    contexts.front()->Render(renderer);
    return renderer->framebuffer;
}

void GameManager::PushNewContext(Context *context) {
    std::cout << "Changing context to: \"" << context->Name() << "\"." << std::endl;
    contexts.push_front(context);
}

void GameManager::EndContext(Context *context) {
    for (int i = 0; i < contexts.size(); ++i) {
        if (contexts[i] == context) {
            std::cout << "Ending selected context: \"" << context->Name() << "\"." << std::endl;
            delete context;
            contexts.erase(contexts.begin() + i);
            return;
        }
    }
}

void GameManager::RefreshLevels(std::string level_folder_path) {
    levels = {};
    levels.emplace_back(level_folder_path + "levels/level_0_open.tmx", assets);
    levels.emplace_back(level_folder_path + "levels/level_1_box.tmx", assets);
    levels.emplace_back(level_folder_path + "levels/level_2_opening.tmx", assets);
    levels.emplace_back(level_folder_path + "levels/level_3_small.tmx", assets);
//    levels.emplace_back(level_folder_path + "levels/level_Z_heart.tmx", assets);
}
