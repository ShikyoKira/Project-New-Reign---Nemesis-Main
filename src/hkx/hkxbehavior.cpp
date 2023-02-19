#include "Global.h"

#include "nemesisinfo.h"

#include "hkx/hkxbehavior.h"

#include "core/querymanager.h"
#include "core/linkedpreprocessline.h"

#include "update/patch.h"

#include "utilities/atomic.h"
#include "utilities/conditionscope.h"
#include "utilities/conditionsyntax.h"
#include "utilities/lineprocess.h"
#include "utilities/readtextfile.h"
#include "utilities/regex.h"
#include "utilities/template.h"
#include "utilities/stringextension.h"

#include "template/processparser.h"

using namespace std;
namespace sf = filesystem;
namespace ns = nemesis::syntax;

using HB = nemesis::HkxBehavior;

const nemesis::regex
    nemesis::HkxBehavior::Parser::statergx_id("<hkobject name=\"(.+?)\" class=\"hkbStateMachineStateInfo\"");
const nemesis::regex nemesis::HkxBehavior::Parser::statergx("<hkparam name=\"stateId\">([0-9]+)<\\/hkparam>");

const nemesis::regex
    nemesis::HkxBehavior::Parser::smachinergx_id("<hkobject name=\"(.+?)\" class=\"hkbStateMachine\"");
const nemesis::regex nemesis::HkxBehavior::Parser::smachinergx_list("[\\s\\t]+(#[^\\s\\t]+?)(?=$|[\\s\\t])");

const USet<string> nemesis::HkxBehavior::Exporter::dataclasses
    = {"hkbBehaviorGraphStringData", "hkbVariableValueSet", "hkbBehaviorGraphData", "hkRootLevelContainer"};

const nemesis::regex nemesis::HkxBehavior::Exporter::datastr_rgx("<hkcstring>(.+?)<\\/hkcstring>");

const string nemesis::HkxBehavior::xmlstart = "\t<hksection name=\"__data__\">\n";
const string nemesis::HkxBehavior::xmlend   = "\t</hksection>\n\n</hkpackfile>\n";
const wstring nemesis::HkxBehavior::xmlext  = L".xml";
const nemesis::regex nemesis::HkxBehavior::nodergx_id("<hkobject name=\"(.+?)\" class=\"(.+?)\"");

nemesis::LinkedPreprocessLine* nemesis::HkxBehavior::Node::operator[](size_t index)
{
    return (begin() + index)->get();
}

size_t nemesis::HkxBehavior::Node::size() const
{
    size_t s = 0;
    auto itr1 = begin();
    auto itr2 = end();

    while (itr1 != itr2)
    {
        s++;
        itr1++;
    }

    return s;
}

Vec<SPtr<nemesis::LinkedPreprocessLine>>::iterator nemesis::HkxBehavior::Node::begin()
{
    if (!start) return Vec<SPtr<nemesis::LinkedPreprocessLine>>::iterator();

    return std::find_if(list->begin(), list->end(), [&](const SPtr<nemesis::LinkedPreprocessLine>& line_ptr) {
        return line_ptr.get() == start;
    });
}

Vec<SPtr<nemesis::LinkedPreprocessLine>>::iterator nemesis::HkxBehavior::Node::end()
{
    if (!ending) return Vec<SPtr<nemesis::LinkedPreprocessLine>>::iterator();

    auto itr
        = std::find_if(list->begin(), list->end(), [&](const SPtr<nemesis::LinkedPreprocessLine>& line_ptr) {
              return line_ptr.get() == ending;
          });

    if (itr == list->end()) return itr;

    return ++itr;
}

Vec<SPtr<nemesis::LinkedPreprocessLine>>::const_iterator nemesis::HkxBehavior::Node::begin() const
{
    if (!start) return Vec<SPtr<nemesis::LinkedPreprocessLine>>::const_iterator();

    return std::find_if(list->begin(), list->end(), [&](const SPtr<nemesis::LinkedPreprocessLine>& line_ptr) {
        return line_ptr.get() == start;
    });
}

Vec<SPtr<nemesis::LinkedPreprocessLine>>::const_iterator nemesis::HkxBehavior::Node::end() const
{
    if (!ending) return Vec<SPtr<nemesis::LinkedPreprocessLine>>::const_iterator();

    auto itr
        = std::find_if(list->begin(), list->end(), [&](const SPtr<nemesis::LinkedPreprocessLine>& line_ptr) {
              return line_ptr.get() == ending;
          });

    if (itr == list->end()) return itr;

    return ++itr;
}

Vec<SPtr<nemesis::LinkedPreprocessLine>>::iterator
nemesis::HkxBehavior::Node::insert(Vec<SPtr<nemesis::LinkedPreprocessLine>>::const_iterator itr,
                                   SPtr<nemesis::LinkedPreprocessLine> value)
{
    return list->insert(itr, value);
}

void nemesis::HkxBehavior::Node::push_back(SPtr<nemesis::LinkedPreprocessLine> lnkline_ptr)
{
    list->insert(end(), lnkline_ptr);
}

USet<const nemesis::TemplateClass*>& nemesis::HkxBehavior::Parser::GetTemplateClasses()
{
    if (!templateclasses.empty()) return templateclasses;

    if (hostref.templatelist.empty()) return templateclasses;

    for (auto& templt : hostref.templatelist)
    {
        templateclasses.insert(&templt->GetTemplateClass());
    }

    return templateclasses;
}

