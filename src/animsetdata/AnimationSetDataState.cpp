#include "animsetdata/AnimationSetDataState.h"

#include "core/IfObject.h"
#include "core/ModObject.h"
#include "core/ForEachObject.h"
#include "core/CollectionObject.h"

Vec<UPtr<nemesis::NObject>> nemesis::AnimationSetDataState::ParseModObjects(nemesis::LineStream& stream,
                                                                            nemesis::SemanticManager& manager,
                                                                            bool& has_new_state)
{

    Deq<const nemesis::Line*> ori_lines;
    Deq<const nemesis::Line*> mod_lines;
    Vec<UPtr<nemesis::NObject>> object_list;

    if (stream.IsEoF()) return object_list;

    auto* lines          = &mod_lines;
    auto* mod_open_token = &stream.GetToken();
    ++stream;

    if (!mod_open_token || mod_open_token->Type != nemesis::LineStream::MOD_OPEN)
    {
        auto& token_value = mod_open_token->Value;
        throw std::runtime_error("Syntax Error: Unsupport syntax within MOD condition (Line: "
                                 + std::to_string(token_value.GetLineNumber())
                                 + ". File: " + token_value.GetFilePath().string() + ")");
    }

    for (; stream.IsEoF(); ++stream)
    {
        auto* token_ptr = &stream.GetToken();

        switch (token_ptr->Type)
        {
            case nemesis::LineStream::MOD_CLOSE:
            {
                auto& mod_code = mod_open_token->Value;
                size_t i       = 0;

                for (; i < ori_lines.size(); i++)
                {
                    auto& line_ptr = *ori_lines[i];
                    auto nline_ptr = std::make_unique<nemesis::NLine>(line_ptr, manager);

                    if (i < mod_lines.size())
                    {
                        nline_ptr->AddModLine(mod_code,
                                              mod_code.GetLineNumber(),
                                              mod_code.GetFilePath(),
                                              manager,
                                              *mod_lines[i]);
                    }
                    else
                    {
                        nline_ptr->AddModLine(
                            mod_code, mod_code.GetLineNumber(), mod_code.GetFilePath(), manager);
                    }

                    object_list.emplace_back(std::move(nline_ptr));
                }

                if (i >= mod_lines.size()) return object_list;

                auto collection = std::make_unique<nemesis::CollectionObject>();
                auto col_ptr    = collection.get();
                auto mod_object = std::make_unique<nemesis::ModObject>(mod_code,
                                                                       mod_code.GetLineNumber(),
                                                                       mod_code.GetFilePath(),
                                                                       manager,
                                                                       std::move(collection));

                for (auto itr = mod_lines.begin() + i; itr != mod_lines.end(); itr++)
                {
                    col_ptr->AddObject(std::make_unique<nemesis::NLine>(**itr, manager));
                }

                object_list.emplace_back(std::move(mod_object));
                return object_list;
            }
            case nemesis::LineStream::MOD_ORG:
            {
                lines = &ori_lines;
                break;
            }
            case nemesis::LineStream::NONE:
            {
                auto& token_value = token_ptr->Value;
                auto& line_ptr    = lines->emplace_back(&token_ptr->Value);

                if (token_value != "V3") break;

                has_new_state = true;
                break;
            }
            default:
            {
                auto& token_value = token_ptr->Value;
                throw std::runtime_error("Syntax Error: Unsupport syntax within MOD condition (Line: "
                                         + std::to_string(token_value.GetLineNumber())
                                         + ". File: " + token_value.GetFilePath().string() + ")");
            }
        }
    }

    auto& token_value = stream.GetToken().Value;
    throw std::runtime_error("Syntax Error: Missing MOD_CODE_OPEN statement (Line: "
                             + std::to_string(token_value.GetLineNumber())
                             + ", File: " + token_value.GetFilePath().string() + ")");
}

