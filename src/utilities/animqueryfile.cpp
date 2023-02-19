#include "utilities/animqueryfile.h"
#include "utilities/stringextension.h"

namespace sf = std::filesystem;

void nemesis::AnimQueryFile::AddToList(SPtr<nemesis::AnimQuery>& query)
{
    constlist.emplace_back(query.get());
    list.emplace_back(std::move(query));
}

void nemesis::AnimQueryFile::AddIndexToQuery(SPtr<nemesis::AnimQuery>& query)
{
    query->AddIndex(list.size());
}

nemesis::AnimQueryFile::AnimQueryFile(const sf::path& filepath)
{
    this->filepath = filepath;
    this->type     = GetListFileType(filepath);
}

void nemesis::AnimQueryFile::AddQuery(SPtr<nemesis::AnimQuery>& query)
{
    AddIndexToQuery(query);
    AddToList(query);
}

void nemesis::AnimQueryFile::AddMotionData(size_t linecount, const std::string& data)
{
    if (list.empty()) ErrorMessage(1143, filepath, linecount);

    list.back()->AddMotion(data);
}

void nemesis::AnimQueryFile::AddRotationData(size_t linecount, const std::string& data)
{
    if (list.empty()) ErrorMessage(1143, filepath, linecount);

    if (list.back()->GetMotion().empty()) ErrorMessage(1090, filepath, linecount);

    list.back()->AddRotation(data);
}

bool nemesis::AnimQueryFile::IsNemesis() const noexcept
{
    switch (type)
    {
        case nemesis::File::FileType::NEMESIS_LIST:
        case nemesis::File::FileType::NEMESIS_TEMPLATE:
            return true;
        default:
            return false;
    }
}

bool nemesis::AnimQueryFile::IsEmpty() const noexcept
{
    return list.empty();
}

Vec<SPtr<nemesis::AnimQuery>>& nemesis::AnimQueryFile::GetList() noexcept
{
    return list;
}

const Vec<const nemesis::AnimQuery*>& nemesis::AnimQueryFile::GetList() const noexcept
{
    return constlist;
}

size_t nemesis::AnimQueryFile::GetListSize() const noexcept
{
    return list.size();
}

const nemesis::AnimQuery& nemesis::AnimQueryFile::GetQueryByIndex(size_t index) const
{
    return *constlist.at(index);
}

nemesis::AnimQueryFile::FileType nemesis::AnimQueryFile::GetListFileType(const sf::path& file)
{
    if (sf::is_directory(file) || file.extension().wstring() != L".txt") return FileType::NONE;

    std::string filename = file.stem().string();

    if (!StringEndWith_NC(filename, "_List")) return FileType::NONE;

    if (StringStartWith_NC(filename, "Nemesis_"))
    {
        if (StringEndWith_NC(filename, "_Template")) return FileType::NEMESIS_TEMPLATE;

        return FileType::NEMESIS_LIST;
    }

    if (StringStartWith_NC(filename, "FNIS_")) return FileType::FNIS_LIST;

    return FileType::NONE;
}
