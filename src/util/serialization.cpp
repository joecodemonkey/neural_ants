#include "util/serialization.hpp"

#include "nlohmann/json_fwd.hpp"
#include "raylib.h"

namespace Util {

auto rectangle_to_json(const Rectangle& rect) -> nlohmann::json {
  return {{"x", rect.x}, {"y", rect.y}, {"width", rect.width}, {"height", rect.height}};
}

auto rectangle_from_json(const nlohmann::json& json) -> Rectangle {
  return {json.at("x").get<float>(),
          json.at("y").get<float>(),
          json.at("width").get<float>(),
          json.at("height").get<float>()};
}

auto vector2_to_json(const Vector2& vec) -> nlohmann::json {
  return {{"x", vec.x}, {"y", vec.y}};
}

auto vector2_from_json(const nlohmann::json& json) -> Vector2 {
  return {json.at("x").get<float>(), json.at("y").get<float>()};
}

auto camera2d_to_json(const Camera2D& camera) -> nlohmann::json {
  return {{"offset", vector2_to_json(camera.offset)},
          {"target", vector2_to_json(camera.target)},
          {"rotation", camera.rotation},
          {"zoom", camera.zoom}};
}

auto camera2d_from_json(const nlohmann::json& j) -> Camera2D {
  return {vector2_from_json(j.at("offset")),
          vector2_from_json(j.at("target")),
          j.at("rotation").get<float>(),
          j.at("zoom").get<float>()};
}

}  // namespace Util