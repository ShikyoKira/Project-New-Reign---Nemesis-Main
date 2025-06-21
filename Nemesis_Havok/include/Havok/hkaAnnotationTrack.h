#pragma once

#include "Havok/hkaAnnotationTrackAnnotation.h"
#include "Havok/hkArray.h"

#include "Havok/Types/hkStringPtr.h"

namespace nemesis
{
    struct hkaAnnotationTrack : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xd4114fdd,
                                                "hkaAnnotationTrack",
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
        REGISTER_HAVOK_POINTER_HEADER(hkaAnnotationTrack);

        nemesis::hkStringPtr m_trackName;
        nemesis::hkArray<nemesis::hkaAnnotationTrackAnnotation> m_annotations;

    public:
        hkaAnnotationTrack() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkaAnnotationTrack::Class;
}
