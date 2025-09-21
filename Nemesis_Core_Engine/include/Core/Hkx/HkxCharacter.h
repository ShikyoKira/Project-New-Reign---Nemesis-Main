#pragma once

#include "Core/Hkx/HkxFile.h"

namespace nemesis
{
    struct SemanticManager;

    struct HkxCharacter : public nemesis::HkxFile
    {
    private:
        static void PopulateContentsFromFile(nemesis::HkxCharacter& character);

        HkxCharacter() = default;

    public:
        void CompileFileAsHkx(const std::filesystem::path& filepath,
                              nemesis::CompileState& state,
                              nemesis::PlatformType platform,
                              nemesis::HavokVersion version,
                              bool include_xml) const override;
        void ScheduleCompileFileAs(const std::filesystem::path& filepath,
                                   nemesis::CompileState& state,
                                   nemesis::PlatformType platform,
                                   nemesis::HavokVersion version,
                                   bool include_xml,
                                   std::function<void()> callback) const override;

        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::HkxCharacter> Clone() const;

        static UPtr<nemesis::HkxCharacter> ParseFromFile(const std::filesystem::path& filepath);
        static UPtr<nemesis::HkxCharacter> ParseFromFile(const std::filesystem::path& filepath,
                                                        nemesis::ThreadPool& thread_pool);
    };
}
