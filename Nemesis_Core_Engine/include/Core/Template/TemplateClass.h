#pragma once

#include "Core/Template/TemplateOptionModel.h"

#include "Core/AnimationRequest.h"

#include "Utilities/ThreadPool.h"
#include "Utilities/Trie.h"

namespace nemesis
{
    struct HkxFile;
    struct ThreadPool;
    struct TemplateObject;
    struct NObjectRepository;
    struct AnimationDataSingleFile;
    struct AnimationSetDataSingleFile;

	struct TemplateClass
    {
    private:
        std::string Name;
        size_t MinArraySize = 0;
        size_t MaxArraySize = 0;
        bool IsArray        = false;

        Vec<UPtr<nemesis::TemplateOptionModel>> OptionModelList;
        nemesis::Trie<nemesis::TemplateOptionModel*> OptionModelTrie;
        Vec<SPtr<nemesis::TemplateObject>> Templates;

        std::filesystem::path InfoPath;

        static void ParseHkxTemplatesLoopDirectory(const std::filesystem::path& relative_parent_path,
                                                   const std::filesystem::path& dir,
                                                   nemesis::TemplateClass& templt_class,
                                                   nemesis::NObjectRepository& repo,
                                                   nemesis::ThreadPool& thread_pool);
        static void AddTemplateToHkxFile(const std::filesystem::path& templt_path,
                                         nemesis::TemplateClass& templt_class,
                                         nemesis::HkxFile& hkx_file,
                                         nemesis::ThreadPool& thread_pool);
        static void AddTemplateToAnimDataSingleFile(const std::filesystem::path& dir,
                                                    nemesis::TemplateClass& templt_class,
                                                    nemesis::AnimationDataSingleFile& singlefile,
                                                    nemesis::ThreadPool& thread_pool);
        static void AddTemplateToAnimSetDataSingleFile(const std::filesystem::path& dir,
                                                       nemesis::TemplateClass& templt_class,
                                                       nemesis::AnimationSetDataSingleFile& singlefile);

    public:
        TemplateClass(const std::filesystem::path& template_info_path);

        void AddTemplate(const SPtr<nemesis::TemplateObject>& template_object);
        SPtr<nemesis::TemplateObject> AddTemplate(const std::filesystem::path& filepath);
        SPtr<nemesis::TemplateObject> AddTemplate(const std::filesystem::path& filepath, nemesis::ThreadPool& thread_pool);

        SPtr<nemesis::TemplateObject> GetTemplate(size_t index);
        const nemesis::TemplateObject* GetTemplate(size_t index) const;

        Vec<const nemesis::TemplateObject*> GetTemplateList() const;

        size_t GetSize() const noexcept;
        const std::string& GetName() const noexcept;
        size_t GetRequestMinArraySize() const noexcept;
        size_t GetRequestMaxArraySize() const noexcept;
        bool IsRequestArray() const noexcept;

        const nemesis::TemplateOptionModel* GetModel(const std::string& name) const;

        const std::filesystem::path& GetInfoPath() const noexcept;

        UPtr<nemesis::AnimationRequest> CreateRequest(const std::string& request_info,
                                                      size_t linenum,
                                                      const std::filesystem::path& filepath) const;

        static UPtr<nemesis::TemplateClass> ParseTemplateClassFromDirectory(const std::filesystem::path& dir,
                                                                            nemesis::NObjectRepository& repo,
                                                                            nemesis::ThreadPool& thread_pool);
    };
}
