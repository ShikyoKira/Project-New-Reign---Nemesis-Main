#pragma once

#include "utilities/animquery.h"

namespace nemesis
{
    struct AnimQueryFile : nemesis::File
    {
    private:
        Vec<SPtr<nemesis::AnimQuery>> list;
        Vec<const nemesis::AnimQuery*> constlist;

        void AddToList(SPtr<nemesis::AnimQuery>& query);
        void AddIndexToQuery(SPtr<nemesis::AnimQuery>& query);

    public:
        AnimQueryFile(const std::filesystem::path& filepath);

        void AddQuery(SPtr<nemesis::AnimQuery>& query);
        void AddMotionData(size_t linecount, const std::string& data);
        void AddRotationData(size_t linecount, const std::string& data);

        Vec<SPtr<nemesis::AnimQuery>>& GetList() noexcept;
        const Vec<const nemesis::AnimQuery*>& GetList() const noexcept;
        size_t GetListSize() const noexcept;
        const nemesis::AnimQuery& GetQueryByIndex(size_t index) const;

        bool IsNemesis() const noexcept;
        bool IsEmpty() const noexcept;

        static FileType GetListFileType(const std::filesystem::path& file);
    };
}
