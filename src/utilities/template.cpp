#include "utilities/regex.h"
#include "utilities/process.h"
#include "utilities/template.h"
#include "utilities/lineprocess.h"
#include "utilities/templateclass.h"

using namespace std;
namespace sf = std::filesystem;

void nemesis::Template::Parser::LineParser::CommitProcess(const nemesis::Line& line)
{
    unique_ptr<nemesis::TemplateLine> templine = make_unique<nemesis::TemplateLine>(line);
    templine->TryAddLineProcess(pLineProcess);
    parser.rTemplate.AddLine(templine);
    ResetStagingProcess();
}

void nemesis::Template::Parser::LineParser::StagingProcess(std::unique_ptr<nemesis::Process>& process)
{
    processlist.emplace_back(move(process));
}

void nemesis::Template::Parser::LineParser::ResetStagingProcess()
{
    pLineProcess = make_shared<nemesis::LineProcess>();
}

nemesis::Template::Parser::LineParser::LineParser(const nemesis::Line& line, Parser& parser)
    : line(line)
    , parser(parser)
{
    ResetStagingProcess();
}

shared_ptr<nemesis::LineProcess> nemesis::Template::Parser::LineParser::GetLineProcess()
{
    return pLineProcess;
}

void nemesis::Template::Parser::LineParser::StartCountingElement()
{
    if (IsCountingElement())
    {
        string classname = parser.IsTemplateImport() ? "import" : parser.GetTemplateClassName();
        ErrorMessage(1136, classname, parser.GetFilePath(), line.GetLineNumber());
    }

    const char tab = '\t';
    const string openParam = "<hkparam name=\"";

    parser.bCountElement = true;
    string templine      = line.substr(0, line.find(openParam));
    parser.iElementRange = count(templine.begin(), templine.end(), tab);

    auto process = GenerateProcess(&nemesis::Process::Compute);
    process->SetFixedVarInt(vector<int>{parser.iElementRange});
    StagingProcess(process);
}

bool nemesis::Template::Parser::LineParser::HasNemesisSyntax()
{
    return line.find("$") != NOT_FOUND;
}

void nemesis::Template::Parser::LineParser::TryCountAnimation()
{
    const auto syntax = "$%$";
    size_t pos        = line.find(syntax);

    while (pos != NOT_FOUND)
    {
        auto process = GenerateProcess(pos, pos + 3, &nemesis::Process::AnimCount);
        StagingProcess(process);
        pos = line.find(syntax, pos + 1);
    }
}

void nemesis::Template::Parser::LineParser::TryAddValidNodeID()
{
    if (parser.IsTemplateImport()) return;

    string classname = parser.GetTemplateClassName();
    const string idrgx     = classname + "\\$([0-9]+)";
    const string grprgx    = classname + "_group\\$([0-9]+)";
    uint pos;

    if (parser.rTemplate.IsGroup())
    {
        for (auto& itr = nemesis::regex_iterator(line, nemesis::regex(idrgx));
             itr != nemesis::regex_iterator();
             ++itr)
        {
            auto pos  = itr->position();
            auto process = GenerateProcess(pos, pos + itr->str().length(), &nemesis::Process::IDRegisAnim);
            process->SetFixedVar(VecStr{itr->str(1), line});
            StagingProcess(process);
        }

        for (auto& itr = nemesis::regex_iterator(line, nemesis::regex(grprgx));
             itr != nemesis::regex_iterator();
             ++itr)
        {
            auto pos     = itr->position();
            auto process = GenerateProcess(pos, pos + itr->str().length(), &nemesis::Process::IDRegisGroup);
            process->SetFixedVar(VecStr{itr->str(1), line});
            StagingProcess(process);
        }

        return;
    }

    for (auto& itr = nemesis::regex_iterator(line, nemesis::regex(grprgx));
         itr != nemesis::regex_iterator();
         ++itr)
    {
        auto pos  = itr->position();
        auto process = GenerateProcess(pos, pos + itr->str().length(), &nemesis::Process::GroupIDRegis);
        process->SetFixedVar(VecStr{itr->str(1)});
        StagingProcess(process);
    }
}

