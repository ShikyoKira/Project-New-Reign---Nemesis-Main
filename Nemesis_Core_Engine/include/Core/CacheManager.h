#pragma once

#include <filesystem>
#include <mutex>

#include "Utilities/Types.h"

#include "Core/CacheEntry.h"

namespace nemesis
{
	struct CacheManager
	{
    private:
        static nemesis::CacheManager Manager;

		UMap<std::string, nemesis::CacheEntry> Entries;
        std::mutex EntryMutex;

	public:
        static void AddEntry(const std::string& checksum, const std::string& data);

		static const nemesis::CacheEntry* GetEntry(const std::string& checksum);

		static void SaveFile(const std::filesystem::path& filepath);
        static void LoadFile(const std::filesystem::path& filepath);
	};
}
