#include "update/animdata/animdatapackcond.h"

using namespace std;

AnimDataPack_Condt::AnimDataPack_Condt(const VecStr& storeline, size_t linenum)
{
    short type = 0;

    for (unsigned int i = 0; i < storeline.size(); ++i)
    {
        const string& line = storeline[i];

        switch (type)
        {
            case 0:
                name = line;
                ++type;
                break;
            case 1:
                uniquecode           = line;
                uniquecode.linecount = linenum + i;
                ++type;
                break;
            case 2:
                unknown1           = line;
                unknown1.linecount = linenum + i;
                ++type;
                break;
            case 3:
                unknown2           = line;
                unknown2.linecount = linenum + i;
                ++type;
                break;
            case 4:
                unknown3           = line;
                unknown3.linecount = linenum + i;
                ++type;
                break;
            case 5:
                // event count
                if (storeline[i + 1].length() == 0) ++type;

                ++type;
                break;
            case 6:
                if (i + 1 < storeline.size() && storeline[i + 1].length() == 0) ++type;

                eventname.push_back(nemesis::LinkedVar(line, linenum + i));
            default:
                break;
        }
    }
}

void AnimDataPack_Condt::getlines(VecStr& storeline) 
{
    // anim data name
    storeline.push_back(name);

    // unique code
    getlinkedline(uniquecode, storeline);

    // unknown variables
    getlinkedline(unknown1, storeline);
    getlinkedline(unknown2, storeline);
    getlinkedline(unknown3, storeline);

    // event name count
    storeline.push_back(to_string(eventname.size()));

    // event name list
    for (auto& each : eventname)
    {
        getlinkedline(each, storeline);
    }

    storeline.push_back("");
}
