#include "Global.h"

#include "hkx/hkxbehavior.h"

#include "utilities/regex.h"
#include "utilities/template.h"
#include "utilities/readtextfile.h"
#include "utilities/conditionscope.h"
#include "utilities/conditionsyntax.h"

using namespace std;
namespace sf = filesystem;
namespace ns = nemesis::syntax;

const string_view nemesis::HkxBehavior::Parser::ignore    = "SERIALIZE_IGNORED";
const string_view nemesis::HkxBehavior::Parser::signature = "signature=";
const string_view nemesis::HkxBehavior::Parser::end       = "\t</hksection>";

const string nemesis::HkxBehavior::xmlstart  = "\t<hksection name=\"__data__\">\n";
const string nemesis::HkxBehavior::xmlend    = "\t</hksection>\n\n</hkpackfile>\n";
const wstring nemesis::HkxBehavior::xmlext   = L".xml";

void nemesis::HkxBehavior::Parser::BonePatch(const sf::path& rigfile, int oribone)
{
    int bonenum = 0;

    if (hostref.sse)
    {
        FILE* bonefile;
        _wfopen_s(&bonefile, rigfile.wstring().c_str(), L"r+b");

        if (!bonefile) ErrorMessage(3002, rigfile);

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
        chlist.shrink_to_fit();
        line.shrink_to_fit();

        using uchar     = unsigned char;
        bool startCount = false;
        bool start      = true;
        uint pos        = line.find("NPC Root [Root]");

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
        const string_view bonemap = R"(<hkparam name="parentIndices" numelements=")";

        for (auto& line : storeline)
        {
            if (line.find(bonemap) == NOT_FOUND) continue;

            size_t pos = line.find(bonemap) + bonemap.length();
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
    const string_view rigname = R"(<hkparam name="rigName">)";
    const string_view bonemap = R"(<hkparam name="bonePairMap" numelements=")";

    if (line.find(bonemap) != NOT_FOUND)
    {
        size_t pos = line.find(bonemap) + bonemap.length();
        oribone    = stoi(line.substr(pos, line.find("\">", pos) - pos));

        if (rigfile.empty()) return false;

        sf::path curFile(filepath);
        curFile          = curFile.parent_path().parent_path();
        wstring wrigfile = curFile.wstring() + L"\\" + nemesis::transform_to<wstring>(rigfile);

        if (!sf::exists(wrigfile)) return false;

        BonePatch(wrigfile, oribone);
        return true;
    }
    else if (line.find(rigname) != NOT_FOUND)
    {
        size_t pos = line.find(rigname) + rigname.length();
        rigfile    = line.substr(pos, line.find("</hkparam>", pos) - pos).ToString();

        if (oribone == 0) return false;

        sf::path curFile(filepath);
        curFile          = curFile.parent_path().parent_path();
        wstring wrigfile = curFile.wstring() + L"\\" + nemesis::transform_to<wstring>(rigfile);

        if (sf::exists(wrigfile)) return false;

        BonePatch(wrigfile, oribone);
        return true;
    }

    return false;
}

void nemesis::HkxBehavior::Parser::ReadBehaviorFile(VecNstr& storeline)
{
    GetFunctionLines(filepath, storeline, false);
    stream.clear();
    hostref.rootnode = storeline.size() < 1 ? "" : storeline[1];

    while (!storeline.empty())
    {
        if (storeline.front().find(signature) != NOT_FOUND) break;

        if (error) throw nemesis::exception();

        storeline.erase(storeline.begin());
    }
}

void nemesis::HkxBehavior::Parser::ParseLine(const nemesis::Line& line)
{
    if (error) throw nemesis::exception();

    if (line.empty() || line.find(ignore) != NOT_FOUND) return;

    auto conditioninfo = cscope->TryGetConditionInfo(line);

    if (!HasConditionMet(line, conditioninfo)) return;

    AddLine(line, conditioninfo);
}

