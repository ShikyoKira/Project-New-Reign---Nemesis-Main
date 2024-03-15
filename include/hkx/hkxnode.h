#pragma once

#include <mutex>

#include "utilities/regex.h"

#include "core/LineStream.h"
#include "core/CollectionObject.h"

namespace nemesis
{
    struct IfObject;
    struct TemplateClass;
    struct ForEachObject;
    struct SemanticManager;

	struct HkxNode : public nemesis::NObject
    {
    private:
        std::string NodeId;
        std::string ClassName;
        UPtr<nemesis::CollectionObject> Data;

        std::mutex UpdaterMutex;

        static const USetStr DataClasses;

        static bool IsNodeEnd(nemesis::LineStream& stream, bool& start);


    public:
        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::HkxNode> Clone() const;

        const std::string& GetNodeId() const noexcept;
        const std::string& GetClassName() const noexcept;

        void MatchAndUpdate(const nemesis::HkxNode& hkxnode);

        static bool IsDataClass(nemesis::LineStream& stream);

        static UPtr<nemesis::NObject>
        ParseHkxNode(nemesis::LineStream& stream, nemesis::SemanticManager& manager);
        static UPtr<nemesis::NObject>
        ParseHkxNode(nemesis::LineStream& stream, nemesis::SemanticManager& manager, nemesis::HkxNode*& node);

        static UPtr<nemesis::HkxNode> ParseHkxNodeFromFile(const std::filesystem::path& filepath);
        static UPtr<nemesis::HkxNode> ParseHkxNodeFromFile(const std::filesystem::path& filepath,
                                                   const nemesis::TemplateClass* template_class);
        static UPtr<nemesis::HkxNode> ParseHkxNodeFromFile(const std::filesystem::path& filepath,
                                                   nemesis::SemanticManager& manager);

        static const nemesis::regex NodeIdRgx;
    };
}
