#include <algorithm>

#include "Serialize/Serializer.h"

std::string nemesis::Serializer::StringToHex(const std::string& str)
{
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setfill('0');

    for (unsigned char c : str)
    {
        oss << std::setw(2) << static_cast<unsigned int>(c);
    }

    return "0x" + oss.str();
}

unsigned char nemesis::Serializer::GetPointerSize() const noexcept
{
    return PointerSize;
}

unsigned char nemesis::Serializer::GetEndian() const noexcept
{
    return Endian;
}

unsigned char nemesis::Serializer::GetPaddingOption() const noexcept
{
    return PaddingOption;
}

const std::string& nemesis::Serializer::GetContentsVersionString() const noexcept
{
    return ContentsVersionString;
}

nemesis::HavokVersion nemesis::Serializer::GetContentsVersion() const noexcept
{
    return ContentsVersion;
}

int nemesis::Serializer::GetCurrentPosition() const
{
    return 0;
}

void nemesis::Serializer::Pad(unsigned int size)
{
}

void nemesis::Serializer::Pad(unsigned int size, unsigned char pad_ch)
{
}

void nemesis::Serializer::PadNode()
{
}

void nemesis::Serializer::TryPadToPointerSize()
{
}

void nemesis::Serializer::Skip(unsigned int step)
{
}

void nemesis::Serializer::WriteValue(const std::string& name,
                                     const nemesis::hkCString& cstring,
                                     unsigned int padding_size)
{
    WriteValue(name, cstring);
}

void nemesis::Serializer::WriteValue(const std::string& name,
                                     const nemesis::hkStringPtr& string_ptr,
                                     unsigned int padding_size)
{
    WriteValue(name, string_ptr);
}

void nemesis::Serializer::WriteObject(const std::string& name, const void* obj_ptr)
{
    const nemesis::HavokObject* hkx_ptr = static_cast<const nemesis::HavokObject*>(obj_ptr);
    WriteObject(name, &hkx_ptr);
}

void nemesis::Serializer::WriteNullPointer()
{
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name, bool val)
{
    WriteValue(name, val);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name, char val)
{
    WriteValue(name, val);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name, unsigned char val)
{
    WriteValue(name, val);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name, short val)
{
    WriteValue(name, val);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name, unsigned short val)
{
    WriteValue(name, val);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name, int val)
{
    WriteValue(name, val);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name, unsigned int val)
{
    WriteValue(name, val);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name, long val)
{
    WriteValue(name, val);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name, unsigned long val)
{
    WriteValue(name, val);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name, long long val)
{
    WriteValue(name, val);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name, unsigned long long val)
{
    WriteValue(name, val);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name, Float16 val)
{
    WriteValue(name, val);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name, float val)
{
    WriteValue(name, val);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name, double val)
{
    WriteValue(name, val);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name,
                                                     const nemesis::hkEventId& event_id)
{
    WriteValue(name, event_id);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name,
                                                     const nemesis::hkAttributeId& attr_id)
{
    WriteValue(name, attr_id);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name,
                                                     const nemesis::hkVariableId& var_id)
{
    WriteValue(name, var_id);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name,
                                                     const nemesis::hkCharacterPropertyId& prop_id)
{
    WriteValue(name, prop_id);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name,
                                                    const nemesis::hkCString& cstring)
{
    WriteValue(name, cstring);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name,
                                                    const nemesis::hkStringPtr& string_ptr)
{
    WriteValue(name, string_ptr);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name,
                                                     const nemesis::hkStringPtr& string_ptr,
                                                     unsigned int padding_size)
{
    WriteValue(name, string_ptr, padding_size);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name, const nemesis::hkEnumBase& val)
{
    WriteValue(name, val);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name, const nemesis::hkVector4& vec4)
{
    WriteValue(name, vec4);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name, const nemesis::hkVector8& vec8)
{
    WriteValue(name, vec8);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name,
                                                    const nemesis::hkQuaternion& quaternion)
{
    WriteValue(name, quaternion);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name,
                                                     const nemesis::hkQsTransform& qs_transform)
{
    WriteValue(name, qs_transform);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name,
                                                     const nemesis::hkTransform& transform)
{
    WriteValue(name, transform);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name,
                                                     const nemesis::hkUFloat8& ufloat8)
{
    WriteValue(name, ufloat8);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name, const nemesis::hkLong& _long)
{
    WriteValue(name, _long);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name, const nemesis::hkUlong& ulong)
{
    WriteValue(name, ulong);
}

void nemesis::Serializer::WriteSerializeIgnoredValue(const std::string& name,
                                                     const nemesis::hkHalf& half)
{
    WriteValue(name, half);
}

