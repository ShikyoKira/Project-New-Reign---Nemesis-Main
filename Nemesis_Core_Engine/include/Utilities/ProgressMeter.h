#pragma once

#include <mutex>
#include <functional>

#include "Utilities/Lockless.h"

namespace nemesis
{
    struct ProgressMeter
	{
    private:
        unsigned int ProgressStep = 0;
        unsigned int TotalSteps;

		std::function<void(unsigned int, unsigned int)> Output;

        std::mutex ProgressMutex;

	public:
        ProgressMeter(unsigned int total_steps,
                      std::function<void(unsigned int, unsigned int)> output) noexcept;

		unsigned int ProgressUp(unsigned int add_step);
        unsigned int MaxSteps() const noexcept;
        unsigned int Complete();
	};
}
