#include <fstream>

#include "Core/FNISaaPexFile.h"

#include "Logger.h"
#include "NemesisInfo.h"


nemesis::FNISaaPexFile::FNISaaPexFile(const std::filesystem::path& pex_filepath)
    : FilePath(pex_filepath)
{
    Logger::Log(L"Reading FNISaaPexFile: " + pex_filepath.wstring());

    std::ifstream pex_file(pex_filepath, std::ios::binary | std::ios::ate);

    if (!pex_file) throw std::runtime_error("Pex file cannot be found (File: " + pex_filepath.string() + ")");

    std::streamsize size = pex_file.tellg();
    pex_file.seekg(0, std::ios::beg);

    Buffer = std::vector<char>(size);

    if (!pex_file.read(Buffer.data(), size))
    {
        throw std::runtime_error("Pex file cannot be read (File: " + pex_filepath.string() + ")");
    }

    pex_file.close();

    const char target[] = "crc_value";
    TargetIterator      = std::search(Buffer.begin(), Buffer.end(), std::begin(target), std::end(target) - 1);

    if (TargetIterator != Buffer.end()) return;

    throw std::runtime_error(
        "\"crc_value\" not found in the provided Pex file (File: " + pex_filepath.string() + ")");
}

void nemesis::FNISaaPexFile::Patch(const nemesis::CompilationManager& manager)
{
    if (Patched) throw std::runtime_error("FNISaaPexFile has already been patched");

    Logger::Log(L"Patching FNISaaPexFile...");
    Patched = true;

    size_t checksum = manager.GetFullCheckSum();
    char replacement[10];
    snprintf(replacement, sizeof(replacement), "%08zu", checksum);
    std::copy(replacement, replacement + 9, TargetIterator);
}

void nemesis::FNISaaPexFile::OutputPexFile(const std::filesystem::path& output_path)
{
    std::filesystem::path path = NemesisInfo::PatchOutputPath(output_path);
    std::filesystem::create_directories(path.parent_path());

    std::ofstream file_out(path, std::ios::binary);

    if (file_out.write(Buffer.data(), Buffer.size()))
    {
        Logger::Log(L"Exported FNISaaPexFile: " + path.wstring());
        return;
    }

    throw std::runtime_error("Failed to write FNISaaPexFile (File: " + path.string() + ")");
}