#pragma once

#include "Core/Hkx/HkxFile.h"

namespace nemesis
{
    struct SemanticManager;

	struct HkxBehavior : public nemesis::HkxFile
    {
    private:
        UPtr<nemesis::CollectionObject> DataNodes;

        static UPtr<nemesis::CollectionObject> ParseDataNodes(nemesis::LineStream& stream,
                                                              nemesis::SemanticManager& manager,
                                                              nemesis::HkxBehavior& behavior);
        static UPtr<nemesis::CollectionObject> ParseRegularNodes(nemesis::LineStream& stream,
                                                                 nemesis::SemanticManager& manager,
                                                                 nemesis::HkxBehavior& behavior);
        static void PopulateContentsFromFile(nemesis::HkxBehavior& behavior);

        HkxBehavior() = default;

    public:
        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::HkxBehavior> Clone() const;

        size_t GetSize() const override;

        static UPtr<nemesis::HkxBehavior> ParseFromFile(const std::filesystem::path& filepath);
        static UPtr<nemesis::HkxBehavior> ParseFromFile(const std::filesystem::path& filepath,
                                                        nemesis::ThreadPool& thread_pool);
    };
}
