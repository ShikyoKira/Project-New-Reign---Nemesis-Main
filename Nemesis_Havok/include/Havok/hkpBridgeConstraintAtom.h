#pragma once

#include "Havok/Base/hkpConstraintAtom.h"
#include "Havok/Base/hkpConstraintData.h"

namespace nemesis
{
    struct hkpBridgeConstraintAtom : nemesis::hkpConstraintAtom
    {
        static constexpr nemesis::hkClass Class{0x87a4f31b,
                                                "hkpBridgeConstraintAtom",
                                                &nemesis::hkpConstraintAtom::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpBridgeConstraintAtom);

        nemesis::hkRefPtr<nemesis::HavokObject> m_buildJacobianFunc;
        nemesis::hkRefPtr<nemesis::hkpConstraintData> m_constraintData;
        std::array<unsigned char, 4> m_padding{};

    public:
        hkpBridgeConstraintAtom() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpBridgeConstraintAtom::Class;
}