UPtr<nemesis::ForEachObject> nemesis::AnimationSetDataState::ParseForEachObject(
    nemesis::LineStream& stream, nemesis::SemanticManager& manager, bool& has_new_state)
{
    if (stream.IsEoF()) return nullptr;

    Deq<UPtr<nemesis::NObject>> object_list;
    auto* fe_token = &stream.GetToken();

    if (fe_token->Type != nemesis::LineStream::FOR_EACH)
    {
        auto& token_value = fe_token->Value;
        throw std::runtime_error("Syntax Error: Unsupport syntax within MOD condition (Line: "
                                 + std::to_string(token_value.GetLineNumber())
                                 + ". File: " + token_value.GetFilePath().string() + ")");
    }

    auto& token_value = fe_token->Value;
    auto collection = std::make_unique<nemesis::CollectionObject>();
    auto fe_obj = std::make_unique<nemesis::ForEachObject>(
        token_value, token_value.GetLineNumber(), token_value.GetFilePath(), manager, std::move(collection));
    ++stream;

    for (; stream.IsEoF(); ++stream)
    {
        auto* token_ptr = &stream.GetToken();

        switch (token_ptr->Type)
        {
            case nemesis::LineStream::CLOSE:
            {
                auto& token_value = token_ptr->Value;
                auto collection   = std::make_unique<nemesis::CollectionObject>();

                for (auto& obj : object_list)
                {
                    collection->AddObject(std::move(obj));
                }

                return std::make_unique<nemesis::ForEachObject>(token_value,
                                                                token_value.GetLineNumber(),
                                                                token_value.GetFilePath(),
                                                                manager,
                                                                std::move(collection));
            }
            case nemesis::LineStream::MOD_CLOSE:
            {
                ++stream;
                auto mod_objects = ParseModObjects(stream, manager, has_new_state);

                for (auto it = mod_objects.rbegin(); it != mod_objects.rend(); ++it)
                {
                    object_list.emplace_front(std::move(*it));
                }

                break;
            }
            case nemesis::LineStream::END_IF:
            {
                ++stream;
                auto if_obj = ParseIfObjects(stream, manager, has_new_state);
                object_list.emplace_front(std::move(if_obj));
                break;
            }
            case nemesis::LineStream::CLOSE:
            {
                ++stream;
                auto fe_obj = ParseForEachObject(stream, manager, has_new_state);
                object_list.emplace_front(std::move(fe_obj));
                break;
            }
            case nemesis::LineStream::NONE:
            {
                auto& token_value = token_ptr->Value;
                object_list.emplace_front(std::make_unique<nemesis::NLine>(
                    token_value, token_value.GetLineNumber(), token_value.GetFilePath(), manager));

                if (token_value != "V3") break;

                has_new_state = true;
                break;
            }
            default:
            {
                auto& token_value = token_ptr->Value;
                throw std::runtime_error("Syntax Error: Unsupport syntax within MOD condition (Line: "
                                         + std::to_string(token_value.GetLineNumber())
                                         + ". File: " + token_value.GetFilePath().string() + ")");
            }
        }
    }

    auto& token_value = stream.GetToken().Value;
    throw std::runtime_error("Syntax Error: Missing FOREACH opening statement (Line: "
                             + std::to_string(token_value.GetLineNumber())
                             + ", File: " + token_value.GetFilePath().string() + ")");
}

