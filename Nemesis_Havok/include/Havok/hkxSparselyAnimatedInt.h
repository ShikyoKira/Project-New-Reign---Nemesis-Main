#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkxSparselyAnimatedInt : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xca961951,
                                                "hkxSparselyAnimatedInt",
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
        REGISTER_HAVOK_POINTER_HEADER(hkxSparselyAnimatedInt);

        nemesis::hkArray<int> m_ints;
        nemesis::hkArray<float> m_times;

    public:
        hkxSparselyAnimatedInt() noexcept;
        hkxSparselyAnimatedInt(const nemesis::hkClass& cls) noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxSparselyAnimatedInt::Class;
}
