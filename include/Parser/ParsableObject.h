#pragma once

namespace nemesis
{
	struct ParsableObject
    {
        virtual void AppendLine(const nemesis::Line& line) = 0;
    };
}
