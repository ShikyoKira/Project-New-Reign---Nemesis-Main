#include "template/processparser.h"

#include "utilities/lineprocess.h"
#include "utilities/templatecategory.h"

const std::string relativename = "relativeToEndOfClip";
const std::string timename     = "localTime";
const std::string bhvname      = "behaviorName";
const std::string animname     = "animationName";

const nemesis::regex_iterator end;

nemesis::regex nemesis::ProcessParser::relative_rgx(HkxParamToRgxStr(relativename));
nemesis::regex nemesis::ProcessParser::time_rgx(HkxParamToRgxStr(timename));
nemesis::regex nemesis::ProcessParser::nodeid_rgx("#NID~([0-9]+)");
nemesis::regex nemesis::ProcessParser::bhv_rgx(HkxParamToRgxStr(bhvname));
nemesis::regex nemesis::ProcessParser::anim_rgx(HkxParamToRgxStr(animname));
nemesis::regex nemesis::ProcessParser::import_rgx("#\\$import\\[(.+?)\\](?=\\$)");
nemesis::regex nemesis::ProcessParser::multichoice_rgx("<\\!--\\s(?:(.+?)\\s|)(.+?)\\s-->");
nemesis::regex nemesis::ProcessParser::process_rgx("(\\$)([^\\$]+?)(\\$)");

UMap<std::string, nemesis::regex> nemesis::ProcessParser::idregex_byclass;
UMap<std::string, nemesis::regex> nemesis::ProcessParser::groupidregex_byclass;

nemesis::ProcessParser::Position::Position(nemesis::regex_iterator& itr, size_t index)
{
    begin = itr->position(index);
    end   = begin + itr->str(index).length();
}

nemesis::ProcessParser::Position::Position(nemesis::AnimVarPtr::Lexer::Iterator& itr)
{
    begin = itr.pos;
    end   = begin + itr.match.length();
}

nemesis::ProcessParser::Position::Position(size_t begin, size_t end)
{
    this->begin = begin;
    this->end   = end;
}

bool nemesis::ProcessParser::Position::IsWithin(const Position& pos)
{
    if (begin < pos.begin) return false;

    return end <= pos.end;
}

void nemesis::ProcessParser::Reset()
{
    lineprocessptr = std::make_unique<nemesis::LineProcess>();
    processlist.clear();
}

void nemesis::ProcessParser::StageProcess(UPtr<nemesis::Process>& process)
{
    processlist.emplace_back(std::move(process));
}

nemesis::regex nemesis::ProcessParser::GetNodeIdRegex()
{
    auto name = std::string(importer.file.GetFileClassName());
    auto itr  = idregex_byclass.find(name);

    if (itr != idregex_byclass.end()) return itr->second;

    nemesis::regex rgx("#" + name + nodeidSuffix);
    idregex_byclass[name] = rgx;
    return rgx;
}

nemesis::regex nemesis::ProcessParser::GetGroupNodeIdRegex()
{
    auto name = std::string(importer.file.GetFileClassName());
    auto itr  = groupidregex_byclass.find(name);

    if (itr != groupidregex_byclass.end()) return itr->second;

    nemesis::regex rgx("#" + name + grpidSuffix);
    groupidregex_byclass[name] = rgx;
    return rgx;
}

bool nemesis::ProcessParser::HasNemesisSyntax()
{
    auto counter = std::count(lineptr->begin(), lineptr->end(), '$');

    if (counter < 2) return false;

    bool result = false;

    for (nemesis::regex_iterator itr(*lineptr, process_rgx); itr != end; ++itr)
    {
        int begin    = itr->position(1);
        int end      = itr->position(3);
        auto process = GenerateProcess(0, -1, &nemesis::Process::RemoveProcessSyntax);
        process->SetFixedVarInt(Vec<int>{begin, end});
        StageProcess(process);
        result = true;
    }

    return result;
}

void nemesis::ProcessParser::TryCountAnimation()
{
    size_t pos = lineptr->find(count_syntax);

    while (pos != NOT_FOUND)
    {
        auto process = GenerateProcess(pos, pos + 3, &nemesis::Process::AnimCount);
        StageProcess(process);
        pos = lineptr->find(count_syntax, pos + 1);
    }
}