bool nemesis::HkxBehavior::Parser::TrySetNewNodeZone()
{
    if (!findnewnodezone) return false;

    auto raw = stream.back()->back()->GetRawPtr();

    if (!raw) return false;

    std::string classname;

    if (!Exporter::IsNewClass(*raw, classname)) return false;

    if (Exporter::IsDataClass(classname)) return false;

    findnewnodezone = false;
    hostref.newnode_zone = stream.front()->back().get();
    return true;
}

bool nemesis::HkxBehavior::Parser::TryGetNodeId(const std::string& line, std::string& nodeid)
{
    if (line.find(hkobj_prefix) == NOT_FOUND) return false;

    nemesis::smatch sm;

    if (!nemesis::regex_search(line, sm, nodergx_id)) return false;

    nodeid = sm.str(1);
    return true;
}

void nemesis::HkxBehavior::Parser::TrySaveTopNodeId(const std::string& nodeid)
{
    if (!topnode_active) return;

    if (!isOnlyNumber(nodeid)) return;

    size_t curid = std::stoi(nodeid);

    if (curid < hostref.topnodeid.Read()) return;

    hostref.topnodeid.Write().AsObject() = curid + 1;
}

void nemesis::HkxBehavior::Parser::TryCacheStateInfo(const std::string& line)
{
    if (!state_active) return;

    (this->*cachestate)(line);
}

void nemesis::HkxBehavior::Parser::TryCacheStateNodeId(const std::string& line)
{
    if (line.find(statemachineinfo_str) == NOT_FOUND) return;

    if (!nemesis::regex_search(line, statergx_id)) return;

    cachestate                       = &nemesis::HkxBehavior::Parser::TryCacheStateId;
    hostref.node_map[currentid].list = stream.back();
}

void nemesis::HkxBehavior::Parser::TryCacheStateId(const std::string& line)
{
    if (line.find(stateid_str) == NOT_FOUND) return;

    nemesis::smatch sm;

    if (!nemesis::regex_search(line, sm, statergx)) return;

    hostref.stateids[currentid] = std::stoi(sm.str(1));
    cachestate                  = &nemesis::HkxBehavior::Parser::TryCacheStateNodeId;
}

void nemesis::HkxBehavior::Parser::TryCacheStateMachineInfo(const std::string& line)
{
    if (!state_active) return;

    (this->*cachestatemachine)(line);
}

void nemesis::HkxBehavior::Parser::TryCacheTemplateSMInfo(const std::string& line,
                                                          const nemesis::ConditionInfo* conditioninfo)
{
    if (!state_active) return;

    if (conditioninfo || cscope->Empty()) return;

    switch (cscope->Back().GetType())
    {
        case nemesis::CondType::IF:
        case nemesis::CondType::FOREACH:
            break;
        default:
            return;
    }

    if (line.find("#") == NOT_FOUND) return;

    const nemesis::regex_iterator end;

    for (nemesis::regex_iterator itr(line, smachinergx_list); itr != end; ++itr)
    {
        hostref.statelinks[smachineid].emplace_back(itr->str(1));
    }
}

void nemesis::HkxBehavior::Parser::TryCacheStateMachineNodeId(const std::string& line)
{
    if (line.find(statemachine_str) == NOT_FOUND) return;

    nemesis::smatch sm;

    if (!nemesis::regex_search(line, sm, smachinergx_id)) return;

    smachineid        = sm.str(1);
    cachestatemachine = &nemesis::HkxBehavior::Parser::TryCacheStateMachineNum;
}

void nemesis::HkxBehavior::Parser::TryCacheStateMachineNum(const std::string& line)
{
    if (line.find(smachine_num) == NOT_FOUND) return;

    cachestatemachine = &nemesis::HkxBehavior::Parser::TryCacheStateMachineId;
}

void nemesis::HkxBehavior::Parser::TryCacheStateMachineId(const std::string& line)
{
    if (line.find(smachine_close) != NOT_FOUND)
    {
        cachestatemachine = &nemesis::HkxBehavior::Parser::TryCacheStateMachineNodeId;
        return;
    }

    const nemesis::regex_iterator end;

    for (nemesis::regex_iterator itr(line, smachinergx_list); itr != end; ++itr)
    {
        auto state = itr->str(1);
        hostref.statemachines[smachineid].emplace_back(make_pair(state, &hostref.stateids[state]));
    }
}

void nemesis::HkxBehavior::Parser::BonePatch(const sf::path& rigfile, int oribone)
{
    int bonenum = 0;

    if (hostref.sse)
    {
        FILE* bonefile = _wfsopen(rigfile.wstring().c_str(), L"r+b", 1);

        if (!bonefile) ErrorMessage(3002, rigfile, "Unable to access bone file");

        uint16_t num = 0;
        vector<char> chlist;
        chlist.reserve(50000);
        string line;
        line.reserve(50000);

        while (!feof(bonefile))
        {
            char c = fgetc(bonefile);
            chlist.push_back(c);
            line.push_back(c);
        }

        fclose(bonefile);
        size_t pos        = line.find("NPC Root [Root]");

        if (pos != NOT_FOUND && pos > 64)
        {
            pos -= 64;
            char* ch = reinterpret_cast<char*>(&num);
            ch[0]    = chlist[pos];
            ch[1]    = chlist[pos + 1];
        }

        if (error) throw nemesis::exception();

        if (oribone >= num) return;

        bonenum = num - oribone;
        newbone = true;
    }
    else
    {
        VecStr storeline;
        HkxCompiler::hkxcmdXmlInput(rigfile, storeline);

        for (auto& line : storeline)
        {
            if (line.find(parentindices) == NOT_FOUND) continue;

            size_t pos = line.find(parentindices) + parentindices.length();
            int num    = stoi(line.substr(pos, line.find("\">", pos) - pos));

            if (oribone >= num) continue;

            bonenum = num - oribone;
            newbone = true;
            break;
        }
    }

    if (error) throw nemesis::exception();

    DebugLogging(L"Bone patch complete. Rig File: " + rigfile.wstring());
}

