#pragma once

#include <unordered_map>
#include <memory>
#include "raylib-cpp.hpp"

class TextureCache {
    public:
        static TextureCache &get_instance() {
            static TextureCache instance;
            return instance;
        }

        raylib::Texture2D & get_texture(std::string const &path) {
            if (_textures.find(path) == _textures.end()) {
                _textures[path] = LoadTexture(path.c_str());
            }
            return _textures[path];
        }

        protected:
            TextureCache() = default;
            TextureCache(TextureCache const &) = delete;
            TextureCache &operator=(TextureCache const &) = delete;
            ~TextureCache() = default;
            
            std::unordered_map<std::string, raylib::Texture2D> _textures;
};
