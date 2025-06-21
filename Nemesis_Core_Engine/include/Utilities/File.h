#pragma once

#include <filesystem>
#include <functional>

#include "Nemesis_Core_Engine/include/Utilities/Line.h"
#include "Nemesis_Core_Engine/include/Utilities/Types.h"

void GetFileLines(const std::filesystem::path& filename,
                  VecNstr& functionlines,
                  std::function<std::string(const std::string&)> selector,
                  bool emptylast = true);
void GetFileLines(const std::filesystem::path& filename,
                  VecNwstr& functionlines,
                  std::function<std::wstring(const std::wstring&)> selector,
                  bool emptylast = true);

void GetFileLines(const std::filesystem::path& filename,
                  VecStr& functionlines,
                  std::function<std::string(const std::string&)> selector,
                  bool emptylast = true);
void GetFileLines(const std::filesystem::path& filename,
                  VecWstr& functionlines,
                  std::function<std::wstring(const std::wstring&)> selector,
                  bool emptylast = true);

void GetFileLines(const std::filesystem::path& filename,
                  VecNstr& functionlines,
                  std::function<bool(std::string&)> predicament,
                  bool emptylast = true);
void GetFileLines(const std::filesystem::path& filename,
                  VecNwstr& functionlines,
                  std::function<bool(std::wstring&)> predicament,
                  bool emptylast = true);

void GetFileLines(const std::filesystem::path& filename,
                  VecStr& functionlines,
                  std::function<bool(std::string&)> predicament,
                  bool emptylast = true);
void GetFileLines(const std::filesystem::path& filename,
                  VecWstr& functionlines,
                  std::function<bool(std::wstring&)> predicament,
                  bool emptylast = true);

void GetFileLines(const std::filesystem::path& filename, VecNstr& functionlines, bool emptylast = true);
void GetFileLines(const std::filesystem::path& filename, VecNwstr& functionlines, bool emptylast = true);

void GetFileLines(const std::filesystem::path& filename, VecStr& functionlines, bool emptylast = true);
void GetFileLines(const std::filesystem::path& filename, VecWstr& functionlines, bool emptylast = true);
