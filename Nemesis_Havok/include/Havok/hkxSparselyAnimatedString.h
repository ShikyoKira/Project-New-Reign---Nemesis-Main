#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkxSparselyAnimatedStringStringType.h"

namespace nemesis
{
    struct hkxSparselyAnimatedString : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x185da6fd,
                                                "hkxSparselyAnimatedString",
                                                &nemesis::hkReferencedObject::Class,
                                                48,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkxSparselyAnimatedString);

        mutable nemesis::hkArray<nemesis::hkxSparselyAnimatedStringStringType> m_strings_0;
        mutable nemesis::hkArray<nemesis::hkCString> m_strings_1;
        nemesis::hkArray<float> m_times;

    public:
        hkxSparselyAnimatedString() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxSparselyAnimatedString::Class;
}