void nemesis::ProcessParser::TryAddValidNodeID()
{
    return;

    //if (importer.GetFile().IsTemplateImport()) return;

    auto idrgx  = GetNodeIdRegex();
    auto grprgx = GetGroupNodeIdRegex();

    //if (!parser.rTemplate.IsGroup())
    {
        for (nemesis::regex_iterator itr(*lineptr, grprgx); itr != nemesis::regex_iterator(); ++itr)
        {
            auto process = GenerateProcess(Position(itr), &nemesis::Process::GroupIDRegis);
            process->SetFixedVar(VecStr{itr->str(1)});
            StageProcess(process);
        }

        return;
    }

    for (nemesis::regex_iterator itr(*lineptr, idrgx); itr != nemesis::regex_iterator(); ++itr)
    {
        auto process = GenerateProcess(Position(itr), &nemesis::Process::IDRegisAnim);
        process->SetFixedVar(VecStr{itr->str(1), *lineptr});
        StageProcess(process);
    }

    for (nemesis::regex_iterator itr(*lineptr, grprgx); itr != nemesis::regex_iterator(); ++itr)
    {
        auto process = GenerateProcess(Position(itr), &nemesis::Process::IDRegisGroup);
        process->SetFixedVar(VecStr{itr->str(1), *lineptr});
        StageProcess(process);
    }
}

bool nemesis::ProcessParser::TryStartCountingElement()
{
    if (lineptr->find(closeParam) != NOT_FOUND) return false;

    auto computepos = lineptr->rfind(computeFunc);

    if (computepos == NOT_FOUND) return false;

    auto pos = lineptr->find(openParam);

    if (pos == NOT_FOUND) return false;

    pos = lineptr->find(numelements, pos);

    if (pos == NOT_FOUND || pos > computepos) return false;

    StartCountingElement();
    return true;
}

bool nemesis::ProcessParser::TryCountingElement()
{
    if (!IsCountingElement()) return false;

    FuncPtr func;
    std::string templine = *lineptr;

    if (lineptr->find(openObject) != NOT_FOUND)
    {
        templine = templine.substr(0, templine.find(openObject));
        func     = &nemesis::Process::UpCounter;
    }
    else if (templine.find(refIdElement) != NOT_FOUND)
    {
        templine = templine.substr(0, templine.find(refIdPrefix));
        func     = &nemesis::Process::UpCounterPlus;
    }
    else
    {
        return false;
    }

    if (importer.iElementRange + 1 != std::count(templine.begin(), templine.end(), tab)) return false;

    auto process = GenerateProcess(0, lineptr->length(), func);
    StageProcess(process);
    return true;
}

bool nemesis::ProcessParser::TryStopCountingElement()
{
    if (!IsCountingElement()) return false;

    auto pos = lineptr->find(closeParam);

    if (pos == NOT_FOUND) return false;

    std::string templine = lineptr->substr(0, pos);

    if (importer.iElementRange != std::count(templine.begin(), templine.end(), tab)) return false;

    StopCountingElement();
    return true;
}

bool nemesis::ProcessParser::IsCountingElement()
{
    return importer.bCountElement;
}

bool nemesis::ProcessParser::TryRegisterAnimation()
{
    if (lineptr->find(animname) == NOT_FOUND) return false;

    for (nemesis::regex_iterator itr(*lineptr, anim_rgx); itr != nemesis::regex_iterator(); ++itr)
    {
        auto process = GenerateProcess(Position(itr, 1), &nemesis::Process::RegisAnim);
        StageProcess(process);
    }

    return true;
}

bool nemesis::ProcessParser::TryRegisterBehavior()
{
    if (lineptr->find(bhvname) == NOT_FOUND) return false;

    for (nemesis::regex_iterator itr(*lineptr, bhv_rgx); itr != nemesis::regex_iterator(); ++itr)
    {
        auto process = GenerateProcess(Position(itr, 1), &nemesis::Process::RegisBehavior);
        StageProcess(process);
    }

    return true;
}

void nemesis::ProcessParser::AutoEndAnim()
{
    if (!importer.bEnding) return;

    if (lineptr->find(timename) != NOT_FOUND)
    {
        for (nemesis::regex_iterator itr(*lineptr, time_rgx); itr != nemesis::regex_iterator();
             ++itr)
        {
            auto process = GenerateProcess(Position(itr, 1), &nemesis::Process::LocalNegative);
            StageProcess(process);
        }

        return;
    }

    if (lineptr->find(relativename) == NOT_FOUND) return;

    for (nemesis::regex_iterator itr(*lineptr, relative_rgx); itr != nemesis::regex_iterator();
         ++itr)
    {
        importer.bEnding = false;
        auto process     = GenerateProcess(Position(itr, 1), &nemesis::Process::RelativeNegative);
        StageProcess(process);
    }
}

