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
        size_t modcode_linecount;
        Vec<SPtr<nemesis::ConditionInfo>> scopelayers;
        SPtr<nemesis::ConditionInfo> tobedeleted;
        std::string modcode;
        std::filesystem::path path;

        void ScopeValidation(const nemesis::Line& line, const SPtr<nemesis::ConditionInfo>& current);
        void ScopeCountValidation(size_t linenum);
        void OriginalValidation(size_t linenum);
        void LowerOriginalValidation(size_t linenum);
        void EndIfValidation(size_t linenum);
        void ElseIfValidation(size_t linenum);
        void CloseValidation(size_t linenum);
        void ModCodeValidation(size_t linenum);

        void ReplaceScope(const SPtr<nemesis::ConditionInfo>& current);

    public:
        ConditionScope(const std::string& _modcode, const std::filesystem::path& _path, bool reverse = false);
        ~ConditionScope() noexcept(false);

        SPtr<nemesis::ConditionInfo>& operator[](size_t index);

        nemesis::ConditionInfo& Front();
        nemesis::ConditionInfo& Back();

        SPtr<nemesis::ConditionInfo> TryGetConditionInfo(const nemesis::Line& line);
        nemesis::ConditionInfo& GetToBeDeleted();
        std::string GetCurrentCondition() const;
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
