#include "Core/Template/TemplateAnimDataClipData.h"

#include "Core/NObjectParser.h"

nemesis::TemplateAnimDataClipData::TemplateAnimDataClipData(
    const nemesis::TemplateClass* template_class) noexcept
    : nemesis::TemplateObject(template_class)
{
}

SPtr<nemesis::TemplateAnimDataClipData>
nemesis::TemplateAnimDataClipData::ParseFromFile(const nemesis::TemplateClass* template_class,
                                                 const std::filesystem::path& filepath)
{
    std::string name(template_class->GetName());

    if (std::find_if(name.begin(), name.end(), [](const char& ch) { return !std::isalpha(ch); })
        != name.end())
    {
        throw std::runtime_error("Invalid template name (" + name + ")");
    }

    std::string filename = filepath.stem().string();

    if (filename != "$" + name + "$")
    {
        throw std::runtime_error("Invalid filename "
                                 "(Template: "
                                 + name + ", File: " + filepath.string() + ")");
    }

    return NewCustomTemplateObject<nemesis::TemplateAnimDataClipData>(template_class, 0, filepath);
}

SPtr<nemesis::TemplateAnimDataClipData>
nemesis::TemplateAnimDataClipData::ParseFromFile(const nemesis::TemplateClass* template_class,
                                                 const std::filesystem::path& filepath,
                                                 nemesis::ThreadPool& thread_pool)
{
    std::string name(template_class->GetName());

    if (std::find_if(name.begin(), name.end(), [](const char& ch) { return !std::isalpha(ch); })
        != name.end())
    {
        throw std::runtime_error("Invalid template name (" + name + ")");
    }

    std::string filename = filepath.stem().string();

    if (filename != "$" + name + "$")
    {
        throw std::runtime_error("Invalid filename "
                                 "(Template: "
                                 + name + ", File: " + filepath.string() + ")");
    }

    return NewCustomTemplateObject<nemesis::TemplateAnimDataClipData>(template_class, 0, filepath, thread_pool);
}
