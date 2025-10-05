#include <algorithm>
#include <chrono>
#include <expected>
#include <filesystem>
#include <fstream>
#include <util/file.hpp>

#include "fmt/format.h"

auto Util::File::get_files_with_time(const std::string& directory, const std::string& extension)
    -> std::expected<std::vector<std::pair<std::string, std::filesystem::file_time_type>>,
                     std::string> {
  std::vector<std::pair<std::string, std::filesystem::file_time_type>> saveFilesWithTime;
  try {
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
      if (entry.is_regular_file() && entry.path().extension() == extension) {
        auto filename = entry.path().filename().string();
        auto modTime = entry.last_write_time();
        saveFilesWithTime.emplace_back(filename, modTime);
      }
    }

    std::sort(saveFilesWithTime.begin(), saveFilesWithTime.end(), [](const auto& a, const auto& b) {
      return a.second > b.second;
    });

    return saveFilesWithTime;
  } catch (const std::filesystem::filesystem_error& e) {
    return std::unexpected(
        fmt::format("Error [{}] reading files from directory [{}].", e.what(), directory));
  }
}
auto Util::File::get_time_string(const std::filesystem::file_time_type& time) -> std::string {
  // Convert file_time to system_clock time using duration arithmetic
  auto sysTime = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
      time - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());

  // Convert to time_t
  auto timeT = std::chrono::system_clock::to_time_t(sysTime);

  // Convert to tm struct
  std::tm tm = *std::localtime(&timeT);

  // Format as YYYY, MM, DD, HH:MM:SS using fmt
  auto timeStr = fmt::format("{:04},{:02}, {:02}, {:02}:{:02}:{:02}",
                             tm.tm_year + 1900,
                             tm.tm_mon + 1,
                             tm.tm_mday,
                             tm.tm_hour,
                             tm.tm_min,
                             tm.tm_sec);

  return timeStr;
}

auto Util::File::get_files(const std::string& directory, const std::string& extension)
    -> std::expected<std::vector<FileInfo>, std::string> {
  auto files = get_files_with_time(directory, extension);

  if (!files) {
    return std::unexpected<std::string>(files.error());
  }

  std::vector<FileInfo> fileInfoVec;
  fileInfoVec.reserve(files->size());
  for (const auto& file : *files) {
    fileInfoVec.emplace_back(FileInfo{file.first, get_time_string(file.second)});
  }
  return fileInfoVec;
}

auto Util::File::write_file(const std::string& path, const std::string& content)
    -> std::expected<void, std::string> {
  try {
    std::ofstream file(path);
    if (!file.is_open()) {
      return std::unexpected(fmt::format("Failed to open file for writing: {}", path));
    }
    file << content;
    if (file.fail()) {
      return std::unexpected(fmt::format("Failed to write to file: {}", path));
    }
    return {};
  } catch (const std::exception& e) {
    return std::unexpected(fmt::format("Error writing file {}: {}", path, e.what()));
  }
}

auto Util::File::read_file(const std::string& path) -> std::expected<std::string, std::string> {
  try {
    std::ifstream file(path);
    if (!file.is_open()) {
      return std::unexpected(fmt::format("Failed to open file for reading: {}", path));
    }
    
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    
    if (file.bad()) {
      return std::unexpected(fmt::format("Error reading file: {}", path));
    }
    
    return content;
  } catch (const std::exception& e) {
    return std::unexpected(fmt::format("Error reading file {}: {}", path, e.what()));
  }
}

auto Util::File::delete_file(const std::string& path) -> std::expected<void, std::string> {
  try {
    if (!std::filesystem::exists(path)) {
      return std::unexpected(fmt::format("File does not exist: {}", path));
    }
    
    if (!std::filesystem::remove(path)) {
      return std::unexpected(fmt::format("Failed to delete file: {}", path));
    }
    
    return {};
  } catch (const std::filesystem::filesystem_error& e) {
    return std::unexpected(fmt::format("Filesystem error deleting {}: {}", path, e.what()));
  } catch (const std::exception& e) {
    return std::unexpected(fmt::format("Error deleting file {}: {}", path, e.what()));
  }
}
