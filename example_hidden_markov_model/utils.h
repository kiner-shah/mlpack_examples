#pragma once

#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

std::vector<std::string> read_unique_strings_from_file(fs::path path);

int binary_search(const std::vector<std::string>& v, const std::string& key);