void nemesis::ProcessParser::AutoIdUpdate()
{
    if (lineptr->find(nodeidPrefix) != NOT_FOUND)
    {
        for (nemesis::regex_iterator itr(*lineptr, nodeid_rgx); itr != end; ++itr)
        {
            auto process = GenerateProcess(Position(itr), &nemesis::Process::IDRegis);
            process->SetFixedVar(VecStr{itr->str(0)});
            process->SetFixedVarInt(Vec<int>{static_cast<int>(lineptr->GetLineNumber())});
            StageProcess(process);
        }
    }

    for (auto& rgx_search : importer.animidrgx_list)
    {
        if (!rgx_search.IsMatch(*lineptr)) continue;

        for (nemesis::regex_iterator itr(*lineptr, rgx_search.GetRegex()); itr != end; ++itr)
        {
            auto pos = itr->position(0);
            auto process = GenerateProcess(Position(itr), &nemesis::Process::IDRegisAnim);
            process->SetFixedVar(VecStr{
                itr->str(1), itr->str(0).replace(itr->position(1) - pos, itr->str(1).length(), "NID")});
            process->SetFixedVarInt(Vec<int>{stoi(itr->str(2)), static_cast<int>(lineptr->GetLineNumber())});
            StageProcess(process);
        }
    }

    for (auto& rgx_search : importer.groupidrgx_list)
    {
        if (!rgx_search.IsMatch(*lineptr)) continue;

        for (nemesis::regex_iterator itr(*lineptr, rgx_search.GetRegex()); itr != end; ++itr)
        {
            auto pos = itr->position(0);
            auto process
                = GenerateProcess(pos + 1, pos + itr->str(0).length(), &nemesis::Process::IDRegisGroup);
            process->SetFixedVar(VecStr{
                itr->str(1), itr->str(0).replace(itr->position(1) - pos, itr->str(1).length(), "NID")});
            process->SetFixedVarInt(Vec<int>{static_cast<int>(lineptr->GetLineNumber())});
            StageProcess(process);
        }
    }

    for (auto& rgx_search : importer.masteridrgx_list)
    {
        if (!rgx_search.IsMatch(*lineptr)) continue;

        for (nemesis::regex_iterator itr(*lineptr, rgx_search.GetRegex()); itr != end; ++itr)
        {
            auto pos = itr->position(0);
            auto process
                = GenerateProcess(pos + 1, pos + itr->str(0).length(), &nemesis::Process::IDRegisMaster);
            process->SetFixedVar(VecStr{
                itr->str(1), itr->str(0).replace(itr->position(1) - pos, itr->str(1).length(), "NID")});
            process->SetFixedVarInt(Vec<int>{static_cast<int>(lineptr->GetLineNumber())});
            StageProcess(process);
        }
    }
}

void nemesis::ProcessParser::NodeIdRegister()
{
    FuncPtr func;
    nemesis::regex rgx;
    nemesis::regex group_rgx;
    const nemesis::Template::Parser* parser = dynamic_cast<const nemesis::Template::Parser*>(&importer);

    if (parser && parser->rTemplate.IsImport())
    {
        rgx       = import_rgx;
        //group_rgx = groupimport_rgx;
    }
    else
    {
        rgx       = GetNodeIdRegex();
        group_rgx = GetGroupNodeIdRegex();
    }

    if (parser->rTemplate.IsGroup())
    {
        for (nemesis::regex_iterator itr(*lineptr, rgx); itr != nemesis::regex_iterator(); ++itr)
        {
            auto process = GenerateProcess(Position(itr), &nemesis::Process::IDRegisAnim);
            process->SetFixedVar(VecStr{itr->str(1), *lineptr});
            StageProcess(process);
        }

        func = &nemesis::Process::IDRegisGroup;
    }
    else
    {
        func = &nemesis::Process::GroupIDRegis;
    }

    for (nemesis::regex_iterator itr(*lineptr, group_rgx); itr != nemesis::regex_iterator(); ++itr)
    {
        auto process = GenerateProcess(Position(itr), func);
        process->SetFixedVar(VecStr{itr->str(1), *lineptr});
        StageProcess(process);
    }
}

