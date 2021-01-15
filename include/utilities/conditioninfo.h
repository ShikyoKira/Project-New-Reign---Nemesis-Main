#pragma once

#include <string>
#include "utilities/conditiondetails.h"

namespace nemesis
{
    struct ConditionInfo
    {
    private:
        nemesis::Line condition;
        nemesis::CondType type;
        Deq<nemesis::Line> contents;

    public:
        ConditionInfo(const std::string& line);
        ConditionInfo(const nemesis::Line& line);

        bool operator==(const std::string& condition) const noexcept;
        bool operator==(const ConditionInfo& condition) const noexcept;
        bool operator==(const nemesis::CondType type) const noexcept;

        bool operator!=(const std::string& condition) const noexcept;
        bool operator!=(const ConditionInfo& condition) const noexcept;
        bool operator!=(const nemesis::CondType type) const noexcept;

        std::string& operator=(const std::string& line);
        nemesis::Line& operator=(const nemesis::Line& line);

        void AddContent(const nemesis::Line& line);
        void Pop_Front();
        void Pop_Back();

        Deq<nemesis::Line>& GetRefContents() noexcept;
        const Deq<nemesis::Line>& GetContents() const noexcept;
        const std::string& GetCondition() const noexcept;
        uint GetLineNumber() const noexcept;
        const nemesis::CondType& GetType() const noexcept;
    };
}