bool nemesis::HkxBehavior::Parser::TryPatchBone(const nemesis::Line& line)
{
    size_t pos = line.find(bonemap);

    if (pos != NOT_FOUND)
    {
        pos += bonemap.length();
        oribone = stoi(line.substr(pos, line.find("\">", pos) - pos));

        if (rigfile.empty()) return false;

        sf::path curFile(file.GetFilePath().parent_path().parent_path());
        wstring wrigfile = curFile.wstring() + L"\\" + nemesis::transform_to<wstring>(rigfile);

        if (!sf::exists(wrigfile)) return false;

        BonePatch(wrigfile, oribone);
        return true;
    }

    pos = line.find(rigname);

    if (pos != NOT_FOUND)
    {
        pos += rigname.length();
        rigfile = line.substr(pos, line.find("</hkparam>", pos) - pos).ToString();

        if (oribone == 0) return false;

        sf::path curFile(file.GetFilePath().parent_path().parent_path());
        wstring wrigfile = curFile.wstring() + L"\\" + nemesis::transform_to<wstring>(rigfile);

        if (sf::exists(wrigfile)) return false;

        BonePatch(wrigfile, oribone);
        return true;
    }

    return false;
}

void nemesis::HkxBehavior::Parser::UpdateHightStateId()
{
    for (auto& pairlist : hostref.statemachines)
    {
        for (auto& pair : pairlist.second)
        {
            hostref.AddStateId(pairlist.first, *pair.second);
        }
    }

    for (auto& statelink : hostref.statelinks)
    {
        if (hostref.statemachines.find(statelink.first) != hostref.statemachines.end()) continue;

        hostref.AddStateId(statelink.first, 0);
    }
}

void nemesis::HkxBehavior::Parser::GetFileLines()
{
    stream.clear();
    int linestoremove = 1;
    bool done         = false;

    filelines = file.GetLines(
        [&](const std::string& line)
        {
            if (done) return line;

            if (line.find(hkobj_prefix) != NOT_FOUND)
            {
                done = true;
                return line;
            }

            if (error) throw nemesis::exception();

            linestoremove++;
            return line;
        });

    if (filelines.size() < 2)
    {
        hostref.rootnode = nemesis::Line("", file.GetFilePath());
    }
    else
    {
        hostref.rootnode = filelines.front();
        hostref.rootnode.append("\n");
        hostref.rootnode.append(filelines[1]);
    }

    if (linestoremove == 0) return;

    filelines.erase(filelines.begin(), filelines.begin() + linestoremove);
}

std::string nemesis::HkxBehavior::Parser::GetRegexClassNames()
{
    std::string classnames;

    for (auto& templt : hostref.templatelist)
    {
        classnames.append(templt->GetFileClassName());
        classnames.append("|");
    }

    if (!classnames.empty())
    {
        classnames.pop_back();
    }

    return classnames;
}

void nemesis::HkxBehavior::Parser::PrepareAllRegexes()
{
    PrepareStateIdRegex();
    PrepareNodeIdRegex();
    PrepareAnimObjRegex();
}

void nemesis::HkxBehavior::Parser::PrepareStateIdRegex()
{
    std::stringstream ss;
    ss << "\\$.*?((?:(";
    ss << GetRegexClassNames();
    ss << ")(?:_group\\[([0-9]*)\\]|)\\[(F|N|L|B|[0-9]*)\\]\\[S@(?=[0-9]+\\]\\$)|S@(?=[0-9]+\\$))([0-9]+)\\]?"
          ").*?\\$";
    stateid_rgx = std::make_unique<nemesis::regex>(ss.str());
}

void nemesis::HkxBehavior::Parser::PrepareNodeIdRegex()
{
    for (auto& templt : hostref.templatelist)
    {
        switch (templt->GetType())
        {
            case nemesis::File::FileType::SINGLE:
                AddAnimNodeIdRegex(templt->GetTemplateClass().GetName().ToString());
                break;
            case nemesis::File::FileType::GROUP:
                AddGroupNodeIdRegex(templt->GetTemplateClass().GetName().ToString());
                break;
            case nemesis::File::FileType::MASTER:
                AddMasterNodeIdRegex(templt->GetTemplateClass().GetName().ToString());
                break;
        }
    }
}

void nemesis::HkxBehavior::Parser::PrepareAnimObjRegex()
{
    std::stringstream ss;
    ss << "\\$.*?((?:(";
    ss << GetRegexClassNames();
    ss << ")(?:_group\\[([0-9]*)\\]|)\\[(F|N|L|B|[0-9]*)\\]\\[(?=AnimObject/[0-9]+\\])|(?=AnimObject/[0-9]+[^"
          "\\]]))AnimObject/[0-9]+\\]?).*?\\$";
    animobj_rgx = std::make_unique<nemesis::regex>(ss.str());
}

