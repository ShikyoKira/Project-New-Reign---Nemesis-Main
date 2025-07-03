#pragma once

#include <mutex>
#include <regex>

#include "Core/LineStream.h"
#include "Core/CollectionObject.h"

namespace nemesis
{
    struct HkxFile;
    struct ModClass;
    struct IfObject;
    struct ThreadPool;
    struct TemplateClass;
    struct ForEachObject;
    struct SemanticManager;

	struct HkxNode : public nemesis::NObject
    {
    private:
        std::filesystem::path FilePath;
        std::string NodeId, ClassName;
        UPtr<nemesis::CollectionObject> Data;
        const nemesis::HkxFile* HkxDocument = nullptr;

        std::mutex UpdaterMutex;

        static bool ClassHasVecf(const std::string& class_name);
        static bool IsNodeEnd(nemesis::LineStream& stream, bool& start);

        void TryInjectEventNames(DeqNstr& lines, size_t start_pos, nemesis::CompileState& state) const;
        void TryInjectVariableNames(DeqNstr& lines, size_t start_pos, nemesis::CompileState& state) const;

    public:
        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::HkxNode> Clone() const;

        const std::string& GetNodeId() const noexcept;
        const std::string& GetClassName() const noexcept;

        const std::filesystem::path& GetFilePath() const noexcept;
        
        const nemesis::HkxFile* GetHkxDocument() const noexcept;
        void SetHkxDocument(const nemesis::HkxFile* hkx_document) noexcept;

        void MatchAndUpdate(const nemesis::HkxNode& hkxnode);
        void MatchAndUpdate(const std::string& mod_code, const nemesis::HkxNode& hkxnode);

        static bool IsDataClass(nemesis::LineStream& stream);

        static UPtr<nemesis::NObject>
        ParseHkxNode(nemesis::LineStream& stream, nemesis::SemanticManager& manager);
        static UPtr<nemesis::NObject>
        ParseHkxNode(nemesis::LineStream& stream, nemesis::SemanticManager& manager, nemesis::HkxNode*& node);

        static void
        Deserialize(nemesis::HkxNode& hkx_node, nemesis::LineStream& stream, nemesis::SemanticManager& manager);
        static UPtr<nemesis::HkxNode> DeserializeHkxNodeFromFile(const std::filesystem::path& filepath);
        static UPtr<nemesis::HkxNode>
        DeserializeHkxNodeFromFile(const std::filesystem::path& filepath,
                                   const nemesis::TemplateClass* template_class);
        static UPtr<nemesis::HkxNode> DeserializeHkxNodeFromFile(const std::filesystem::path& filepath,
                                                                 nemesis::SemanticManager& manager);
        static UPtr<nemesis::HkxNode> DeserializeHkxNodeFromFile(const std::filesystem::path& filepath,
                                                                 nemesis::ThreadPool& threadpool);
        static UPtr<nemesis::HkxNode> DeserializeHkxNodeFromFile(const std::filesystem::path& filepath,
                                                                 const nemesis::TemplateClass* template_class,
                                                                 nemesis::ThreadPool& threadpool);

        static const std::regex NodeIdRgx;
    };
}
