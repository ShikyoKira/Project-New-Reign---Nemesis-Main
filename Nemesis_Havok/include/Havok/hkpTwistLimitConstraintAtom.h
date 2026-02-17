#pragma once

#include "Havok/Base/hkpConstraintAtom.h"

namespace nemesis
{
    struct hkpTwistLimitConstraintAtom : nemesis::hkpConstraintAtom
    {
        static constexpr nemesis::hkClass Class{0x7c9b1052,
                                                "hkpTwistLimitConstraintAtom",
                                                &nemesis::hkpConstraintAtom::Class,
                                                20,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpTwistLimitConstraintAtom);

        unsigned char m_isEnabled{};
        unsigned char m_twistAxis{};
        unsigned char m_refAxis{};
        float m_minAngle{};
        float m_maxAngle{};
        float m_angularLimitsTauFactor{};

    public:
        hkpTwistLimitConstraintAtom() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpTwistLimitConstraintAtom::Class;
}
