#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkpMoppCodeCodeInfo.h"

namespace nemesis
{
    struct hkpMoppCode : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x924c2661,
                                                "hkpMoppCode",
                                                &nemesis::hkReferencedObject::Class,
                                                64,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpMoppCode);

        using BuildType = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkpMoppCodeCodeInfo m_info;
        nemesis::hkArray<unsigned char> m_data;
        nemesis::hkEnum<BuildType, char> m_buildType = {
            {"BUILT_WITH_CHUNK_SUBDIVISION", 0},
            {"BUILT_WITHOUT_CHUNK_SUBDIVISION", 1},
            {"BUILD_NOT_SET", 2},
        };

    public:
        hkpMoppCode() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpMoppCode::Class;
}
