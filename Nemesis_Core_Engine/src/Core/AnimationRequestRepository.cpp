#include "Core/AnimationRequestRepository.h"
#include "Core/Template.h"

#include <iostream>
#include <regex>

#include "Utilities/Algorithm.h"

#include "AlternateAnimation/AlterAnimRepository.h"

#include "Logger.h"

void nemesis::AnimationRequestRepository::AddRequestsFromFile(const std::filesystem::path& filepath,
                                                              nemesis::TemplateRepository& templt_repo,
                                                              nemesis::AlterAnimRepository& alter_anim_repo)
{
    Logger::Log("Animation Request File: " + nemesis::to_utf8_string(filepath.stem()), true);

    VecNstr lines;
    GetFileLines(filepath, lines, false);

    Vec<nemesis::AnimationRequest*> request_layers;
    std::string aa_prefix;
    std::filesystem::path dir_path = filepath.parent_path();
    std::string list_name          = nemesis::to_utf8_string(dir_path.stem());

    for (auto& line : lines)
    {
        if (nemesis::starts_with(line, "MD "))
        {
            request_layers.back()->SetMotionData(line.substr(3));
            continue;
        }

        if (nemesis::starts_with(line, "RD "))
        {
            request_layers.back()->SetRotationData(line.substr(3));
            continue;
        }

        if (nemesis::starts_with(line, "AAprefix "))
        {
            aa_prefix = line.substr(9).ToString();
            continue;
        }

        std::smatch match;

        if (nemesis::starts_with(line, "AAset "))
        {
            static std::regex aaset_rgx("^AAset ([^\\s]+) ([0-9]+)$");

            if (!std::regex_match(line.ToString(), match, aaset_rgx))
            {
                throw std::runtime_error("Invalid Command: Incorrect AAset syntax (Line: "
                                         + std::to_string(line.GetLineNumber())
                                         + ", File: " + nemesis::to_utf8_string(filepath) + ")");
            }

            auto group = alter_anim_repo.GetAlterAnimGroupByName(match[1]);

            if (!group)
            {
                throw std::runtime_error("Invalid Command: AAset group (" + match[1].str()
                                         + ") cannot be found (Line: " + std::to_string(line.GetLineNumber())
                                         + ", File: " + nemesis::to_utf8_string(filepath) + ")");
            }

            group->AddPrefix(dir_path, aa_prefix, std::stol(match[2]));
            continue;
        }

        static std::regex ext_rgx("^([\\+]+) .+$");

        if (std::regex_match(line.ToString(), match, ext_rgx))
        {
            std::string plus  = match[1];
            size_t length     = plus.length();
            size_t layer_size = request_layers.size();

            if (length == layer_size)
            {
                auto parent = request_layers.back();
                auto child_request
                    = parent->GetTemplateClass().CreateRequest(list_name, line, line.GetLineNumber(), filepath);
                request_layers.emplace_back(child_request.get());
                parent->AddRequest(std::move(child_request));
                continue;
            }

            if (length > layer_size)
            {
                throw std::runtime_error("Invalid Command: Parent level (" + std::to_string(length)
                                         + ") cannot be found (Line: " + std::to_string(line.GetLineNumber())
                                         + ", File: " + nemesis::to_utf8_string(filepath) + ")");
            }

            while (length + 1 < request_layers.size())
            {
                request_layers.pop_back();
            }

            auto parent = request_layers[length - 1];
            auto child_request
                = parent->GetTemplateClass().CreateRequest(list_name, line, line.GetLineNumber(), filepath);
            request_layers.back() = child_request.get();
            parent->AddRequest(std::move(child_request));
            continue;
        }

        auto request = templt_repo.CreateRequest(
            list_name, line.substr(0, line.find("'")), line.GetLineNumber(), filepath);

        if (request)
        {
            request_layers = {request.get()};
            AddRequest(std::move(request));
            continue;
        }
    }
}

void nemesis::AnimationRequestRepository::AddRequestsFromDirectory(
    const std::filesystem::path& dir,
    nemesis::TemplateRepository& templt_repo,
    nemesis::AlterAnimRepository& alter_anim_repo)
{
    auto fnis_list_file = LITERAL_PATH("fnis_") + PATH_TO_STRING(dir.stem()) + LITERAL_PATH("_list");
    auto nemesis_list_file = LITERAL_PATH("nemesis_") + PATH_TO_STRING(dir.stem()) + LITERAL_PATH("_list");

    for (auto& entry : std::filesystem::directory_iterator(dir))
    {
        auto path = entry.path();

        if (entry.is_directory())
        {
            AddRequestsFromDirectory(path, templt_repo, alter_anim_repo);
            continue;
        }

        if (!nemesis::iequals(PATH_TO_STRING(path.extension()), LITERAL_PATH(".txt"))) continue;

        auto file = PATH_TO_STRING(path.stem());

        if (!nemesis::iequals(file, fnis_list_file) && !nemesis::iequals(file, nemesis_list_file)) continue;

        AddRequestsFromFile(path, templt_repo, alter_anim_repo);
    }
}

nemesis::AnimationRequestRepository::AnimationRequestRepository(const std::filesystem::path& data_dir,
                                                                nemesis::TemplateRepository& templt_repo,
                                                                nemesis::AlterAnimRepository& alter_anim_repo)
{
    Logger::Log("Processing Animation Requests...", true);
    std::cout << std::endl;

    AddRequestsFromDirectory(data_dir, templt_repo, alter_anim_repo);
}

void nemesis::AnimationRequestRepository::AddRequest(UPtr<nemesis::AnimationRequest>&& request)
{
    auto& collection = Requests[request->GetTemplateName()];
    request->SetIndex(collection.size());
    collection.emplace_back(std::move(request));
}

const nemesis::AnimationRequestCollection&
nemesis::AnimationRequestRepository::GetRequests(const std::string& tmplt_name) const
{
    static nemesis::AnimationRequestCollection Proxy;
    auto itr = Requests.find(tmplt_name);
    return itr == Requests.end() ? Proxy : itr->second;
}