void nemesis::HkxBehavior::Parser::PrepareAllLexers()
{
    for (auto& templtclass : GetTemplateClasses())
    {
        PrepareVariableLexer(templtclass);

        std::string classname = templtclass->GetName().ToString();

        PrepareLexer("main_anim_event", classname, [templtclass](nemesis::ScopeInfo& scopeinfo) {
            return std::string(scopeinfo.GetAnim(templtclass)->GetAnimationName());
        });
        PrepareLexer("FilePath", classname, [templtclass](nemesis::ScopeInfo& scopeinfo) {
            return scopeinfo.GetAnim(templtclass)->GetAnimPath().string();
        });
        PrepareLexer("Index", classname, [templtclass](nemesis::ScopeInfo& scopeinfo) {
            return std::to_string(scopeinfo.GetAnim(templtclass)->GetBehaviorIndex());
        });
        PrepareLexer("GroupIndex", classname, [templtclass](nemesis::ScopeInfo& scopeinfo) {
            return std::to_string(scopeinfo.GetAnim(templtclass)->GetIndex());
        });
        PrepareLexer("ArrayIndex", classname, [templtclass](nemesis::ScopeInfo& scopeinfo) {
            return std::to_string(scopeinfo.GetAnim(templtclass)->GetArrayIndex());
        });
    }
}

void nemesis::HkxBehavior::Parser::PrepareLexer(const std::string& keyword,
                                                const std::string& classname,
                                                RtnFunc callback)
{
    auto uptr = std::make_unique<LexerSearch>(keyword);
    uptr->AddLexer(classname, callback);
    lexersearch_list.emplace_back(std::move(uptr));
}

void nemesis::HkxBehavior::Parser::PrepareVariableLexer(const nemesis::TemplateClass* templtclass)
{
    for (auto& option : templtclass->GetOptionModels().GetOptionList())
    {
        auto optname = option->GetName();

        for (auto& varptr : option->GetVariablesList())
        {
            auto varname = varptr->GetName();
            nemesis::AnimVarPtr::Lexer lexer(
                std::string(templtclass->GetName()), std::string(optname), std::string(varname));
            varlexer_list.emplace_back(std::make_pair(varname, lexer));
        }
    }
}

void nemesis::HkxBehavior::Parser::TryUpdateNode()
{
    if (currentid.empty()) return;

    if (currentid == currentid_late) return;

    TryEndNode();
    AddStartNode();
}

void nemesis::HkxBehavior::Parser::AddStartNode()
{
    currentid_late  = currentid;
    node_ptr        = &hostref.node_map[currentid_late];
    node_ptr->list  = stream.back();
    node_ptr->start = stream.back()->back().get();
}

void nemesis::HkxBehavior::Parser::TryEndNode()
{
    if (!node_ptr) return;

    auto line_ptr    = node_ptr->list != stream.back() ? node_ptr->list->back().get()
                                                       : stream.back()->at(stream.back()->size() - 2).get();
    node_ptr->ending = line_ptr;
}

void nemesis::HkxBehavior::Parser::ParseProcesses(const nemesis::Line& line)
{
    if (!process_active) return;

    processparser = std::make_unique<nemesis::ProcessParser>(*this);
    processparser->SetLine(line);
    processparser->Parse();
}

void nemesis::HkxBehavior::Parser::ParseLines(const VecNstr& storeline)
{
    size_t i = 0;
    stream.clear();
    stream.emplace_back(&hostref.lines);

    if (hostref.ischaracter)
    {
        for (i; i < storeline.size(); ++i)
        {
            if (error) throw nemesis::exception();

            auto& line = storeline[i];

            if (line.find(end) != NOT_FOUND) return;

            if (TryPatchBone(line)) break;

            ParseLine(line);
        }
    }

    for (i; i < storeline.size(); ++i)
    {
        auto& line = storeline[i];

        if (line.find(end) != NOT_FOUND) break;

        ParseLine(line);
    }
}

void nemesis::HkxBehavior::Parser::ReadFile(const std::filesystem::path& filepath)
{
    Base::ReadFile(filepath);
    GetFileLines();
}

void nemesis::HkxBehavior::Parser::TryCacheData(const nemesis::Line& line,
                                                const nemesis::ConditionInfo* conditioninfo)
{
    ParseProcesses(line);

    if (stream.size() > 1)
    {
        TryCacheTemplateSMInfo(line, conditioninfo);
        return;
    }

    TryGetNodeId(line, currentid);
    TryCacheStateInfo(line);
    TryCacheStateMachineInfo(line);
    TrySaveTopNodeId(currentid.front() == '#' ? currentid.substr(1) : currentid);
}

void nemesis::HkxBehavior::Parser::PostAddLineProcess(nemesis::LinkedPreprocessLine& linkedline)
{
    if (linkedline.HasCondition() || linkedline.GetRawPtr()->empty()) return;

    auto raw = linkedline.GetRawPtr();
    raw->AddBehavior(hostref);

    if (!processparser || processparser->IsEmpty()) return;

    processparser->ToLine(*raw);
}

void nemesis::HkxBehavior::Parser::AddLine(const nemesis::Line& line)
{
    Base::AddLine(line);
    TryUpdateNode();
    TrySetNewNodeZone();
}

nemesis::HkxBehavior::Parser::Parser(nemesis::HkxBehavior& host, const VecStr& behaviorpriotiy)
    : hostref(host)
    , Base(host)
{
    AddModPriority(behaviorpriotiy);
    ResetAsterCache();

    PrepareAllRegexes();
    PrepareAllLexers();
}

nemesis::HkxBehavior::Parser::~Parser()
{
    cscope.reset();
    DebugLogging(L"Parsing behavior complete: " + file.GetFilePath().wstring());
}

void nemesis::HkxBehavior::Parser::SetParseProcess(bool is_active) noexcept
{
    process_active = is_active;
}

