#pragma once

#include <nlohmann/json.hpp>

#include "raylib.h"

namespace Util {

auto rectangle_to_json(const Rectangle& rect) -> nlohmann::json;
auto rectangle_from_json(const nlohmann::json& j) -> Rectangle;
auto vector2_to_json(const Vector2& vec) -> nlohmann::json;
auto vector2_from_json(const nlohmann::json& j) -> Vector2;
auto camera2d_to_json(const Camera2D& camera) -> nlohmann::json;
auto camera2d_from_json(const nlohmann::json& j) -> Camera2D;

}  // namespace Util