#pragma once

#include "update/animdata/datapack.h"
#include "update/animdata/animdatapackcond.h"

namespace nemesis
{
    struct AnimDataPack : public nemesis::DataPack<AnimDataPack_Condt, AnimDataPack>
    {
        void GetRawData(VecNstr& rawdata) const {}
    };

    struct LinkedAnimData : public nemesis::Linked<AnimDataPack, LinkedAnimData>
    {
    private:
        static void (nemesis::LinkedAnimData::*get_rawdata)(VecNstr&) const;

    public:
        void _GetRawHeaderData(VecNstr& rawdata) const;
        void _GetRawBaseData(VecNstr& rawdata) const;

        void GetRawData(VecNstr& rawdata) const override;
        void GetRawHeaderData(VecNstr& rawdata) const;
        void GetRawBaseData(VecNstr& rawdata) const;
    };
}
