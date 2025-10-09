#include <imgui.h>

#include <string>
#pragma once
namespace UI {
class Buttons {
 public:
  static auto GroupedImage(const std::string& id,
                           const std::string& label,
                           ImTextureID textureID,
                           ImVec2 imageSize) -> bool;
  static auto Image(const std::string& label, ImTextureID textureID, ImVec2 imageSize) -> bool;

  static auto begin_button_style() -> void;
  static auto end_button_style() -> void;
};
}