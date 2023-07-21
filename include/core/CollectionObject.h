#pragma once

#include "core/NObject.h"

namespace nemesis
{
    struct NLine;

    struct CollectionObject : public nemesis::NObject
    {
    private:
        struct ObjectMatcher
        {
        private:
            Vec<UPtr<nemesis::NObject>>& Objects;

            size_t BaseIndex;

            nemesis::NLine* GetNextLine();

        public:
            ObjectMatcher(Vec<UPtr<nemesis::NObject>>& objects);

            void MatchAndUpdate(Vec<UPtr<nemesis::NObject>>& objects);
        };

    protected:
        Vec<UPtr<nemesis::NObject>> Objects;

    public:
        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        void AddObject(UPtr<nemesis::NObject>&& object) noexcept;

        void MatchAndUpdate(nemesis::CollectionObject& object_list);

        size_t Size() noexcept;
        bool Empty() noexcept;
    };
}