void nemesis::HkxBehavior::Parser::SetParseState(bool is_active) noexcept
{
    state_active = is_active;
}

void nemesis::HkxBehavior::Parser::SetParseTopNode(bool is_active) noexcept
{
    topnode_active = is_active;
}

void nemesis::HkxBehavior::Parser::ParseFile()
{
    Base::ParseFile();
    UpdateHightStateId();
}

const nemesis::HkxBehavior& nemesis::HkxBehavior::Exporter::GetSelf()
{
    return static_cast<const nemesis::HkxBehavior&>(file);
}

void nemesis::HkxBehavior::Exporter::ProcessDataLine(const nemesis::Line& line)
{
    if (!isdata) return;

    if (line.find(charpropelem) != NOT_FOUND)
    {
        isdata = false;
        return;
    }
    else if (line.find(varelem) != NOT_FOUND)
    {
        isevent = false;
        return;
    }

    if (line.find(hkxstr_str) == NOT_FOUND) return;

    nemesis::smatch sm;

    if (!nemesis::regex_search(line.ToString(), sm, datastr_rgx)) return;

    size_t size       = sm.size();
    std::string value = sm.str(1);

    if (isevent)
    {
        events->Add(value, eventcounter++);
        return;
    }

    variables->Add(value, varcounter++);
}

void nemesis::HkxBehavior::Exporter::CompileBehavior(VecNstr& datalines, VecNstr& behaviorlines)
{
    std::string classname;
    auto& self = GetSelf();

    auto bhvfunc  = [&](const nemesis::Line& line) {
        behaviorlines.emplace_back(line);
    };
    auto datafunc = [&](const nemesis::Line& line) {
        ProcessDataLine(line);

        if (!isdata && line.find(eventelem) != NOT_FOUND) isdata = true;

        datalines.emplace_back(line);
    };

    std::function<void(const nemesis::Line&)> funcptr = bhvfunc;

    for (auto& line : self.lines)
    {
        VecNstr compiledlines = line->GetProcessedLines(*scopeinfo);

        for (auto& compiledline : compiledlines)
        {
            if (IsNewClass(compiledline, classname))
            {
                if (IsDataClass(classname))
                {
                    funcptr = datafunc;
                }
                else
                {
                    funcptr = bhvfunc;
                }
            }

            if (error) throw nemesis::exception();

            funcptr(compiledline);
        }
    }

    for (auto& line : behaviorlines)
    {
        CheckNumElement(line);
    }

    for (auto& line : datalines)
    {
        CheckNumElement(line);
    }

    if (error) throw nemesis::exception();
}

void nemesis::HkxBehavior::Exporter::CompileTemplate(VecNstr& templines)
{
    auto& self = GetSelf();

    if (self.queries.empty()) return;

    for (auto& each : self.templatelist)
    {
        auto templateptr = each;
        auto itr         = self.queries.find(templateptr);

        if (itr == self.queries.end()) continue;

        for (auto& query : self.queries.at(templateptr))
        {
            each->GetQueryResult(*query, templines, *this);

            if (error) throw nemesis::exception();
        }
    }

    if (error) throw nemesis::exception();
}

void nemesis::HkxBehavior::Exporter::CompileImport(VecNstr& importlines)
{
    auto& self = GetSelf();

    if (self.queries.empty()) return;

    auto* templt     = self.templatelist.front();
    auto& animtemplt = templt->GetTemplateClass().GetAnimTemplate();
    Vec<VecNstr> vec_lines;

    for (auto& animimport : animimportpair_list)
    {
        vec_lines.emplace_back(VecNstr());
        scopeinfo->SetCurrentImport(animimport.second.get());
        animimport.second->GetLines(vec_lines.back(), *this);
    }

    for (int i = vec_lines.size() - 1; i >= 0; i--)
    {
        auto& lines = vec_lines[i];
        importlines.insert(importlines.end(), lines.begin(), lines.end());
    }
}

void nemesis::HkxBehavior::Exporter::AddStateIdManager()
{
    scopeinfo->GenerateStateIdManager(GetSelf());
}

nemesis::HkxBehavior::Exporter::Exporter(const HkxBehavior& host, VecNstr& storeline)
    : linesref(storeline)
    , nemesis::Exporter(host)
{
    AddEvents();
    AddVariables();
    AddAttributes();
    AddStateIdManager();
}

const nemesis::AnimTemplate* nemesis::HkxBehavior::Exporter::GetAnimTemplate()
{
    return GetSelf().GetAnimTemplate();
}

const nemesis::TemplateClass* nemesis::HkxBehavior::Exporter::GetTemplateClass(const std::string& name)
{
    return GetAnimTemplate()->GetClass(name);
}

void nemesis::HkxBehavior::Exporter::Export()
{
    auto& self = GetSelf();
    topid      = self.topnodeid.Read();

    VecNstr templatelines;
    VecNstr datalines;
    VecNstr behaviorlines;
    VecNstr importlines;

    CompileBehavior(datalines, behaviorlines);
    CompileTemplate(templatelines);
    CompileImport(importlines);

    linesref.clear();
    linesref.reserve(datalines.size() + templatelines.size() + behaviorlines.size() + 3);
    linesref.emplace_back(self.rootnode + "\n\n");
    linesref.emplace_back(xmlstart);

    // datalines: must be initialized first before being possibly used by any node
    // templatelines: must be initizlized first before being used in base behavior
    linesref.insert(linesref.end(), datalines.begin(), datalines.end());
    linesref.insert(linesref.end(), importlines.begin(), importlines.end());
    linesref.insert(linesref.end(), templatelines.begin(), templatelines.end());
    linesref.insert(linesref.end(), behaviorlines.begin(), behaviorlines.end());

    linesref.emplace_back(xmlend);
}

