#include "ExAnimation/ExAnimationPack.h"

#include <regex>

#include "Utilities/Algorithm.h"

#include "Logger.h"


namespace sf = std::filesystem;

nemesis::ExAnimationPack::ExAnimationPack(const std::filesystem::path& pack_dir,
                                          const std::filesystem::path& aim_dir,
                                          const std::filesystem::path& ex_anim_dir)
{
    static const std::regex folder_rgx("^([0-9]+)(.+)$");
    std::smatch match;
    std::string folder_name = pack_dir.stem().string();

    if (!std::regex_match(folder_name, match, folder_rgx))
    {
        throw std::runtime_error("Invalid ExAnimation folder name (ExAnimation folder: " + pack_dir.string()
                                 + ")");
    }

    Order = std::stoi(match.str(1));
    Name  = match.str(2);
    Logger::Log("ExAnimations: " + Name + " (" + std::to_string(Order) + ")");

    if (!std::filesystem::exists(pack_dir)) return;

    for (auto entry : sf::directory_iterator(pack_dir))
    {
        if (entry.is_directory()) continue;

        sf::path path         = entry.path();
        sf::path ex_anim_name = L"ex_" + std::to_wstring(Order) + L"_" + path.stem().wstring()
                                + nemesis::to_lower_copy(path.extension().wstring());
        RequestList.emplace_back(*this,
                                 aim_dir / (path.stem().wstring() + path.extension().wstring()),
                                 ex_anim_dir / ex_anim_name);
    }
}

unsigned short nemesis::ExAnimationPack::GetOrder() const noexcept
{
    return Order;
}

const std::string& nemesis::ExAnimationPack::GetName() const noexcept
{
    return Name;
}

std::string nemesis::ExAnimationPack::GetVariableName() const noexcept
{
    return "Nemesis_Ex_" + Name;
}

const Vec<nemesis::ExAnimationRequest>& nemesis::ExAnimationPack::GetRequestList() const noexcept
{
    return RequestList;
}

const nemesis::ExAnimationRequest*
nemesis::ExAnimationPack::GetExAnimRequest(const std::filesystem::path& canon_anim_path) const
{
    {
        std::shared_lock<std::shared_mutex> lock(ExAnimMapMutex);
        auto req_it = ExAnimMap.find(canon_anim_path);

        if (req_it != ExAnimMap.end()) return req_it->second;
    }

    std::unique_lock<std::shared_mutex> writer_lock(OngoingGetterMutex);
    auto getter_it = OngoingGetter.find(canon_anim_path);

    if (getter_it != OngoingGetter.end())
    {
        auto future = getter_it->second;
        writer_lock.unlock();
        return future.get();
    }

    std::promise<const nemesis::ExAnimationRequest*> promise;
    std::shared_future<const nemesis::ExAnimationRequest*> future = promise.get_future();
    OngoingGetter.insert({canon_anim_path, future});

    writer_lock.unlock();

    const nemesis::ExAnimationRequest* cur_req = nullptr;

    for (auto& request : RequestList)
    {
        auto req_path = request.GetCanonAnimPath();

        if (!nemesis::iequals(req_path, canon_anim_path)) continue;

        cur_req = &request;
        break;
    }

    {
        std::unique_lock<std::shared_mutex> lock(ExAnimMapMutex);
        ExAnimMap.insert({canon_anim_path, cur_req});
    }

    promise.set_value(cur_req);

    std::unique_lock<std::shared_mutex> lock(OngoingGetterMutex);
    OngoingGetter.erase(canon_anim_path);
    return cur_req;
}
