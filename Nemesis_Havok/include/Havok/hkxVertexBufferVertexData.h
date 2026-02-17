#pragma once

#include "Havok/hkArray.h"
#include "Havok/hkClass.h"

#include "Havok/Types/hkVector4.h"

namespace nemesis
{
    struct hkxVertexBufferVertexData : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xd72b6fd0,
                                                "hkxVertexBufferVertexData",
                                                nullptr,
                                                104,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkxVertexBufferVertexData);

        mutable nemesis::hkArray<nemesis::hkVector4> m_vectorData_0;
        mutable nemesis::hkArray<float> m_vectorData_1;
        mutable nemesis::hkArray<int> m_vectorData_2;

        mutable nemesis::hkArray<float> m_floatData_0;
        mutable nemesis::hkArray<int> m_floatData_1;

        nemesis::hkArray<unsigned int> m_uint32Data;
        nemesis::hkArray<unsigned short> m_uint16Data;
        nemesis::hkArray<unsigned char> m_uint8Data;
        unsigned int m_numVerts{};
        unsigned int m_vectorStride{};
        unsigned int m_floatStride{};
        unsigned int m_uint32Stride{};
        unsigned int m_uint16Stride{};
        unsigned int m_uint8Stride{};

        static int FloatToInt(float val);
        static float IntToFloat(int val);

    public:
        hkxVertexBufferVertexData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxVertexBufferVertexData::Class;
}