void nemesis::ProcessParser::StartCountingElement()
{
    if (IsCountingElement())
    {
        auto classname = importer.GetFile().GetFileClassName();
        ErrorMessage(1136, classname, importer.GetFile().GetFilePath(), lineptr->GetLineNumber());
    }

    const char tab         = '\t';
    importer.bCountElement = true;
    std::string templine   = lineptr->substr(0, lineptr->find(openParam));
    importer.iElementRange = count(templine.begin(), templine.end(), tab);

    auto process = GenerateProcess(&nemesis::Process::Compute);
    process->SetFixedVarInt(std::vector<int>{importer.iElementRange});
    StageProcess(process);
}

void nemesis::ProcessParser::StopCountingElement()
{
    importer.bCountElement = false;
    auto process           = GenerateProcess(&nemesis::Process::RangeCompute);
    StageProcess(process);
}

void nemesis::ProcessParser::TryCountState()
{
    constexpr std::string_view keyword = "S@";

    if (lineptr->find(keyword) == NOT_FOUND) return;

    for (nemesis::regex_iterator itr(*lineptr, *importer.stateid_rgx); itr != end; ++itr)
    {
        auto pos     = itr->position(1);
        auto express = itr->str(1);
        auto process = GenerateProcess(pos, pos + itr->str(1).length(), &nemesis::Process::StateSingle);
        process->AddAnimVarPtr(
            std::make_unique<AnimVarPtr>(express, lineptr->GetLineNumber(), importer.GetFile()));
        StageProcess(process);
    }
}

void nemesis::ProcessParser::TryRegisterMultiChoice()
{
    int mc_pos = lineptr->find(mc_syntax);

    if (mc_pos == NOT_FOUND) return;

    nemesis::smatch smtch;

    for (nemesis::regex_iterator itr(*lineptr, multichoice_rgx); itr != end; ++itr)
    {
        auto pos       = itr->position(2);
        auto condition
            = std::make_unique<Condition>(itr->str(1), *lineptr, importer.GetFile(), CondType::MULTI_CHOICE);
        lineprocessptr->AddChoice(pos, pos + itr->str(2).length(), condition);
    }

    auto process = GenerateProcess(0, 0, &nemesis::Process::MultiChoicePre);
    process = GenerateProcess(0, -1, &nemesis::Process::MultiChoicePost);
    process->SetFixedVarInt(Vec<int>{mc_pos, static_cast<int>(mc_pos + mc_syntax.length())});
    StageProcess(process);
}

void nemesis::ProcessParser::TryOptionAddVariable()
{
    for (auto& lexer : importer.varlexer_list)
    {
        if (lineptr->find(lexer.first) == NOT_FOUND) continue;

        nemesis::AnimVarPtr::Lexer::Iterator end;

        for (auto itr = lexer.second.TokenizeString(*lineptr); itr != end; ++itr)
        {
            nemesis::AnimVarPtr::Parser parser(itr.groups, lineptr->GetLineNumber(), importer.GetFile());
            size_t pos   = itr.pos;
            auto varptr  = parser.Parse();
            auto process = GenerateProcess(pos, pos + itr.match.length(), &nemesis::Process::AddOnSingle);
            process->AddAnimVarPtr(std::move(varptr));
            int firstpos = lineptr->rfind("$", pos + 1);
            int lastpos  = lineptr->find("$", pos + 1);
            process->SetFixedVarInt(Vec<int>{firstpos, lastpos});
            StageProcess(process);
        }
    }
}

void nemesis::ProcessParser::TryAddHkxEventProcessor()
{
    TryAnimVarProcessor("HkxEvent", &nemesis::Process::HkxEvent);
}

void nemesis::ProcessParser::TryAddVariableIdProcessor()
{
    TryAnimVarProcessor("variableId", &nemesis::Process::VariableID);
}

void nemesis::ProcessParser::TryAddAnimObject()
{
    if (lineptr->find(animobj_str) == NOT_FOUND) return;

    for (nemesis::regex_iterator itr(*lineptr, *importer.animobj_rgx); itr != end; ++itr)
    {
        auto match   = itr->str(1);
        auto pos     = itr->position(1);
        auto varptr  = std::make_unique<AnimVarPtr>(match, lineptr->GetLineNumber(), importer.GetFile());
        auto process = GenerateProcess(pos, pos + match.length(), &nemesis::Process::AnimVarPtr);
        process->AddAnimVarPtr(std::move(varptr));
        StageProcess(process);
    }
}

