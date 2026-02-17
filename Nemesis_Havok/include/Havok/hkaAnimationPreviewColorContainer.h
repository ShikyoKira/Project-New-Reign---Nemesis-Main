#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkaAnimationPreviewColorContainer : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x4bc4c3e0,
                                                "hkaAnimationPreviewColorContainer",
                                                &nemesis::hkReferencedObject::Class,
                                                32,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkaAnimationPreviewColorContainer);

        nemesis::hkRefPtr<nemesis::HavokObject> m_previewColor_0;
        int m_numPreviewColor{};
        nemesis::hkArray<unsigned int> m_previewColor_1;

    public:
        hkaAnimationPreviewColorContainer() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkaAnimationPreviewColorContainer::Class;
}
