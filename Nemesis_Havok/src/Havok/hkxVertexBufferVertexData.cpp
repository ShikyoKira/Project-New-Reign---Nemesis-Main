#include "Havok/hkxVertexBufferVertexData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxVertexBufferVertexData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxVertexBufferVertexData::Class; });

int nemesis::hkxVertexBufferVertexData::FloatToInt(float val)
{
    union
    {
        float* f_ptr;
        int* i_ptr;
    } f2i;
    f2i.f_ptr = &val;
    return *f2i.i_ptr;
}

float nemesis::hkxVertexBufferVertexData::IntToFloat(int val)
{
    union
    {
        float* f_ptr;
        int* i_ptr;
    } f2i;
    f2i.i_ptr = &val;
    return *f2i.f_ptr;
}

nemesis::hkxVertexBufferVertexData::hkxVertexBufferVertexData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkxVertexBufferVertexData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxVertexBufferVertexData::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_2013_3_0)
    {
        if (m_vectorData_2.IsEmpty())
        {
            if (m_vectorData_1.IsEmpty())
            {
                for (auto& vect : m_vectorData_0)
                {
                    m_vectorData_2.PushBack(new int(FloatToInt(vect->GetX())));
                    m_vectorData_2.PushBack(new int(FloatToInt(vect->GetY())));
                    m_vectorData_2.PushBack(new int(FloatToInt(vect->GetZ())));
                    m_vectorData_2.PushBack(new int(FloatToInt(vect->GetW())));
                }
            }
            else
            {
                for (auto& val : m_vectorData_1)
                {
                    m_vectorData_2.PushBack(new int(FloatToInt(*val)));
                }
            }
        }

        if (m_floatData_1.IsEmpty())
        {
            for (auto& val : m_floatData_0)
            {
                m_floatData_1.PushBack(new int(FloatToInt(*val)));
            }
        }

        serializer.WriteObject("vectorData", m_vectorData_2);
        serializer.WriteObject("floatData", m_floatData_1);
    }
    else if (serializer.GetContentsVersion() > HavokVersion::HK_2012_2_0)
    {
        if (m_vectorData_1.IsEmpty())
        {
            if (m_vectorData_2.IsEmpty())
            {
                for (auto& vect : m_vectorData_0)
                {
                    m_vectorData_1.PushBack(new float(vect->GetX()));
                    m_vectorData_1.PushBack(new float(vect->GetY()));
                    m_vectorData_1.PushBack(new float(vect->GetZ()));
                    m_vectorData_1.PushBack(new float(vect->GetW()));
                }
            }
            else
            {
                for (auto& val : m_vectorData_2)
                {
                    m_vectorData_1.PushBack(new float(IntToFloat(*val)));
                }
            }
        }

        if (m_floatData_0.IsEmpty())
        {
            for (auto& val : m_floatData_1)
            {
                m_floatData_0.PushBack(new float(IntToFloat(*val)));
            }
        }

        serializer.WriteObject("vectorData", m_vectorData_1);
        serializer.WriteObject("floatData", m_floatData_0);
    }
    else
    {
        if (m_vectorData_0.IsEmpty())
        {
            if (m_vectorData_2.IsEmpty())
            {
                for (size_t i = 0; i < m_vectorData_1.GetSize();)
                {
                    auto& vec4 = m_vectorData_0.PushBack(new nemesis::hkVector4());
                    vec4.SetX(m_vectorData_1[i++]);

                    if (i >= m_vectorData_1.GetSize()) break;

                    vec4.SetY(m_vectorData_1[i++]);

                    if (i >= m_vectorData_1.GetSize()) break;

                    vec4.SetZ(m_vectorData_1[i++]);

                    if (i >= m_vectorData_1.GetSize()) break;

                    vec4.SetW(m_vectorData_1[i++]);
                }
            }
            else
            {
                for (size_t i = 0; i < m_vectorData_2.GetSize();)
                {
                    auto& vec4 = m_vectorData_0.PushBack(new nemesis::hkVector4());
                    vec4.SetX(IntToFloat(m_vectorData_2[i++]));

                    if (i >= m_vectorData_2.GetSize()) break;

                    vec4.SetY(IntToFloat(m_vectorData_2[i++]));

                    if (i >= m_vectorData_2.GetSize()) break;

                    vec4.SetZ(IntToFloat(m_vectorData_2[i++]));

                    if (i >= m_vectorData_2.GetSize()) break;

                    vec4.SetW(IntToFloat(m_vectorData_2[i++]));
                }
            }
        }

        if (m_floatData_0.IsEmpty())
        {
            for (auto& val : m_floatData_1)
            {
                m_floatData_0.PushBack(new float(IntToFloat(*val)));
            }
        }

        serializer.WriteObject("vectorData", m_vectorData_0); // offset: 0/0 size: 12/16 align: 4/8
        serializer.WriteObject("floatData", m_floatData_0);   // offset: 12/16 size: 12/16 align: 4/8
    }

    serializer.WriteObject("uint32Data", m_uint32Data);    // offset: 24/32 size: 12/16 align: 4/8
    serializer.WriteObject("uint16Data", m_uint16Data);    // offset: 36/48 size: 12/16 align: 4/8
    serializer.WriteObject("uint8Data", m_uint8Data);      // offset: 48/64 size: 12/16 align: 4/8
    serializer.WriteValue("numVerts", m_numVerts);         // offset: 60/80 size: 4/4 align: 4/4
    serializer.WriteValue("vectorStride", m_vectorStride); // offset: 64/84 size: 4/4 align: 4/4
    serializer.WriteValue("floatStride", m_floatStride);   // offset: 68/88 size: 4/4 align: 4/4
    serializer.WriteValue("uint32Stride", m_uint32Stride); // offset: 72/92 size: 4/4 align: 4/4
    serializer.WriteValue("uint16Stride", m_uint16Stride); // offset: 76/96 size: 4/4 align: 4/4
    serializer.WriteValue("uint8Stride", m_uint8Stride);   // offset: 80/100 size: 4/4 align: 4/4
    // class size: 84/104 align: 4/8
}

