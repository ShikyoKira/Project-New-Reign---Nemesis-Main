#include "Core/Template/TemplateAnimDataClipData.h"

#include "Utilities/Algorithm.h"

nemesis::TemplateAnimDataClipData::TemplateAnimDataClipData(
    const nemesis::TemplateClass* template_class) noexcept
    : nemesis::TemplateObject(template_class)
{
}

UPtr<nemesis::TemplateAnimDataClipData> nemesis::TemplateAnimDataClipData::ParseFromFile(
    const std::filesystem::path& filepath, const nemesis::TemplateClass* template_class, size_t index)
{
    std::string name(template_class->GetName());

    if (std::find_if(name.begin(), name.end(), [](const char& ch) { return !std::isalpha(ch); })
        != name.end())
    {
        throw std::runtime_error("Invalid template name '" + name
                                 + "' (File: " + nemesis::to_utf8_string(filepath) + ")");
    }

    std::string filename = nemesis::to_utf8_string(filepath.stem());
    return NewCustomTemplateObject<nemesis::TemplateAnimDataClipData>(template_class, index, filepath);
}

UPtr<nemesis::TemplateAnimDataClipData>
nemesis::TemplateAnimDataClipData::ParseFromFile(const std::filesystem::path& filepath,
                                                 const nemesis::TemplateClass* template_class,
                                                 size_t index,
                                                 nemesis::ThreadPool& thread_pool)
{
    std::string name(template_class->GetName());

    if (std::find_if(name.begin(), name.end(), [](const char& ch) { return !std::isalpha(ch); })
        != name.end())
    {
        throw std::runtime_error("Invalid template name '" + name
                                 + "' (File: " + nemesis::to_utf8_string(filepath) + ")");
    }

    std::string filename = nemesis::to_utf8_string(filepath.stem());
    return NewCustomTemplateObject<nemesis::TemplateAnimDataClipData>(
        template_class, index, filepath, thread_pool);
}