bool nemesis::Template::Parser::LineParser::TryStartCountingElement()
{
    const string openParam      = "<hkparam name=\"";
    const string closeParam     = "</hkparam>";
    const string numElementName = "numelements=\"";
    const string computeFunc    = "<!-- COMPUTE -->";

    if (line.find(closeParam) != NOT_FOUND) return false;

    auto computepos = line.rfind(computeFunc);

    if (computepos == NOT_FOUND) return false;

    auto pos = line.find(openParam);

    if (pos == NOT_FOUND) return false;

    pos = line.find(numElementName, pos);

    if (pos == NOT_FOUND || pos > computepos) return false;

    StartCountingElement();
    return true;
}

bool nemesis::Template::Parser::LineParser::IsCountingElement() const
{
    return parser.bCountElement;
}

bool nemesis::Template::Parser::LineParser::TryRegisterAnimation()
{
    const string animname  = "animationName";

    if (line.find(animname) == NOT_FOUND) return false;

    const string animregex = HkxParamToRgxStr(animname);

    for (auto& itr = nemesis::regex_iterator(line, nemesis::regex(animregex));
         itr != nemesis::regex_iterator();
         ++itr)
    {
        size_t pos = itr->position(1);
        auto process = GenerateProcess(pos, pos + itr->str(1).length(), &nemesis::Process::RegisAnim);
        StagingProcess(move(process));
    }

    return true;
}

bool nemesis::Template::Parser::LineParser::TryRegisterBehavior()
{
    const string bhvname = "behaviorName";

    if (line.find(bhvname) == NOT_FOUND) return false;

    const string bhvregex = HkxParamToRgxStr(bhvname);

    for (auto& itr = nemesis::regex_iterator(line, nemesis::regex(bhvregex));
         itr != nemesis::regex_iterator();
         ++itr)
    {
        size_t pos = itr->position(1);
        auto process = GenerateProcess(pos, pos + itr->str(1).length(), &nemesis::Process::RegisBehavior);
        StagingProcess(move(process));
    }

    return true;
}

void nemesis::Template::Parser::LineParser::AutoEndAnim()
{
    if (!parser.bEnding) return;

    const string relativename  = "relativeToEndOfClip";
    const string timename      = "localTime";
    const string relativeregex = HkxParamToRgxStr(relativename);
    const string timeregex     = HkxParamToRgxStr(timename);

    if (line.find(relativename) != NOT_FOUND)
    {
        for (auto& itr = nemesis::regex_iterator(line, nemesis::regex(relativeregex));
             itr != nemesis::regex_iterator();
             ++itr)
        {
            parser.bEnding = false;
            size_t pos     = itr->position(1);
            StagingProcess(
                GenerateProcess(pos, pos + itr->str(1).length(), &nemesis::Process::RelativeNegative));
        }
    }
    else if (line.find(timename) != NOT_FOUND)
    {
        for (auto& itr = nemesis::regex_iterator(line, nemesis::regex(timeregex));
             itr != nemesis::regex_iterator();
             ++itr)
        {
            size_t pos = itr->position(1);
            StagingProcess(
                GenerateProcess(pos, pos + itr->str(1).length(), &nemesis::Process::LocalNegative));
        }
    }
}

void nemesis::Template::Parser::LineParser::AutoIdUpdate()
{
    uint pos = line.find("MID$");

    // set function ID
    if (pos != NOT_FOUND)
    {
        void (nemesis::Process::*func)(VecStr&, const AnimThreadInfo&) const;

        if (parser.rTemplate.IsMaster())
        {
            func = &nemesis::Process::IDRegisMaster;
        }
        else if (parser.rTemplate.IsGroup())
        {
            func = &nemesis::Process::IDRegisGroup;
        }
        else
        {
            func = &nemesis::Process::IDRegis;
        }

        for (auto& itr = nemesis::regex_iterator(line, nemesis::regex("MID\\$([0-9]+)"));
             itr != nemesis::regex_iterator();
             ++itr)
        {
            pos = itr->position();
            auto process = GenerateProcess(pos, pos + itr->str().length(), func);
            process->SetFixedVar(VecStr{itr->str(1), line});
            StagingProcess(process);
        }
    }
}

