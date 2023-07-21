#pragma once

#include "core/linked.h"

#include "utilities/linkedvar.h"
#include "utilities/modpriority.h"
#include "utilities/conditionscope.h"

#include "template/processparser.h"

namespace nemesis
{
    template<typename _LnkTy>
    struct Importer : public nemesis::ImporterBase
    {
        const std::string ignore = "SERIALIZE_IGNORED";

    private:
        nemesis::regex GetNodeIdRegexByType(const std::string_view& type, const std::string_view& classname)
        {
            std::stringstream ss;
            ss << "#(";
            ss << classname;
            ss << ")";
            ss << type;
            ss << "~([0-9]+)";
            return nemesis::regex(ss.str());
        }

    protected:
        bool success = true;

        UPtr<ConditionScope> cscope;
        Vec<Vec<SPtr<_LnkTy>>*> stream;

        std::pair<SPtr<const nemesis::ConditionInfo>, const nemesis::Line*> astercache;
        const nemesis::ConditionInfo* openmodcondinfo;
        UPtr<ModPriority> priorities;

        UPtr<nemesis::ProcessParser> processparser;

        Importer(nemesis::File& file)
            : nemesis::ImporterBase(file)
        {
        }

        virtual void PostAddLineProcess(_LnkTy& linkedline)
        {
        }

        virtual void AddLinkedLine(SPtr<_LnkTy> linkedline)
        {
            stream.back()->emplace_back(linkedline);
            PostAddLineProcess(*stream.back()->back());
        }

        virtual void AddLine(const nemesis::Line& line)
        {
            if (line.ToString()._Starts_with(nemesis::syntax::DeleteLine())) return;

            if constexpr (std::is_base_of_v<nemesis::Line, _LnkTy>)
            {
                AddLinkedLine(std::make_shared<_LnkTy>(line));
            }
        }

        bool IsValidLine(const nemesis::Line& line)
        {
            return line.find(ignore) == NOT_FOUND;
        }

        void AsteriskCondition(const nemesis::Line& line, const nemesis::ConditionInfo* conditioninfo)
        {
            if (!priorities) ErrorMessage(1224, file.GetFilePath(), conditioninfo->GetLineNumber());

            auto condition = cscope->Back().GetCondition();

            if (!priorities->Contains(condition)) return;

            if (!astercache.first || priorities->IsHigherThan(condition, astercache.first->GetCondition()))
            {
                AddToAsterCache(&line, &cscope->Back());
            }
        }

        void ModCodeCondition(const nemesis::ConditionInfo* conditioninfo)
        {
            if (!priorities) ErrorMessage(1224, file.GetFilePath(), conditioninfo->GetLineNumber());

            openmodcondinfo = conditioninfo;
            success = priorities->Contains(conditioninfo->GetCondition());
        }

        void ModCloseCondition(const nemesis::ConditionInfo* tobedeleted)
        {
            if (openmodcondinfo != tobedeleted) return;

            openmodcondinfo = nullptr;
            success = true;
        }

        void LowerOriginal(const nemesis::Line& line, const nemesis::ConditionInfo* conditioninfo)
        {
            const nemesis::Line* lineptr;
            const nemesis::ConditionInfo* conditionptr;

            if (!TryGetAsterInfo(lineptr, conditionptr))
            {
                lineptr      = &line;
                conditionptr = conditioninfo;
            }

            TryCacheData(*lineptr, conditionptr);

            if (!TryProcessCondition(conditionptr))
            {
                AddLine(*lineptr);
            }

            ResetAsterCache();
        }

        bool TryGetAsterInfo(const nemesis::Line*& lineptr, const nemesis::ConditionInfo*& conditionptr)
        {
            if (!astercache.first) return false;

            lineptr      = astercache.second;
            conditionptr = astercache.first.get();
            return true;
        }

        void ResetAsterCache() noexcept(true)
        {
            AddToAsterCache(nullptr, nullptr);
        }

        void AddToAsterCache(const nemesis::Line* line,
                             const nemesis::ConditionInfo* conditioninfo) noexcept(true)
        {
            astercache = std::make_pair(conditioninfo ? conditioninfo->shared_from_this() : nullptr, line);
        }

        bool HasConditionMet(const nemesis::Line& line, const nemesis::ConditionInfo* conditioninfo)
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
                    AsteriskCondition(line, conditioninfo);
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
                    auto& tobedeleted = cscope->GetToBeDeleted();

                    if (tobedeleted.GetType() != nemesis::CondType::MOD_CODE) return true;

