#pragma once

#include <string>

namespace nemesis
{
	struct HashableObject
	{
    protected:
        mutable std::string HashCache;

	public:
        virtual std::string GetHash() const = 0;
	};
}
