#pragma once

#include "core/LineStream.h"
#include "core/SubTemplateObject.h"

#include "core/Template/TemplateObject.h"

#include "utilities/threadpool.h"

namespace nemesis
{
    struct HkxNode;
    struct SemanticManager;

	struct HkxBehavior : public nemesis::NObject
    {
    private:
        Map<std::string, nemesis::HkxNode*> NodeMap;

        nemesis::HkxNode* RootNode;

        UPtr<nemesis::CollectionObject> DataNodes;
        UPtr<nemesis::CollectionObject> RegularNodes;

        UMap<std::string, Map<size_t, SPtr<nemesis::TemplateObject>>> TemplateMap;
        UMap<std::string, SPtr<nemesis::SubTemplateObject>> SubTemplateNodes;

        USetStr ModInUsedList;

        std::filesystem::path FilePath;
        std::filesystem::path CachedFilePath;

        static UPtr<nemesis::CollectionObject> ParseDataNodes(nemesis::LineStream& stream,
                                                              nemesis::SemanticManager& manager,
                                                              Map<std::string, nemesis::HkxNode*>& node_map);
        static UPtr<nemesis::CollectionObject>
        ParseRegularNodes(nemesis::LineStream& stream,
                          nemesis::SemanticManager& manager,
                          Map<std::string, nemesis::HkxNode*>& node_map);

        HkxBehavior() = default;

        void CompileTemplate(const nemesis::AnimationRequest* request,
                             DeqNstr& lines,
                             nemesis::CompileState& state) const;
        DeqNstr CompileAllTemplates(nemesis::CompileState& state) const;
        DeqNstr CompileAllSubTemplates(nemesis::CompileState& state) const;

        void UpdateNodeFromFile(const std::filesystem::path& filepath, nemesis::SemanticManager& manager);

    public:
        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::HkxBehavior> Clone() const;

        void AddTemplate(const SPtr<nemesis::TemplateObject>& templt_obj);
        void AddSubTemplateNode(const SPtr<nemesis::SubTemplateObject>& node);

        void UpdateNodeFromFile(const std::filesystem::path& filepath);
        void UpdateNodeFromFile(const std::filesystem::path& filepath, const nemesis::TemplateClass* templt_class);

        nemesis::HkxNode* GetNodeById(const std::string& node_id);
        const nemesis::HkxNode* GetNodeById(const std::string& node_id) const;

        const std::filesystem::path& GetFilePath() const noexcept;
        const std::filesystem::path& GetCachedFilePath() const noexcept;

        bool IsSameAsCached(nemesis::CompileState& state) const;

        static UPtr<nemesis::HkxBehavior> CreateFromFile(const std::filesystem::path& filepath);
        static UPtr<nemesis::HkxBehavior> CreateFromFile(const std::filesystem::path& filepath, nemesis::ThreadPool& thread_pool);
    };
}
