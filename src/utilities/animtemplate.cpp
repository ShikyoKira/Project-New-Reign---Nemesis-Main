#include "Global.h"

#include "utilities/animtemplate.h"

namespace sf = std::filesystem;


void nemesis::AnimTemplate::Parser::ParseQuery(const nemesis::Line& query)
{
    std::smatch matches;

    if (!std::regex_match(query.ToString(), matches, host.queryrgx)) return;

    std::string classname = matches.str(1);

    if (classname != "+")
    {
        querylist->AddQuery(CreateQuery(query, classname, matches));
        return;
    }

    if (querylist->IsEmpty()) ErrorMessage(1065, filepath, query.GetLineNumber());

    auto& last     = querylist->GetList().back();
    auto animquery = CreateQuery(query, last->GetAnimClass()->GetName(), matches);
    last->AddQueryToGroup(animquery);
}

nemesis::AnimTemplate::AnimQueryPtr nemesis::AnimTemplate::Parser::CreateQuery(const nemesis::Line& query,
                                                                               const std::string& classname,
                                                                               const std::smatch& matches)
{
    auto animclass = host.GetClass(classname);

    if (animclass == nullptr) ErrorMessage(1016, filepath, query.GetLineNumber());

    // group 1 = template class
    // group 2 = options
    // group 3 = animation name
    // group 4 = animation filename
    // group 5 = animation objects
    auto animquery = std::make_shared<nemesis::AnimQuery>(query, *querylist, animclass);
    animquery->AddOptions(matches.str(2));
    animquery->AddAnimName(matches.str(3));
    animquery->AddAnimFile(matches.str(4));
    animquery->AddAnimObject(matches.str(5));
    return animquery;
}

nemesis::AnimTemplate::Parser::Parser(const AnimTemplate& host, const std::filesystem::path& filepath)
    : host(host)
{
    this->filepath = filepath;
}

void nemesis::AnimTemplate::Parser::ParseLines()
{
    VecNstr storeline;
    GetFunctionLines(filepath, storeline);
    querylist = std::make_shared<nemesis::AnimQueryList>(filepath);

    for (auto& line : storeline)
    {
        auto pos   = line.find("'");
        auto nline = pos == NOT_FOUND ? line : line.substr(pos);

        if (nline.empty()) continue;

        ParseQuery(nline);
    }
}

SPtr<nemesis::AnimQueryList> nemesis::AnimTemplate::Parser::GetResult() const noexcept
{
    return querylist;
}

void nemesis::AnimTemplate::TryAddTemplate(const sf::path& classdir)
{
    auto optionlist = nemesis::TemplateClass::CreateTemplate(classdir);

    if (!optionlist) return;

    optionlist->AddBehaviorTemplate(classdir);
    classlist.emplace_back(optionlist);
}

void nemesis::AnimTemplate::UpdateRegex()
{
    const char* prefix = "^(";
    const char* suffix = "\\+)\\s+(-.+?(?=\\s)|)\\s*([^,]+?)\\s+([^,]+?)(?:\\s+([^,]*?)|\\s*)$";
    std::string classes;

    for (auto& tempclass : classlist)
    {
        classes.append(tempclass->GetName() + "|");
    }

    queryrgx = std::regex(prefix + classes + suffix);
}

SPtr<const nemesis::TemplateClass> nemesis::AnimTemplate::GetClass(std::string name) const
{
    for (auto& each : classlist)
    {
        if (each->GetName() == name) return each;
    }

    return nullptr;
}

nemesis::AnimTemplate::AnimTemplate(const sf::path& templatedir)
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
        else if (fullpath.extension().wstring() == L".txt")
        {
            imports.emplace_back(std::make_shared<Template>());
            imports.back()->ReadFile(fullpath);
        }
    }

    UpdateRegex();
}

SPtr<nemesis::AnimQueryList> nemesis::AnimTemplate::ReadListFile(const sf::path& listfile) const
{
    Parser parser(*this, listfile);
    parser.ParseLines();
    return parser.GetResult();
}

const SPtr<nemesis::Template>& nemesis::AnimTemplate::GetImport(const std::string importname) const
{
    std::wstring wimport = nemesis::transform_to(importname);

    for (auto& imp : imports)
    {
        if (imp->GetBehaviorFileW() == wimport) return imp;
    }

    return nullptr;
}

VecSPtr<nemesis::Template>
nemesis::AnimTemplate::GetBehaviorTemplateList(const std::wstring behaviorname) const
{
    VecSPtr<nemesis::Template> templatelist;

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
