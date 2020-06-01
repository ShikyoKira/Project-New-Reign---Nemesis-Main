#pragma once

#include <filesystem>

std::filesystem::path papyrusTempCompile();
bool PapyrusCompile(std::filesystem::path pscfile,
                    std::filesystem::path import,
                    std::filesystem::path destination,
                    std::filesystem::path filepath,
                    std::filesystem::path compiling_path,
                    std::filesystem::path compilerpath);