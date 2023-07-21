#include "base/exporter.h"

#include "hkx/HkxBehaviorFile.h"

#include "utilities/animquery.h"

#include "scope/scopeinfo.h"

std::regex nemesis::Exporter::lexing_rgx("(\\]\\[|\\[|\\])");

std::string nemesis::Exporter::NodeIdList::GetNewId(nemesis::Exporter& exporter) const
{
    std::stringstream ss;
    ss << "#";
    ss << exporter.topid.Write().value++;
    return ss.str();
}

std::string_view nemesis::Exporter::NodeIdList::GetNodeId(const std::string& key,
                                                          nemesis::Exporter& exporter) const
{
    auto itr = nodeidlist.find(key);

    if (itr != nodeidlist.end()) return itr->second;

    nodeidlist[key] = GetNewId(exporter);
    return nodeidlist[key];
}

const nemesis::Exporter::NodeIdList*
nemesis::Exporter::NodeIdByQuery::GetByQuery(const nemesis::AnimQuery* animquery) const
{
    return &nodemap[animquery];
}

const nemesis::Exporter::NodeIdByQuery* nemesis::Exporter::NodeIdByFile::GetByFile(const nemesis::File* file)
{
    auto itr = nodemap.find(file);

    if (itr != nodemap.end()) return &itr->second;

    return &nodemap[file];
}

const nemesis::Exporter::NodeIdByQuery*
nemesis::Exporter::NodeIdByFile::GetByFile(const nemesis::File* file) const
{
    auto itr = nodemap.find(file);

    if (itr != nodemap.end()) return &itr->second;

    return nullptr;
}

nemesis::Exporter::Exporter(const nemesis::File& file)
    : file(file)
{
    RenewScopeInfo();
}

void nemesis::Exporter::CheckNumElement(std::string& line)
{
    UPtr<nemesis::NumElement> numele_ptr;

    if (nemesis::NumElement::TryCreateNumElement(line, numele_ptr))
    {
        numelement_layer.emplace_back(std::move(numele_ptr));
        return;
    }

    if (numelement_layer.empty()) return;

    auto& curlayer = numelement_layer.back();
    curlayer->CheckLine(line);

    if (!curlayer->IsEnd()) return;

    numelement_layer.pop_back();
}

void nemesis::Exporter::RenewScopeInfo()
{
    scopeinfo           = std::shared_ptr<nemesis::ScopeInfo>(new nemesis::ScopeInfo());
    scopeinfo->exporter = this;
}

bool nemesis::Exporter::ContainsAnimImport(const std::string& key)
{
    for (auto& animimportpair : animimportpair_list)
    {
        if (animimportpair.first == key) return true;
    }

    return false;
}

void nemesis::Exporter::SetCurrentQuery(const nemesis::AnimQuery& query)
{
    scopeinfo->SetCurrentQuery(query);
}

void nemesis::Exporter::AddEvents()
{
    events = std::make_unique<nemesis::HkxEventList>();
}

void nemesis::Exporter::AddVariables()
{
    variables = std::make_unique<nemesis::HkxVariableList>();
}

void nemesis::Exporter::AddAttributes()
{
    attibutes = std::make_unique<nemesis::HkxAttributeList>();
}

void nemesis::Exporter::AddAnimImport(const std::string& key, UPtr<nemesis::AnimImport>& animimport_ptr)
{
    animimportpair_list.emplace_back(
        std::make_pair<std::string, UPtr<nemesis::AnimImport>>(std::string(key), std::move(animimport_ptr)));
}

void nemesis::Exporter::SetInnerExporter(nemesis::Exporter& exporter)
{
    scopeinfo = exporter.scopeinfo;
}

bool nemesis::Exporter::IsBehavior() const
{
    return events || variables;
}

std::string_view nemesis::Exporter::GetID(const std::string& key,
                                          const nemesis::File* file,
                                          const nemesis::AnimQuery* animquery)
{
    auto& reader = nodeidlist.Read();
    auto ptr     = reader.GetByFile(file);

    if (ptr) return ptr->GetByQuery(animquery)->GetNodeId(key, *this);

    return nodeidlist.Write().value.GetByFile(file)->GetByQuery(animquery)->GetNodeId(key, *this);
}

std::string_view nemesis::Exporter::GetImportID(const std::string& key)
{
    auto sv = import_nodeidlist.Read().GetNodeId(key, *this);

    if (!ContainsAnimImport(key))
    {
        DeqStr components(std::sregex_token_iterator(key.begin(), key.end(), lexing_rgx, -1),
                          std::sregex_token_iterator());
        components.pop_front();
        auto imprt_templt = GetAnimTemplate()->GetImport(components.front());
        components.pop_front();
        auto animimport_ptr = std::make_unique<nemesis::AnimImport>(std::string(sv));
        animimport_ptr->AddImportTemplate(imprt_templt.get());

        for (auto& component : components)
        {
            animimport_ptr->AddParameter(component);
        }

        AddAnimImport(key, animimport_ptr);
    }

    return sv;
}

const nemesis::HkxEvent* nemesis::Exporter::GetHkxEventPtr(const std::string& eventname) const
{
    if (!IsBehavior()) ErrorMessage(1228, file.GetFilePath());

    return events->GetEvent(eventname);
}

const nemesis::HkxVariable* nemesis::Exporter::GetVariableIDPtr(const std::string& varname) const
{
    if (!IsBehavior()) return &scopeinfo->GetVariableID(varname);

    return variables->GetVariable(varname);
}

const nemesis::File& nemesis::Exporter::GetFile() const
{
    return file;
}
