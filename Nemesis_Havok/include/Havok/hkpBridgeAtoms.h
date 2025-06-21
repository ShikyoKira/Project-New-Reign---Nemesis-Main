#pragma once

#include "Havok/hkpBridgeConstraintAtom.h"

namespace nemesis
{
    struct hkpBridgeAtoms : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xde152a4d,
                                                "hkpBridgeAtoms",
                                                nullptr,
                                                24,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpBridgeAtoms);

        nemesis::hkpBridgeConstraintAtom m_bridgeAtom;

    public:
        hkpBridgeAtoms() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpBridgeAtoms::Class;
}
