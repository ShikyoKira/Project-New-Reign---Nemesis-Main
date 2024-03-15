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

            void MatchAndUpdate(const Vec<UPtr<nemesis::NObject>>& objects);
        };

    protected:
        Vec<UPtr<nemesis::NObject>> Objects;

    public:
        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        UPtr<nemesis::CollectionObject> Clone() const;

        UPtr<nemesis::NObject>& AddObject(UPtr<nemesis::NObject>&& object) noexcept;
        nemesis::NObject* GetByIndex(size_t index) noexcept;
        const nemesis::NObject* GetByIndex(size_t index) const noexcept;

        void MatchAndUpdate(const nemesis::CollectionObject& object_list);

        size_t Size() noexcept;
        bool Empty() noexcept;
    };
}