void nemesis::Template::Parser::LineParser::NodeIdRegister()
{
    string classname = parser.IsTemplateImport() ? "import" : parser.GetTemplateClassName();

    if (parser.rTemplate.IsGroup())
    {
        for (auto& itr = nemesis::regex_iterator(line, nemesis::regex(classname + "\\$([0-9]+)"));
             itr != nemesis::regex_iterator();
             ++itr)
        {
            uint pos     = itr->position();
            auto process = GenerateProcess(pos, pos + itr->str().length(), &nemesis::Process::IDRegisAnim);
            process->SetFixedVar(VecStr{itr->str(1), line});
            StagingProcess(process);
        }

        for (auto& itr = nemesis::regex_iterator(line, nemesis::regex(classname + "_group\\$([0-9]+)"));
             itr != nemesis::regex_iterator();
             ++itr)
        {
            uint pos     = itr->position();
            auto process = GenerateProcess(pos, pos + itr->str().length(), &nemesis::Process::IDRegisGroup);
            process->SetFixedVar(VecStr{itr->str(1), line});
            StagingProcess(process);
        }
    }
    else
    {
        for (auto& itr = nemesis::regex_iterator(line, nemesis::regex(classname + "_group\\$([0-9]+)"));
             itr != nemesis::regex_iterator();
             ++itr)
        {
            uint pos     = itr->position();
            auto process = GenerateProcess(pos, pos + itr->str().length(), &nemesis::Process::GroupIDRegis);
            process->SetFixedVar(VecStr{itr->str(1), line});
            StagingProcess(process);
        }
    }
}

void nemesis::Template::Parser::LineParser::StopCountingElement()
{
    parser.bCountElement = false;
    StagingProcess(GenerateProcess(&nemesis::Process::RangeCompute));
}

bool nemesis::Template::Parser::LineParser::TryStopCountingElement()
{
    if (!IsCountingElement()) return false;

    const string closeParam = "</hkparam>";
    const char tab          = '\t';

    auto pos                = line.find(closeParam);

    if (pos == NOT_FOUND) return false;

    string templine = line.substr(0, pos);

    if (parser.iElementRange != count(templine.begin(), templine.end(), tab)) return false;

    StopCountingElement();
    return true;
}

bool nemesis::Template::Parser::LineParser::TryCountingElement()
{
    if (!IsCountingElement()) return false;

    const char tab          = '\t';
    const string openObject = "<hkobject>";
    const string refElement = "\t\t\t#";
    const string refPrefix  = "#";
    funcptr pFunc;

    string templine = line;

    if (line.find(openObject) != NOT_FOUND)
    {
        templine = templine.substr(0, templine.find(openObject));
        pFunc    = &nemesis::Process::UpCounter;
    }
    else if (templine.find(refElement) != NOT_FOUND)
    {
        templine = templine.substr(0, templine.find(refPrefix));
        pFunc    = &nemesis::Process::UpCounterPlus;
    }
    else
    {
        return false;
    }

    if (parser.iElementRange + 1 != count(templine.begin(), templine.end(), tab)) return false;

    StagingProcess(move(GenerateProcess(0, 99999, pFunc)));
    return true;
}

string nemesis::Template::Parser::LineParser::HkxParamToRgxStr(string name)
{
    return R"(<hkparam name\=")" + name + R"(">(.+?)<\/hkparam>)";
}

unique_ptr<nemesis::Process> nemesis::Template::Parser::LineParser::GenerateProcess(funcptr func)
{
    return move(GenerateProcess(0, 0, func));
}

unique_ptr<nemesis::Process>
nemesis::Template::Parser::LineParser::GenerateProcess(uint begin, uint end, funcptr func)
{
    unique_ptr<nemesis::Process> process = make_unique<nemesis::Process>();
    process->SetBegin(begin);
    process->SetEnd(end);
    process->AddFunction(func);
    return move(process);
}

bool nemesis::Template::Parser::IsTemplateImport() const noexcept
{
    return rTemplate.IsImport();
}

const string& nemesis::Template::Parser::GetTemplateClassName() const
{
    return rTemplate.GetTemplateClassName();
}

const sf::path& nemesis::Template::Parser::GetFilePath() const noexcept
{
    return rTemplate.behaviorfile;
}

void nemesis::Template::Parser::ParseLines(const VecNstr& lineList)
{
    for (auto& line : lineList)
    {
        ParseLine(line);
    }
}

nemesis::Template::Parser::Parser(Template& host, const filesystem::path& filepath)
    : rTemplate(host)
{
    VecNstr lineList;
    GetFunctionLines(filepath, lineList);
    ParseLines(lineList);
}

