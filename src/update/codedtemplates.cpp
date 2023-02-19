#include "update/codedtemplates.h"

#include "utilities/algorithm.h"
#include "utilities/constants.h"

namespace sf = std::filesystem;

void nemesis::CodedTemplates::AddCharcAnimDataTemplate(const std::string& character,
                                                       const std::filesystem::path& path)
{
    VecStr headerlist;
    read_directory(path, headerlist);

    for (auto& header : headerlist)
    {
        sf::path curfile(path / header);

        if (sf::is_directory(curfile)) continue;

        ValidateAnimDataTemplateFile(curfile);
        ValidateAnimDataTemplateCharc(character, curfile);

        nemesis::AnimDataTemplatePatch patch(curfile);
        patch.character = character;
        patch.header    = header;
        animdata_patches.emplace_back(patch);

        if (error) throw nemesis::exception();
    }
}

void nemesis::CodedTemplates::ValidateAnimDataTemplateFile(const std::filesystem::path& path)
{
    std::string filename = path.stem().string();
    std::string tempname
        = nemesis::regex_replace(filename, nemesis::regex("[^~]*~([0-9]+)"), std::string("$1"));

    if (tempname == filename) return;

    if (isOnlyNumber(tempname) && hasAlpha(filename.substr(0, filename.find("~")))) return;

    ErrorMessage(2004, path);
}

void nemesis::CodedTemplates::ValidateAnimDataTemplateCharc(const std::string& character,
                                                            const std::filesystem::path& path)
{
    if (character.find("~") != NOT_FOUND) return;

    if (character != "$header$") return;

    ErrorMessage(3022, path);
}

void nemesis::CodedTemplates::ParseFolder(const std::filesystem::path& path)
{
    if (!sf::is_directory(path)) return;

    VecWstr contents;
    read_directory(path, contents);
    auto currentfolder = path.stem();

    for (auto& each : contents)
    {
        auto filepath = path / each;

        if (!sf::is_directory(filepath)) continue;

        if (nemesis::iequals(each, nemesis::foldername::animdata))
        {
            ParseAnimDataFolder(filepath);
            continue;
        }

        if (nemesis::iequals(each, nemesis::foldername::animsetdata))
        {
            ParseAnimSetDataFolder(filepath);
            continue;
        }

        ParseBehaviorFolder(currentfolder / each, filepath);
    }
}

void nemesis::CodedTemplates::ParseAnimDataFolder(const std::filesystem::path& path)
{
    VecStr characterlist;
    read_directory(path, characterlist);

    for (auto& character : characterlist)
    {
        sf::path charfile(path / character);

        if (sf::is_directory(charfile))
        {
            AddCharcAnimDataTemplate(character, charfile);
            continue;
        }

        std::string filename = charfile.stem().string();

        if (filename == "$header$")
        {
            nemesis::File file(charfile);
            VecNstr lines = file.GetLines();
            auto& first   = lines.front();

            if (hasAlpha(first)) continue;

            if (isOnlyNumber(first)) continue;

            ErrorMessage(3006, filename, charfile);
        }

        if (!nemesis::regex_match(filename, nemesis::regex("^\\$(?!" + code + ").+\\$(?:UC|)$"))) continue;

        ErrorMessage(3023, "$" + code + "$" + (filename.find("$UC") != NOT_FOUND ? "UC" : ""));
    }
}

void nemesis::CodedTemplates::ParseAnimSetDataFolder(const std::filesystem::path& path)
{
    VecStr projectfilelist;
    read_directory(path, projectfilelist);

    for (const auto& file : projectfilelist)
    {
        std::string projfile = nemesis::to_lower_copy(file);
        sf::path filepath    = path / file;

        if (!sf::is_directory(filepath) || file.find("~") == NOT_FOUND) continue;

        VecStr headerfiles;
        read_directory(path, headerfiles);

        if (headerfiles.empty()) continue;

        std::replace_if(
            projfile.begin(), projfile.end(), [](const char& ch) { return ch == '~'; }, '\\');

        for (auto& file : headerfiles)
        {
            std::filesystem::path filepath(path / file);

            if (std::filesystem::is_directory(filepath)) continue;

            if (!nemesis::iequals(filepath.extension().string(), ".txt")) continue;

            std::string name = filepath.stem().string();

            if (name.length() > 2 && name.front() == '$' && name.back() == '$') continue;

            animsetdata_patches.emplace_back(code, projfile, filepath);
        }
    }
}

void nemesis::CodedTemplates::ParseBehaviorFolder(const std::filesystem::path& datapath,
                                                  const std::filesystem::path& path)
{
    VecWstr filelist;
    read_directory(path, filelist);

    for (auto& file : filelist)
    {
        auto filepath = path / file;

        if (sf::is_directory(filepath))
        {
            ParseBehaviorFolder(datapath / file, filepath);
            continue;
        }

        if (file.front() != '#') continue;

        patches.emplace_back(code, filepath, datapath);
    }
}

nemesis::CodedTemplates::CodedTemplates(const std::filesystem::path& path)
    : code(path.stem().string())
{
    if (!sf::is_directory(path)) return;

    VecWstr contents;
    read_directory(path, contents);

    for (auto& each : contents)
    {
        ParseFolder(path / each);
    }
}

std::string_view nemesis::CodedTemplates::GetCode() const
{
    return code;
}

const Vec<nemesis::TemplatePatch>& nemesis::CodedTemplates::GetPatches() const
{
    return patches;
}

const Vec<nemesis::AnimDataTemplatePatch>& nemesis::CodedTemplates::GetADPatches() const
{
    return animdata_patches;
}

const Vec<nemesis::AnimSetDataTemplatePatch>& nemesis::CodedTemplates::GetASDPatches() const
{
    return animsetdata_patches;
}
