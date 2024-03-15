#include "animsetdata/AnimationSetDataState.h"

#include "core/ModLine.h"
#include "core/IfObject.h"
#include "core/ModObject.h"
#include "core/ForEachObject.h"
#include "core/CollectionObject.h"

#include "core/Template/TemplateClass.h"

#include "utilities/writetextfile.h"

nemesis::AnimationSetDataState::AnimationSetDataState(const std::string& name) noexcept
    : Name(name)
{
}

void nemesis::AnimationSetDataState::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    DeqNstr templines;
    StateData->CompileTo(templines, state);

    if (templines.empty()) return;

    auto data_type = DataType::EquipCounter;
    size_t counter = 0;
    nemesis::Line* counter_nline;

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
                ++itr;
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
                ++itr;
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
                ++itr;
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
                            auto step1 = itr + 1;
                            auto step2 = itr + 2;

                            if (step2 >= templines.end())
                            {
                                if (step1 != templines.end())
                                {
                                    throw std::runtime_error("AnimationSetDataSingleFile format error (Line: "
                                                             + std::to_string(itr->GetLineNumber())
                                                             + ", File: " + itr->GetFilePath().string()
                                                             + ")");
                                }

                                // -- Case Example --
                                // 2                << Animation Counter
                                // 1HM_AttackLeft   << Animation(s)
                                // 1HM_AttackRight  << Animation(s)
                                // 0                << Crc32 Counter (YOU ARE HERE)
                                --itr;
                                break;
                            }

                            if (step1 == templines.end())
                            {
                                throw std::runtime_error("AnimationSetDataSingleFile format error (Line: "
                                                         + std::to_string(itr->GetLineNumber())
                                                         + ", File: " + itr->GetFilePath().string() + ")");
                            }

                            if (!isOnlyNumber(*step2))
                            {
                                if (!isOnlyNumber(*step1))
                                {
                                    throw std::runtime_error("AnimationSetDataSingleFile format error (Line: "
                                                             + std::to_string(itr->GetLineNumber())
                                                             + ", File: " + itr->GetFilePath().string()
                                                             + ")");
                                }

                                // -- Case Example 1 --
                                // 1                << Animation Counter
                                // shd_BashPower    << Animation(s)
                                // attackStart      << Event
                                // 0                << Unknown (YOU ARE HERE)
                                // 1                << Animation Counter ("step1" IS HERE)
                                // bashStart        << Event ("step2" IS HERE)

                                // -- Case Example 2 --
                                // 1                << Animation Counter
                                // shd_BashPower    << Animation(s)
                                // attackStart      << Event
                                // 0                << Unknown (YOU ARE HERE)
                                // 1                << Animation Counter ("step1" IS HERE)
                                // 1HM_AttackLeft   << Animation ("step2" IS HERE)
                                anim_counter--;
                                itr -= 2;
                                break;
                            }

                            auto step3 = step2 + 1;

                            if (step3 >= templines.end())
                            {
                                if (!isOnlyNumber(*step1))
                                {
                                    throw std::runtime_error("AnimationSetDataSingleFile format error (Line: "
                                                             + std::to_string(itr->GetLineNumber())
                                                             + ", File: " + itr->GetFilePath().string()
                                                             + ")");
                                }

                                // -- Case Example --
                                // 1                << Animation Counter
                                // 1HM_AttackLeft   << Animation(s)
                                // attackStart      << Event
                                // 0                << Unknown (YOU ARE HERE)
                                // 0                << Animation Counter ("step1" IS HERE)
                                // 0                << Crc32 Counter ("step2" IS HERE)
                                anim_counter--;
                                itr -= 2;
                                break;
                            }

                            if (isOnlyNumber(*step3))
                            {
                                if (!isOnlyNumber(*step1))
                                {
                                    throw std::runtime_error("AnimationSetDataSingleFile format error (Line: "
                                                             + std::to_string(itr->GetLineNumber())
                                                             + ", File: " + itr->GetFilePath().string()
                                                             + ")");
                                }

                                // -- Case Example --
                                // 2                << Animation Counter
                                // 1HM_AttackLeft   << Animation(s)
                                // 1HM_AttackRight  << Animation(s)
                                // 1                << Crc32 Counter (YOU ARE HERE)
                                // 3064642194       << Directory Path ("step1" IS HERE)
                                // 2909749619       << File Name ("step2" IS HERE)
                                // 7891816          << Extension ("step3" IS HERE)
                                --itr;
                                break;
                            }

                            if (!isOnlyNumber(*step1))
                            {
                                throw std::runtime_error("AnimationSetDataSingleFile format error (Line: "
                                                         + std::to_string(itr->GetLineNumber())
                                                         + ", File: " + itr->GetFilePath().string() + ")");
                            }

                            // -- Case Example --
                            // 1                    << Animation Counter
                            // shd_BashPower        << Animation(s)
                            // attackStart          << Event
                            // 0                    << Unknown (YOU ARE HERE)
                            // 0                    << Animation Counter ("step1" IS HERE)
                            // 1                    << Crc32 Counter ("step2" IS HERE)
                            // 3064642194           << Directory Path ("step3" IS HERE)
                            anim_counter--;
                            itr -= 2;
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
                ++itr;
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