bool nemesis::HkxBehavior::Exporter::IsTemplateActive(const std::string& name)
{
    auto* querylist_ptr = GetQueriesByTemplate(name);
    return querylist_ptr && !querylist_ptr->empty();
}

const Vec<const nemesis::AnimQuery*>*
nemesis::HkxBehavior::Exporter::GetQueriesByTemplate(const std::string& name)
{
    auto& self     = GetSelf();
    auto* classptr = self.animtemplate->GetClass(name);

    if (classptr == nullptr) ErrorMessage(1225, self.GetFilePath(), name);

    for (auto& queryinfo : self.queries)
    {
        if (classptr != &queryinfo.first->GetTemplateClass()) continue;

        return &queryinfo.second;
    }

    return nullptr;
}

bool nemesis::HkxBehavior::Exporter::IsDataClass(const std::string& classname)
{
    return dataclasses.find(classname) != dataclasses.end();
}

bool nemesis::HkxBehavior::Exporter::IsNewClass(const nemesis::Line& line, std::string& classname)
{
    if (line.find(hkobj_prefix) == NOT_FOUND) return false;

    nemesis::smatch sm;
    classname.clear();

    if (!nemesis::regex_search(line.ToString(), sm, nodergx_id)) return false;

    classname = sm.str(2);
    return true;
}

nemesis::HkxBehavior::HkxBehavior()
{
    classname = "Behavior Base";
}

void nemesis::HkxBehavior::CompileBehavior(VecNstr& contents)
{
    HkxBehavior::Exporter exporter(*this, contents);
    exporter.Export();
}

nemesis::HkxBehavior::Node* nemesis::HkxBehavior::TryGetNodeExist(const nemesis::Patch& patch)
{
    auto node_id = patch.GetFilePath().stem().string();
    auto itr     = node_map.find(node_id);

    if (itr == node_map.end()) return nullptr;

    return &itr->second;
}

void nemesis::HkxBehavior::AddNewNode(const nemesis::Patch& patch)
{
    auto itr = std::find_if(
        lines.begin(), lines.end(), [&](const SPtr<nemesis::LinkedPreprocessLine>& linkedline)
        {
              return linkedline.get() == newnode_zone;
        });
    itr--;
    auto contents = patch.contents;
    auto& node    = node_map[patch.GetFilePath().stem().string()];
    node.start    = contents.front().get();
    node.ending   = contents.back().get();

    if (!lines.empty() && (*itr)->HasCondition()
        && nemesis::iequals((*itr)->GetLastCondition().GetExpression(), patch.code))
    {
        auto list = &(*itr)->GetLastCondition().GetDataList();
        node.list = list;
        list->insert(list->end(), contents.begin(), contents.end());
        return;
    }

    auto modcode_line = CreateCondLine(patch);
    auto list         = &modcode_line->GetLastCondition().GetDataList();
    list->insert(list->end(), contents.begin(), contents.end());
    lines.insert(++itr, modcode_line);
    node.list = list;
}

void nemesis::HkxBehavior::PatchNode(nemesis::HkxBehavior::Node& node, const nemesis::Patch& patch)
{
    try
    {
        auto contents = patch.contents;
        auto p_itr    = contents.begin();

        //VecNstr templist;
        //VecNstr temp2list;
        //VecNstr temp3list;
        //VecNstr temp4list;

        //for (auto itr = node.begin(); itr < node.end(); ++itr)
        //{
        //    auto raw = *itr;

        //    if (!raw->GetRawPtr()) continue;

        //    templist.push_back(raw->GetRawPtr()->ToString());
        //}

        //for (size_t i = 0; i < contents.size(); i++)
        //{
        //    auto raw = contents[i]->GetRawPtr();

        //    if (!raw) continue;

        //    temp2list.push_back(raw->ToString());
        //}

        for (auto itr = node.begin(); itr < node.end(); ++itr)
        {
            auto& line = **itr;

            if (!line.GetRawPtr()) continue;

            //temp3list.push_back(line.GetRawPtr()->ToString());
            auto p_line = *p_itr++;

            //if (p_line->GetRawPtr())
            //{
            //    temp4list.push_back(p_line->GetRawPtr()->ToString());
            //}
            //else
            //{
            //    int i = 2;
            //}

            if (!p_line->HasCondition()) continue;

            if (!p_line->GetRawPtr())
            {
                do
                {
                    itr = ++node.insert(itr, p_line);
                    p_line->SetToStandAlone();
                    p_line = *p_itr++;
                } while (!p_line->GetRawPtr());

                //temp4list.push_back(p_line->GetRawPtr()->ToString());
                //std::string str1 = *p_line->GetRawPtr(), str2 = *line.GetRawPtr();

                //if (str1 != str2)
                //{
                //    int i = 0;
                //}

                if (!p_line->HasCondition()) continue;
            }

            line.AddCondition(p_line->GetLastCondition());
            p_line->SetToStandAlone();
        }

        //VecNstr temp5list;
        //
        //for (auto& itr = node.begin(); itr < node.end(); ++itr)
        //{
        //    auto raw = *itr;
        //    raw->GetRawData(temp5list);
        //}

        if (p_itr == contents.end()) return;

        auto cond_line = CreateCondLine(patch);
        auto& list     = cond_line->GetLastCondition().GetDataList();

        for (; p_itr != contents.end(); ++p_itr)
        {
            list.emplace_back(*p_itr);
        }

        node.push_back(cond_line);
    }
    catch (const std::exception&)
    {
        ErrorMessage(1231, patch.GetFilePath());
    }
}

