#include "core/NLine.h"
#include "core/condition.h"
#include "core/CompileState.h"
#include "core/CollectionObject.h"

#include "core/Statements/ConditionalStatement.h"

#include "utilities/conditionsyntax.h"

namespace ns = nemesis::syntax;

nemesis::NLine* nemesis::CollectionObject::ObjectMatcher::GetNextLine()
{
    for (size_t i = BaseIndex; i < Objects.size(); i++)
    {
        nemesis::NLine* line_ptr = dynamic_cast<nemesis::NLine*>(Objects[i].get());

        if (!line_ptr) continue;

        BaseIndex = i + 1;
        return line_ptr;
    }

    return nullptr;
}

nemesis::CollectionObject::ObjectMatcher::ObjectMatcher(Vec<UPtr<nemesis::NObject>>& objects)
    : Objects(objects)
{
}

void nemesis::CollectionObject::ObjectMatcher::MatchAndUpdate(Vec<UPtr<nemesis::NObject>>& objects)
{
    if (Objects.size() > objects.size())
    {
        throw std::runtime_error("Failed to update node. Missing data or node ID mismatched");
    }

    BaseIndex = 0;
    size_t new_index_begin = 0;

    for (size_t i = 0; i < objects.size(); i++)
    {
        auto& object   = objects[i];
        auto* line_ptr = dynamic_cast<nemesis::NLine*>(object.get());

        if (!line_ptr)
        {
            Objects.insert(Objects.begin() + BaseIndex, std::move(object));
            continue;
        }

        nemesis::NLine* orig_line_ptr = GetNextLine();

        if (!orig_line_ptr)
        {
            if (i == 0) return;

            new_index_begin = i;
            break;
        }

        orig_line_ptr->MatchAndUpdate(*line_ptr);
    }

    if (new_index_begin == 0) return;

    for (size_t i = new_index_begin; i < objects.size(); i++)
    {
        auto& object   = objects[i];
        auto* line_ptr = dynamic_cast<nemesis::NLine*>(object.get());

        if (!line_ptr) throw std::runtime_error("Template update line to line does not  match");

        Objects.emplace_back(std::move(object));
    }
}

void nemesis::CollectionObject::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    for each (auto& object in Objects)
    {
        object->CompileTo(lines, state);
    }
}

void nemesis::CollectionObject::SerializeTo(DeqNstr& lines) const
{
    for each (auto& object in Objects)
    {
        object->SerializeTo(lines);
    }
}

void nemesis::CollectionObject::AddObject(UPtr<nemesis::NObject>&& object) noexcept
{
    Objects.emplace_back(std::move(object));
}

void nemesis::CollectionObject::MatchAndUpdate(nemesis::CollectionObject& object_list)
{
    ObjectMatcher matcher(Objects);
    matcher.MatchAndUpdate(object_list.Objects);
}

size_t nemesis::CollectionObject::Size() noexcept
{
    return Objects.size();
}

bool nemesis::CollectionObject::Empty() noexcept
{
    return Objects.empty();
}
