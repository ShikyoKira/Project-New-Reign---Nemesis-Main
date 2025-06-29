#include <fstream>

#include "Core/AnimationSetData/AnimationSetDataState.h"

#include "Core/CollectionObject.h"
#include "Core/ForEachObject.h"
#include "Core/IfObject.h"
#include "Core/ModClass.h"
#include "Core/ModLine.h"
#include "Core/ModObject.h"

#include "Core/Template/TemplateClass.h"

#include "Utilities/Algorithm.h"
#include "Utilities/File.h"
#include "Utilities/ThreadPool.h"

nemesis::AnimationSetDataState::AnimationSetDataState(const std::string& name) noexcept
    : Name(name)
{
}

void nemesis::AnimationSetDataState::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    DeqNstr templines;

    {
        std::scoped_lock<std::mutex> lock(DataMutex);
        StateData->CompileTo(templines, state);
    }

    if (templines.empty()) return;

    auto data_type = DataType::EquipCounter;
    size_t counter = 0;
    nemesis::Line* counter_nline;

    if (templines.front() != "V3")
    {
        auto& first = templines.front();
        throw std::runtime_error("AnimationSetDataSingleFile format error (Line: "
                                 + std::to_string(first.GetLineNumber())
                                 + ", File: " + nemesis::to_utf8_string(first.GetFilePath()) + ")");
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
                if (!is_only_number(*itr))
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
                if (!is_only_number(*itr))
                {
                    if (!is_only_number(*++itr))
                    {
                        throw std::runtime_error("AnimationSetDataSingleFile format error (Line: "
                                                 + std::to_string(itr->GetLineNumber()) + ", File: "
                                                 + nemesis::to_utf8_string(itr->GetFilePath()) + ")");
                    }

                    if (!is_only_number(*++itr))
                    {
                        throw std::runtime_error("AnimationSetDataSingleFile format error (Line: "
                                                 + std::to_string(itr->GetLineNumber()) + ", File: "
                                                 + nemesis::to_utf8_string(itr->GetFilePath()) + ")");
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
                if (!is_only_number(*itr))
                {
                    if (!is_only_number(*++itr))
                    {
                        throw std::runtime_error("AnimationSetDataSingleFile format error (Line: "
                                                 + std::to_string(itr->GetLineNumber()) + ", File: "
                                                 + nemesis::to_utf8_string(itr->GetFilePath()) + ")");
                    }

                    if (!is_only_number(*++itr))
                    {
                        throw std::runtime_error("AnimationSetDataSingleFile format error (Line: "
                                                 + std::to_string(itr->GetLineNumber()) + ", File: "
                                                 + nemesis::to_utf8_string(itr->GetFilePath()) + ")");
                    }

                    auto* anim_counter_nline = &(*itr);
                    size_t anim_counter      = 0;

                    for (++itr; itr != templines.end(); ++itr)
                    {
                        if (is_only_number(*itr))
                        {
                            auto step1 = itr + 1;
                            auto step2 = itr + 2;

                            if (step2 >= templines.end())
                            {
                                if (step1 != templines.end())
                                {
                                    throw std::runtime_error(
                                        "AnimationSetDataSingleFile format error (Line: "
                                        + std::to_string(itr->GetLineNumber())
                                        + ", File: " + nemesis::to_utf8_string(itr->GetFilePath()) + ")");
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
                                                         + std::to_string(itr->GetLineNumber()) + ", File: "
                                                         + nemesis::to_utf8_string(itr->GetFilePath()) + ")");
                            }

                            if (!is_only_number(*step2))
                            {
                                if (!is_only_number(*step1))
                                {
                                    throw std::runtime_error(
                                        "AnimationSetDataSingleFile format error (Line: "
                                        + std::to_string(itr->GetLineNumber())
                                        + ", File: " + nemesis::to_utf8_string(itr->GetFilePath()) + ")");
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
                                if (!is_only_number(*step1))
                                {
                                    throw std::runtime_error(
                                        "AnimationSetDataSingleFile format error (Line: "
                                        + std::to_string(itr->GetLineNumber())
                                        + ", File: " + nemesis::to_utf8_string(itr->GetFilePath()) + ")");
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

                            if (is_only_number(*step3))
                            {
                                if (!is_only_number(*step1))
                                {
                                    throw std::runtime_error(
                                        "AnimationSetDataSingleFile format error (Line: "
                                        + std::to_string(itr->GetLineNumber())
                                        + ", File: " + nemesis::to_utf8_string(itr->GetFilePath()) + ")");
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

                            if (!is_only_number(*step1))
                            {
                                throw std::runtime_error("AnimationSetDataSingleFile format error (Line: "
                                                         + std::to_string(itr->GetLineNumber()) + ", File: "
                                                         + nemesis::to_utf8_string(itr->GetFilePath()) + ")");
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
                    if (!is_only_number(*itr))
                    {
                        throw std::runtime_error("AnimationSetDataSingleFile format error (Line: "
                                                 + std::to_string(itr->GetLineNumber()) + ", File: "
                                                 + nemesis::to_utf8_string(itr->GetFilePath()) + ")");
                    }

                    counter++;
                }

                if (counter % 3 != 0)
                {
                    itr--;
                    throw std::runtime_error("AnimationSetDataSingleFile format error (Line: "
                                             + std::to_string(itr->GetLineNumber()) + ", File: "
                                             + nemesis::to_utf8_string(itr->GetFilePath()) + ")");
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
                             + std::to_string(last.GetLineNumber())
                             + ", File: " + nemesis::to_utf8_string(last.GetFilePath()) + ")");
}

void nemesis::AnimationSetDataState::SerializeTo(DeqNstr& lines) const
{
    std::scoped_lock<std::mutex> lock(DataMutex);
    StateData->SerializeTo(lines);
}

UPtr<nemesis::NObject> nemesis::AnimationSetDataState::CloneNObject() const
{
    return Clone();
}

UPtr<nemesis::AnimationSetDataState> nemesis::AnimationSetDataState::Clone() const
{
    std::scoped_lock<std::mutex> lock(DataMutex);
    auto state       = std::make_unique<nemesis::AnimationSetDataState>(Name);
    state->FilePath  = FilePath;
    state->StateData = StateData->Clone();
    return state;
}

UPtr<nemesis::AnimationSetDataState>
nemesis::AnimationSetDataState::Clone(const nemesis::ModClass& mod_class,
                                      const std::filesystem::path& filepath) const
{
    auto clone    = Clone();
    auto new_data = std::make_unique<nemesis::CollectionObject>();
    auto mod_obj
        = std::make_unique<nemesis::ModObject>(mod_class.GetCode(), 0, filepath, std::move(clone->StateData));
    new_data->AddObject(std::move(mod_obj));
    clone->StateData = std::move(new_data);
    return clone;
}

void nemesis::AnimationSetDataState::MatchAndUpdate(const nemesis::AnimationSetDataState& state)
{
    std::scoped_lock<std::mutex> lock(DataMutex);
    StateData->MatchAndUpdate(*state.StateData);
}

void nemesis::AnimationSetDataState::MatchAndUpdate(const std::string& mod_code,
                                                    const nemesis::AnimationSetDataState& state)
{
    std::scoped_lock<std::mutex> lock(DataMutex);
    StateData->MatchAndUpdate(mod_code, *state.StateData);
}

const std::string& nemesis::AnimationSetDataState::GetName() const noexcept
{
    return Name;
}

const std::filesystem::path& nemesis::AnimationSetDataState::GetFilePath() const noexcept
{
    return FilePath;
}

void nemesis::AnimationSetDataState::SerializeToFile(const std::filesystem::path& filepath) const
{
    DeqNstr lines = Serialize();
    std::ofstream file(filepath);

    if (!file.is_open())
    {
        std::error_code ec(errno, std::system_category());
        throw std::runtime_error("Failed to open file: \"" + to_utf8_string(filepath)
                                 + "\"\nMessage: " + ec.message());
    }

    for (auto& line : lines)
    {
        file << line + "\n";
    }

    file.close();
}

void nemesis::AnimationSetDataState::Deserialize(nemesis::CollectionObject& collection,
                                                 nemesis::LineStream& stream,
                                                 nemesis::SemanticManager& manager)
{
    if (stream.IsEoF()) return;

    bool has_new_state = false;

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
                    collection.AddObject(std::move(each));
                }

                break;
            }
            case nemesis::LineStream::FOR_EACH:
            {
                auto fe_obj = nemesis::NObject::ParseForEachObject(stream, manager, new_state_checker);
                collection.AddObject(std::move(fe_obj));
                break;
            }
            case nemesis::LineStream::IF:
            {
                auto if_obj = nemesis::NObject::ParseIfObject(stream, manager, new_state_checker);
                collection.AddObject(std::move(if_obj));
                break;
            }
            case nemesis::LineStream::NONE:
            {
                auto& value = token.Value;
                collection.AddObject(std::make_unique<nemesis::NLine>(
                    value, value.GetLineNumber(), value.GetFilePath(), manager));
                break;
            }
            default:
            {
                auto& value = token.Value;
                throw std::runtime_error("Syntax Error: Unsupport syntax (Line: "
                                         + std::to_string(value.GetLineNumber())
                                         + ", File: " + nemesis::to_utf8_string(value.GetFilePath()) + ")");
            }
        }
    }
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
    std::string name = nemesis::to_utf8_string(filepath.filename());
    auto state       = std::make_unique<nemesis::AnimationSetDataState>(name);
    auto collection  = std::make_unique<nemesis::CollectionObject>();

    state->FilePath  = filepath;
    state->StateData = std::move(collection);
    VecNstr lines;
    GetFileLines(filepath, lines, false);
    nemesis::LineStream stream(lines.begin(), lines.end());
    Deserialize(*collection, stream, manager);
    return state;
}

UPtr<nemesis::AnimationSetDataState>
nemesis::AnimationSetDataState::DeserializeFromFile(const std::filesystem::path& filepath,
                                                    nemesis::ThreadPool& threadpool)
{
    std::string name = nemesis::to_utf8_string(filepath.filename());
    auto state       = std::make_unique<nemesis::AnimationSetDataState>(name);
    auto collection  = std::make_unique<nemesis::CollectionObject>();
    auto col_ptr     = collection.get();

    state->FilePath  = filepath;
    state->StateData = std::move(collection);

    threadpool.enqueue(
        [filepath, col_ptr]()
        {
            VecNstr lines;
            GetFileLines(filepath, lines, false);
            nemesis::LineStream stream(lines.begin(), lines.end());
            nemesis::SemanticManager manager;
            Deserialize(*col_ptr, stream, manager);
        });
    return state;
}

UPtr<nemesis::AnimationSetDataState>
nemesis::AnimationSetDataState::DeserializeFromFile(const std::filesystem::path& filepath,
                                                    const nemesis::TemplateClass* template_class,
                                                    nemesis::ThreadPool& threadpool)
{
    std::string name = nemesis::to_utf8_string(filepath.filename());
    auto state       = std::make_unique<nemesis::AnimationSetDataState>(name);
    auto collection  = std::make_unique<nemesis::CollectionObject>();
    auto col_ptr     = collection.get();

    state->FilePath  = filepath;
    state->StateData = std::move(collection);

    threadpool.enqueue(
        [filepath, template_class, col_ptr]()
        {
            VecNstr lines;
            GetFileLines(filepath, lines, false);
            nemesis::LineStream stream(lines.begin(), lines.end());
            nemesis::SemanticManager manager;
            manager.SetCurrentTemplateClass(template_class);
            Deserialize(*col_ptr, stream, manager);
        });
    return state;
}

Vec<UPtr<nemesis::AnimationSetDataState>>
nemesis::AnimationSetDataState::ParseObjects(nemesis::LineStream& stream,
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
                                 + ", File: " + nemesis::to_utf8_string(value.GetFilePath()) + ")");
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
                    state->FilePath  = value.GetFilePath();
                    state->StateData = std::make_unique<nemesis::CollectionObject>();
                    auto& data_ptr   = state->StateData;
                    add_object       = [&data_ptr](UPtr<nemesis::NObject>&& obj_ptr)
                    { data_ptr->AddObject(std::move(obj_ptr)); };

                    state_list.emplace_back(std::move(state));
                }
                else if (is_only_number(value.ToString()))
                {
                    for (size_t i = 1;; ++i)
                    {
                        auto* ftoken_ptr = stream.GetForwardToken(i);

                        if (!ftoken_ptr) break;

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
                throw std::runtime_error(
                    "Syntax Error: Unsupport syntax (Line: " + std::to_string(token_value.GetLineNumber())
                    + ", File: " + nemesis::to_utf8_string(token_value.GetFilePath()) + ")");
            }
        }
    }

    return state_list;
}
