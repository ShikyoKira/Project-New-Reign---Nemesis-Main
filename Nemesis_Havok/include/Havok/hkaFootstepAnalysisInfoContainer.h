#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkaFootstepAnalysisInfo.h"

namespace nemesis
{
    struct hkaFootstepAnalysisInfoContainer : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x1d81207c,
                                                "hkaFootstepAnalysisInfoContainer",
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
        REGISTER_HAVOK_POINTER_HEADER(hkaFootstepAnalysisInfoContainer);

        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkaFootstepAnalysisInfo>> m_previewInfo;

    public:
        hkaFootstepAnalysisInfoContainer() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkaFootstepAnalysisInfoContainer::Class;
}