void nemesis::ProcessParser::TryAddImportTemplateRef()
{
    if (lineptr->find(import_str) == NOT_FOUND) return;

    Vec<Pair<nemesis::ProcessParser::Position, int>> importindex_list;
    Vec<nemesis::ProcessParser::Position> importnode_list;
    TryGetImport(*lineptr, importindex_list, importnode_list);

    for (auto& importnode : importnode_list)
    {
        auto process = GenerateProcess(importnode, &nemesis::Process::ImportNode);
        StageProcess(process);
    }

    for (auto& pair : importindex_list)
    {
        auto process = GenerateProcess(pair.first, &nemesis::Process::ImportIndex);
        process->SetFixedVarInt(Vec<int>{pair.second});
        StageProcess(process);
    }
}

void nemesis::ProcessParser::TryAddProcessWithLexerList()
{
    nemesis::AnimVarPtr::Lexer::Iterator end;
    Vec<Tuple<Position, nemesis::AnimVarPtr::Lexer::Iterator, nemesis::AnimVarPtr::RtnFunc>> itrtuple_list;
    Vec<Position> installed_positions;

    for (auto& lexersearch : importer.lexersearch_list)
    {
        if (!lexersearch->IsMatch(*lineptr)) continue;

        for (auto& lexer : lexersearch->GetLexerList())
        {
            for (auto itr = lexer.TokenizeString(*lineptr); itr != end; ++itr)
            {
                if (itr.groups.size() == 1)
                {
                    itrtuple_list.emplace_back(std::make_tuple(Position(itr), itr, lexer.GetResultFunc()));
                    continue;
                }

                AddProcessByLexerIterator(itr, &nemesis::Process::AnimVarPtr, lexer.GetResultFunc());
                installed_positions.emplace_back(Position(itr));
            }
        }
    }

    for (auto& itr_tuple : itrtuple_list)
    {
        if (IsWithinAny(std::get<0>(itr_tuple), installed_positions)) continue;

        AddProcessByLexerIterator(
            std::get<1>(itr_tuple), &nemesis::Process::AnimVarPtr, std::get<2>(itr_tuple));
    }
}

void nemesis::ProcessParser::TryAnimVarProcessor(const std::string& syntax, FuncPtr func)
{
    if (nemesis::isearch(*lineptr, syntax + "[") == NOT_FOUND) return;

    for (nemesis::regex_iterator itr(*lineptr, process_rgx); itr != end; ++itr)
    {
        int begin = nemesis::isearch(itr->str(2), syntax + "[");

        if (begin == NOT_FOUND) continue;

        begin += itr->position(2);
        size_t counter   = 1;
        int end        = 0;
        int eventbegin = begin + syntax.length() + 1;

        for (size_t i = eventbegin; i < lineptr->length(); ++i)
        {
            const char& ch = (*lineptr)[i];

            if (ch == '[')
            {
                counter++;
            }
            else if (ch == ']')
            {
                if (--counter != 0) continue;

                end = i;
                break;
            }
        }

        auto process = GenerateProcess(begin, end, func);
        process->SetFixedVarInt(Vec<int>{eventbegin, end, static_cast<int>(itr->position(1))});
        StageProcess(process);
    }
}

void nemesis::ProcessParser::TryGetImport(const std::string& line,
                                          Vec<Pair<nemesis::ProcessParser::Position, int>>& importindex_list,
                                          Vec<nemesis::ProcessParser::Position>& importnode_list)
{
    importindex_list.clear();
    importnode_list.clear();
    constexpr std::string_view importindex_surfix = "]$";

    size_t pos = line.find(import_str);

    if (pos == NOT_FOUND) return;

    std::string_view sv(line);
    size_t pos2 = sv.find(importindex_surfix, pos + 1);

    if (pos2 == NOT_FOUND) return;

    std::string nline = line;
    sv                = nline;

    do
    {
        pos += import_str.length();
        std::string substr = nline.substr(pos, pos2 - pos);

        if (isOnlyNumber(substr))
        {
            int index = std::stoi(substr);
            pos += (1 * (index != 0) - 1 * (index == 0));
            importindex_list.emplace_back(std::make_pair(
                Position(pos - import_str.length(), pos2 + importindex_surfix.length()), index));
        }
        else
        {
            importnode_list.emplace_back(
                Position(pos - import_str.length() - 1, pos2 + importindex_surfix.length() - 1));
        }

        nline.erase(0, pos2 + importindex_surfix.length());
        pos  = nline.find(import_str);
        pos2 = sv.find(importindex_surfix, pos);
    } while (pos != NOT_FOUND && pos2 != NOT_FOUND);
}

