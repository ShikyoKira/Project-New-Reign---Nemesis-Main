#include <stdexcept>
#include <fstream>
#include <iostream>

#include "Core/CacheManager.h"

#include "Utilities/Sha256.h"

#include "NemesisInfo.h"

nemesis::CacheManager nemesis::CacheManager::Manager;

void nemesis::CacheManager::AddEntry(const std::string& checksum, const std::string& data)
{
    std::scoped_lock<std::mutex> lock(Manager.EntryMutex);
    auto& entry      = Manager.Entries[checksum];
    entry.Data       = data;
    entry.CreateTime = std::chrono::system_clock::now();
}

const nemesis::CacheEntry* nemesis::CacheManager::GetEntry(const std::string& checksum)
{
    std::scoped_lock<std::mutex> lock(Manager.EntryMutex);
    auto itr = Manager.Entries.find(checksum);

    if (itr == Manager.Entries.end()) return nullptr;

    itr->second.CreateTime = std::chrono::system_clock::now();
    return &itr->second;
}

void nemesis::CacheManager::SaveFile(const std::filesystem::path& filepath)
{
    std::scoped_lock<std::mutex> lock(Manager.EntryMutex);
    std::ofstream ofs(filepath, std::ios::binary);

    if (!ofs) throw std::runtime_error("Cannot open file for writing");

    for (const auto& entry : Manager.Entries)
    {
        auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(entry.second.CreateTime.time_since_epoch())
                      .count();

        if (entry.second.CreateTime + std::chrono::hours(24) < std::chrono::system_clock::now()) continue;

        uint64_t length = static_cast<uint64_t>(entry.second.Data.size());
        ofs.write(reinterpret_cast<const char*>(&ns), sizeof(ns));
        ofs.write(entry.first.data(), 66);
        ofs.write(reinterpret_cast<const char*>(&length), sizeof(length));
        ofs.write(entry.second.Data.data(), length);
    }
}

void nemesis::CacheManager::LoadFile(const std::filesystem::path& filepath)
{
    std::scoped_lock<std::mutex> lock(Manager.EntryMutex);
    Manager.Entries.clear();

    if (!std::filesystem::exists(filepath)) return;

    std::ifstream ifs(filepath, std::ios::binary);

    if (!ifs) throw std::runtime_error("Cannot open file for reading");

    while (ifs.peek() != EOF)
    {
        uint64_t ns, length;

        ifs.read(reinterpret_cast<char*>(&ns), sizeof(ns));
        
        if (!ifs) break;

        std::string checksum(66, '\0');
        ifs.read(&checksum[0], 66);

        if (!ifs) break;

        ifs.read(reinterpret_cast<char*>(&length), sizeof(length));

        if (!ifs) break;

        std::string data(length, '\0');
        ifs.read(&data[0], length);

        auto create_time = std::chrono::system_clock::from_time_t(ns / 1000000000);

        nemesis::CacheEntry& entry = Manager.Entries[checksum];
        entry.CreateTime           = create_time;
        entry.Data                 = std::move(data);
    }
}
