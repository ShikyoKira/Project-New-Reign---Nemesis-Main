#pragma once

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

        static const USetStr DataClasses;

        static bool IsNodeEnd(nemesis::LineStream& stream, bool& start);


    public:
        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        //void AddData(UPtr<nemesis::NObject>&& data);

        const std::string& GetNodeId() const noexcept;
        const std::string& GetClassName() const noexcept;

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

        void MatchAndUpdate(const UPtr<nemesis::HkxNode>&& hkxnode);

        static const nemesis::regex NodeIdRgx;
    };
}
