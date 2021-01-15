#pragma once

#include <any>

#include "utilities/conditioninfo.h"
#include "utilities/conditionsyntax.h"
#include "utilities/conditiondetails.h"

namespace nemesis
{
    struct ConditionScope
    {
    private:
        bool reverse;
        Vec<SPtr<nemesis::ConditionInfo>> scopelayers;
        SPtr<nemesis::ConditionInfo> tobedeleted;
        std::string modcode;
        std::filesystem::path path;

        void ScopeValidation(const nemesis::Line& line, const SPtr<nemesis::ConditionInfo>& current);
        void ScopeCountValidation(uint linenum);
        void OriginalValidation(uint linenum);
        void LowerOriginalValidation(uint linenum);
        void EndIfValidation(uint linenum);
        void ElseIfValidation(uint linenum);
        void CloseValidation(uint linenum);
        void ModCodeValidation(uint linenum);

        void ReplaceScope(const SPtr<nemesis::ConditionInfo>& current);

    public:
        ConditionScope(const std::string& _modcode, const std::filesystem::path& _path, bool reverse = false);
        ~ConditionScope() noexcept(false);

        SPtr<nemesis::ConditionInfo>& operator[](size_t index);

        SPtr<nemesis::ConditionInfo>& Front();
        SPtr<nemesis::ConditionInfo>& Back();

        SPtr<nemesis::ConditionInfo> TryGetConditionInfo(const nemesis::Line& line);
        SPtr<nemesis::ConditionInfo> GetToBeDeleted();
        std::string_view GetCurrentCondition() const;
        bool Empty() const noexcept;
        bool Reverse() const noexcept;
        size_t Size() const noexcept;
        void ScopeEnd() const;
        void Clear() noexcept;
        void Pop_Back();

        Vec<SPtr<nemesis::ConditionInfo>>::iterator begin();
        Vec<SPtr<nemesis::ConditionInfo>>::iterator end();
    };
}
