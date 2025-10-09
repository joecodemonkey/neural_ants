#include <imgui.h>
#include <raylib.h>

#include <ui/buttons.hpp>
#include <ui/menu/save_load.hpp>

#include "game.hpp"
#include "ui/state.hpp"
#include "util/file.hpp"

UI::Menu::SaveLoad::SaveLoad(UI::State& state, Game& game, TextureCache& textureCache)
    : _game(game), _state(state), _textureCache(textureCache) {}

auto UI::Menu::SaveLoad::draw() -> void {
  if (!_state.is_maximized(State::SAVELOAD)) {
    return;
  }
  static const ImGuiWindowFlags saveLoadWindowFlags =
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoTitleBar;

  auto screenWidth = GetScreenWidth();
  auto screenHeight = GetScreenHeight();

  ImVec2 saveLoadWindowDimensions = {screenWidth * 0.8F, screenHeight * 0.8F};
  ImVec2 centerScreen = ImVec2{screenWidth / 2.0F, screenHeight / 2.0F};
  ImVec2 saveLoadWindowPosition = {centerScreen.x - saveLoadWindowDimensions.x / 2.0f,
                                   centerScreen.y - saveLoadWindowDimensions.y / 2.0f};

  ImGui::SetNextWindowPos(saveLoadWindowPosition);
  ImGui::SetNextWindowSize(saveLoadWindowDimensions);

  auto loadId = _textureCache.get_texture("ui_load").id;
  auto deleteId = _textureCache.get_texture("ui_delete").id;
  auto saveId = _textureCache.get_texture("ui_save").id;
  auto exitId = _textureCache.get_texture("ui_exit").id;
  auto buttonDim = ImVec2(30, 30);

  bool saveLoadMaximized = true;
  if (ImGui::Begin("Save/Load Menu", &saveLoadMaximized, saveLoadWindowFlags)) {
    auto files = Util::File::get_files(".", ".save");
    if (!files) {
      ImGui::Text("Error reading save files: %s", files.error().c_str());
    }
    ImGui::SetWindowFontScale(1.5f);  // 1.5x larger

    ImGui::Text("Save Files");
    ImGui::SetWindowFontScale(1.0f);  // Reset to normal

    // Calculate available space for the child window (80% of remaining space)
    float availableHeight = ImGui::GetContentRegionAvail().y;  // Leave space for return button
    float childHeight = availableHeight * 0.8f;

    // Begin scrolling child window with calculated height
    if (ImGui::BeginChild(
            "SaveFilesList", ImVec2(0, childHeight), true, ImGuiWindowFlags_HorizontalScrollbar)) {
      ImGui::Text("New Save");
      if (ImGui::BeginTable(
              "#saveTable",
              3,
              ImGuiTableFlags_RowBg,
              ImVec2(saveLoadWindowDimensions.x * 0.8f, saveLoadWindowDimensions.x * 0.6f))) {
        ImGui::TableSetupColumn("File Name", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Button 1", ImGuiTableColumnFlags_WidthFixed, buttonDim.x * 2.0F);
        ImGui::TableSetupColumn("Button 2", ImGuiTableColumnFlags_WidthFixed, buttonDim.x * 2.0F);
        ImGui::TableNextColumn();

        static char newSaveName[256] = "new_save.save";
        ImGui::InputText("##new_save_input", newSaveName, sizeof(newSaveName) - 1);
        std::string new_save = std::string(newSaveName);
        ImGui::TableNextColumn();
        if (UI::Buttons::GroupedImage("#save", "Save", saveId, buttonDim)) {
          auto result = _game.save_game(new_save);
          if (result) {
            _statusMessage = "Game saved successfully!";
          } else {
            _statusMessage = "Save failed: " + result.error();
          }
          _messageTimer = 3.0f;
        }
        ImGui::TableNextColumn();

        for (const auto& saveFile : *files) {
          ImGui::TableNextRow();

          ImGui::TableNextColumn();
          ImGui::Text("%s", saveFile.name.c_str());
          ImGui::TableNextColumn();
          if (UI::Buttons::GroupedImage(("##load_" + saveFile.name), "Load", loadId, buttonDim)) {
            auto result = _game.load_game(saveFile.name);
            if (result) {
              _statusMessage = "Game loaded successfully!";
            } else {
              _statusMessage = "Load failed: " + result.error();
            }
            _messageTimer = 3.0f;
          }
          ImGui::TableNextColumn();
          if (UI::Buttons::GroupedImage(
                  ("##delete_" + saveFile.name), "Delete", deleteId, buttonDim)) {
            auto result = _game.delete_save(saveFile.name);
            if (result) {
              _statusMessage = "Save file deleted!";
            } else {
              _statusMessage = "Delete failed: " + result.error();
            }
            _messageTimer = 3.0f;
          }
        }
        ImGui::EndTable();
        ImGui::EndChild();
      }

      // Display status messages
      if (_messageTimer > 0.0f) {
        ImGui::SetWindowFontScale(1.2f);
        if (_statusMessage.find("failed") != std::string::npos ||
            _statusMessage.find("failed") != std::string::npos) {
          ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "%s", _statusMessage.c_str());
        } else {
          ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "%s", _statusMessage.c_str());
        }
        ImGui::SetWindowFontScale(1.0f);
        _messageTimer -= GetFrameTime();
      }

      if (UI::Buttons::Buttons::GroupedImage("#exit", "Exit", exitId, buttonDim)) {
        _state.maximize(State::SETTINGS);
        _state.minimize(State::SAVELOAD);
      }
    }

    ImGui::End();
  }
}
