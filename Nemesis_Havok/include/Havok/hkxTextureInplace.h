#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkxTextureInplace : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xd45841d6,
                                                "hkxTextureInplace",
                                                &nemesis::hkReferencedObject::Class,
                                                56,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkxTextureInplace);

        std::string m_fileType;
        nemesis::hkArray<unsigned char> m_data;
        nemesis::hkStringPtr m_name;
        nemesis::hkStringPtr m_originalFilename;

    public:
        hkxTextureInplace() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxTextureInplace::Class;
}