SPtr<nemesis::LinkedPreprocessLine> nemesis::HkxBehavior::CreateCondLine(const nemesis::Patch& patch)
{
    return CreateCondLine(patch.code, patch.GetConditionType(), patch);
}

SPtr<nemesis::LinkedPreprocessLine> nemesis::HkxBehavior::CreateCondLine(const nemesis::Line& condition,
                                                                         nemesis::CondType type,
                                                                         const nemesis::File& file)
{
    auto cond_line = std::make_shared<nemesis::LinkedPreprocessLine>();
    ConditionInfo condinfo;
    condinfo.SetCondition(condition, type);
    cond_line->AddCondition(condinfo, file);
    return cond_line;
}

SPtr<nemesis::LinkedPreprocessLine> nemesis::HkxBehavior::CreateCondLine(const std::string& condition,
                                                                         nemesis::CondType type,
                                                                         const nemesis::File& file)
{
    nemesis::Line nline(condition, file.GetFilePath());
    return CreateCondLine(nline, type, file);
}

std::wstring nemesis::HkxBehavior::GetBehaviorName() const
{
    return (isfirstperson ? L"_1stperson\\" : L"") + filepath.stem().wstring();
}

const nemesis::AnimTemplate* nemesis::HkxBehavior::GetAnimTemplate() const noexcept
{
    return animtemplate;
}

void nemesis::HkxBehavior::PatchTemplateNode(nemesis::Template& templt)
{
    auto filename = templt.GetFilePath().stem().string();
    auto itr      = node_map.find(filename);

    if (itr == node_map.end()) return;

    auto& contents = templt.GetContents();

    for (size_t i = 0, b = 0; i < contents.size(); ++i, ++b)
    {
        auto* nodeline = itr->second[b];

        while (nodeline->HasCondition())
        {
            b++;
        }

        auto& templtline = contents[i];

        if (templtline->RawToString() == nodeline->RawToString())
        {
        }
    }

    // continue here
}

void nemesis::HkxBehavior::AddStateId(const string& smid, size_t id)
{
    if (topstate[smid] > id) return;

    topstate[smid] = id + 1;
}

void nemesis::HkxBehavior::AddSelection(const VecStr& _behaviorlist)
{
    behaviorinfo.behaviorlist = _behaviorlist;
    behaviorinfo.behaviorset.insert(behaviorinfo.behaviorlist.begin(), behaviorinfo.behaviorlist.end());
}

void nemesis::HkxBehavior::ClearSelection()
{
    behaviorinfo.Clear();
}

void nemesis::HkxBehavior::AddQueries(const VecSPtr<const nemesis::AnimQueryFile>& animqueries)
{
    USet<const nemesis::Template*> templateset;

    for (auto& each : templatelist)
    {
        templateset.insert(each);
    }

    for (auto& queryfile : animqueries)
    {
        AddQueriesByFile(*queryfile, templateset);
    }
}

void nemesis::HkxBehavior::AddQueries(const QueryManager& query_manager)
{
    if (type == FileType::NEMESIS_TEMPLATE)
    {
        queryfiles = &query_manager.GetFileList();
        return;
    }

    for (auto& templt : templatelist)
    {
        auto list_ptr = query_manager.GetQueryList(&templt->GetTemplateClass());

        if (list_ptr == nullptr) continue;

        for (auto& each : *list_ptr)
        {
            queries[templt].emplace_back(each);
        }
    }
}

void nemesis::HkxBehavior::AddQueriesByFile(const nemesis::AnimQueryFile& queryfile,
                                            USet<const nemesis::Template*>& templateset)
{
    for (auto& query : queryfile.GetList())
    {
        auto templateptr = query->MatchTemplate(templateset);

        if (templateptr == nullptr) continue;

        queries[templateptr].emplace_back(query);
    }
}

void nemesis::HkxBehavior::AddPatch(const nemesis::Patch& patch)
{
    std::string behavior = patch.GetFilePath().parent_path().stem().string();

    if (!nemesis::iequals(filepath.stem().string(), "nemesis_" + behavior)) return;

    auto node_ptr = TryGetNodeExist(patch);

    if (!node_ptr)
    {
        AddNewNode(patch);
        return;
    }

    PatchNode(*node_ptr, patch);
}

void nemesis::HkxBehavior::ClearQueries() noexcept
{
    queries.clear();
}

void nemesis::HkxBehavior::AddTemplate(const nemesis::Template& ntemplate)
{
    templatelist.emplace_back(&ntemplate);
}

void nemesis::HkxBehavior::ClearTemplateList() noexcept
{
    templatelist.clear();
}

void nemesis::HkxBehavior::AddAnimImport(const nemesis::Template& animimport)
{
    animimportlist.emplace_back(&animimport);
}

void nemesis::HkxBehavior::ClearAnimImportList() noexcept
{
    animimportlist.clear();
}

void nemesis::HkxBehavior::SetToFirstPerson()
{
    isfirstperson = true;
}

void nemesis::HkxBehavior::SetToCharacter()
{
    ischaracter = true;
}

void nemesis::HkxBehavior::SetToSSE()
{
    sse = true;
}

void nemesis::HkxBehavior::SetAnimTemplate(nemesis::AnimTemplate* animtemplate) noexcept
{
    this->animtemplate = animtemplate;
}

