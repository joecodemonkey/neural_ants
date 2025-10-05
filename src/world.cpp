#include <raylib.h>

#include <population.hpp>
#include <resources.hpp>
#include <util/serialization.hpp>
#include <world.hpp>

World::World() : _resources(*this), _population(*this) {
  _bounds = World::DEFAULT_BOUNDS;
  _spawnMargin = DEFAULT_SPAWN_MARGIN;
  update_spawn_rect();
}

World::World(const nlohmann::json& j) : _resources(*this), _population(*this) {
  _bounds = Util::rectangle_from_json(j.at("bounds"));
  _spawnBounds = Util::rectangle_from_json(j.at("spawn_bounds"));
  _spawnMargin = j.at("spawn_margin").get<float>();

  _resources = Resources(j.at("resources"), *this);
  _population = Population(j.at("population"), *this);
}

// Copy constructor
World::World(const World& other) : _resources(*this), _population(*this) {
  _bounds = other._bounds;
  _spawnBounds = other._spawnBounds;
  _spawnMargin = other._spawnMargin;

  // Reconstruct resources and population with the new world reference
  _resources = Resources(other._resources);
  _population = Population(other._population);
}

// Move constructor
World::World(World&& other) noexcept : _resources(*this), _population(*this) {
  _bounds = other._bounds;
  _spawnBounds = other._spawnBounds;
  _spawnMargin = other._spawnMargin;

  // Move resources and population
  _resources = std::move(other._resources);
  _population = std::move(other._population);

  // Reset other's members
  other._bounds = DEFAULT_BOUNDS;
  other._spawnBounds = {0.0f, 0.0f, 0.0f, 0.0f};
  other._spawnMargin = DEFAULT_SPAWN_MARGIN;
}

// Copy assignment operator
World& World::operator=(const World& other) {
  if (this != &other) {
    _bounds = other._bounds;
    _spawnBounds = other._spawnBounds;
    _spawnMargin = other._spawnMargin;

    // Reconstruct resources and population with the new world reference
    _resources = Resources(other._resources);
    _population = Population(other._population);
  }
  return *this;
}

// Move assignment operator
World& World::operator=(World&& other) noexcept {
  if (this != &other) {
    _bounds = other._bounds;
    _spawnBounds = other._spawnBounds;
    _spawnMargin = other._spawnMargin;

    // Move resources and population
    _resources = std::move(other._resources);
    _population = std::move(other._population);

    // Reset other's members
    other._bounds = DEFAULT_BOUNDS;
    other._spawnBounds = {0.0f, 0.0f, 0.0f, 0.0f};
    other._spawnMargin = DEFAULT_SPAWN_MARGIN;
  }
  return *this;
}

// Equality operator
bool World::operator==(const World& other) const {
  return _bounds.x == other._bounds.x && _bounds.y == other._bounds.y &&
         _bounds.width == other._bounds.width && _bounds.height == other._bounds.height &&
         _spawnBounds.x == other._spawnBounds.x && _spawnBounds.y == other._spawnBounds.y &&
         _spawnBounds.width == other._spawnBounds.width &&
         _spawnBounds.height == other._spawnBounds.height && _spawnMargin == other._spawnMargin &&
         _resources == other._resources && _population == other._population;
}

auto World::get_bounds() const -> const Rectangle& {
  return _bounds;
}

auto World::get_population() const -> const Population& {
  return _population;
}

auto World::get_population() -> Population& {
  return _population;
}

auto World::get_resources() -> Resources& {
  return _resources;
}

auto World::get_resources() const -> const Resources& {
  return _resources;
}

auto World::get_spawn_bounds() const -> const Rectangle& {
  return _spawnBounds;
}

auto World::set_spawn_margin(float margin) -> void {
  _spawnMargin = margin;
  update_spawn_rect();
}

auto World::get_spawn_margin() const -> float {
  return _spawnMargin;
}

auto World::update_spawn_rect() -> void {
  float margin = _bounds.width * _spawnMargin;
  _spawnBounds = {_bounds.x + margin,
                  _bounds.y + margin,
                  _bounds.width - (2 * margin),
                  _bounds.height - (2 * margin)};
}

auto World::update(float time) -> void {
  _resources.update(time);
  _resources.feed_ants(_population);
  _population.update(time);
}

auto World::draw() -> void {
  DrawRectangle(_bounds.x, _bounds.y, _bounds.width, _bounds.height, WHITE);
  _resources.draw();
}

auto World::out_of_bounds(const Vector2& position) const -> bool {
  return !CheckCollisionPointRec(position, _bounds);
}

[[nodiscard]] auto World::spawn_position(const Vector2& dimensions) const -> Vector2 {
  float margin = dimensions.x;
  auto x = GetRandomValue(_bounds.x + margin, _bounds.x + _bounds.width - dimensions.x - margin);
  auto y = GetRandomValue(_bounds.y + margin, _bounds.y + _bounds.height - dimensions.y - margin);
  return Vector2{static_cast<float>(x), static_cast<float>(y)};
}

auto World::to_json() const -> nlohmann::json {
  nlohmann::json j;

  j["bounds"] = Util::rectangle_to_json(_bounds);
  j["spawn_bounds"] = Util::rectangle_to_json(_spawnBounds);
  j["spawn_margin"] = _spawnMargin;
  j["resources"] = _resources.to_json();
  j["population"] = _population.to_json();

  return j;
}

auto World::set_texture_cache(std::shared_ptr<TextureCache> cache) -> void {
  _textureCache = cache;
}

auto World::get_texture_cache() -> std::shared_ptr<TextureCache>& {
  return _textureCache;
}
