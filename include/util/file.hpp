#include <expected>
#include <filesystem>
#include <string>
#include <vector>

#pragma once

namespace Util {

class File {
 public:
  struct FileInfo {
    std::string name;
    std::string time;
  };

  static auto get_files(const std::string& directory, const std::string& extension)
      -> std::expected<std::vector<FileInfo>, std::string>;
  static auto get_time_string(const std::filesystem::file_time_type& time) -> std::string;
  static auto get_files_with_time(const std::string& directory, const std::string& extension)
      -> std::expected<std::vector<std::pair<std::string, std::filesystem::file_time_type>>,
                       std::string>;

  static auto write_file(const std::string& path, const std::string& content)
      -> std::expected<void, std::string>;
  static auto read_file(const std::string& path) -> std::expected<std::string, std::string>;
  static auto delete_file(const std::string& path) -> std::expected<void, std::string>;
};
};  // namespace Util