#pragma once

namespace nemesis
{
    template <size_t A, typename T>
	struct hkAlign
	{
        T Data;

		constexpr size_t GetAlignment() const
		{
            return A;
		}
	};
}
