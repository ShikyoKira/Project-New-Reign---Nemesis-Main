#pragma once

#include "Nemesis_Core_Engine/include/Utilities/Types.h"

namespace nemesis
{
	struct AlterAnimPrefix
    {
    private:
        size_t Id;
        std::string Name;

    public:
        AlterAnimPrefix(size_t id, const std::string& name);

        size_t GetId() const noexcept;
        const std::string& GetName() const noexcept;
    };
}