UPtr<nemesis::NObject> nemesis::AnimationSetDataState::CloneNObject() const
{
    return Clone();
}

UPtr<nemesis::AnimationSetDataState> nemesis::AnimationSetDataState::Clone() const
{
    auto state       = std::make_unique<nemesis::AnimationSetDataState>(Name);
    state->StateData = StateData->Clone();
    return state;
}

void nemesis::AnimationSetDataState::MatchAndUpdate(const nemesis::AnimationSetDataState& state)
{
    std::scoped_lock<std::mutex> lock(UpdaterMutex);
    StateData->MatchAndUpdate(*state.StateData);
}

const std::string& nemesis::AnimationSetDataState::GetName() const noexcept
{
    return Name;
}

void nemesis::AnimationSetDataState::SerializeToFile(const std::filesystem::path& filepath) const
{
    DeqNstr lines = Serialize();

    FileWriter writer(filepath);

    if (!writer.is_open())
    {
        throw std::runtime_error("Failed to open file (File: " + filepath.string()
                                 + ", Error: " + std::string(std::strerror(errno)) + ")");
    }

    for (auto& line : lines)
    {
        writer << line;
    }
}

UPtr<nemesis::AnimationSetDataState> nemesis::AnimationSetDataState::Deserialize(const std::string& name,
                                                                                 nemesis::LineStream& stream)
{
    nemesis::SemanticManager manager;
    return Deserialize(name, stream, manager);
}

UPtr<nemesis::AnimationSetDataState>
nemesis::AnimationSetDataState::Deserialize(const std::string& name, nemesis::LineStream& stream, nemesis::SemanticManager& manager)
{
    if (stream.IsEoF()) return nullptr;

    bool has_new_state = false;
    auto state         = std::make_unique<nemesis::AnimationSetDataState>(name);
    auto collection    = std::make_unique<nemesis::CollectionObject>();
    auto col_ptr       = collection.get();

    state->StateData = std::move(collection);

    std::function<void(const nemesis::Line&)> new_state_checker = [&has_new_state](const nemesis::Line& nline)
    {
        if (nline != "V3") return;

        has_new_state = true;
    };

    for (; !stream.IsEoF(); ++stream)
    {
        auto& token = stream.GetToken();

        switch (token.Type)
        {
            case nemesis::LineStream::MOD_OPEN:
            {
                auto list = nemesis::NObject::ParseModObjects(stream, manager, new_state_checker);

                for (auto& each : list)
                {
                    col_ptr->AddObject(std::move(each));
                }

                break;
            }
            case nemesis::LineStream::FOR_EACH:
            {
                auto fe_obj = nemesis::NObject::ParseForEachObject(stream, manager, new_state_checker);
                col_ptr->AddObject(std::move(fe_obj));
                break;
            }
            case nemesis::LineStream::IF:
            {
                auto if_obj = nemesis::NObject::ParseIfObject(stream, manager, new_state_checker);
                col_ptr->AddObject(std::move(if_obj));
                break;
            }
            case nemesis::LineStream::NONE:
            {
                auto& value = token.Value;
                col_ptr->AddObject(std::make_unique<nemesis::NLine>(
                    value, value.GetLineNumber(), value.GetFilePath(), manager));
                break;
            }
            default:
            {
                auto& value = token.Value;
                throw std::runtime_error("Syntax Error: Unsupport syntax (Line: "
                                         + std::to_string(value.GetLineNumber())
                                         + ". File: " + value.GetFilePath().string() + ")");
            }
        }
    }

    return state;
}

UPtr<nemesis::AnimationSetDataState>
nemesis::AnimationSetDataState::DeserializeFromFile(const std::filesystem::path& filepath)
{
    nemesis::SemanticManager manager;
    return DeserializeFromFile(filepath, manager);
}

UPtr<nemesis::AnimationSetDataState>
nemesis::AnimationSetDataState::DeserializeFromFile(const std::filesystem::path& filepath,
                                                    const nemesis::TemplateClass* template_class)
{
    nemesis::SemanticManager manager;
    manager.SetCurrentTemplateClass(template_class);
    return DeserializeFromFile(filepath, manager);
}

UPtr<nemesis::AnimationSetDataState>
nemesis::AnimationSetDataState::DeserializeFromFile(const std::filesystem::path& filepath,
                                                    nemesis::SemanticManager& manager)
{
    VecNstr lines;
    GetFileLines(filepath, lines, false);
    nemesis::LineStream stream(lines.begin(), lines.end());
    return Deserialize(filepath.filename().string(), stream, manager);
}

