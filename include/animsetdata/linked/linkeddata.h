#pragma once

#include "core/linked.h"

namespace nemesis
{
    namespace animsetdata
    {
        template<typename T>
        struct LinkedData : nemesis::Linked<T, nemesis::animsetdata::LinkedData<T>>
        {
            using LinkedCondition = nemesis::LinkedCond<T, nemesis::animsetdata::LinkedData<T>>;

        public:
            LinkedData() = default;
            LinkedData(const nemesis::Line& line)
            {
                throw std::runtime_error("Invalid LinkedData constructor");
            }

            void SetRawDataTo(VecNstr& lines) const
            {
                auto templines = raw->GetLines();
                lines.insert(lines.end(), templines.begin(), templines.end());
            }

            void SetNewConditionedDataTo(VecNstr& lines,
                                         const nemesis::Line& newline,
                                         const LinkedCondition& cond) const override
            {
                throw std::runtime_error("animsetdata::LinkedData does not support SetNewConditionedDataTo");
            }

            void SetRawConditionedDataTo(VecNstr& lines) const override
            {
                throw std::runtime_error("animsetdata::LinkedState does not support SetRawConditionedDataTo");
            }
        };
    }
}