void nemesis::AnimationSetDataState::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    DeqNstr templines;
    StateData->CompileTo(templines, state);

    auto data_type = DataType::EquipList;
    size_t counter = 0;
    nemesis::Line* counter_nline;

    if (templines.empty())
    {
        throw std::runtime_error("AnimationSetDataSingleFile empty project is not supported");
    }

    if (templines.front() != "V3")
    {
        auto& first = templines.front();
        throw std::runtime_error("AnimationSetDataSingleFile format error (Line: "
                                 + std::to_string(first.GetLineNumber())
                                 + ", File: " + first.GetFilePath().string() + ")");
    }

    for (auto itr = templines.begin() + 1; itr != templines.end(); ++itr)
    {
        switch (data_type)
        {
            case DataType::EquipCounter:
            {
                counter_nline = &(*itr);
                counter       = 0;
                data_type     = DataType::EquipList;
                itr++;
            }
            case DataType::EquipList:
            {
                if (!isOnlyNumber(*itr))
                {
                    counter++;
                    break;
                }

                *counter_nline = nemesis::Line(
                    std::to_string(counter), counter_nline->GetLineNumber(), counter_nline->GetFilePath());
            }
            case DataType::TypeCounter:
            {
                counter_nline = &(*itr);
                counter       = 0;
                data_type     = DataType::TypeList;
                itr++;
            }
            case DataType::TypeList:
            {
                if (!isOnlyNumber(*itr))
                {
                    if (!isOnlyNumber(*++itr))
                    {
                        throw std::runtime_error("AnimationSetDataSingleFile format error (Line: "
                                                 + std::to_string(itr->GetLineNumber())
                                                 + ", File: " + itr->GetFilePath().string() + ")");
                    }

                    if (!isOnlyNumber(*++itr))
                    {
                        throw std::runtime_error("AnimationSetDataSingleFile format error (Line: "
                                                 + std::to_string(itr->GetLineNumber())
                                                 + ", File: " + itr->GetFilePath().string() + ")");
                    }

                    counter++;
                    break;
                }

                *counter_nline = nemesis::Line(
                    std::to_string(counter), counter_nline->GetLineNumber(), counter_nline->GetFilePath());
            }
            case DataType::AnimationCounter:
            {
                counter_nline = &(*itr);
                counter       = 0;
                data_type     = DataType::AnimationList;
                itr++;
            }
            case DataType::AnimationList:
            {
                if (!isOnlyNumber(*itr))
                {
                    if (!isOnlyNumber(*++itr))
                    {
                        throw std::runtime_error("AnimationSetDataSingleFile format error (Line: "
                                                 + std::to_string(itr->GetLineNumber())
                                                 + ", File: " + itr->GetFilePath().string() + ")");
                    }

                    if (!isOnlyNumber(*++itr))
                    {
                        throw std::runtime_error("AnimationSetDataSingleFile format error (Line: "
                                                 + std::to_string(itr->GetLineNumber())
                                                 + ", File: " + itr->GetFilePath().string() + ")");
                    }

                    auto* anim_counter_nline = &(*itr);
                    size_t anim_counter      = 0;

                    for (++itr; itr != templines.end(); ++itr)
                    {
                        if (isOnlyNumber(*itr))
                        {
                            auto step2 = itr + 2;

                            if (step2 >= templines.end())
                            {
                                // -- Case Example --
                                // 1                << Animation Counter
                                // 1HM_AttackLeft   << Animation(s)
                                // 1HM_AttackRight  << Animation(s)
                                // 0                << Crc32 Counter (YOU ARE HERE)
                                break;
                            }

                            if (!isOnlyNumber(*step2))
                            {
                                // -- Case Example 1 --
                                // 1                << Animation Counter
                                // shd_BashPower    << Animation(s)
                                // attackStart      << Event
                                // 0                << Unknown (YOU ARE HERE)
                                // 0                << Animation Counter
                                // bashStart        << Event ("step2" IS HERE)

                                // -- Case Example 2 --
                                // 1                << Animation Counter
                                // shd_BashPower    << Animation(s)
                                // attackStart      << Event
                                // 0                << Unknown (YOU ARE HERE)
                                // 1                << Animation Counter
                                // 1HM_AttackLeft   << Animation ("step2" IS HERE)
                                anim_counter--;
                                itr--;
                            }

                            auto step3 = step2 + 1;

                            if (step3 >= templines.end())
                            {
                                // -- Case Example --
                                // 1                << Animation Counter
                                // 1HM_AttackLeft   << Animation(s)
                                // attackStart      << Event
                                // 0                << Unknown (YOU ARE HERE)
                                // 0                << Animation Counter
                                // 0                << Crc32 Counter ("step2" IS HERE)
                                anim_counter--;
                                itr--;
                                break;
                            }

                            if (*step3 == "7891816")
                            {
                                // -- Case Example --
                                // 1                << Animation Counter
                                // 1HM_AttackLeft   << Animation(s)
                                // 1HM_AttackRight  << Animation(s)
                                // 1                << Crc32 Counter (YOU ARE HERE)
                                // 3064642194       << Directory Path
                                // 2909749619       << File Name ("step2" IS HERE)
                                // 7891816          << Extension ("step3" IS HERE)
                                break;
                            }

                            // -- Case Example --
                            // 1          w          << Animation Counter
                            // shd_BashPower        << Animation(s)
                            // attackStart          << Event
                            // 0                    << Unknown (YOU ARE HERE)
                            // 0                    << Animation Counter
                            // 1                    << Crc32 Counter ("step2" IS HERE)
                            // 3064642194           << Directory Path ("step3" IS HERE)
                            anim_counter--;
                            itr--;
                            break;
                        }

                        anim_counter++;
                    }

                    *anim_counter_nline = nemesis::Line(std::to_string(anim_counter),
                                                        anim_counter_nline->GetLineNumber(),
                                                        anim_counter_nline->GetFilePath());
                    counter++;
                    break;
                }

                *counter_nline = nemesis::Line(
                    std::to_string(counter), counter_nline->GetLineNumber(), counter_nline->GetFilePath());
            }
            case DataType::Crc32Counter:
            {
                counter_nline = &(*itr);
                counter       = 0;
                data_type     = DataType::Crc32List;
                itr++;
            }
            case DataType::Crc32List:
            {
                for (; itr != templines.end(); ++itr)
                {
                    if (!isOnlyNumber(*itr))
                    {
                        throw std::runtime_error("AnimationSetDataSingleFile format error (Line: "
                                                 + std::to_string(itr->GetLineNumber())
                                                 + ", File: " + itr->GetFilePath().string() + ")");
                    }

                    counter++;
                }

                if (counter % 3 != 0)
                {
                    itr--;
                    throw std::runtime_error("AnimationSetDataSingleFile format error (Line: "
                                             + std::to_string(itr->GetLineNumber())
                                             + ", File: " + itr->GetFilePath().string() + ")");
                }

                *counter_nline = nemesis::Line(std::to_string(counter / 3),
                                               counter_nline->GetLineNumber(),
                                               counter_nline->GetFilePath());

                for (auto& line : templines)
                {
                    lines.emplace_back(std::move(line));
                }

                return;
            }
        }
    }

    auto& last = templines.back();
    throw std::runtime_error("AnimationSetDataSingleFile format error (Line: "
                             + std::to_string(last.GetLineNumber()) + ", File: " + last.GetFilePath().string()
                             + ")");
}

