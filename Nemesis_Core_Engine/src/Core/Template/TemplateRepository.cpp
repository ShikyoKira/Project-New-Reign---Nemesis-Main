#include "Core/Template/TemplateRepository.h"

#include <iostream>

#include "Utilities/Algorithm.h"
#include "Utilities/ThreadPool.h"

#include "Logger.h"

nemesis::TemplateRepository::TemplateRepository(const std::filesystem::path& templt_dir,
                                                nemesis::NObjectRepository& repo)
{
    Logger::Log("Processing Templates...", true);

    if (!std::filesystem::exists(templt_dir)) return;

    nemesis::ThreadPool thread_pool;

    for (auto& entry : std::filesystem::directory_iterator(templt_dir))
    {
        auto path = entry.path();

        if (!entry.is_directory())
        {
            if (!nemesis::iequals(PATH_TO_STRING(path.extension()), LITERAL_PATH(".nemx"))) continue;

            auto st_obj = nemesis::SubTemplateObject::ParseFromFile(path, thread_pool);

            if (!st_obj) continue;

            SubTemplateList.emplace_back(std::move(st_obj));
            continue;
        }

        auto templt_class = nemesis::TemplateClass::ParseTemplateClassFromDirectory(path, repo, thread_pool);

        if (!templt_class) continue;

        TemplateClassList.emplace_back(std::move(templt_class));
    }

    thread_pool.join_all();
}

UPtr<nemesis::AnimationRequest> nemesis::TemplateRepository::CreateRequest(
    const std::string& request_info, size_t linenum, const std::filesystem::path& filepath)
{
    std::string templt_code;

    for (auto& ch : request_info)
    {
        if (ch == ' ') break;

        templt_code.push_back(ch);
    }

    for (auto& templt_class : TemplateClassList)
    {
        if (!nemesis::iequals(templt_class->GetName(), templt_code)) continue;

        return templt_class->CreateRequest(request_info, linenum, filepath);
    }

    return nullptr;
}

Vec<const nemesis::TemplateClass*> nemesis::TemplateRepository::GetTemplateClassList() const noexcept
{
    if (TemplateClassList_Cache) return *TemplateClassList_Cache;

    TemplateClassList_Cache = std::make_unique<Vec<const nemesis::TemplateClass*>>();

    for (auto& templt_class : TemplateClassList)
    {
        TemplateClassList_Cache->emplace_back(templt_class.get());
    }

    return *TemplateClassList_Cache;
}

Vec<const nemesis::SubTemplateObject*> nemesis::TemplateRepository::GetSubTemplateList() const noexcept
{
    if (SubTemplateList_Cache) return *SubTemplateList_Cache;

    SubTemplateList_Cache = std::make_unique<Vec<const nemesis::SubTemplateObject*>>();

    for (auto& sub_templt : SubTemplateList)
    {
        SubTemplateList_Cache->emplace_back(sub_templt.get());
    }

    return *SubTemplateList_Cache;
}
