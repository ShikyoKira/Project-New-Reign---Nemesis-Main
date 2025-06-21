#pragma once

#include "Core/NObject.h"

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

            size_t BaseIndex, OriginalSize;

            nemesis::NLine* GetNextLine();

        public:
            ObjectMatcher(Vec<UPtr<nemesis::NObject>>& objects, size_t ori_size);

            void MatchAndUpdate(const Vec<UPtr<nemesis::NObject>>& objects);
            void MatchAndUpdate(const std::string& mod_code, const Vec<UPtr<nemesis::NObject>>& objects);
        };

    protected:
        size_t OriginalSize = 0;
        Vec<UPtr<nemesis::NObject>> Objects;

    public:
        void CompileTo(DeqNstr& lines, nemesis::CompileState& state) const override;
        void SerializeTo(DeqNstr& lines) const override;

        UPtr<nemesis::NObject> CloneNObject() const override;
        virtual UPtr<nemesis::CollectionObject> Clone() const;

        UPtr<nemesis::NObject>& AddObject(UPtr<nemesis::NObject>&& object) noexcept;
        nemesis::NObject* GetByIndex(size_t index) noexcept;
        const nemesis::NObject* GetByIndex(size_t index) const noexcept;

        void MatchAndUpdate(const nemesis::CollectionObject& object_list);
        void MatchAndUpdate(const std::string& mod_code, const nemesis::CollectionObject& object_list);

        size_t Size() noexcept;
        bool Empty() noexcept;
    };
}
