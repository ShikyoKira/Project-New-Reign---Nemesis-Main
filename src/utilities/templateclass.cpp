#include "utilities/templateclass.h"

namespace sf = std::filesystem;

const char* nemesis::TemplateClass::invalidnames[] = {"t", "+", "md", "rd", "aaset", "aaprefix"};

void nemesis::TemplateClass::AddTemplate(const sf::path& behaviordir)
{
    VecWstr templatefiles;
    read_directory(behaviordir, templatefiles);

    for (auto& file : templatefiles)
    {
        sf::path filepath(behaviordir.wstring() + L"\\" + file);

        if (!IsValidTemplateFormat(filepath)) continue;

        templatelist.emplace_back(std::make_shared<nemesis::Template>(*this));
        templatelist.back()->ReadFile(filepath);
    }
}

nemesis::TemplateClass::TemplateClass(const sf::path& optionlistfile)
    : optionmodels(optionlistfile, *this)
{
    name = optionlistfile.parent_path().filename().string();

    if (!IsNameValid(name)) ErrorMessage(3009, name);
}

void nemesis::TemplateClass::AddBehaviorTemplate(const sf::path& classdir)
{
    VecWstr behaviorfolders;
    read_directory(classdir, behaviorfolders);

    for (auto& behavior : behaviorfolders)
    {
        if (nemesis::iequals(behavior, L"_1stperson")) continue;

        sf::path behaviordir(classdir.wstring() + L"\\" + behavior);

        if (!sf::is_directory(behaviordir)) continue;

        AddTemplate(behaviordir);
    }
}

const nemesis::Line& nemesis::TemplateClass::GetName() const noexcept
{
    return name;
}

const nemesis::OptionModelList& nemesis::TemplateClass::GetOptionModels() const
{
    return optionmodels;
}

const VecSPtr<nemesis::Template>& nemesis::TemplateClass::GetTemplateList() const
{
    return templatelist;
}

bool nemesis::TemplateClass::HasOptionList(const sf::path& classdir)
{
    return sf::exists(classdir.wstring() + L"\\option_list.txt");
}

std::shared_ptr<nemesis::TemplateClass> nemesis::TemplateClass::CreateTemplate(const sf::path& classdir)
{
    if (!HasOptionList(classdir)) return nullptr;

    return std::make_shared<nemesis::TemplateClass>(classdir.wstring() + L"\\option_list.txt");
}

bool nemesis::TemplateClass::IsValidTemplateFormat(const std::filesystem::path& file)
{
    if (sf::is_directory(file)) return false;

    if (!nemesis::iequals(file.extension().wstring(), L".txt")) return false;

    return file.stem().wstring().front() != L'#';
}

bool nemesis::TemplateClass::IsNameValid(const std::string& name)
{
    for (auto& invalidname : invalidnames)
    {
        if (name == invalidname) return false;
    }

    return true;
}