//void nemesis::ProcessParser::AddMainAnimEventProcessor(nemesis::AnimVarPtr::Lexer::Iterator& itr)
//{
//    auto pos = itr.pos;
//    nemesis::AnimVarPtr::Parser parser(itr.groups, lineptr->GetLineNumber(), importer.GetFile());
//    auto varptr  = parser.Parse();
//    auto process = GenerateProcess(pos, pos + itr.match.length(), &nemesis::Process::MAESingle);
//    process->AddAnimVarPtr(std::move(varptr));
//    StageProcess(process);
//}

void nemesis::ProcessParser::AddProcessByLexerIterator(nemesis::AnimVarPtr::Lexer::Iterator& itr,
                                                       FuncPtr callback)
{
    auto pos = itr.pos;
    nemesis::AnimVarPtr::Parser parser(itr.groups, lineptr->GetLineNumber(), importer.GetFile());
    auto varptr  = parser.Parse();
    auto process = GenerateProcess(pos, pos + itr.match.length(), callback);
    process->AddAnimVarPtr(std::move(varptr));
    StageProcess(process);
}

void nemesis::ProcessParser::AddProcessByLexerIterator(nemesis::AnimVarPtr::Lexer::Iterator& itr,
                                                       FuncPtr callback,
                                                       RtnFunc selector)
{
    auto pos = itr.pos;
    nemesis::AnimVarPtr::Parser parser(itr.groups, lineptr->GetLineNumber(), importer.GetFile());
    parser.SetSelector(selector);
    auto varptr  = parser.Parse();
    auto process = GenerateProcess(pos, pos + itr.match.length(), callback);
    process->AddAnimVarPtr(std::move(varptr));
    StageProcess(process);
}

bool nemesis::ProcessParser::IsWithinAny(Position& pos, Vec<Position> installed_positions)
{
    for (auto& each : installed_positions)
    {
        if (pos.IsWithin(each)) return true;
    }

    return false;
}

nemesis::ProcessParser::ProcessParser(nemesis::ImporterBase& parser)
    : importer(parser)
{
    lineprocessptr = std::make_shared<nemesis::LineProcess>();
}

void nemesis::ProcessParser::ToLine(nemesis::PreprocessLine& line)
{
    lineprocessptr->AddProcessLinePtr(&line);

    for (auto& processptr : processlist)
    {
        processptr->AddProcessLinePtr(&line);
        lineprocessptr->AddProcess(processptr);
    }

    line.TryAddLineProcess(lineprocessptr);
}

void nemesis::ProcessParser::SetLine(const nemesis::Line& lineref)
{
    lineptr = &lineref;
}

void nemesis::ProcessParser::Parse()
{
    if (!TryStartCountingElement())
    {
        if (!TryCountingElement())
        {
            TryStopCountingElement();
        }
    }

    AutoEndAnim();
    TryRegisterAnimation();
    TryRegisterBehavior();

    if (HasNemesisSyntax())
    {
        TryCountAnimation();
        TryCountState();
        TryOptionAddVariable();
        TryAddHkxEventProcessor();
        TryAddVariableIdProcessor();
        TryAddAnimObject();
        TryAddImportTemplateRef();
        TryAddProcessWithLexerList();
    }

    TryAddValidNodeID();
    AutoIdUpdate();
    TryRegisterMultiChoice();
}

bool nemesis::ProcessParser::IsEmpty() const
{
    return processlist.empty();
}

std::string nemesis::ProcessParser::HkxParamToRgxStr(std::string name)
{
    return R"(<hkparam name\=")" + name + R"(">(.+?)<\/hkparam>)";
}

UPtr<nemesis::Process> nemesis::ProcessParser::GenerateProcess(FuncPtr func)
{
    return std::move(GenerateProcess(0, 0, func));
}

UPtr<nemesis::Process> nemesis::ProcessParser::GenerateProcess(Position pos, FuncPtr func)
{
    return std::move(GenerateProcess(pos.begin, pos.end, func));
}

UPtr<nemesis::Process> nemesis::ProcessParser::GenerateProcess(size_t begin, size_t end, FuncPtr func)
{
    UPtr<nemesis::Process> process = std::make_unique<nemesis::Process>();
    process->SetBegin(begin);
    process->SetEnd(end);
    process->AddFunction(func);
    return std::move(process);
}
