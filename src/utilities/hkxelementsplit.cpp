#include "utilities/hkxelementsplit.h"
#include "utilities/stringextension.h"

void nemesis::HkxElementSplit::Split(nemesis::Line& line)
{
    if (line.length() == 0) return;

    if (line.find("</hkparam>") != NOT_FOUND)
    {
        start = false;
        return;
    }

    VecStr curElements;
    StringSplit(line, curElements);
    std::string templine;

    for (auto& element : curElements)
    {
        templine.append("\t\t\t\t");
        templine.append(element);
        templine.push_back('\n');
    }

    templine.pop_back();
    line.clear();
    line.append(templine);
}

void nemesis::HkxElementSplit::SplitValidation(nemesis::Line& line)
{
    if (start)
    {
        Split(line);
    }
    else if (ready)
    {
        start = line.find("\t\t\t\t#") != NOT_FOUND;
        ready = false;
        Split(line);
    }
    else
    {
        ready = line.find("numelements=\"", 0) != NOT_FOUND;
    }
}

bool nemesis::HkxElementSplit::IsStart() const noexcept
{
    return start;
}
