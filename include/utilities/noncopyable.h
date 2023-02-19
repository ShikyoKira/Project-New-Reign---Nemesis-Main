#pragma once

#include <boost/noncopyable.hpp>

namespace nemesis
{
	struct NonCopyableStruct
    {
    protected:
        NonCopyableStruct() = default;

    private:
        NonCopyableStruct(const NonCopyableStruct&) = delete;
        NonCopyableStruct& operator=(const NonCopyableStruct&) = delete;
    };

	class NonCopyableClass
    {
    protected:
        NonCopyableClass() = default;

    private:
        NonCopyableClass(const NonCopyableClass&) = delete;
        NonCopyableClass& operator=(const NonCopyableClass&) = delete;
    };
}
