#pragma once

#include <vector>
#include <filesystem>

#include "utilities/animquery.h"

namespace nemesis
{
    struct AnimQueryList
    {
        enum ListType
        {
            NONE,
            FNIS,
            NEMESIS,
        };

    private:
        ListType listtype;
        std::filesystem::path filepath;
        std::vector<std::shared_ptr<const nemesis::AnimQuery>> list;

    public:
        AnimQueryList(const std::filesystem::path& filepath);

        void AddQuery(const std::shared_ptr<const nemesis::AnimQuery>& query);

        const std::vector<std::shared_ptr<const nemesis::AnimQuery>>& GetList() const noexcept;

        bool IsNemesis() const noexcept;
        bool IsEmpty() const noexcept;

        const std::filesystem::path& GetFilePath() const noexcept;
        static ListType GetListFileType(const std::filesystem::path& file);
    };
}
