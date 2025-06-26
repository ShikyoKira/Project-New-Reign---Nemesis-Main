#include "Core/Template/TemplateAnimDataMotionData.h"

#include "Utilities/Algorithm.h"

nemesis::TemplateAnimDataMotionData::TemplateAnimDataMotionData(
    const nemesis::TemplateClass* template_class) noexcept
    : nemesis::TemplateObject(template_class)
{
}

SPtr<nemesis::TemplateAnimDataMotionData>
nemesis::TemplateAnimDataMotionData::ParseFromFile(const nemesis::TemplateClass* template_class,
                                                   const std::filesystem::path& filepath)
{
    std::string name(template_class->GetName());

    if (std::find_if(name.begin(), name.end(), [](const char& ch) { return !std::isalpha(ch); })
        != name.end())
    {
        throw std::runtime_error("Invalid template name (" + name + ")");
    }

    std::string filename = nemesis::to_utf8_string(filepath.stem());

    if (filename != "$" + name + "$UC")
    {
        throw std::runtime_error("Invalid filename "
                                 "(Template: "
                                 + name + ", File: " + nemesis::to_utf8_string(filepath) + ")");
    }

    return NewCustomTemplateObject<nemesis::TemplateAnimDataMotionData>(template_class, 0, filepath);
}

SPtr<nemesis::TemplateAnimDataMotionData>
nemesis::TemplateAnimDataMotionData::ParseFromFile(const nemesis::TemplateClass* template_class,
                                                   const std::filesystem::path& filepath,
                                                   nemesis::ThreadPool& thread_pool)
{
    std::string name(template_class->GetName());

    if (std::find_if(name.begin(), name.end(), [](const char& ch) { return !std::isalpha(ch); })
        != name.end())
    {
        throw std::runtime_error("Invalid template name (" + name + ")");
    }

    std::string filename = nemesis::to_utf8_string(filepath.stem());

    if (filename != "$" + name + "$UC")
    {
        throw std::runtime_error("Invalid filename "
                                 "(Template: "
                                 + name + ", File: " + nemesis::to_utf8_string(filepath) + ")");
    }

    return NewCustomTemplateObject<nemesis::TemplateAnimDataMotionData>(
        template_class, 0, filepath, thread_pool);
}
