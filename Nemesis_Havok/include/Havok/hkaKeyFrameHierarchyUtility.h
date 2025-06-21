#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkaKeyFrameHierarchyUtility : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x7bd5c66f,
                                                "hkaKeyFrameHierarchyUtility",
                                                nullptr,
                                                1,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkaKeyFrameHierarchyUtility);

        unsigned char unk0;

    public:
        hkaKeyFrameHierarchyUtility() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkaKeyFrameHierarchyUtility::Class;
}
