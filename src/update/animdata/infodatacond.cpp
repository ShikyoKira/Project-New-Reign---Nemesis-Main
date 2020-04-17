#include "update/animdata/infodatapackcond.h"

using namespace std;

InfoDataPack_Condt::InfoDataPack_Condt(const VecStr& storeline, size_t linenum)
{
    short type = 0;

    for (unsigned int i = 0; i < storeline.size(); ++i)
    {
        const string& line = storeline[i];

        switch (type)
        {
            case 0:
                uniquecode = line;
                ++type;
                break;
            case 1:
                duration = line;
                ++type;
                break;
            case 2:
                // motion data counter
                ++type;
                break;
            case 3:
                if (i + 1 < storeline.size())
                {
                    const string& nextline = storeline[i + 1];

                    if (count(nextline.begin(), nextline.end(), ' ') == 0 && isOnlyNumber(nextline)) ++type;
                }

                motiondata.push_back(line);
                break;
            case 4:
                // rotation data counter
                ++type;
                break;
            case 5:
                if (i + 1 < storeline.size() && storeline[i + 1].length() == 0) ++type;

                motiondata.push_back(line);
                break;
            default:
                break;
        }
    }
}

void InfoDataPack_Condt::getlines(VecStr& storeline)
{
    // unique code
    storeline.push_back(uniquecode);

    // duration
    storeline.push_back(duration);

    // motion data count
    storeline.push_back(to_string(motiondata.size()));

    for (auto& data : motiondata)
    {
        storeline.push_back(data);
    }

    // rotation data count
    storeline.push_back(to_string(rotationdata.size()));

    for (auto& data : rotationdata)
    {
        storeline.push_back(data);
    }

    storeline.push_back("");
}