Vec<UPtr<nemesis::AnimationSetDataState>> nemesis::AnimationSetDataState::ParseObjects(nemesis::LineStream& stream,
                                             nemesis::SemanticManager& manager,
                                             const std::string& project_name,
                                             const VecNstr& state_names)
{
    Vec<UPtr<nemesis::AnimationSetDataState>> state_list;

    if (stream.IsEoF()) return state_list;
    
    std::function<void(UPtr<nemesis::NObject>&&)> add_object;

    auto& token   = stream.GetToken();
    auto name_itr = state_names.begin();

    if (token.Value != "V3")
    {
        auto& value = token.Value;
        throw std::runtime_error("Invalid nemesis::AnimationSetDataState::ParseObjects format (Line: "
                                 + std::to_string(value.GetLineNumber())
                                 + ". File: " + value.GetFilePath().string() + ")");
    }

    for (; !stream.IsEoF(); ++stream)
    {
        auto& token = stream.GetToken();

        switch (token.Type)
        {
            //case nemesis::LineStream::MOD_OPEN:
            //{
            //    has_new_state = false;
            //    auto list = ParseModObjects(stream, manager, has_new_state);

            //    if (!has_new_state)
            //    {
            //        for (auto& each : list)
            //        {
            //            add_object(std::move(each));
            //        }

            //        break;
            //    }

            //    auto collection = std::make_unique<nemesis::CollectionObject>();
            //    auto* data_ptr  = collection.get();
            //    add_object      = [data_ptr](UPtr<nemesis::NObject>&& obj_ptr)
            //    { data_ptr->AddObject(std::move(obj_ptr)); };

            //    for (auto& each : list)
            //    {
            //        collection->AddObject(std::move(each));
            //    }

            //    object_list.emplace_back(std::move(collection));
            //    break;
            //}
            //case nemesis::LineStream::FOR_EACH:
            //{
            //    has_new_state = false;
            //    auto fe_obj = ParseForEachObject(stream, manager, has_new_state);

            //    if (!has_new_state)
            //    {
            //        add_object(std::move(fe_obj));
            //        break;
            //    }

            //    auto collection = std::make_unique<nemesis::CollectionObject>();
            //    auto* data_ptr  = collection.get();
            //    add_object      = [data_ptr](UPtr<nemesis::NObject>&& obj_ptr)
            //    { data_ptr->AddObject(std::move(obj_ptr)); };

            //    collection->AddObject(std::move(fe_obj));
            //    object_list.emplace_back(std::move(collection));
            //    break;
            //}
            //case nemesis::LineStream::IF:
            //{
            //    has_new_state = false;
            //    auto if_obj = ParseIfObject(stream, manager, has_new_state);

            //    if (!has_new_state)
            //    {
            //        add_object(std::move(if_obj));
            //        break;
            //    }

            //    auto collection = std::make_unique<nemesis::CollectionObject>();
            //    auto* data_ptr  = collection.get();
            //    add_object = [data_ptr](UPtr<nemesis::NObject>&& obj_ptr)
            //    { data_ptr->AddObject(std::move(obj_ptr)); };

            //    collection->AddObject(std::move(if_obj));
            //    object_list.emplace_back(std::move(collection));
            //    break;
            //}
            case nemesis::LineStream::NONE:
            {
                auto& value = token.Value;

                if (value == "V3")
                {
                    if (name_itr == state_names.end())
                    {
                        throw std::runtime_error(
                            "nemesis::AnimationSetDataState::ParseObjects parsing error (" + project_name
                            + "). State name count and state body count do not match");
                    }

                    auto state       = std::make_unique<nemesis::AnimationSetDataState>(*name_itr++);
                    state->StateData = std::make_unique<nemesis::CollectionObject>();
                    auto& data_ptr   = state->StateData;
                    add_object       = [&data_ptr](UPtr<nemesis::NObject>&& obj_ptr)
                    { data_ptr->AddObject(std::move(obj_ptr)); };

                    state_list.emplace_back(std::move(state));
                }
                else if (isOnlyNumber(value))
                {
                    for (size_t i = 1; ; ++i)
                    {
                        auto* ftoken_ptr = stream.GetForwardToken(i);

                        if (ftoken_ptr == nullptr) break;

                        if (ftoken_ptr->Type != nemesis::LineStream::NONE) continue;

                        if (ftoken_ptr->Value.find(".txt") != NOT_FOUND) return state_list;

                        break;
                    }
                }

                add_object(std::make_unique<nemesis::NLine>(
                    value, value.GetLineNumber(), value.GetFilePath(), manager));
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

    return state_list;
}
