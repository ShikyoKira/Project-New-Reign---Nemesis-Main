#include "Global.h"

#include "hkx/HkxBehaviorFile.h"

#include "utilities/animtemplate.h"

namespace sf = std::filesystem;


const std::string nemesis::AnimTemplate::Parser::animation_dir("\\animations\\");

void nemesis::AnimTemplate::Parser::ParseQuery(const nemesis::Line& query)
{
    nemesis::smatch matches;

    if (!regex_match(query.ToString(), matches, host.queryrgx)) return;

    std::string classname = matches.str(1);

    if (classname == "+")
    {
        if (querylist->IsEmpty()) ErrorMessage(1065, filepath, query.GetLineNumber());

        auto& last     = querylist->GetList().back();
        auto animquery = CreateQuery(query, last->GetAnimClass().GetName(), matches);
        animquery->AddBehaviorIndex(host.behaviorindex++);
        last->AddQueryToGroup(*animquery);
        return;
    }
    else if (classname == "MD")
    {
        auto translation = GetTranslation(matches);
        querylist->AddMotionData(query.GetLineNumber(), translation);
        return;
    }
    else if (classname == "RD")
    {
        auto translation = GetTranslation(matches);
        querylist->AddRotationData(query.GetLineNumber(), translation);
        return;
    }

    auto queryptr = CreateQuery(query, classname, matches);
    queryptr->AddSelfToArray();
    queryptr->AddBehaviorIndex(host.behaviorindex++);
    querylist->AddQuery(queryptr);
}

nemesis::AnimTemplate::AnimQueryPtr nemesis::AnimTemplate::Parser::CreateQuery(const nemesis::Line& query,
                                                                               const std::string& classname,
                                                                               const nemesis::smatch& matches)
{
    auto animclass = host.GetClass(classname);

    if (animclass == nullptr) ErrorMessage(1016, filepath, query.GetLineNumber());

    auto animpath = filepath.parent_path().string() + "\\" + matches.str(4);
    animpath      = animpath.substr(nemesis::risearch(animpath, animation_dir) + animation_dir.length());

    // group 1 = template class
    // group 2 = options
    // group 3 = animation name
    // group 4 = animation filename
    // group 5 = animation objects
    auto animquery = std::make_shared<nemesis::AnimQuery>(query, *querylist, *animclass);
    animquery->AddOptions(matches.str(2));
    animquery->AddAnimName(matches.str(3));
    animquery->AddAnimPath(animpath);
    animquery->AddAnimObjects(matches.str(5));
    return animquery;
}

std::string nemesis::AnimTemplate::Parser::GetTranslation(const nemesis::smatch& matches)
{
    auto fullmatch = matches.str(0);
    return fullmatch.substr(matches.str(1).length() + 1);
}

nemesis::AnimTemplate::Parser::Parser(const nemesis::AnimTemplate& host,
                                      const std::filesystem::path& filepath)
    : host(host)
{
    this->filepath = filepath;
}

void nemesis::AnimTemplate::Parser::ParseLines()
{
    VecNstr storeline;
    GetFileLines(filepath, storeline);
    querylist = std::make_shared<nemesis::AnimQueryFile>(filepath);

    for (auto& line : storeline)
    {
        auto pos   = line.find("'");
        auto nline = pos == NOT_FOUND ? line : line.substr(pos);

        if (nline.empty()) continue;

        ParseQuery(nline);
    }
}

SPtr<const nemesis::AnimQueryFile> nemesis::AnimTemplate::Parser::GetResult() const noexcept
{
    return querylist;
}

void nemesis::AnimTemplate::TryAddTemplate(const sf::path& classdir)
{
    auto templateclass = nemesis::TemplateCategory::CreateTemplate(classdir);

    if (!templateclass) return;

    templateclass->AddBehaviorTemplate(classdir);
    templateclass->SetAnimTemplate(this);
    classlist.emplace_back(templateclass);
}

void nemesis::AnimTemplate::AddImportTemplate(const std::filesystem::path& filepath)
{
    auto ptr = nemesis::Template::CreateImport(filepath);
    imports.emplace_back(ptr);
}

void nemesis::AnimTemplate::UpdateRegex()
{
    const char* prefix = "^(";
    const char* suffix = "MD|RD|\\+)\\s+(-.+?(?=\\s)|)\\s*([^,]+?)\\s+([^,]+?)(?:\\s+([^,]*?)|\\s*)$";
    std::string classes;

    for (auto& tempclass : classlist)
    {
        classes.append(tempclass->GetName() + "|");
    }

    queryrgx = prefix + classes + suffix;
}

nemesis::AnimTemplate::AnimTemplate(const sf::path& templatedir)
{
    try
    {
        VecWstr contents;
        read_directory(templatedir, contents);

        for (auto& each : contents)
        {
            sf::path fullpath = templatedir.wstring() + L"\\" + each;

            if (sf::is_directory(fullpath))
            {
                TryAddTemplate(fullpath);
            }
            else if (nemesis::iequals(fullpath.extension().wstring(), L".txt"))
            {
                AddImportTemplate(fullpath);
            }
        }

        UpdateRegex();
    }
    catch (const nemesis::exception&)
    {
    }
    catch (const std::exception& ex)
    {
        ErrorMessage(6002, "None", ex.what());
    }
}

const nemesis::TemplateCategory* nemesis::AnimTemplate::GetClass(const std::string& name) const
{
    for (auto& each : classlist)
    {
        if (nemesis::iequals(each->GetName(), name)) return each.get();
    }

    return nullptr;
}

void nemesis::AnimTemplate::LinkToBehaviorList(const VecSPtr<nemesis::HkxBehaviorFile>& behaviorlist)
{
    for (auto& each : classlist)
    {
        each->LinkToBehaviorList(behaviorlist);
    }
}

SPtr<const nemesis::AnimQueryFile> nemesis::AnimTemplate::ReadListFile(const sf::path& path) const
{
    Parser parser(*this, path);
    parser.ParseLines();
    return parser.GetResult();
}

SPtr<const nemesis::Template> nemesis::AnimTemplate::GetImport(const std::string importname) const
{
    std::wstring wimport = nemesis::transform_to(importname);

    for (auto& imp : imports)
    {
        if (nemesis::iequals(imp->GetFilePath().stem().wstring(), wimport)) return imp;
    }

    return nullptr;
}

VecSPtr<const nemesis::Template>
nemesis::AnimTemplate::GetBehaviorTemplateList(const std::wstring behaviorname) const
{
    VecSPtr<const nemesis::Template> templatelist;

    for (auto& eachclass : classlist)
    {
        auto& list = eachclass->GetTemplateList();

        for (auto& each : list)
        {
            if (behaviorname != each->GetBehaviorFileW()) continue;

            templatelist.emplace_back(each);
        }
    }

    return templatelist;
}
