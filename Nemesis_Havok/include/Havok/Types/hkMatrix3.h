#pragma once

#include "Havok/Types/hkVector4.h"

namespace nemesis
{
    struct hkMatrix3 : nemesis::hkVariant
    {
    protected:
        nemesis::hkVector4 m_col0;
        nemesis::hkVector4 m_col1;
        nemesis::hkVector4 m_col2;

    public:
        nemesis::hkVector4& GetCol0();
        const nemesis::hkVector4& GetCol0() const;

        nemesis::hkVector4& GetCol1();
        const nemesis::hkVector4& GetCol1() const;

        nemesis::hkVector4& GetCol2();
        const nemesis::hkVector4& GetCol2() const;
    };
}
