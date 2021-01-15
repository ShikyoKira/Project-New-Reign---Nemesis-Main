#pragma once

#include <functional>

#include "utilities/types.h"
#include "utilities/linkedvar.h"
#include "utilities/conditionscope.h"

namespace nemesis
{
    template<typename T>
    struct LinkedParser
    {
        using FuncPtr = std::function<void()>;
        using ParseFuncPtr = std::function<void(const nemesis::Line&)>;

        short type = 0;
        uint index = 0;

        Vec<Vec<nemesis::LinkedVar<T>>*> stream;
        SPtr<nemesis::ConditionInfo> tobedeleted;

    private:
        UPtr<ConditionScope> cscope;

        Vec<std::function<bool(const nemesis::Line&)>> typeconditions;
        Vec<std::function<void(nemesis::LinkedVar<std::string>&)>> typerefs;

        FuncPtr pointinglist;
        FuncPtr pointingoriginal;
        FuncPtr closinglist;

        ParseFuncPtr to_parse;

        void InParseLine(const nemesis::Line& line)
        {
            auto conditioninfo = cscope->TryGetConditionInfo(line);

            if (!conditioninfo)
            {
                if (cscope->Empty())
                {
                    pointinglist();
                }
                else if (cscope->Back()->GetType() == nemesis::CondType::ORIGINAL)
                {
                    pointingoriginal();
                }
                else
                {
                    stream.back()->emplace_back(line.ToString());
                }

                return;
            }

            switch (conditioninfo->GetType())
            {
                case nemesis::CondType::IF:
                case nemesis::CondType::FOREACH:
                {
                    auto cond = nemesis::CondVar<T>(*conditioninfo);
                    Vec<nemesis::LinkedVar<T>>* listptr;

                    stream.back()->emplace_back(nemesis::LinkedVar<T>(cond, line.GetLineNumber()));
                    listptr = &stream.back()->back().backCond().rawlist;
                    stream.emplace_back(listptr);
                    break;
                }
                case nemesis::CondType::ELSEIF:
                {
                    auto& cond    = nemesis::CondVar<T>(*conditioninfo);
                    stream.back() = &stream.back()->back().addCond(cond).rawlist;
                    break;
                }
                case nemesis::CondType::ELSE:
                {
                    auto& cond    = nemesis::CondVar<T>(conditioninfo->GetType());
                    stream.back() = &stream.back()->back().addCond(cond).rawlist;
                    break;
                }
                case nemesis::CondType::ENDIF:
                {
                    stream.pop_back();
                    break;
                }
                case nemesis::CondType::ORIGINAL:
                {
                    tobedeleted = cscope->GetToBeDeleted();
                    break;
                }
                case nemesis::CondType::CLOSE:
                {
                    if (!tobedeleted)
                    {
                        tobedeleted = cscope->GetToBeDeleted();
                    }

                    closinglist();
                    tobedeleted = nullptr;
                    break;
                }
            }
        }

        void NotMatch(const nemesis::Line& line)
        {
            ErrorMessage(1219);
        }

        void StateValidation() noexcept
        {
            if (typerefs.size() != typeconditions.size() || pointinglist == nullptr
                || pointingoriginal == nullptr || closinglist == nullptr)
            {
                to_parse = std::bind(&LinkedParser<T>::NotMatch, this, std::placeholders::_1);
                return;
            }

            to_parse = std::bind(&LinkedParser<T>::InParseLine, this, std::placeholders::_1);
        }

    public:
        LinkedParser() = default;

        void ParseLine(const nemesis::Line& line);

        void SetTypeCondition(std::function<bool(const nemesis::Line&)> func);
        void SetTypeRef(std::function<void(nemesis::LinkedVar<std::string>&)> func);

        void SetPointingListFunc(FuncPtr func) noexcept
        {
            pointinglist = func;
            StateValidation();
        }

        void SetPointingOriginalFunc(FuncPtr func) noexcept
        {
            pointingoriginal = func;
            StateValidation();
        }

        void SetClosingListFunc(FuncPtr func) noexcept
        {
            closinglist = func;
            StateValidation();
        }

        static nemesis::LinkedParser<T> CreateParser(Vec<nemesis::LinkedVar<T>>& list,
                                                     const std::string& modcode,
                                                     const std::filesystem::path& path);
    };
} // namespace nemesis