                    ModCloseCondition(&tobedeleted);
                    return false;
                }
                default:
                    return true;
            }
        }

        virtual void TryCacheData(const nemesis::Line& line, const nemesis::ConditionInfo* conditioninfo)
        {
        }

        void LevelUpScope()
        {
            stream.emplace_back(&stream.back()->back()->GetLastCondition().GetDataList());
        }

        void LevelDownScope()
        {
            stream.pop_back();
        }

        virtual void AddConditionScope(const nemesis::ConditionInfo* conditioninfo)
        {
            SPtr<_LnkTy> linked = std::make_shared<_LnkTy>();
            linked->AddCondition(*conditioninfo, file);
            AddLinkedLine(linked);
            LevelUpScope();
        }

        virtual void UpdateConditionScope(const nemesis::ConditionInfo* conditioninfo)
        {
            SPtr<_LnkTy>& linked = stream.at(stream.size() - 2)->back();
            linked->AddCondition(*conditioninfo, file);
            stream.back() = &linked->GetLastCondition().GetDataList();
        }
        
        virtual void RemoveConditionScope()
        {
            if (cscope->GetToBeDeleted().GetType() != nemesis::CondType::ORIGINAL)
            {
                LevelDownScope();
                return;
            }

            Vec<SPtr<_LnkTy>>* ori = stream.back();

            LevelDownScope();

            SPtr<_LnkTy>& lastlinked = stream.back()->back();
            size_t size              = lastlinked->GetConditionSize() - 1;

            for (size_t o = 0; o < ori->size(); ++o)
            {
                SPtr<_LnkTy>& ori_lnk = ori->at(o);

                for (size_t i = 0; i < size; ++i)
                {
                    auto& lnkcond  = lastlinked->GetCondition(i);
                    auto* raw_cond = lnkcond.GetCondition();
                    ori_lnk->AddCondition(raw_cond->shared_from_this());

                    auto& datalist = lnkcond.GetDataList();
                    auto& lastcond = ori_lnk->GetLastCondition();
                    lastcond.GetDataList().emplace_back(datalist.front());
                    datalist.erase(datalist.begin());

                    if (datalist.empty())
                    {
                        lastlinked->RemoveCondition(i);
                        size--;
                        i--;
                    }
                }

                stream.back()->insert(stream.back()->end() - 1, ori_lnk);
            }

            if (lastlinked->GetConditionSize() == 0)
            {
                stream.back()->pop_back();
            }
        }

        virtual void AddAsteriskCondition(const nemesis::ConditionInfo* conditioninfo)
        {
            auto& curlist = stream.back();
            SPtr<_LnkTy> linked;

            if (curlist->empty() || curlist->back()->GetRawPtr() || curlist->back()->GetCondition(0).GetType() != nemesis::CondType::ASTERISK)
            {
                linked = std::make_shared<_LnkTy>();
                AddLinkedLine(linked);
            }
            else
            {
                linked = curlist->back();
            }

            if constexpr (std::is_base_of_v<nemesis::Line, _LnkTy>)
            {
                linked->AddCondition(*conditioninfo, file);
                linked->GetLastCondition().GetDataList().emplace_back(conditioninfo->GetContents().front());
            }
        }

        virtual void AddLowerOriginal(const nemesis::ConditionInfo* conditioninfo)
        {
            if constexpr (std::is_base_of_v<nemesis::Line, _LnkTy>)
            {
                auto& curlist = stream.back();
                curlist->back()->SetRaw(conditioninfo->GetContents().front());
            }

            throw std::runtime_error("Invalid lower original");
        }

        virtual bool TryProcessCondition(const nemesis::ConditionInfo* conditioninfo)
        {
            if (!conditioninfo) return false;

            auto type = conditioninfo->GetType();

            switch (type)
            {
                case nemesis::CondType::IF:
                case nemesis::CondType::FOREACH:
                case nemesis::CondType::MOD_CODE:
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
                case nemesis::CondType::ORIGINAL:
                {
                    UpdateConditionScope(conditioninfo);
                    break;
                }
                case nemesis::CondType::ENDIF:
                case nemesis::CondType::CLOSE:
                {
                    RemoveConditionScope();
                    break;
                }
                case nemesis::CondType::ASTERISK:
                {
                    AddAsteriskCondition(conditioninfo);
                    break;
                }
                case nemesis::CondType::LOWER_ORIGINAL:
                {
                    AddLowerOriginal(conditioninfo);
                    break;
                }
                default:
                    return false;
            }

            return true;
        }

        virtual void ParseLine(const nemesis::Line& line)
        {
            if (error) throw nemesis::exception();

            if (!line.empty())
            {
                if (!IsValidLine(line)) return;

                auto conditioninfo = cscope->TryGetConditionInfo(line);

                if (!HasConditionMet(line, conditioninfo.get())) return;    // will be removed

                TryCacheData(line, conditioninfo.get());

                if (TryProcessCondition(conditioninfo.get())) return;
            }

            AddLine(line);
        }

        void AddMasterNodeIdRegex(const std::string& classname)
        {
            masteridrgx_list.emplace_back(
                RegexSearch(GetNodeIdRegexByType("_master", classname), classname + "_master~"));
        }

        void AddGroupNodeIdRegex(const std::string& classname)
        {
            groupidrgx_list.emplace_back(
                RegexSearch(GetNodeIdRegexByType("_group", classname), classname + "_group~"));
        }

        void AddAnimNodeIdRegex(const std::string& classname)
        {
            animidrgx_list.emplace_back(RegexSearch(GetNodeIdRegexByType("", classname), classname + "~"));
        }

        virtual void ParseLines(const VecNstr& lines) = 0;

    public:
        void AddModPriority(const VecStr& behaviorpriority)
        {
            priorities = std::make_unique<ModPriority>(behaviorpriority);
        }

        virtual void ReadFile(const std::filesystem::path& filepath)
        {
            DebugLogging(L"Parsing behavior: " + filepath.wstring());
            this->cscope   = std::make_unique<nemesis::ConditionScope>("Base", filepath);
        }

        virtual void ParseFile()
        {
            ParseLines(filelines);
        }
    };
} // namespace nemesis
