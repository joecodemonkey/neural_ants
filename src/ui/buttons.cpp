#include <ui/buttons.hpp>

#include "imgui.h"

bool UI::Buttons::GroupedImage(const std::string& id,
                               const std::string& label,
                               ImTextureID textureID,
                               ImVec2 imageSize) {
  ImGui::BeginGroup();

  bool clicked = Image(id, textureID, imageSize);

  // Center text under button
  ImVec2 textSize = ImGui::CalcTextSize(label.c_str());
  float textOffset = (imageSize.x - textSize.x) * 0.5f;
  if (textOffset > 0) {
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + textOffset);
  }
  ImGui::Text("%s", label.c_str());

  ImGui::EndGroup();

  return clicked;
}

auto UI::Buttons::begin_button_style() -> void {
  static const ImVec4 buttonColor{0.2f, 0.2f, 0.2f, 1.0f};
  static const ImVec4 buttonHoverColor{0.3f, 0.3f, 0.3f, 1.0f};
  static const ImVec4 activeButtonColor{0.1f, 0.1f, 0.1f, 1.0f};

  ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonHoverColor);
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeButtonColor);
}
auto UI::Buttons::end_button_style() -> void {
  ImGui::PopStyleColor(3);
}

auto UI::Buttons::Image(const std::string& label, ImTextureID textureID, ImVec2 imageSize) -> bool {
  begin_button_style();
  if (ImGui::ImageButton(label.c_str(), textureID, imageSize)) {
    end_button_style();
    return true;
  }
  end_button_style();
  return false;
}
