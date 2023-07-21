#include "utilities/lineprocess.h"
#include "utilities/templatecategory.h"

#include "hkx/HkxBehaviorFile.h"

namespace sf = std::filesystem;

const char* nemesis::TemplateCategory::invalidnames[] = {"t", "+", "md", "rd", "aaset", "aaprefix"};

void nemesis::TemplateCategory::AddTemplate(const sf::path& behaviordir)
{
    VecWstr templatefiles;
    read_directory(behaviordir, templatefiles);

    for (auto& file : templatefiles)
    {
        sf::path filepath(behaviordir.wstring() + L"\\" + file);

        if (!IsValidTemplateFormat(filepath)) continue;

        SPtr<nemesis::Template> tempptr(new nemesis::Template(*this));
        tempptr->ReadFile(filepath);
        templatelist.emplace_back(tempptr);
    }
}

nemesis::TemplateCategory::TemplateCategory() noexcept
    : optionmodels(*this)
{
}

nemesis::TemplateCategory::TemplateCategory(const sf::path& optionlistfile)
    : optionmodels(optionlistfile, *this)
{
    try
    {
        name = optionlistfile.parent_path().filename().string();

        if (!IsNameValid(name)) ErrorMessage(3009, name);
    }
    catch (const nemesis::exception&)
    {
    }
    catch (const std::exception& ex)
    {
        ErrorMessage(6002, "None", ex.what());
    }
}

void nemesis::TemplateCategory::SetAnimTemplate(const nemesis::AnimTemplate* animtemplt)
{
    animtemplate = animtemplt;
}

void nemesis::TemplateCategory::AddBehaviorTemplate(const sf::path& classdir)
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

void nemesis::TemplateCategory::TryLinkToBehavior(nemesis::HkxBehaviorFile& behavior) const
{
    for (auto& each : templatelist)
    {
        if (each->TryAddBehavior(behavior)) return;
    }

    ErrorMessage(1220, name, behavior.GetFilePath());
}

void nemesis::TemplateCategory::LinkToBehaviorList(const VecSPtr<nemesis::HkxBehaviorFile>& behaviorlist) const
{
    for (auto& each : templatelist)
    {
        if (!each->TryAddBehaviorList(behaviorlist)) ErrorMessage(1220, name, each->GetFilePath());
    }
}

const nemesis::AnimTemplate& nemesis::TemplateCategory::GetAnimTemplate() const noexcept
{
    return *animtemplate;
}

const nemesis::Line& nemesis::TemplateCategory::GetName() const noexcept
{
    return name;
}

const nemesis::OptionModelList& nemesis::TemplateCategory::GetOptionModels() const noexcept
{
    return optionmodels;
}

const VecSPtr<const nemesis::Template>& nemesis::TemplateCategory::GetTemplateList() const noexcept
{
    return templatelist;
}

bool nemesis::TemplateCategory::HasGroup() const noexcept
{
    for (auto& templt : templatelist)
    {
        if (templt->IsGroup()) return true;
    }

    return false;
}

bool nemesis::TemplateCategory::HasMaster() const noexcept
{
    for (auto& templt : templatelist)
    {
        if (templt->IsMaster()) return true;
    }

    return false;
}

bool nemesis::TemplateCategory::HasOptionList(const sf::path& classdir)
{
    return sf::exists(classdir.wstring() + L"\\option_list.txt");
}

std::shared_ptr<nemesis::TemplateCategory> nemesis::TemplateCategory::CreateTemplate(const sf::path& classdir)
{
    if (!HasOptionList(classdir)) return nullptr;

    return std::make_shared<nemesis::TemplateCategory>(classdir.wstring() + L"\\option_list.txt");
}

bool nemesis::TemplateCategory::IsValidTemplateFormat(const std::filesystem::path& file)
{
    if (sf::is_directory(file)) return false;

    if (!nemesis::iequals(file.extension().wstring(), L".txt")) return false;

    return file.stem().wstring().front() != L'#';
}

bool nemesis::TemplateCategory::IsNameValid(const std::string& name)
{
    for (auto& invalidname : invalidnames)
    {
        if (name == invalidname) return false;
    }

    return true;
}