void nemesis::AnimationSetDataState::SerializeTo(DeqNstr& lines) const
{
    StateData->SerializeTo(lines);
}

Vec<UPtr<nemesis::NObject>> nemesis::AnimationSetDataState::ParseObjects(nemesis::LineStream& stream,
                                             nemesis::SemanticManager& manager,
                                             Vec<nemesis::AnimationSetDataState*>& project_list)
{
    Vec<UPtr<nemesis::NObject>> object_list;
    std::function<void(UPtr<nemesis::NObject>&&)> add_object;

    bool has_new_state = false;
    auto* token_ptr    = &stream.GetToken();

    if (token_ptr->Value != "V3")
    {
        auto& token_value = token_ptr->Value;
        throw std::runtime_error("Syntax Error: Unsupport syntax (Line: "
                                 + std::to_string(token_value.GetLineNumber())
                                 + ". File: " + token_value.GetFilePath().string() + ")");
    }

    for (; stream.IsEoF(); ++stream)
    {
        token_ptr = &stream.GetToken();

        switch (token_ptr->Type)
        {
            case nemesis::LineStream::MOD_OPEN:
            {
                ++stream;
                has_new_state = false;
                auto list = ParseModObjects(stream, manager, has_new_state);

                if (!has_new_state)
                {
                    for (auto& each : list)
                    {
                        add_object(std::move(each));
                    }

                    break;
                }

                auto collection = std::make_unique<nemesis::CollectionObject>();
                auto* data_ptr  = collection.get();
                add_object      = [data_ptr](UPtr<nemesis::NObject>&& obj_ptr)
                { data_ptr->AddObject(std::move(obj_ptr)); };

                for (auto& each : list)
                {
                    collection->AddObject(std::move(each));
                }

                object_list.emplace_back(std::move(collection));
                break;
            }
            case nemesis::LineStream::FOR_EACH:
            {
                ++stream;
                auto fe_obj = ParseForEachObject(stream, manager, has_new_state);

                if (!has_new_state)
                {
                    add_object(std::move(fe_obj));
                    break;
                }

                auto collection = std::make_unique<nemesis::CollectionObject>();
                auto* data_ptr  = collection.get();
                add_object      = [data_ptr](UPtr<nemesis::NObject>&& obj_ptr)
                { data_ptr->AddObject(std::move(obj_ptr)); };

                collection->AddObject(std::move(fe_obj));
                object_list.emplace_back(std::move(collection));
                break;
            }
            case nemesis::LineStream::IF:
            {
                ++stream;
                auto if_obj = ParseIfObjects(stream, manager, has_new_state);

                if (!has_new_state)
                {
                    add_object(std::move(if_obj));
                    break;
                }

                auto collection = std::make_unique<nemesis::CollectionObject>();
                auto* data_ptr  = collection.get();
                add_object = [data_ptr](UPtr<nemesis::NObject>&& obj_ptr)
                { data_ptr->AddObject(std::move(obj_ptr)); };

                collection->AddObject(std::move(if_obj));
                object_list.emplace_back(std::move(collection));
                break;
            }
            case nemesis::LineStream::NONE:
            {
                if (token_ptr->Value == "V3")
                {
                    auto state       = std::make_unique<nemesis::AnimationSetDataState>();
                    state->StateData = std::make_unique<nemesis::CollectionObject>();
                    auto& data_ptr   = state->StateData;
                    add_object       = [&data_ptr](UPtr<nemesis::NObject>&& obj_ptr)
                    { data_ptr->AddObject(std::move(obj_ptr)); };

                    object_list.emplace_back(std::move(state));
                }

                add_object(std::make_unique<nemesis::NLine>(token_ptr->Value, manager));
                break;
            }
            default:
            {
                auto& token_value = stream.GetToken().Value;
                throw std::runtime_error("Syntax Error: Unsupport syntax (Line: "
                                         + std::to_string(token_value.GetLineNumber())
                                         + ". File: " + token_value.GetFilePath().string() + ")");
            }
        }
    }

    return object_list;
}
