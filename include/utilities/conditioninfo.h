#pragma once

#include "utilities/conditiondetails.h"

namespace nemesis
{
    struct ConditionInfo : std::enable_shared_from_this<ConditionInfo>
    {
    private:
        nemesis::Line condition;
        const nemesis::Line* refline = nullptr;
        nemesis::CondType type = nemesis::CondType::NONE;
        Deq<nemesis::Line> contents;
        bool Success = true;

    public:
        ConditionInfo() = default;
        ConditionInfo(const nemesis::Line& line);

        bool operator==(const std::string& condition) const noexcept;
        bool operator==(const ConditionInfo& condition) const noexcept;
        bool operator==(const nemesis::CondType type) const noexcept;

        bool operator!=(const std::string& condition) const noexcept;
        bool operator!=(const ConditionInfo& condition) const noexcept;
        bool operator!=(const nemesis::CondType type) const noexcept;

        bool IsSuccess() const noexcept;

        std::string& operator=(const std::string& line);
        nemesis::Line& operator=(const nemesis::Line& line);

        void AddContent(const nemesis::Line& line);
        void Pop_Front();
        void Pop_Back();

        void SetCondition(const std::string& condition, nemesis::CondType type);
        void SetCondition(const nemesis::Line& condition, nemesis::CondType type);

        Deq<nemesis::Line>& GetRefContents() noexcept;
        const Deq<nemesis::Line>& GetContents() const noexcept;
        std::string GetCondition() const noexcept;
        const nemesis::Line& GetRefLine() const noexcept;
        size_t GetLineNumber() const noexcept;
        nemesis::CondType GetType() const noexcept;
    };
}
