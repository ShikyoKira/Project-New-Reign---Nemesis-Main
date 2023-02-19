#pragma once

#include "utilities/line.h"

namespace nemesis
{
	struct HkxElementSplit
    {
    private:
        bool start = false;
        bool ready = false;

        void Split(nemesis::Line& line);

    public:
        void SplitValidation(nemesis::Line& line);
        bool IsStart() const noexcept;
    };
}