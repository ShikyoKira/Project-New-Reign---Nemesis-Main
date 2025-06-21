#include "Utilities/ProgressMeter.h"

nemesis::ProgressMeter::ProgressMeter(
    unsigned int total_steps, std::function<void(unsigned int, unsigned int)> output) noexcept
    : TotalSteps(total_steps)
    , Output(output)
{
}

unsigned int nemesis::ProgressMeter::ProgressUp(unsigned int add_step)
{
    std::scoped_lock<std::mutex> lock(ProgressMutex);
    unsigned int current_step = ProgressStep += add_step;
    Output(current_step, TotalSteps);
    return current_step;
}

unsigned int nemesis::ProgressMeter::MaxSteps() const noexcept
{
    return TotalSteps;
}

unsigned int nemesis::ProgressMeter::Complete()
{
    std::scoped_lock<std::mutex> lock(ProgressMutex);
    unsigned int current_step = ProgressStep = TotalSteps;
    Output(current_step, TotalSteps);
    return current_step;
}
