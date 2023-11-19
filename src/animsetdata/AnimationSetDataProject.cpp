#include "animsetdata/AnimationSetDataProject.h"

#include "core/ModObject.h"
#include "core/ForEachObject.h"

bool nemesis::AnimationSetDataProject::IsProjectEnd(nemesis::LineStream& stream, bool& start)
{
    auto* token_ptr = &stream.GetToken();

    while (token_ptr->Type != nemesis::LineStream::NONE)
    {
        ++stream;
        token_ptr = &stream.GetToken();
    }

    return token_ptr->Value == "V3";
}

Vec<UPtr<nemesis::NObject>> nemesis::AnimationSetDataProject::ParseModObjects(
    nemesis::LineStream& stream, nemesis::SemanticManager& manager, int start_position)
{
    bool has_new_project = false;
    return ParseModObjects(stream, manager, start_position, has_new_project);
}

Vec<UPtr<nemesis::NObject>> nemesis::AnimationSetDataProject::ParseModObjects(
    nemesis::LineStream& stream, nemesis::SemanticManager& manager, int start_position, bool& has_new_project)
{
    has_new_project = false;
    Deq<const nemesis::Line*> ori_lines;
    Deq<const nemesis::Line*> mod_lines;
    Deq<const nemesis::Line*>* lines = &mod_lines;
    Vec<UPtr<nemesis::NObject>> object_list;
     
    for (; stream.IsEoF(); ++stream)
    {
        auto* token_ptr = &stream.GetToken();

        switch (token_ptr->Type)
        {
            case nemesis::LineStream::MOD_CLOSE:
            {
                auto& mod_code = token_ptr->Value;

                while (lines->empty())
                {
                    auto& line_ptr = lines->front();
                    auto nline_ptr = std::make_unique<nemesis::NLine>(*line_ptr, manager);

                    if (mod_lines.empty())
                    {
                        nline_ptr->AddModLine(
                            mod_code, mod_code.GetLineNumber(), mod_code.GetFilePath(), manager);
                    }
                    else
                    {
                        nline_ptr->AddModLine(mod_code,
                                              mod_code.GetLineNumber(),
                                              mod_code.GetFilePath(),
                                              manager,
                                              *mod_lines.front());
                        mod_lines.pop_front();
                    }

                    lines->pop_front();
                    object_list.emplace_back(std::move(nline_ptr));
                }

                if (mod_lines.empty()) return object_list;

                auto collection = std::make_unique<nemesis::CollectionObject>();
                auto col_ptr    = collection.get();
                auto mod_object = std::make_unique<nemesis::ModObject>(mod_code,
                                                                       mod_code.GetLineNumber(),
                                                                       mod_code.GetFilePath(),
                                                                       manager,
                                                                       std::move(collection));

                for (auto& mod_line : mod_lines)
                {
                    col_ptr->AddObject(std::make_unique<nemesis::NLine>(*mod_line, manager));
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

                has_new_project = true;
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

UPtr<nemesis::ForEachObject> nemesis::AnimationSetDataProject::ParseForEachObjects(
    nemesis::LineStream& stream, nemesis::SemanticManager& manager, int start_position)
{
    bool has_new_project = false;
    return ParseForEachObjects(stream, manager, start_position, has_new_project);
}

UPtr<nemesis::ForEachObject> nemesis::AnimationSetDataProject::ParseForEachObjects(
    nemesis::LineStream& stream, nemesis::SemanticManager& manager, int start_position, bool& has_new_project)
{
    has_new_project = false;
    Deq<UPtr<nemesis::NObject>> object_list;

    for (; stream.GetPosition() == start_position - 1; --stream)
    {
        auto* token_ptr = &stream.GetToken();

        switch (token_ptr->Type)
        {
            case nemesis::LineStream::FOR_EACH:
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
                auto mod_objects = ParseModObjects(stream, manager, start_position);

                for (auto it = mod_objects.rbegin(); it != mod_objects.rend(); ++it)
                {
                    object_list.emplace_front(std::move(*it));
                }

                break;
            }
            case nemesis::LineStream::END_IF:
            {
                ++stream;
                auto if_obj = ParseIfObjects(stream, manager, start_position);
                object_list.emplace_front(std::move(if_obj));
                break;
            }
            case nemesis::LineStream::CLOSE:
            {
                ++stream;
                auto fe_obj = ParseForEachObjects(stream, manager, start_position);
                object_list.emplace_front(std::move(fe_obj));
                break;
            }
            case nemesis::LineStream::NONE:
            {
                auto& token_value = token_ptr->Value;
                object_list.emplace_front(std::make_unique<nemesis::NLine>(
                    token_value, token_value.GetLineNumber(), token_value.GetFilePath(), manager));

                if (token_value != "V3") break;

                has_new_project = true;
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

UPtr<nemesis::IfObject> nemesis::AnimationSetDataProject::ParseIfObjects(nemesis::LineStream& stream,
                                                                         nemesis::SemanticManager& manager,
                                                                         int start_position)
{
    bool has_new_project = false;
    return ParseIfObjects(stream, manager, start_position, has_new_project);
}

UPtr<nemesis::IfObject> nemesis::AnimationSetDataProject::ParseIfObjects(nemesis::LineStream& stream,
                                                                         nemesis::SemanticManager& manager,
                                                                         int start_position,
                                                                         bool& has_new_project)
{
    has_new_project = false;
    Deq<UPtr<nemesis::NObject>> object_list;
    UPtr<nemesis::IfObject> if_obj;

    Vec<Pair<nemesis::Line, UPtr<nemesis::CollectionObject>>> else_if_list;
    UPtr<nemesis::CollectionObject> else_collection;

    for (; stream.GetPosition() == start_position - 1; --stream)
    {
        auto* token_ptr = &stream.GetToken();

        switch (token_ptr->Type)
        {
            case nemesis::LineStream::IF:
            {
                auto& line      = token_ptr->Value;
                auto collection = std::make_unique<nemesis::CollectionObject>();
                auto* col_ptr   = collection.get();
                if_obj          = std::make_unique<nemesis::IfObject>(
                    line, line.GetLineNumber(), line.GetFilePath(), manager, std::move(collection));

                for (auto& obj : object_list)
                {
                    col_ptr->AddObject(std::move(obj));
                }

                object_list.clear();

                for (auto& else_if : else_if_list)
                {
                    if_obj->ElseIf(else_if.first,
                                   else_if.first.GetLineNumber(),
                                   else_if.first.GetFilePath(),
                                   manager,
                                   std::move(else_if.second));
                }

                if (else_collection)
                {
                    if_obj->Else(std::move(else_collection));
                }

                return if_obj;
            }
            case nemesis::LineStream::ELSE_IF:
            {
                auto collection = std::make_unique<nemesis::CollectionObject>();

                for (auto& obj : object_list)
                {
                    collection->AddObject(std::move(obj));
                }

                object_list.clear();
                else_if_list.emplace_back(token_ptr->Value, std::move(collection));
                break;
            }
            case nemesis::LineStream::ELSE:
            {
                if (!else_if_list.empty())
                {
                    std::runtime_error("Syntax error: ELSE IF syntax cannot come after ELSE (Line: "
                                       + std::to_string(token_ptr->Value.GetLineNumber())
                                       + ", File: " + token_ptr->Value.GetFilePath().string() + ")");
                }

                else_collection = std::make_unique<nemesis::CollectionObject>();

                for (auto& obj : object_list)
                {
                    else_collection->AddObject(std::move(obj));
                }

                object_list.clear();
                break;
            }
            case nemesis::LineStream::MOD_CLOSE:
            {
                ++stream;
                auto mod_objects = ParseModObjects(stream, manager, start_position);

                for (auto it = mod_objects.rbegin(); it != mod_objects.rend(); ++it)
                {
                    object_list.emplace_front(std::move(*it));
                }

                break;
            }
            case nemesis::LineStream::CLOSE:
            {
                ++stream;
                auto fe_obj = ParseForEachObjects(stream, manager, start_position);
                object_list.emplace_front(std::move(fe_obj));
                break;
            }
            case nemesis::LineStream::NONE:
            {
                auto& token_value = token_ptr->Value;
                object_list.emplace_front(std::make_unique<nemesis::NLine>(
                    token_value, token_value.GetLineNumber(), token_value.GetFilePath(), manager));

                if (token_value != "V3") break;

                has_new_project = true;
                break;
            }
            case nemesis::LineStream::END_IF:
            {
                ++stream;
                auto if_obj = ParseIfObjects(stream, manager, start_position);
                object_list.emplace_front(std::move(if_obj));
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
    throw std::runtime_error("Syntax Error: Missing IF statement (Line: "
                             + std::to_string(token_value.GetLineNumber())
                             + ", File: " + token_value.GetFilePath().string() + ")");
}

void nemesis::AnimationSetDataProject::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    DeqNstr templines;
    ProjectData->CompileTo(templines, state);

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
            case nemesis::AnimationSetDataProject::EquipCounter:
            {
                counter_nline = &(*itr);
                counter       = 0;
                data_type     = nemesis::AnimationSetDataProject::EquipList;
                itr++;
            }
            case nemesis::AnimationSetDataProject::EquipList:
            {
                if (!isOnlyNumber(*itr))
                {
                    counter++;
                    break;
                }

                *counter_nline = nemesis::Line(
                    std::to_string(counter), counter_nline->GetLineNumber(), counter_nline->GetFilePath());
            }
            case nemesis::AnimationSetDataProject::TypeCounter:
            {
                counter_nline = &(*itr);
                counter       = 0;
                data_type     = nemesis::AnimationSetDataProject::TypeList;
                itr++;
            }
            case nemesis::AnimationSetDataProject::TypeList:
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
            case nemesis::AnimationSetDataProject::AnimationCounter:
            {
                counter_nline = &(*itr);
                counter       = 0;
                data_type     = nemesis::AnimationSetDataProject::AnimationList;
                itr++;
            }
            case nemesis::AnimationSetDataProject::AnimationList:
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
            case nemesis::AnimationSetDataProject::Crc32Counter:
            {
                counter_nline = &(*itr);
                counter       = 0;
                data_type     = nemesis::AnimationSetDataProject::Crc32List;
                itr++;
            }
            case nemesis::AnimationSetDataProject::Crc32List:
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

                *counter_nline = nemesis::Line(
                    std::to_string(counter / 3), counter_nline->GetLineNumber(), counter_nline->GetFilePath());

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

void nemesis::AnimationSetDataProject::SerializeTo(DeqNstr& lines) const
{
    ProjectData->SerializeTo(lines);
}

Deq<UPtr<nemesis::NObject>>
nemesis::AnimationSetDataProject::ParseObjects(nemesis::LineStream& stream,
                                               nemesis::SemanticManager& manager,
                                               Deq<nemesis::AnimationSetDataProject*>& project_list)
{
    Deq<UPtr<nemesis::NObject>> object_list;
    Deq<UPtr<nemesis::NObject>> temp_object_list;

    bool has_new_project = false;

    size_t size = stream.GetSize();
    int cur_pos = stream.GetPosition();
    stream.GetForwardToken(size - cur_pos);

    for (; stream.GetPosition() == cur_pos - 1; --stream)
    {
        auto* token_ptr = &stream.GetToken();

        switch (token_ptr->Type)
        {
            case nemesis::LineStream::MOD_CLOSE:
            {
                ++stream;
                auto list = ParseModObjects(stream, manager, cur_pos, has_new_project);

                if (!has_new_project)
                {
                    std::move(
                        list.begin(), list.end(), std::inserter(temp_object_list, temp_object_list.begin()));
                    break;
                }

                if (temp_object_list.empty())
                {
                    std::move(list.begin(), list.end(), std::inserter(object_list, object_list.begin()));
                    break;
                }

                auto collection = std::make_unique<nemesis::CollectionObject>();

                for (auto& each : list)
                {
                    collection->AddObject(std::move(each));
                }

                for (auto& temp_obj : temp_object_list)
                {
                    collection->AddObject(std::move(temp_obj));
                }

                temp_object_list.clear();
                object_list.emplace_front(std::move(collection));
                break;
            }
            case nemesis::LineStream::CLOSE:
            {
                ++stream;
                auto fe_obj = ParseForEachObjects(stream, manager, cur_pos, has_new_project);

                if (!has_new_project)
                {
                    temp_object_list.emplace_front(std::move(fe_obj));
                    break;
                }

                if (temp_object_list.empty())
                {
                    object_list.emplace_front(std::move(fe_obj));
                    break;
                }

                auto collection = std::make_unique<nemesis::CollectionObject>();
                collection->AddObject(std::move(fe_obj));

                for (auto& temp_obj : temp_object_list)
                {
                    collection->AddObject(std::move(temp_obj));
                }

                temp_object_list.clear();
                object_list.emplace_front(std::move(collection));
                break;
            }
            case nemesis::LineStream::END_IF:
            {
                ++stream;
                auto if_obj = ParseIfObjects(stream, manager, cur_pos, has_new_project);

                if (!has_new_project)
                {
                    temp_object_list.emplace_front(std::move(if_obj));
                    break;
                }

                if (temp_object_list.empty())
                {
                    object_list.emplace_front(std::move(if_obj));
                    break;
                }

                auto collection = std::make_unique<nemesis::CollectionObject>();
                collection->AddObject(std::move(if_obj));

                for (auto& temp_obj : temp_object_list)
                {
                    collection->AddObject(std::move(temp_obj));
                }

                temp_object_list.clear();
                object_list.emplace_front(std::move(collection));
                break;
            }
            case nemesis::LineStream::NONE:
            {
                temp_object_list.emplace_front(std::make_unique<nemesis::NLine>(token_ptr->Value, manager));

                if (token_ptr->Value != "V3") break;

                auto project         = std::make_unique<nemesis::AnimationSetDataProject>();
                project->ProjectData = std::make_unique<nemesis::CollectionObject>();
                auto& data_ptr       = project->ProjectData;

                for (auto& temp_obj : temp_object_list)
                {
                    data_ptr->AddObject(std::move(temp_obj));
                }

                temp_object_list.clear();
                object_list.emplace_front(std::move(project));
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
