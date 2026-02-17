#pragma once

#include "Havok/hkArray.h"
#include "Havok/hkxVertexDescriptionElementDecl.h"

namespace nemesis
{
    struct hkxVertexDescription : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x2df6313d,
                                                "hkxVertexDescription",
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
        REGISTER_HAVOK_POINTER_HEADER(hkxVertexDescription);

        unsigned int m_stride{};
        nemesis::hkArray<nemesis::hkxVertexDescriptionElementDecl> m_decls;

    public:
        hkxVertexDescription() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxVertexDescription::Class;
}