void nemesis::HkxBehavior::Parser::ParseLines(const VecNstr& storeline)
{
    int i = 0;

    if (hostref.ischaracter)
    {
        for (i; i < storeline.size(); ++i)
        {
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

void nemesis::HkxBehavior::Parser::AddNewNode(const nemesis::Line& line)
{
    nemesis::Line id(string(HkxNode::GetID(line.ToString())), line.GetLineNumber());
    stream.emplace_back(HkxNode(id, hostref));
    stream.back().raw->InitializeStream();
}

void nemesis::HkxBehavior::Parser::AddLine(const nemesis::Line& line,
                                           const SPtr<nemesis::ConditionInfo>& conditioninfo)
{
    if (line.find(signature) != NOT_FOUND)
    {
        AddNewNode(line);
    }

    stream.back().raw->AddLine(line, conditioninfo);
}

bool nemesis::HkxBehavior::Parser::HasConditionMet(const nemesis::Line& line,
                                                   const SPtr<nemesis::ConditionInfo>& conditioninfo)
{
    if (!conditioninfo)
    {
        if (cscope->Empty()) return true;

        return success;
    }

    switch (conditioninfo->GetType())
    {
        case nemesis::CondType::ASTERISK:
        {
            AsteriskCondition(line);
            return false;
        }
        case nemesis::CondType::LOWER_ORIGINAL:
        {
            LowerOriginal(line, conditioninfo);
            return false;
        }
        case nemesis::CondType::MOD_CODE:
        {
            ModCodeCondition(conditioninfo);
            return false;
        }
        case nemesis::CondType::CLOSE:
        {
            auto tobedeleted = cscope->GetToBeDeleted();

            if (tobedeleted->GetType() != nemesis::CondType::MOD_CODE) return true;

            ModCloseCondition(tobedeleted);
            return false;
        }
        default:
            return true;
    }
}

void nemesis::HkxBehavior::Parser::AsteriskCondition(const nemesis::Line& line)
{
    auto condition = cscope->Back()->GetCondition();

    if (!priorities->Contains(condition)) return;

    if (!astercache.first || priorities->IsHigherThan(condition, astercache.first->GetCondition()))
    {
        AddToAsterCache(&line, cscope->Back());
    }
}

void nemesis::HkxBehavior::Parser::ModCodeCondition(const SPtr<nemesis::ConditionInfo>& conditioninfo)
{
    success = priorities->Contains(conditioninfo->GetCondition());
}

void nemesis::HkxBehavior::Parser::ModCloseCondition(const SPtr<nemesis::ConditionInfo>& tobedeleted)
{
    if (openmodcondinfo != tobedeleted) return;

    success = true;
}

void nemesis::HkxBehavior::Parser::LowerOriginal(const nemesis::Line& line,
                                                 const SPtr<nemesis::ConditionInfo>& conditioninfo)
{
    if (astercache.first)
    {
        AddLine(*astercache.second, astercache.first);
    }
    else
    {
        AddLine(line, conditioninfo);
    }

    ResetAsterCache();
}

void nemesis::HkxBehavior::Parser::ResetAsterCache()
{
    AddToAsterCache(nullptr, nullptr);
}

void nemesis::HkxBehavior::Parser::AddToAsterCache(const nemesis::Line* line,
                                                   SPtr<nemesis::ConditionInfo> conditioninfo)
{
    astercache = std::make_pair(conditioninfo, line);
}

nemesis::HkxBehavior::Parser::Parser(const string& modcode, const sf::path& filepath, HkxBehavior& host)
    : stream(host.nodes)
    , hostref(host)
{
    DebugLogging(L"Parsing behavior: " + filepath.wstring());
    this->cscope   = std::make_unique<nemesis::ConditionScope>(modcode, filepath);
    this->filepath = filepath;
    ResetAsterCache();
}

nemesis::HkxBehavior::Parser::~Parser()
{
    cscope.reset();
    DebugLogging(L"Parsing behavior complete: " + filepath.wstring());
}

VecNstr outlines;

void nemesis::HkxBehavior::Parser::GenerateNodes(const VecStr& behaviorpriority)
{
    priorities = make_unique<ModPriority>(behaviorpriority);
    VecNstr storeline = outlines;
    //ReadBehaviorFile(storeline);
    ParseLines(storeline);
}

void nemesis::HkxBehavior::Exporter::Compile()
{
    for (auto& curstack : hostref.templatelist)
    {
        bool uniqueskip = false;
        VecStr lineblocks;

        // continue here

        if (error) throw nemesis::exception();
    }
}

nemesis::HkxBehavior::Exporter::Exporter(const HkxBehavior& host, VecNstr& storeline)
    : hostref(host)
    , linesref(storeline)
{
}

void nemesis::HkxBehavior::Exporter::CompileNodes()
{
    for (auto& node : hostref.nodes)
    {
        CompileNode(node);
    }
}

void nemesis::HkxBehavior::Exporter::CompileNode(const LinkedNode& linkednode)
{
    if (linkednode.nestedcond.empty())
    {
    }
}

void nemesis::HkxBehavior::CompileBehavior(VecNstr& storeline)
{
    Exporter exporter(*this, storeline);
    exporter.Compile();
}

sf::path nemesis::HkxBehavior::GetFilePath() const
{
    return filepath;
}

std::wstring nemesis::HkxBehavior::GetBehaviorName() const
{
    return (isfirstperson ? L"_1stperson\\" : L"") + filepath.stem().wstring();
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

void nemesis::HkxBehavior::AddQueries(const VecSPtr<nemesis::AnimQueryList>& queries)
{
    this->queries = queries;
}

void nemesis::HkxBehavior::ClearQueries()
{
    queries.clear();
}

void nemesis::HkxBehavior::AddTemplateList(const VecSPtr<nemesis::Template> templatelist)
{
    this->templatelist = templatelist;
}

void nemesis::HkxBehavior::ClearTemplateList()
{
    templatelist.clear();
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

void nemesis::HkxBehavior::getlines(VecNstr& storeline)
{
    for (auto& each : nodes)
    {
        getLinkedLines(each, storeline);
    }
}

void nemesis::HkxBehavior::SaveStagedBehavior(const sf::path& outputpath) {}

void nemesis::HkxBehavior::SaveAsXml(const sf::path& outputpath)
{
    sf::path _outputpath;

    if (_outputpath.has_extension() || _outputpath.extension().wstring() != xmlext)
    {
        _outputpath.replace_extension(xmlext);
    }

    ofstream outstream(_outputpath.string());
    VecNstr contents;
    CompileBehavior(contents);

    if (!outstream.is_open()) ErrorMessage(1025, _outputpath);

    outstream << rootnode.ToString() + "\n\n";
    outstream << xmlstart;

    for (auto& line : contents)
    {
        outstream << line.ToString() + "\n";
    }

    outstream << xmlend;
    outstream.clear();
}

void nemesis::HkxBehavior::SaveAsHkx(const sf::path& outputpath)
{
    SaveAsXml(outputpath);
    HkxCompiler::hkxcmdProcess(outputpath, outputpath);
}

void nemesis::HkxBehavior::ReadAndProcess(const VecStr& behaviorpriority)
{
    Parser parser("Base", filepath, *this);
    parser.GenerateNodes(behaviorpriority);
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

SPtr<nemesis::HkxBehavior> nemesis::HkxBehavior::File(const sf::path& filepath)
{
    HkxBehavior hkxbhv; // must pre-create the object because of private constructor
    auto bhv      = std::make_shared<nemesis::HkxBehavior>(hkxbhv);
    bhv->filepath = filepath;

    auto wstrpath      = nemesis::to_lower_copy(filepath.wstring());
    auto file          = L"_1stperson\\" + filepath.stem().wstring();
    auto pos           = wstrpath.rfind(file);
    bhv->isfirstperson = wstrpath.length() - file.length() == pos;

    if (sf::is_directory(filepath)) ErrorMessage(3001, filepath);

    return bhv;
}

SPtr<nemesis::HkxBehavior> nemesis::HkxBehavior::ReadPatchedFile(const sf::path& filepath,
                                                                 const VecStr& behaviorpriority)
{
    auto bhv = File(filepath);
    bhv->ReadAndProcess(behaviorpriority);
    return bhv;
}

void nemesis::HkxBehavior::AddFileLines()
{
    GetFunctionLines("temp_behaviors\\0_master.txt", outlines, false);

    while (!outlines.empty())
    {
        if (outlines.front().find("signature=") != NOT_FOUND) break;

        if (error) throw nemesis::exception();

        outlines.erase(outlines.begin());
    }
}
