#include <ui/buttons.hpp>

#include "imgui.h"

bool UI::Buttons::GroupedImage(const std::string& id,
                               const std::string& label,
                               ImTextureID textureID,
                               ImVec2 imageSize) {
  ImGui::BeginGroup();

  begin_button_style();
  bool clicked = ImGui::ImageButton(id.c_str(), textureID, imageSize);

  // Center text under button
  ImVec2 textSize = ImGui::CalcTextSize(label.c_str());
  float textOffset = (imageSize.x - textSize.x) * 0.5f;
  if (textOffset > 0) {
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + textOffset);
  }
  ImGui::Text("%s", label.c_str());

  ImGui::EndGroup();
  end_button_style();

  return clicked;
}

auto UI::Buttons::begin_button_style() -> void {
  static const ImVec4 buttonColor{0.2f, 0.2f, 0.2f, 1.0f};
  //_buttonHoverColor{0.4f, 0.4f, 0.4f, 1.0f},
  //_activeButtonColor{0.3f, 0.3f,begin_button_style 0.3f, 1.0f},

  ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonColor);
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonColor);
}
auto UI::Buttons::end_button_style() -> void {
  ImGui::PopStyleColor(3);
}

auto UI::Buttons::Image(const std::string& label, ImTextureID textureID, ImVec2 imageSize) -> bool {
  if (ImGui::ImageButton(label.c_str(), textureID, imageSize)) {
    return true;
  }
  return false;
}
