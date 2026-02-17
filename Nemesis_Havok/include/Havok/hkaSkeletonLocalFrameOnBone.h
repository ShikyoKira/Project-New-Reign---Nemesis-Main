#pragma once

#include "Havok/Base/hkLocalFrame.h"

namespace nemesis
{
    struct hkaSkeletonLocalFrameOnBone : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x52e8043,
                                                "hkaSkeletonLocalFrameOnBone",
                                                nullptr,
                                                16,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkaSkeletonLocalFrameOnBone);

        nemesis::hkRefPtr<nemesis::hkLocalFrame> m_localFrame;
        int m_boneIndex{};

    public:
        hkaSkeletonLocalFrameOnBone() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkaSkeletonLocalFrameOnBone::Class;
}