nemesis::Template::Parser::Parser(Template& host, const VecNstr& lineList)
    : rTemplate(host)
{
    ParseLines(lineList);
}

void nemesis::Template::Parser::ParseLine(const nemesis::Line& line)
{
    bool hasProcess          = false;
    const uint& linenum      = line.GetLineNumber();
    shared_ptr<nemesis::LineProcess> pLineProcess;
    LineParser lineparser(line, *this);

    if (!lineparser.TryStartCountingElement())
    {
        if (!lineparser.TryCountingElement())
        {
            lineparser.TryStopCountingElement();
        }
    }

    lineparser.AutoEndAnim();
    lineparser.TryRegisterAnimation();
    lineparser.TryRegisterBehavior();

    if (lineparser.HasNemesisSyntax())
    {
        lineparser.TryCountAnimation();
        lineparser.TryAddValidNodeID();
        lineparser.AutoIdUpdate();
    }

    /*
    if (line.find("<hkparam name=\"") != NOT_FOUND && line.find("numelements=\"") != NOT_FOUND
        && line.find("</hkparam>") == NOT_FOUND
        && line.find("<!-- COMPUTE -->", line.find("numelements=\"")) != NOT_FOUND)
    {
        if (!IsCountingElement())
        {
            elementCount    = true;
            hasProcess      = true;
            string templine = line.substr(0, line.find("<hkparam name=\"", 0));
            iElementRange   = count(templine.begin(), templine.end(), '\t');
            process.installBlock(nemesis::scope(0, 0, vector<int>{openRange}, &proc::compute), linenum);
        }
        else
        {
            ErrorMessage(1136, GetFormat(), GetFilePath(), linenum);
        }
    }
    else if (elementCount && line.find("</hkparam>") != NOT_FOUND)
    {
        string templine = line.substr(0, line.find("</hkparam>"));

        if (openRange == count(templine.begin(), templine.end(), '\t'))
        {
            hasProcess = true;
            elementCount = false;
            process.installBlock(nemesis::scope(0, 0, &proc::rangeCompute), linenum);
        }
    }

    if (elementCount)
    {
        string_view templine = line;

        if (line.find("<hkobject>") != NOT_FOUND)
        {
            templine = templine.substr(0, templine.find("<hkobject>"));

            if (openRange + 1 == count(templine.begin(), templine.end(), '\t'))
            {
                hasProcess = true;
                process.installBlock(nemesis::scope(0, 99999, &proc::upCounter), linenum);
            }
        }
        else if (templine.find("\t\t\t#") != NOT_FOUND)
        {
            templine = templine.substr(0, templine.find("#", 0));

            if (openRange + 1 == count(templine.begin(), templine.end(), '\t'))
            {
                hasProcess = true;
                process.installBlock(nemesis::scope(0, 99999, &proc::upCounterPlus), linenum);
            }
        }
    }

    if (line.find("$") != NOT_FOUND)
    {
        nemesis::regex exp("(?<!MID)(?<!\\$MC)(?<!" + GetTemplateClassName() + "_master)(?<!" + GetTemplateClassName()
                           + R"(_group)(?<!\$%)\$(?!%\$)(?!MC\$)(?!elements\$)(.+?)(?<!MID)(?<!\$MC)(?<!)"
                           + GetTemplateClassName() + "_master)(?<!" + GetTemplateClassName()
                           + R"(_group)(?<!\$%)\$(?!%\$)(?!MC\$)(?!elements\$))");

        for (nemesis::regex_iterator itr(line, exp); itr != nemesis::regex_iterator(); ++itr)
        {
            bool isChange = false;
            string change = itr->str(1);
            size_t curPos = itr->position();
            process.brackets[linenum].emplace_back(itr->position());
            process.brackets[linenum].emplace_back(itr->position() + itr->str().length() - 1);
            map<int, vector<shared_ptr<nemesis::scope>>> dummy1;
            vector<AddOnInfo> dummy2;
            bool dummy3;
            ProcessFunction(change,
                            line,
                            GetTemplateClassName(),
                            GetFilePath(),
                            multiOption,
                            isEnd,
                            linenum,
                            curPos + 1,
                            optionlist,
                            dummy1,
                            dummy2,
                            dummy3,
                            isGroup,
                            isMaster,
                            false,
                            process);
            hasProcess = true;
        }

        size_t pos = line.find("$%$");

        if (pos != NOT_FOUND)
        {
            while (pos != NOT_FOUND)
            {
                process.installBlock(nemesis::scope(pos, pos + 3, &proc::animCount), linenum);
                pos = line.find("$%$", pos + 1);
            }

            hasProcess = true;
        }

        pos = line.find("$MC$");

        // multi choice selection
        if (pos != NOT_FOUND)
        {
            vector<nemesis::MultiChoice> m_conditions;
            process.hasMC.insert(linenum);

            for (auto& itr = nemesis::regex_iterator(line, nemesis::regex("[\\s]+<!-- (.+?) -->[\\s]*?"));
                 itr != nemesis::regex_iterator();
                 ++itr)
            {
                string output = itr->str(1);
                pos           = itr->position(1);
                VecStr curElements;
                StringSplit(output, curElements);

                if (curElements.size() == 1)
                {
                    m_conditions.emplace_back(nemesis::MultiChoice("",
                                                                   GetTemplateClassName(),
                                                                   GetFilePath(),
                                                                   multiOption,
                                                                   linenum,
                                                                   isGroup,
                                                                   isMaster,
                                                                   optionlist,
                                                                   pos,
                                                                   pos + output.length()));
                }
                else if (curElements.size() > 1)
                {
                    pos = pos + output.length();
                    m_conditions.emplace_back(nemesis::MultiChoice(curElements[0],
                                                                   GetTemplateClassName(),
                                                                   GetFilePath(),
                                                                   multiOption,
                                                                   linenum,
                                                                   isGroup,
                                                                   isMaster,
                                                                   optionlist,
                                                                   pos - curElements.back().length(),
                                                                   pos));
                }
            }

            for (auto& itr = nemesis::regex_iterator(line, nemesis::regex("\\$MC\\$"));
                 itr != nemesis::regex_iterator();
                 ++itr)
            {
                pos = itr->position();
                process.installBlock(nemesis::scope(pos, pos + itr->str().length(), &proc::multiChoiceRegis),
                                     linenum,
                                     m_conditions);
                hasProcess = true;
            }
        }

        // get group node ID
        if (isGroup)
        {
            for (auto& itr = nemesis::regex_iterator(line, nemesis::regex(GetTemplateClassName() + "\\$([0-9]+)"));
                 itr != nemesis::regex_iterator();
                 ++itr)
            {
                string ID = itr->str(1);
                pos       = itr->position();
                process.installBlock(
                    nemesis::scope(pos, pos + itr->str().length(), VecStr{ID, line}, &proc::IDRegisAnim),
                    linenum);
                hasProcess = true;
            }

            for (auto& itr = nemesis::regex_iterator(line, nemesis::regex(GetTemplateClassName() + "_group\\$([0-9]+)"));
                 itr != nemesis::regex_iterator();
                 ++itr)
            {
                string ID = itr->str(1);
                pos       = itr->position();
                process.installBlock(
                    nemesis::scope(pos, pos + itr->str().length(), VecStr{ID, line}, &proc::IDRegisGroup),
                    linenum);
                hasProcess = true;
            }
        }
        else
        {
            for (auto& itr = nemesis::regex_iterator(line, nemesis::regex(GetTemplateClassName() + "_group\\$([0-9]+)"));
                 itr != nemesis::regex_iterator();
                 ++itr)
            {
                string ID = itr->str(1);
                pos       = itr->position();
                process.installBlock(
                    nemesis::scope(pos, pos + itr->str().length(), VecStr{ID}, &proc::groupIDRegis), linenum);
                hasProcess = true;
            }
        }

        pos = line.find("MID$");

        // set function ID
        if (pos != NOT_FOUND)
        {
            if (GetTemplateClassName() == "fu")
            {
                isEnd = false;
            }

            void (proc::*func)(nemesis::scope, VecStr&, AnimThreadInfo&) const;

            if (isMaster)
            {
                func = &proc::IDRegisMaster;
            }
            else if (isGroup)
            {
                func = &proc::IDRegisGroup;
            }
            else
            {
                func = &proc::IDRegis;
            }

            for (auto& itr = nemesis::regex_iterator(line, nemesis::regex("MID\\$([0-9]+)"));
                 itr != nemesis::regex_iterator();
                 ++itr)
            {
                pos = itr->position();
                process.installBlock(
                    nemesis::scope(pos, pos + itr->str().length(), VecStr{itr->str(1), line}, func), linenum);
                hasProcess = true;
            }
        }
    }

    if (isEnd)
    {
        for (auto& itr = nemesis::regex_iterator(
                 line, nemesis::regex("<hkparam name\\=\"relativeToEndOfClip\">(.+?)<\\/hkparam>"));
             itr != nemesis::regex_iterator();
             ++itr)
        {
            isEnd      = false;
            size_t pos = itr->position(1);
            process.installBlock(nemesis::scope(pos, pos + itr->str(1).length(), &proc::relativeNegative),
                                 linenum);
        }

        for (auto& itr = nemesis::regex_iterator(
                 line, nemesis::regex("<hkparam name\\=\"localTime\">(.+?)<\\/hkparam>"));
             itr != nemesis::regex_iterator();
             ++itr)
        {
            hasProcess = true;
            size_t pos = itr->position(1);
            process.installBlock(nemesis::scope(pos, pos + itr->str(1).length(), &proc::localNegative),
                                 linenum);
        }
    }

    for (auto& itr = nemesis::regex_iterator(
             line, nemesis::regex("<hkparam name\\=\"animationName\">(.+?)<\\/hkparam>"));
         itr != nemesis::regex_iterator();
         ++itr)
    {
        hasProcess = true;
        size_t pos = itr->position(1);
        process.installBlock(nemesis::scope(pos, pos + itr->str(1).length(), &proc::regisAnim), linenum);
    }

    for (auto& itr = nemesis::regex_iterator(
             line, nemesis::regex("<hkparam name\\=\"behaviorName\">(.+?)<\\/hkparam>"));
         itr != nemesis::regex_iterator();
         ++itr)
    {
        hasProcess = true;
        size_t pos = itr->position(1);
        process.installBlock(nemesis::scope(pos, pos + itr->str(1).length(), &proc::regisBehavior), linenum);
    }
    */

    lineparser.CommitProcess(line);
}