void nemesis::HkxBehavior::GetRawBehavior(VecNstr& storeline)
{
    storeline.emplace_back(rootnode + "\n");
    storeline.emplace_back(xmlstart);

    for (auto& each : lines)
    {
        each->GetRawData(storeline);
    }

    storeline.emplace_back(xmlend);
}

void nemesis::HkxBehavior::SaveStagedBehavior(const sf::path& outputpath) {}

void nemesis::HkxBehavior::SaveAs(const sf::path& outputpath, bool compile)
{
    sf::create_directories(outputpath.parent_path());
    ofstream outstream(outputpath.string());

    if (!outstream.is_open()) ErrorMessage(1025, outputpath);

    VecNstr contents;
    
    if (compile)
    {
        CompileBehavior(contents);
    }
    else
    {
        GetRawBehavior(contents);
    }

    for (auto& line : contents)
    {
        outstream << line.ToString() + "\n";
    }
}

void nemesis::HkxBehavior::SaveAsHkx(const std::filesystem::path& xmldir)
{
    try
    {
        constexpr std::string_view prefix = "nemesis_";
        std::wstring filename             = GetFilePath().filename().wstring().substr(prefix.length());
        sf::path shortpath
            = GetFilePath().wstring().substr(NemesisInfo::GetInstance()->GetDataPath().wstring().length());
        sf::path xmlpath = xmldir.wstring() + shortpath.wstring();
        xmlpath.replace_filename(filename);
        SaveAs(xmlpath, true);
        HkxCompiler::hkxcmdProcess(xmlpath, GetFilePath().replace_filename(filename));
        return;
    }
    catch (const nemesis::exception&)
    {
    }
#ifndef _DEBUG
    catch (const std::exception& ex)
    {
        ErrorMessage(6001, ex.what());
    }
#endif
}

void nemesis::HkxBehavior::SaveAsStaged(const std::filesystem::path& xmldir)
{
    try
    {
        constexpr std::string_view prefix = "nemesis_";
        std::wstring filename             = GetFilePath().filename().wstring().substr(prefix.length());
        sf::path shortpath
            = GetFilePath().wstring().substr(NemesisInfo::GetInstance()->GetDataPath().wstring().length());
        sf::path xmlpath = xmldir.wstring() + shortpath.wstring();
        xmlpath.replace_filename(filename);
        SaveAs(xmlpath, false);
    }
    catch (const nemesis::exception&)
    {
    }
#ifndef _DEBUG
    catch (const std::exception& ex)
    {
        ErrorMessage(6001, ex.what());
    }
#endif
}

void nemesis::HkxBehavior::SaveAsTemplateBehavior()
{
    try
    {
        if (!queryfiles || queryfiles->empty()) return;

        sf::path xmlpath = "temp_behaviors\\xml\\f";
        USet<const nemesis::Template*> templateset;

        for (auto& each : templatelist)
        {
            templateset.insert(each);
        }

        for (auto& file : *queryfiles)
        {
            AddQueriesByFile(*file, templateset);

            auto filename = file->GetFilePath().parent_path().filename();
            filename.replace_filename("nemesis_" + filename.string());
            filename.replace_extension(".xml");
            xmlpath.replace_filename(filename);
            SaveAs(xmlpath, true);
            return;

            filename.replace_extension(".hkx");
            HkxCompiler::hkxcmdProcess(xmlpath, filename);
            ClearQueries();
        }
    }
    catch (const nemesis::exception&)
    {
    }
#ifndef _DEBUG
    catch (const std::exception& ex)
    {
        ErrorMessage(6001, ex.what());
    }
#endif
}

void nemesis::HkxBehavior::ParseBehavior(const VecStr& behaviorpriority)
{
    Parser parser(*this, behaviorpriority);
    parser.SetParseProcess(false);
    parser.SetParseState(false);
    parser.SetParseTopNode(false);
    parser.ReadFile(filepath);
    parser.ParseFile();
}

bool nemesis::HkxBehavior::IsFirstPerson() const
{
    return isfirstperson;
}

bool nemesis::HkxBehavior::IsCharacter() const
{
    return ischaracter;
}

bool nemesis::HkxBehavior::IsSSE() const
{
    return sse;
}

SPtr<HB> nemesis::HkxBehavior::File(const sf::path& filepath)
{
    SPtr<HkxBehavior> bhvptr(new HkxBehavior());
    bhvptr->SetFilePath(filepath);

    auto wstrpath         = nemesis::to_lower_copy(filepath.wstring());
    auto file             = L"_1stperson\\" + filepath.filename().wstring();
    auto pos              = wstrpath.rfind(file);
    bhvptr->isfirstperson = wstrpath.length() - file.length() == pos;

    if (sf::is_directory(filepath)) ErrorMessage(3001, filepath);

    return bhvptr;
}

SPtr<HB> nemesis::HkxBehavior::ReadPatchedFile(const sf::path& filepath, const VecStr& behaviorpriority)
{
    auto bhv = File(filepath);
    bhv->ParseBehavior(behaviorpriority);
    return bhv;
}
//
//VecNstr outlines;
//
//void nemesis::HkxBehavior::AddFileLines()
//{
//    GetFileLines("temp_behaviors\\0_master.txt", outlines, false);
//
//    while (!outlines.empty())
//    {
//        if (outlines.front().find("signature=") != NOT_FOUND) break;
//
//        if (error) throw nemesis::exception();
//
//        outlines.erase(outlines.begin());
//    }
//}
