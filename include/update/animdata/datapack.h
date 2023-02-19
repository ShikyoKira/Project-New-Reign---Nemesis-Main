#pragma once

#include "core/linked.h"

namespace nemesis
{
    template <typename _Ty, typename _LnkTy>
    struct DataPack
    {
        std::string header;
        nemesis::Linked<_Ty, _LnkTy> base;
    };
}
