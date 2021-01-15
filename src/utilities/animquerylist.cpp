#include "utilities/animquerylist.h"
#include "utilities/stringextension.h"

namespace sf = std::filesystem;

nemesis::AnimQueryList::AnimQueryList(const sf::path& filepath)
{
    this->filepath = filepath;
    this->listtype = GetListFileType(filepath);
}

void nemesis::AnimQueryList::AddQuery(const std::shared_ptr<const nemesis::AnimQuery>& query)
{
    list.emplace_back(std::move(query));
}

bool nemesis::AnimQueryList::IsNemesis() const noexcept
{
    return listtype == NEMESIS;
}

bool nemesis::AnimQueryList::IsEmpty() const noexcept
{
    return list.empty();
}

const std::vector<std::shared_ptr<const nemesis::AnimQuery>>& nemesis::AnimQueryList::GetList() const noexcept
{
    return list;
}

const std::filesystem::path& nemesis::AnimQueryList::GetFilePath() const noexcept
{
    return filepath;
}

nemesis::AnimQueryList::ListType nemesis::AnimQueryList::GetListFileType(const sf::path& file)
{
    if (sf::is_directory(file) || file.extension().wstring() != L".txt") return ListType::NONE;

    std::string filename = file.stem().string();

    if (!StringEndWith_NC(filename, "_List")) return ListType::NONE;

    if (StringStartWith_NC(filename, "Nemesis")) return ListType::NEMESIS;

    if (StringStartWith_NC(filename, "FNIS")) return ListType::FNIS;

    return ListType::NONE;
}
