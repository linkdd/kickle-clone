#include <trollworks-backend-sdl.hpp>
#include <imgui/imgui.h>

#include <game/ui/components/menu.hpp>
#include <game/messages.hpp>
#include <game/state.hpp>

namespace game::ui {
  void menu::operator()(tw::ui::hooks&) {
    ImGui::Begin(
      "Menu",
      nullptr,
      ImGuiWindowFlags_NoTitleBar |
      ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoCollapse
    );

    ImGui::SetWindowPos({ 64, 128 });
    ImGui::SetWindowSize({ 1024 - 64 * 2, 768 - 128 * 2 });
    ImGui::SetWindowFontScale(2.0f);

    auto gs = tw::scene_manager::main().registry().ctx().get<state::global*>();

    auto region = ImGui::GetContentRegionAvail();

    for (auto& name : gs->level_names) {
      if (ImGui::Button(name.c_str(), {region.x, 0})) {
        tw::message_bus::main().enqueue<messages::load_level>(messages::load_level{
          .level_id = entt::hashed_string{name.c_str()}
        });
      }
    }

    ImGui::End();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
    ImGui::Begin(
      "Quit",
      nullptr,
      ImGuiWindowFlags_NoTitleBar |
      ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoCollapse
    );
    ImGui::PopStyleVar();

    ImGui::SetWindowPos({ 64, 768 - 64 - 32 });
    ImGui::SetWindowSize({ 1024 - 64 * 2, 64 });
    ImGui::SetWindowFontScale(2.0f);

    region = ImGui::GetContentRegionAvail();

    if (ImGui::Button("Quit", region)) {
      tw::message_bus::main().enqueue<messages::quit_game>(messages::quit_game{});
    }

    ImGui::End();
  }
}
