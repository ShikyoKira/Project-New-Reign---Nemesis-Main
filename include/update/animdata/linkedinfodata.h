#pragma once

#include "update/animdata/datapack.h"
#include "update/animdata/infodatapackcond.h"

namespace nemesis
{
    struct LinkedInfoData : public nemesis::LinkedVar<nemesis::DataPack<InfoDataPack_Condt>>
    {
    };
}
