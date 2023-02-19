#pragma once

#include <string>

namespace nemesis
{
	struct HavokData
    {
    protected:
        std::string name;
        size_t id;

    public:
        HavokData(const std::string& name, size_t id);

        std::string GetName() const noexcept;
        size_t GetId() const noexcept;
    };
}