void nemesis::Template::SetBehaviorFile(const sf::path& filepath)
{
    // import file does not have behavior file
    if (pTemplateClass.expired()) return;

    std::wstring pathstr = filepath.wstring();
    nemesis::to_lower(pathstr);
    std::wstring name = nemesis::transform_to(pTemplateClass.lock()->GetName().ToString()) + L"\\";
    behaviorfile      = sf::path(pathstr.substr(pathstr.rfind(name) + name.length()));
    behaviorfile      = behaviorfile.substr(0, behaviorfile.find(L"\\"));
    nemesis::to_lower(behaviorfile);
}

void nemesis::Template::SetFilePath(const sf::path& filepath)
{
    const wstring master = L"_master";
    const wstring group  = L"_group";
    this->filepath             = filepath;
    wstring lowername = nemesis::to_lower_copy(filepath.stem().wstring());
    bMaster           = lowername.rfind(master) == lowername.length() - master.length();
    bGroup            = bMaster || lowername.rfind(group) == lowername.length() - group.length();
    SetBehaviorFile(filepath);
}

nemesis::Template::Template() noexcept
{
    bMaster = false;
    bGroup  = false;
    bImport = true;
}

nemesis::Template::Template(const nemesis::TemplateClass& templateclass) noexcept
{
    pTemplateClass = templateclass.weak_from_this();
}

bool nemesis::Template::IsGroup() const noexcept
{
    return bGroup;
}

bool nemesis::Template::IsMaster() const noexcept
{
    return bMaster;
}

bool nemesis::Template::IsImport() const noexcept
{
    return bImport;
}

const std::string& nemesis::Template::GetTemplateClassName() const
{
    if (pTemplateClass.expired()) ErrorMessage(6002, filepath, "No class name for imported template");

    return pTemplateClass.lock()->GetName().ToString();
}

const sf::path& nemesis::Template::GetFilePath() const noexcept
{
    return filepath;
}

const std::wstring& nemesis::Template::GetBehaviorFileW() const noexcept
{
    return behaviorfile;
}

void nemesis::Template::ReadFile(const sf::path& filepath)
{
    SetFilePath(filepath);
    Parser parser(*this, filepath);
}

void nemesis::Template::AddLine(std::unique_ptr<nemesis::TemplateLine>& pline)
{
    contents.emplace_back(move(pline));
    contents.back()->AddTemplate(weak_from_this());
}
