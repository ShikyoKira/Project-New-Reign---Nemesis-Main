#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkxMaterialShader.h"

namespace nemesis
{
    struct hkxMaterialShaderSet : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x154650f3,
                                                "hkxMaterialShaderSet",
                                                &nemesis::hkReferencedObject::Class,
                                                32,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkxMaterialShaderSet);

        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkxMaterialShader>> m_shaders;

    public:
        hkxMaterialShaderSet() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxMaterialShaderSet::Class;
}
