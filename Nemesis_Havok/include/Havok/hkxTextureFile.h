#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkxTextureFile : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x1e289259,
                                                "hkxTextureFile",
                                                &nemesis::hkReferencedObject::Class,
                                                40,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkxTextureFile);

        nemesis::hkStringPtr m_filename;
        nemesis::hkStringPtr m_name;
        nemesis::hkStringPtr m_originalFilename;

    public:
        hkxTextureFile() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxTextureFile::Class;
}
