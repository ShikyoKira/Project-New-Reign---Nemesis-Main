#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbAssetBundleStringData : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x46132bad,
                                                "hkbAssetBundleStringData",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbAssetBundleStringData);

        nemesis::hkStringPtr m_bundleName;
        nemesis::hkArray<nemesis::hkCString> m_assetNames;

    public:
        hkbAssetBundleStringData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbAssetBundleStringData::Class;
}
