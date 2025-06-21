#pragma once

#include "Havok/Base/hkpConstraintAtom.h"

#include "Havok/hkpSimpleContactConstraintDataInfo.h"

namespace nemesis
{
    struct hkpSimpleContactConstraintAtom : nemesis::hkpConstraintAtom
    {
        static constexpr nemesis::hkClass Class{0x920df11a,
                                                "hkpSimpleContactConstraintAtom",
                                                &nemesis::hkpConstraintAtom::Class,
                                                48,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpSimpleContactConstraintAtom);

        unsigned short m_sizeOfAllAtoms;
        unsigned short m_numContactPoints;
        unsigned short m_numReservedContactPoints;
        unsigned char m_numUserDatasForBodyA;
        unsigned char m_numUserDatasForBodyB;
        unsigned char m_contactPointPropertiesStriding;
        unsigned short m_maxNumContactPoints;
        nemesis::hkpSimpleContactConstraintDataInfo m_info; // align 16

    public:
        hkpSimpleContactConstraintAtom() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpSimpleContactConstraintAtom::Class;
}
