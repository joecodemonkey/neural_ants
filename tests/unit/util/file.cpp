#include "util/file.hpp"

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <filesystem>
#include <fstream>
#include <string>
#include <thread>
#include <vector>

#include "catch2/matchers/catch_matchers.hpp"

TEST_CASE("File Utility Functions", "[file]") {
  // Create a temporary test directory
  const std::string test_dir = "test_file_utils";
  const std::string test_file = test_dir + "/test.txt";
  const std::string test_file2 = test_dir + "/test2.txt";
  const std::string test_file3 = test_dir + "/test3.dat";
  const std::string nonexistent_file = test_dir + "/nonexistent.txt";
  const std::string nonexistent_dir = "nonexistent_dir/test.txt";
  const std::string test_content = "Hello, World!\nThis is a test file.";
  const std::string empty_content = "";
  const std::string large_content(10000, 'A');
  const std::string special_char_content = "Special chars: \t\n\r\0\x01\x02";

  auto read_test_file = [&](const std::string& filename) -> std::string {
    std::ifstream file(filename);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
  };

  /*
  auto write_test_file = [&]() {
    std::ofstream file(test_file);
    file << test_content;
    file.close();
  };
  */

  auto write_test_file = [&](const std::string& fileName, const std::string content) {
    std::ofstream file(fileName);
    file << content;
    file.close();
  };

  // Setup and teardown
  auto setup = [&]() { std::filesystem::create_directories(test_dir); };

  auto teardown = [&]() {
    if (std::filesystem::exists(test_dir)) {
      std::filesystem::remove_all(test_dir);
    }
  };

  SECTION("write_file function") {
    setup();

    SECTION("successful write") {
      auto result = Util::File::write_file(test_file, test_content);

      REQUIRE(result.has_value());
      REQUIRE(std::filesystem::exists(test_file));
      REQUIRE(test_content == read_test_file(test_file));
    }

    SECTION("write to non-existent directory") {
      auto result = Util::File::write_file(nonexistent_dir, test_content);

      REQUIRE_FALSE(result.has_value());
      REQUIRE_THAT(result.error(),
                   Catch::Matchers::ContainsSubstring("Failed to open file for writing"));
    }

    SECTION("write empty content") {
      auto result = Util::File::write_file(test_file, empty_content);

      REQUIRE(result.has_value());
      REQUIRE(std::filesystem::exists(test_file));
      REQUIRE(read_test_file(test_file) == empty_content);
    }

    SECTION("write large content") {
      auto result = Util::File::write_file(test_file, large_content);

      REQUIRE(result.has_value());
      REQUIRE(std::filesystem::exists(test_file));
      REQUIRE(read_test_file(test_file) == large_content);
    }

    teardown();
  }

  SECTION("read_file function") {
    setup();

    SECTION("successful read") {
      write_test_file(test_file, test_content);
      auto result = Util::File::read_file(test_file);

      REQUIRE(result.has_value());
      REQUIRE(*result == test_content);
    }

    SECTION("read non-existent file") {
      auto result = Util::File::read_file(nonexistent_file);

      REQUIRE_FALSE(result.has_value());
      REQUIRE_THAT(result.error(),
                   Catch::Matchers::ContainsSubstring("Failed to open file for reading"));
    }

    SECTION("read empty file") {
      write_test_file(test_file, "");
      auto result = Util::File::read_file(test_file);

      REQUIRE(result.has_value());
      REQUIRE(*result == "");
    }

    SECTION("read file with special characters") {
      write_test_file(test_file, special_char_content);
      auto result = Util::File::read_file(test_file);

      REQUIRE(result.has_value());
      REQUIRE(*result == special_char_content);
    }

    SECTION("read from non-existent directory") {
      auto result = Util::File::read_file(nonexistent_dir);

      REQUIRE_FALSE(result.has_value());
      REQUIRE_THAT(result.error(),
                   Catch::Matchers::ContainsSubstring("Failed to open file for reading"));
    }

    teardown();
  }

  SECTION("delete_file function") {
    setup();

    SECTION("successful delete") {
      write_test_file(test_file, test_content);

      REQUIRE(std::filesystem::exists(test_file));

      auto result = Util::File::delete_file(test_file);

      REQUIRE(result.has_value());
      REQUIRE_FALSE(std::filesystem::exists(test_file));
    }

    SECTION("delete non-existent file") {
      auto result = Util::File::delete_file(nonexistent_file);

      REQUIRE_FALSE(result.has_value());
      REQUIRE_THAT(result.error(), Catch::Matchers::ContainsSubstring("File does not exist"));
    }

    SECTION("delete file in non-existent directory") {
      auto result = Util::File::delete_file(nonexistent_dir);

      REQUIRE_FALSE(result.has_value());
      REQUIRE_THAT(result.error(), Catch::Matchers::ContainsSubstring("File does not exist"));
    }

    teardown();
  }

  SECTION("get_files_with_time function") {
    setup();

    SECTION("get files with specific extension") {
      // Create test files with different extensions
      write_test_file(test_file, "content1");
      write_test_file(test_file2, "content2");
      write_test_file(test_file3, "content3");

      auto result = Util::File::get_files_with_time(test_dir, ".txt");

      REQUIRE(result.has_value());
      REQUIRE(result->size() == 2);

      // Check that we got the .txt files
      std::vector<std::string> filenames;
      for (const auto& pair : *result) {
        filenames.push_back(pair.first);
      }

      REQUIRE(std::find(filenames.begin(), filenames.end(), "test.txt") != filenames.end());
      REQUIRE(std::find(filenames.begin(), filenames.end(), "test2.txt") != filenames.end());
      REQUIRE(std::find(filenames.begin(), filenames.end(), "test3.dat") == filenames.end());
    }

    SECTION("get files from non-existent directory") {
      auto result = Util::File::get_files_with_time("nonexistent_dir", ".txt");

      REQUIRE_FALSE(result.has_value());
      REQUIRE_THAT(result.error(), Catch::Matchers::ContainsSubstring("Error"));
    }

    SECTION("get files with no matching extension") {
      // Create only .dat files
      write_test_file(test_file3, test_content);

      auto result = Util::File::get_files_with_time(test_dir, ".txt");

      REQUIRE(result.has_value());
      REQUIRE(result->empty());
    }

    SECTION("get files from empty directory") {
      auto result = Util::File::get_files_with_time(test_dir, ".txt");

      REQUIRE(result.has_value());
      REQUIRE(result->empty());
    }

    SECTION("files are sorted by modification time (newest first)") {
      write_test_file(test_file, test_content);

      std::this_thread::sleep_for(std::chrono::milliseconds(10));

      write_test_file(test_file2, test_content);

      auto result = Util::File::get_files_with_time(test_dir, ".txt");

      REQUIRE(result.has_value());
      REQUIRE(result->size() == 2);

      // Check that files are sorted by modification time (newest first)
      REQUIRE(result->at(0).second > result->at(1).second);
    }

    teardown();
  }

  SECTION("get_files function") {
    setup();

    SECTION("get files with time strings") {
      write_test_file(test_file, test_content);
      write_test_file(test_file2, test_content);

      auto result = Util::File::get_files(test_dir, ".txt");

      REQUIRE(result.has_value());
      REQUIRE(result->size() == 2);

      // Check that we have FileInfo objects with name and time
      for (const auto& file_info : *result) {
        REQUIRE_FALSE(file_info.name.empty());
        REQUIRE_FALSE(file_info.time.empty());

        // Check time format: YYYY,MM, DD, HH:MM:SS
        REQUIRE_THAT(file_info.time,
                     Catch::Matchers::Matches(R"(\d{4},\d{2}, \d{2}, \d{2}:\d{2}:\d{2})"));
      }
    }

    SECTION("get files from non-existent directory") {
      auto result = Util::File::get_files("nonexistent_dir", ".txt");

      REQUIRE_FALSE(result.has_value());
      REQUIRE_THAT(result.error(), Catch::Matchers::ContainsSubstring("Error"));
    }

    SECTION("get files with no matching extension") {
      write_test_file(test_file3, test_content);

      auto result = Util::File::get_files(test_dir, ".txt");

      REQUIRE(result.has_value());
      REQUIRE(result->empty());
    }

    teardown();
  }

  SECTION("get_time_string function") {
    SECTION("format current time") {
      auto now = std::filesystem::file_time_type::clock::now();
      auto time_str = Util::File::get_time_string(now);

      // Check format: YYYY,MM, DD, HH:MM:SS
      REQUIRE_THAT(time_str, Catch::Matchers::Matches(R"(\d{4},\d{2}, \d{2}, \d{2}:\d{2}:\d{2})"));

      // Parse the time string to verify it's reasonable
      int year, month, day, hour, minute, second;
      char comma1, comma2, comma3, colon1, colon2;

      std::istringstream iss(time_str);
      iss >> year >> comma1 >> month >> comma2 >> day >> comma3 >> hour >> colon1 >> minute >>
          colon2 >> second;

      REQUIRE(year >= 2024);  // Not in the past
      REQUIRE((month >= 1 && month <= 12));
      REQUIRE((day >= 1 && day <= 31));
      REQUIRE((hour >= 0 && hour <= 23));
      REQUIRE((minute >= 0 && minute <= 59));
      REQUIRE((second >= 0 && second <= 59));
    }

    SECTION("format specific time") {
      // Create a specific time point
      auto time_point =
          std::chrono::system_clock::from_time_t(1640995200);  // 2022-01-01 00:00:00 UTC
      // Convert to file_time using duration arithmetic
      auto file_time = std::filesystem::file_time_type(
          time_point.time_since_epoch() - std::chrono::system_clock::now().time_since_epoch() + 
          std::filesystem::file_time_type::clock::now().time_since_epoch());

      auto time_str = Util::File::get_time_string(file_time);

      // The exact format depends on timezone, so just check it's a valid time format
      REQUIRE_THAT(time_str, Catch::Matchers::Matches(R"(\d{4},\d{2}, \d{2}, \d{2}:\d{2}:\d{2})"));

      // Parse the year to verify it's reasonable (could be 2021 or 2022 due to timezone)
      int year;
      char comma1;
      std::istringstream iss(time_str);
      iss >> year >> comma1;
      REQUIRE((year >= 2021 && year <= 2022));  // Allow for timezone differences
    }
  }

  SECTION("integration tests") {
    setup();

    SECTION("write, read, and delete cycle") {
      // Write file
      auto write_result = Util::File::write_file(test_file, test_content);
      REQUIRE(write_result.has_value());
      REQUIRE(std::filesystem::exists(test_file));

      // Read file
      auto read_result = Util::File::read_file(test_file);
      REQUIRE(read_result.has_value());
      REQUIRE(*read_result == test_content);

      // Delete file
      auto delete_result = Util::File::delete_file(test_file);
      REQUIRE(delete_result.has_value());
      REQUIRE_FALSE(std::filesystem::exists(test_file));

      // Try to read deleted file
      auto read_after_delete = Util::File::read_file(test_file);
      REQUIRE_FALSE(read_after_delete.has_value());
    }

    SECTION("multiple files with different extensions") {
      write_test_file(test_file, test_content);
      write_test_file(test_file2, test_content);
      write_test_file(test_file3, test_content);

      // Get .txt files
      auto txt_files = Util::File::get_files(test_dir, ".txt");
      REQUIRE(txt_files.has_value());
      REQUIRE(txt_files->size() == 2);

      // Get .dat files
      auto dat_files = Util::File::get_files(test_dir, ".dat");
      REQUIRE(dat_files.has_value());
      REQUIRE(dat_files->size() == 1);

      // Get .xyz files (nonexistent extension)
      auto xyz_files = Util::File::get_files(test_dir, ".xyz");
      REQUIRE(xyz_files.has_value());
      REQUIRE(xyz_files->empty());
    }

    teardown();
  }
}