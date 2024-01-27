#include <trollworks-backend-sdl.hpp>
#include <imgui/imgui.h>

#include <game/ui/menu.hpp>

namespace game::ui {
  void menu::operator()(tw::ui::hooks&) {
    if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Quit")) {
          SDL_Event event;
          SDL_memset(&event, 0, sizeof(event));

          event.type = SDL_QUIT;
          event.quit.timestamp = SDL_GetTicks();

          SDL_PushEvent(&event);
        }

        ImGui::EndMenu();
      }

      ImGui::EndMainMenuBar();
    }
  }
}
