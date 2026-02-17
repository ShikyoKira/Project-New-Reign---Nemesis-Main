#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkbCharacterSteppedInfo : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x2eda84f8,
                                                "hkbCharacterSteppedInfo",
                                                &nemesis::hkReferencedObject::Class,
                                                112,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbCharacterSteppedInfo);

        uint64_t m_characterId{};
        float m_deltaTime{};
        nemesis::hkQsTransform m_worldFromModel;
        nemesis::hkArray<nemesis::hkQsTransform> m_poseModelSpace;
        nemesis::hkArray<nemesis::hkQsTransform> m_rigidAttachmentTransforms;

    public:
        hkbCharacterSteppedInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbCharacterSteppedInfo::Class;
}
