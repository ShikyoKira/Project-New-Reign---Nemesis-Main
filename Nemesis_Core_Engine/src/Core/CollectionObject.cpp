#include "Core/CollectionObject.h"
#include "Core/NLine.h"

#include "Core/Statement/ConditionalStatement.h"

#include "Utilities/ConditionSyntax.h"

namespace ns = nemesis::syntax;

nemesis::NLine* nemesis::CollectionObject::ObjectMatcher::GetNextLine()
{
    for (size_t i = BaseIndex; i < Objects.size(); ++i)
    {
        nemesis::NLine* line_ptr = dynamic_cast<nemesis::NLine*>(Objects[i].get());

        if (!line_ptr) continue;

        BaseIndex = i + 1;
        return line_ptr;
    }

    return nullptr;
}

nemesis::CollectionObject::ObjectMatcher::ObjectMatcher(Vec<UPtr<nemesis::NObject>>& objects, size_t ori_size)
    : Objects(objects)
    , OriginalSize(ori_size)
{
}

void nemesis::CollectionObject::ObjectMatcher::MatchAndUpdate(const Vec<UPtr<nemesis::NObject>>& objects)
{
    if (OriginalSize > objects.size())
    {
        throw std::runtime_error("Failed to update node. Missing data or node ID mismatched");
    }

    BaseIndex = 0;
    size_t new_index_begin = 0;

    for (size_t i = 0; i < objects.size(); ++i)
    {
        auto& object   = objects[i];
        auto* line_ptr = dynamic_cast<const nemesis::NLine*>(object.get());

        if (!line_ptr)
        {
            Objects.insert(Objects.begin() + BaseIndex, object->CloneNObject());
            ++BaseIndex;
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

    for (size_t i = new_index_begin; i < objects.size(); ++i)
    {
        auto& object   = objects[i];
        auto* line_ptr = dynamic_cast<const nemesis::NLine*>(object.get());

        if (line_ptr) throw std::runtime_error("Template update line to line does not match");

        Objects.emplace_back(object->CloneNObject());
    }
}

void nemesis::CollectionObject::ObjectMatcher::MatchAndUpdate(const std::string& mod_code,
                                                              const Vec<UPtr<nemesis::NObject>>& objects)
{
    if (OriginalSize > objects.size())
    {
        throw std::runtime_error("Failed to update node. Missing data or node ID mismatched");
    }

    BaseIndex = 0;
    size_t new_index_begin = 0;

    for (size_t i = 0; i < objects.size(); ++i)
    {
        auto& object   = objects[i];
        auto* line_ptr = dynamic_cast<const nemesis::NLine*>(object.get());

        if (!line_ptr)
        {
            Objects.insert(Objects.begin() + BaseIndex, object->CloneNObject());
            ++BaseIndex;
            continue;
        }

        nemesis::NLine* orig_line_ptr = GetNextLine();

        if (!orig_line_ptr)
        {
            if (i == 0) return;

            new_index_begin = i;
            break;
        }

        orig_line_ptr->MatchAndUpdate(mod_code, * line_ptr);
    }

    if (new_index_begin == 0) return;

    for (size_t i = new_index_begin; i < objects.size(); ++i)
    {
        auto& object   = objects[i];
        auto* line_ptr = dynamic_cast<const nemesis::NLine*>(object.get());

        if (line_ptr) throw std::runtime_error("Template update line to line does not match");

        Objects.emplace_back(object->CloneNObject());
    }
}

void nemesis::CollectionObject::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    for (auto& object : Objects)
    {
        object->CompileTo(lines, state);
    }
}

void nemesis::CollectionObject::SerializeTo(DeqNstr& lines) const
{
    for (auto& object : Objects)
    {
        object->SerializeTo(lines);
    }
}

UPtr<nemesis::NObject> nemesis::CollectionObject::CloneNObject() const
{
    return Clone();
}

UPtr<nemesis::CollectionObject> nemesis::CollectionObject::Clone() const
{
    auto collection = std::make_unique<nemesis::CollectionObject>();

    for (auto& object : Objects)
    {
        collection->AddObject(object->CloneNObject());
    }

    return collection;
}

UPtr<nemesis::NObject>& nemesis::CollectionObject::AddObject(UPtr<nemesis::NObject>&& object) noexcept
{
    ++OriginalSize;
    return Objects.emplace_back(std::move(object));
}

nemesis::NObject* nemesis::CollectionObject::GetByIndex(size_t index) noexcept
{
    return Objects[index].get();
}

const nemesis::NObject* nemesis::CollectionObject::GetByIndex(size_t index) const noexcept
{
    return Objects[index].get();
}

void nemesis::CollectionObject::MatchAndUpdate(const nemesis::CollectionObject& object_list)
{
    ObjectMatcher matcher(Objects, OriginalSize);
    matcher.MatchAndUpdate(object_list.Objects);
}

void nemesis::CollectionObject::MatchAndUpdate(const std::string& mod_code,
                                               const nemesis::CollectionObject& object_list)
{
    ObjectMatcher matcher(Objects, OriginalSize);
    matcher.MatchAndUpdate(mod_code, object_list.Objects);
}

size_t nemesis::CollectionObject::Size() noexcept
{
    return Objects.size();
}

bool nemesis::CollectionObject::Empty() noexcept
{
    return Objects.empty();
}
