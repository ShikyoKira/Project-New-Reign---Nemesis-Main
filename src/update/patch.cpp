#include "update/patch.h"

#include "template/processparser.h"

bool nemesis::Patch::PatchParser::IsModCodeScope()
{
    switch (cscope->GetToBeDeleted().GetType())
    {
        case nemesis::CondType::MOD_CODE:
        case nemesis::CondType::ORIGINAL:
            return true;
        default:
            return false;
    }
}

void nemesis::Patch::PatchParser::AddAllModLines()
{
    if (modlines_cache.empty())
    {
        modcondition_cache.reset();
        return;
    }

    AddConditionScope(modcondition_cache.get());
    auto& stream_ref = stream.back();

    for (auto& line : modlines_cache)
    {
        stream_ref->emplace_back(line);
    }

    RemoveConditionScope();
    modcondition_cache.reset();
    modlines_cache.clear();
}

void nemesis::Patch::PatchParser::AddLinkedLine(SPtr<nemesis::LinkedPreprocessLine> linkedline)
{
    if (cscope->Empty())
    {
        nemesis::HkxParserBase::AddLinkedLine(linkedline);
        return;
    }

    switch (cscope->Back().GetType())
    {
        case nemesis::CondType::MOD_CODE:
        {
            modlines_cache.emplace_back(linkedline);
            break;
        }
        case nemesis::CondType::ORIGINAL:
        {
            if (modlines_cache.empty())
            {
                ErrorMessage(
                    1229, patch_ref.code, patch_ref.GetFilePath(), linkedline->GetRawPtr()->GetLineNumber());
            }

            auto& stream_ref = *stream.back();
            stream_ref.emplace_back(linkedline);
            linkedline->AddCondition(*modcondition_cache, patch_ref);
            linkedline->GetLastCondition().GetDataList().emplace_back(modlines_cache.front());
            modlines_cache.pop_front();
            break;
        }
        default:
            nemesis::HkxParserBase::AddLinkedLine(linkedline);
            break;
    }
}

bool nemesis::Patch::PatchParser::TryProcessCondition(const nemesis::ConditionInfo* conditioninfo)
{
    if (!conditioninfo) return false;

    auto type = conditioninfo->GetType();

    switch (type)
    {
        case nemesis::CondType::MOD_CODE:
        {
            modcondition_cache = cscope->Back().shared_from_this();
        }
        case nemesis::CondType::ORIGINAL:
        {
            break;
        }
        case nemesis::CondType::IF:
        case nemesis::CondType::FOREACH:
        {
            AddConditionScope(conditioninfo);
            break;
        }
        case nemesis::CondType::ELSEIF:
        {
            UpdateConditionScope(conditioninfo);
            break;
        }
        case nemesis::CondType::ELSE:
        {
            UpdateConditionScope(conditioninfo);
            break;
        }
        case nemesis::CondType::ENDIF:
        {
            RemoveConditionScope();
            break;
        }
        case nemesis::CondType::CLOSE:
        {
            if (IsModCodeScope())
            {
                AddAllModLines();
                break;
            }

            RemoveConditionScope();
            break;
        }
        default:
            return false;
    }

    return true;
}

void nemesis::Patch::PatchParser::ParseLine(const nemesis::Line& line)
{
    if (error) throw nemesis::exception();

    if (!line.empty())
    {
        if (!IsValidLine(line)) return;

        auto conditioninfo = cscope->TryGetConditionInfo(line);

        if (TryProcessCondition(conditioninfo.get())) return;
    }

    Base::AddLine(line);
}

void nemesis::Patch::PatchParser::ParseLines(const VecNstr& lines)
{
    size_t i = 0;
    stream.clear();
    stream.emplace_back(&patch_ref.contents);

    for (auto& line : lines)
    {
        if (error) throw nemesis::exception();

        ParseLine(line);
    }
}

nemesis::Patch::PatchParser::PatchParser(nemesis::Patch& patch)
    : patch_ref(patch)
    , nemesis::HkxParserBase(patch)
{
}

void nemesis::Patch::PatchParser::ReadFile(const std::filesystem::path& filepath)
{
    if (!GetFileLines(filepath, filelines)) return;

    this->cscope = std::make_unique<nemesis::ConditionScope>(patch_ref.code, filepath);
}

nemesis::Patch::Patch(const std::string& code, const std::filesystem::path& filepath)
    : code(code)
{
    this->filepath = filepath;
}

VecNstr nemesis::Patch::GetRawData() const
{
    VecNstr lines;

    for (auto& each : contents)
    {
        each->GetRawData(lines);
    }

    return lines;
}

std::string_view nemesis::Patch::GetModCode() const
{
    return code;
}
