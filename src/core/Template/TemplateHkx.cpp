#include "core/Template/TemplateHkx.h"
#include "core/Template/TemplateClass.h"

#include "core/LineStream.h"
#include "core/NObjectParser.h"

#include "utilities/stringextension.h"

nemesis::TemplateHkx::TemplateHkx(const nemesis::TemplateClass* template_class) noexcept
    : nemesis::TemplateObject(template_class)
{
}

SPtr<nemesis::TemplateHkx> nemesis::TemplateHkx::CreateFromFile(const nemesis::TemplateClass* template_class,
                                                                const std::filesystem::path& filepath)
{
    std::string name(template_class->GetName());

    if (std::find_if(name.begin(), name.end(), [](const char& ch) { return !std::isalpha(ch); })
        != name.end())
    {
        throw std::runtime_error("Invalid template name (" + name + ")");
    }

    nemesis::regex name_rgx("(" + name + ")_([0-9]+)");
    nemesis::smatch match;

    if (!nemesis::regex_match(filepath.stem().string(), match, name_rgx))
    {
        throw std::runtime_error("Invalid filename "
                                 "(Template: "
                                 + name + ", File: " + filepath.string() + ")");
    }

    auto templt_ptr = new nemesis::TemplateHkx(template_class);
    SPtr<nemesis::TemplateHkx> templt(templt_ptr);
    templt->Index    = std::stoul(match[2]);
    templt->Data     = std::make_unique<nemesis::CollectionObject>();
    templt->FilePath = filepath;

    auto& data_ref = *templt_ptr->Data;
    nemesis::SemanticManager manager;
    manager.SetCurrentTemplate(templt_ptr);
    manager.TryAddRequestToQueue(templt_ptr->GetFilePath().stem().string());

    VecNstr lines;
    GetFileLines(filepath, lines, false);

    for (nemesis::LineStream stream(lines.begin(), lines.end()); !stream.IsEoF(); ++stream)
    {
        auto objects = nemesis::NObjectParser::ParseHkxObjects(stream, manager);

        for (auto& object : objects)
        {
            data_ref.AddObject(std::move(object));
        }
    }

    return templt;
}

SPtr<nemesis::TemplateHkx> nemesis::TemplateHkx::CreateFromFile(const nemesis::TemplateClass* template_class,
                                                                const std::filesystem::path& filepath,
                                                                nemesis::ThreadPool& thread_pool)
{
    std::string name(template_class->GetName());

    if (std::find_if(name.begin(), name.end(), [](const char& ch) { return !std::isalpha(ch); })
        != name.end())
    {
        throw std::runtime_error("Invalid template name (" + name + ")");
    }

    nemesis::regex name_rgx("^(" + name + ")_([0-9]+)$");
    nemesis::smatch match;

    if (!nemesis::regex_match(filepath.stem().string(), match, name_rgx))
    {
        throw std::runtime_error("Invalid filename "
                                 "(Template: "
                                 + name + ", File: " + filepath.string() + ")");
    }

    auto templt_ptr = new nemesis::TemplateHkx(template_class);
    SPtr<nemesis::TemplateHkx> templt(templt_ptr);
    templt->Index    = std::stoul(match[2]);
    templt->Data     = std::make_unique<nemesis::CollectionObject>();
    templt->FilePath = filepath;

    thread_pool.enqueue(
        [templt_ptr, name]()
        {
            auto& data_ref = *templt_ptr->Data;
            nemesis::SemanticManager manager;
            manager.SetCurrentTemplate(templt_ptr);

            for (size_t i = 1; i <= templt_ptr->GetIndex(); ++i)
            {
                manager.TryAddRequestToQueue(name + "_" + std::to_string(i));
            }

            VecNstr lines;
            GetFileLines(templt_ptr->FilePath, lines, false);

            for (nemesis::LineStream stream(lines.begin(), lines.end()); !stream.IsEoF(); ++stream)
            {
                auto objects = nemesis::NObjectParser::ParseHkxObjects(stream, manager);

                for (auto& object : objects)
                {
                    data_ref.AddObject(std::move(object));
                }
            }
        });

    return templt;
}
