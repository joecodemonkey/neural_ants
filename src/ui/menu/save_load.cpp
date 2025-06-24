#include <imgui.h>
#include <raylib.h>

#include <ui/buttons.hpp>
#include <ui/menu/save_load.hpp>

#include "ui/state.hpp"
#include "util/file.hpp"

UI::Menu::SaveLoad::SaveLoad(UI::State& state) : _state(state) {}

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

  auto loadId = _textureCache->get_texture("load").id;
  auto deleteId = _textureCache->get_texture("delete").id;
  auto saveId = _textureCache->get_texture("save").id;
  auto exitId = _textureCache->get_texture("exit").id;
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
        ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, IM_COL32(150, 150, 150, 255));
        static char newSaveName[256] = "new_save.save";
        ImGui::InputText("##new_save_input", newSaveName, sizeof(newSaveName) - 1);
        std::string new_save = std::string(newSaveName);
        ImGui::TableNextColumn();
        UI::Buttons::GroupedImage("#save", "Save", saveId, buttonDim);
        ImGui::TableNextColumn();

        for (const auto& saveFile : *files) {
          ImGui::TableNextRow();
          ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, IM_COL32(200, 200, 200, 255));

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
        _state.maximize(State::SETTINGS);
        _state.minimize(State::SAVELOAD);
      }
    }
    ImGui::End();
  }
}

auto UI::Menu::SaveLoad::add_texture_cache(std::shared_ptr<TextureCache> cache) -> void {
  _textureCache = cache;
}