#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkxVertexBufferVertexData.h"
#include "Havok/hkxVertexDescription.h"
#include "Havok/hkxVertexDescriptionElementDecl.h"

namespace nemesis
{
    struct hkxVertexBuffer : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x4ab10615,
                                                "hkxVertexBuffer",
                                                &nemesis::hkReferencedObject::Class,
                                                136,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkxVertexBuffer);

        nemesis::hkRefPtr<nemesis::HavokObject> m_vertexDataClass;
        nemesis::hkRefPtr<nemesis::HavokObject> m_vertexData;
        int m_numVertexData{};
        nemesis::hkRefPtr<nemesis::hkxVertexDescription> m_vertexDesc;
        nemesis::hkxVertexBufferVertexData m_data;
        nemesis::hkxVertexDescriptionElementDecl m_desc;

    public:
        hkxVertexBuffer() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxVertexBuffer::Class;
}
