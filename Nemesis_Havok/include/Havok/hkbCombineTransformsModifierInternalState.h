#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkbCombineTransformsModifierInternalState : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xa92ed39f,
                                                "hkbCombineTransformsModifierInternalState",
                                                &nemesis::hkReferencedObject::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbCombineTransformsModifierInternalState);

        nemesis::hkVector4 m_translationOut;
        nemesis::hkQuaternion m_rotationOut;

    public:
        hkbCombineTransformsModifierInternalState() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbCombineTransformsModifierInternalState::Class;
}