void nemesis::hkxVertexBufferVertexData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_2013_3_0)
    {
        deserializer.ReadObject("vectorData", m_vectorData_2);
        deserializer.ReadObject("floatData", m_floatData_1);
    }
    else if (deserializer.GetContentsVersion() > HavokVersion::HK_2012_2_0)
    {
        deserializer.ReadObject("vectorData", m_vectorData_1);
        deserializer.ReadObject("floatData", m_floatData_0);
    }
    else
    {
        deserializer.ReadObject("vectorData", m_vectorData_0); // offset: 0/0 size: 12/16 align: 4/8
        deserializer.ReadObject("floatData", m_floatData_0);   // offset: 12/16 size: 12/16 align: 4/8
    }

    deserializer.ReadObject("uint32Data", m_uint32Data);    // offset: 24/32 size: 12/16 align: 4/8
    deserializer.ReadObject("uint16Data", m_uint16Data);    // offset: 36/48 size: 12/16 align: 4/8
    deserializer.ReadObject("uint8Data", m_uint8Data);      // offset: 48/64 size: 12/16 align: 4/8
    deserializer.ReadValue("numVerts", m_numVerts);         // offset: 60/80 size: 4/4 align: 4/4
    deserializer.ReadValue("vectorStride", m_vectorStride); // offset: 64/84 size: 4/4 align: 4/4
    deserializer.ReadValue("floatStride", m_floatStride);   // offset: 68/88 size: 4/4 align: 4/4
    deserializer.ReadValue("uint32Stride", m_uint32Stride); // offset: 72/92 size: 4/4 align: 4/4
    deserializer.ReadValue("uint16Stride", m_uint16Stride); // offset: 76/96 size: 4/4 align: 4/4
    deserializer.ReadValue("uint8Stride", m_uint8Stride);   // offset: 80/100 size: 4/4 align: 4/4
    // class size: 84/104 align: 4/8
}
