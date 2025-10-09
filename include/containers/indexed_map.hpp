#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

template <typename T>
class IndexedMap {
 public:
  // Add or overwrite
  void insert(const std::string& key, const T& value) {
    if (_key_to_index.find(key) == _key_to_index.end()) {
      _key_to_index[key] = _data.size();
      _keys.push_back(key);
      _data.push_back(value);
    } else {
      _data[_key_to_index[key]] = value;
    }
  }

  // Access by key
  T& operator[](const std::string& key) { return _data.at(_key_to_index.at(key)); }

  const T& operator[](const std::string& key) const { return _data.at(_key_to_index.at(key)); }

  // Access by index
  T& operator[](size_t index) { return _data.at(index); }

  const T& operator[](size_t index) const { return _data.at(index); }

  // Check if key exists
  bool contains(const std::string& key) const {
    return _key_to_index.find(key) != _key_to_index.end();
  }

  // Get key at index
  const std::string& key_at(size_t index) const { return _keys.at(index); }

  // Get size
  size_t size() const { return _data.size(); }

  // Iterator support
  auto begin() { return _data.begin(); }
  auto end() { return _data.end(); }
  auto begin() const { return _data.begin(); }
  auto end() const { return _data.end(); }

  auto clear() -> void {
    _data.clear();
    _keys.clear();
    _key_to_index.clear();
  }

 private:
  std::unordered_map<std::string, size_t> _key_to_index;
  std::vector<std::string> _keys;
  std::vector<T> _data;
};
