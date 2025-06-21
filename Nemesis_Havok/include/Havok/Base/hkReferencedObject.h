#pragma once

#include "Havok/Base/hkBaseObject.h"

namespace nemesis
{
    struct hkPackfile;

    struct hkReferencedObject : nemesis::hkBaseObject
    {
    protected:
        static constexpr nemesis::hkClass Class{0x3b1c1113,
                                                "hkReferencedObject",
                                                &nemesis::hkBaseObject::Class,
                                                16,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

        const nemesis::hkClass* RefClass;

        unsigned short m_memSizeAndFlags;
        short m_referenceCount;

    public:
        hkReferencedObject(const nemesis::hkClass& cls) noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;

        friend nemesis::Serializer;
        friend nemesis::Deserializer;
    };

    constexpr nemesis::hkClass nemesis::hkReferencedObject::Class;
}