void nemesis::Serializer::WriteSerializeIgnoredObject(const std::string& name,
                                                      const nemesis::hkSmallArrayBase& array)
{
    WriteObject(name, array);
}

void nemesis::Serializer::WriteSerializeIgnoredObject(const std::string& name,
                                                     const nemesis::hkArrayBase& array)
{
    WriteObject(name, array);
}

void nemesis::Serializer::WriteSerializeIgnoredObject(const std::string& name,
                                                     const nemesis::hkRefVariant& ref_obj)
{
    WriteObject(name, ref_obj);
}

void nemesis::Serializer::WriteSerializeIgnoredObject(const std::string& name,
                                                     const nemesis::HavokObject& havok_obj)
{
    WriteObject(name, havok_obj);
}

void nemesis::Serializer::WriteSerializeIgnoredObject(const std::string& name,
                                                      const nemesis::HavokObject* const* ptr_obj_ptr)
{
    WriteObject(name, ptr_obj_ptr);
}

void nemesis::Serializer::WriteSerializeIgnoredObject(const std::string& name, void* obj_ptr)
{
    WriteObject(name, obj_ptr);
}

void nemesis::Serializer::WriteUSize(uint64_t val)
{
}

void nemesis::Serializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                         const void* (&list)[],
                                                         size_t size,
                                                         size_t type_size)
{
    WriteArrayValue(name, list, size, type_size);
}

void nemesis::Serializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                         const nemesis::hkCString* (&list)[],
                                                         size_t size)
{
    WriteArrayValue(name, list, size);
}

void nemesis::Serializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                         const nemesis::hkStringPtr* (&list)[],
                                                         size_t size)
{
    WriteArrayValue(name, list, size);
}

void nemesis::Serializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                         const nemesis::hkEnumBase* (&list)[],
                                                         size_t size)
{
    WriteArrayValue(name, list, size);
}

void nemesis::Serializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                         const nemesis::hkVector4* (&list)[],
                                                         size_t size)
{
    WriteArrayValue(name, list, size);
}

void nemesis::Serializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                         const nemesis::hkVector8* (&list)[],
                                                         size_t size)
{
    WriteArrayValue(name, list, size);
}

void nemesis::Serializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                         const nemesis::hkQuaternion* (&list)[],
                                                         size_t size)
{
    WriteArrayValue(name, list, size);
}

void nemesis::Serializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                         const nemesis::hkQsTransform* (&list)[],
                                                         size_t size)
{
    WriteArrayValue(name, list, size);
}

void nemesis::Serializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                         const nemesis::hkUFloat8* (&list)[],
                                                         size_t size)
{
    WriteArrayValue(name, list, size);
}

void nemesis::Serializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                         const nemesis::hkLong* (&list)[],
                                                         size_t size)
{
    WriteArrayValue(name, list, size);
}

void nemesis::Serializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                         const nemesis::hkUlong* (&list)[],
                                                         size_t size)
{
    WriteArrayValue(name, list, size);
}

void nemesis::Serializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                         const nemesis::hkHalf* (&list)[],
                                                         size_t size)
{
    WriteArrayValue(name, list, size);
}

void nemesis::Serializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                         const nemesis::hkTransform* (&list)[],
                                                         size_t size)
{
    WriteArrayValue(name, list, size);
}

void nemesis::Serializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                         const nemesis::hkMatrix3* (&list)[],
                                                         size_t size)
{
    WriteArrayValue(name, list, size);
}

void nemesis::Serializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                         const nemesis::hkMatrix4* (&list)[],
                                                         size_t size)
{
    WriteArrayValue(name, list, size);
}

void nemesis::Serializer::WriteSerializeIgnoreArrayObject(const std::string& name,
                                                          const nemesis::HavokObject* (&list)[],
                                                          size_t size)
{
    WriteArrayObject(name, list, size);
}

void nemesis::Serializer::WritePointerSize(unsigned char pointer_size)
{
    PointerSize = pointer_size;
    WriteValue("", pointer_size);
}

void nemesis::Serializer::WriteEndian(unsigned char endian)
{
    Endian = endian;
    WriteValue("", endian);
}

void nemesis::Serializer::WritePaddingOption(unsigned char padding_option)
{
    PaddingOption = padding_option;
    WriteValue("", padding_option);
}

void nemesis::Serializer::WriteContentsVersionString(const std::string& version_string)
{
    ContentsVersionString = version_string;
    WriteValue("", version_string, 16, 0xFF);
}
