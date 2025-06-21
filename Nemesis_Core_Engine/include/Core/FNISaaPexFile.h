#include <filesystem>

#include "Core/CompilationManager.h"

namespace nemesis
{
    struct FNISaaPexFile
    {
    private:
        std::filesystem::path FilePath;
        Vec<char> Buffer;
        Vec<char>::iterator TargetIterator;

        bool Patched = false;

    public:
        FNISaaPexFile(const std::filesystem::path& pex_filepath);

        void Patch(const nemesis::CompilationManager& manager);
        void OutputPexFile(const std::filesystem::path& output_path);
    };
}
