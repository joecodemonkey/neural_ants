#include <imgui.h>
#include <raylib.h>

#include <ui/buttons.hpp>
#include <ui/menu/save_load.hpp>

#include "util/file.hpp"

auto UI::Menu::SaveLoad::maximizer() -> UI::Behaviors::Maximizable& {
  return _maximizer;
}
auto UI::Menu::SaveLoad::maximizer() const -> const UI::Behaviors::Maximizable& {
  return _maximizer;
}

auto UI::Menu::SaveLoad::settingsMaximizer() -> UI::Behaviors::Maximizable& {
  return _settingsMaximizer;
}
auto UI::Menu::SaveLoad::settingsMaximizer() const -> const UI::Behaviors::Maximizable& {
  return _settingsMaximizer;
}

auto UI::Menu::SaveLoad::draw() -> void {
  if (!_maximizer.maximized()) {
    return;
  }

  static const ImGuiWindowFlags saveLoadWindowFlags =
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoTitleBar;

  const auto screenWidth = GetScreenWidth();
  const auto screenHeight = GetScreenHeight();

  const ImVec2 saveLoadWindowDimensions = {screenWidth * 0.8F, screenHeight * 0.8F};
  const ImVec2 centerScreen = ImVec2{screenWidth / 2.0F, screenHeight / 2.0F};
  const ImVec2 saveLoadWindowPosition = {centerScreen.x - saveLoadWindowDimensions.x / 2.0f,
                                         centerScreen.y - saveLoadWindowDimensions.x / 2.0f};

  ImGui::SetNextWindowPos(saveLoadWindowPosition);
  ImGui::SetNextWindowSize(saveLoadWindowDimensions);

  const auto loadId = _textureCache->get_texture("load").id;
  const auto deleteId = _textureCache->get_texture("delete").id;
  const auto saveId = _textureCache->get_texture("save").id;
  const auto exitId = _textureCache->get_texture("exit").id;
  const auto buttonDim = ImVec2(30, 30);

  bool saveLoadMaximized = true;
  if (ImGui::Begin("Save/Load Menu", &saveLoadMaximized, saveLoadWindowFlags)) {
    // Get list of .save files in current directory

    auto files = Util::File::get_files(".", ".save");
    if (!files) {
      ImGui::Text("Error reading save files: %s", files.error().c_str());
    }

    ImGui::SetWindowFontScale(1.5f);  // 1.5x larger
    ImGui::Text("Save Files");
    ImGui::SetWindowFontScale(1.0f);  // Reset to normal
    // Draw the table header

    ImGui::Separator();
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
              0,
              ImVec2(saveLoadWindowDimensions.x * 0.8f, saveLoadWindowDimensions.x * 0.6f))) {
        ImGui::TableNextColumn();

        static char newSaveName[256] = "new_save.save";
        ImGui::InputText("##new_save_input", newSaveName, sizeof(newSaveName) - 1);
        std::string new_save = std::string(newSaveName);
        ImGui::TableNextColumn();
        UI::Buttons::Image("#save", saveId, buttonDim);
        ImGui::TableNextColumn();
        for (const auto& saveFile : *files) {
          ImGui::TableNextColumn();
          ImGui::Text("%s", saveFile.name.c_str());
          ImGui::TableNextColumn();
          UI::Buttons::GroupedImage(("##load_" + saveFile.name), "Load", loadId, buttonDim);
          ImGui::TableNextColumn();
          UI::Buttons::GroupedImage(("##delete_" + saveFile.name), "Delete", deleteId, buttonDim);
        }
        ImGui::EndTable();
        ImGui::EndChild();
      }
      if (UI::Buttons::Buttons::GroupedImage("#exit", "Exit", exitId, buttonDim)) {
        _maximizer.minimize();
        _settingsMaximizer.maximize();
      }
    }
    ImGui::End();
  }
}

auto UI::Menu::SaveLoad::add_texture_cache(std::shared_ptr<TextureCache> cache) -> void {
  _textureCache = cache;
}