#include <functional>

#include "Global.h"

#include "utilities/conditionscope.h"

#include "hkx/hkxbehavior.h"

using namespace std;
namespace ns = nemesis::syntax;

void nemesis::ConditionScope::ScopeValidation(const nemesis::Line& line, const SPtr<nemesis::ConditionInfo>& current)
{
    auto type = current->GetType();

    if (tobedeleted) tobedeleted.reset();

    switch (type)
    {
        case nemesis::CondType::ASTERISK:
        {
            if (!scopelayers.empty() && *scopelayers.back() == type) break;

            scopelayers.emplace_back(current);
            break;
        }
        case nemesis::CondType::IF:
        case nemesis::CondType::FOREACH:
        {
            scopelayers.emplace_back(current);
            break;
        }
        case nemesis::CondType::MOD_CODE:
        {
            ModCodeValidation(line.GetLineNumber());
            scopelayers.emplace_back(current);
            break;
        }
        case nemesis::CondType::ORIGINAL:
        {
            OriginalValidation(line.GetLineNumber());
            break;
        }
        case nemesis::CondType::ELSEIF:
        {
            ReplaceScope(current);
            break;
        }
        case nemesis::CondType::LOWER_ORIGINAL:
        {
            LowerOriginalValidation(line.GetLineNumber());
            Pop_Back();
            break;
        }
        case nemesis::CondType::ENDIF:
        {
            EndIfValidation(line.GetLineNumber());
            Pop_Back();
            break;
        }
        case nemesis::CondType::CLOSE:
        {
            CloseValidation(line.GetLineNumber());
            Pop_Back();
            break;
        }
        case nemesis::CondType::NONE:
        {
            if (scopelayers.empty()) break;

            scopelayers.back()->AddContent(line);
            break;
        }
    }
}

void nemesis::ConditionScope::ScopeCountValidation(size_t linenum)
{
    if (scopelayers.empty()) ErrorMessage(1118, modcode, path, linenum);
}

void nemesis::ConditionScope::OriginalValidation(size_t linenum)
{
    if (!scopelayers.empty() && *scopelayers.back() == nemesis::CondType::MOD_CODE) return;

    ErrorMessage(1216, modcode, path, linenum);
}

void nemesis::ConditionScope::LowerOriginalValidation(size_t linenum)
{
    if (!scopelayers.empty() && *scopelayers.back() == nemesis::CondType::ASTERISK) return;

    ErrorMessage(1209, modcode, path, linenum);
}

void nemesis::ConditionScope::EndIfValidation(size_t linenum)
{
    ScopeCountValidation(linenum);

    switch (scopelayers.back()->GetType())
    {
        case nemesis::CondType::IF:
        case nemesis::CondType::ELSEIF:
        case nemesis::CondType::ELSE:
            return;
    }

    ErrorMessage(1214, modcode, path, linenum);
}

void nemesis::ConditionScope::ElseIfValidation(size_t linenum)
{
    ScopeCountValidation(linenum);

    switch (scopelayers.back()->GetType())
    {
        case nemesis::CondType::IF:
        case nemesis::CondType::ELSEIF:
            return;
    }

    ErrorMessage(1119, modcode, path, linenum);
}

void nemesis::ConditionScope::CloseValidation(size_t linenum)
{
    ScopeCountValidation(linenum);

    switch (scopelayers.back()->GetType())
    {
        case nemesis::CondType::FOREACH:
        case nemesis::CondType::MOD_CODE:
        case nemesis::CondType::ORIGINAL:
            return;
    }

    ErrorMessage(1213, modcode, path, linenum);
}

void nemesis::ConditionScope::ModCodeValidation(size_t linenum)
{
    if (scopelayers.empty()) return;

    ErrorMessage(1217, modcode, path, linenum);
}

void nemesis::ConditionScope::ReplaceScope(const SPtr<nemesis::ConditionInfo>& current)
{
    tobedeleted        = scopelayers.back();
    scopelayers.back() = current;
}

nemesis::ConditionScope::ConditionScope(const string& _modcode, const filesystem::path& _path, bool reverse)
{
    modcode = _modcode;
    path    = _path;
}

nemesis::ConditionScope::~ConditionScope() noexcept(false)
{
    if (!error) ScopeEnd();
}

SPtr<nemesis::ConditionInfo>& nemesis::ConditionScope::operator[](size_t index)
{
    return scopelayers[index];
}

nemesis::ConditionInfo& nemesis::ConditionScope::Front()
{
    return *scopelayers.front();
}

nemesis::ConditionInfo& nemesis::ConditionScope::Back()
{
    return *scopelayers.back();
}

SPtr<nemesis::ConditionInfo> nemesis::ConditionScope::TryGetConditionInfo(const nemesis::Line& line)
{
    SPtr<nemesis::ConditionInfo> conditioninfo = make_shared<nemesis::ConditionInfo>(line);
    ScopeValidation(line, conditioninfo);
    return conditioninfo->GetType() == nemesis::CondType::NONE ? nullptr : conditioninfo;
}

nemesis::ConditionInfo& nemesis::ConditionScope::GetToBeDeleted()
{
    return *tobedeleted;
}

std::string nemesis::ConditionScope::GetCurrentCondition() const
{
    return scopelayers.back()->GetCondition();
}

bool nemesis::ConditionScope::Empty() const noexcept
{
    return scopelayers.empty();
}

bool nemesis::ConditionScope::Reverse() const noexcept
{
    return reverse;
}

size_t nemesis::ConditionScope::Size() const noexcept
{
    return scopelayers.size();
}

void nemesis::ConditionScope::ScopeEnd() const
{
    if (!scopelayers.empty()) ErrorMessage(1114, modcode, path, "Not Found");
}

void nemesis::ConditionScope::Clear() noexcept
{
    tobedeleted = nullptr;
    scopelayers.clear();
}

void nemesis::ConditionScope::Pop_Back()
{
    tobedeleted = scopelayers.back();
    scopelayers.pop_back();
}

Vec<SPtr<nemesis::ConditionInfo>>::iterator nemesis::ConditionScope::begin()
{
    return scopelayers.begin();
}

Vec<SPtr<nemesis::ConditionInfo>>::iterator nemesis::ConditionScope::end()
{
    return scopelayers.end();
}
