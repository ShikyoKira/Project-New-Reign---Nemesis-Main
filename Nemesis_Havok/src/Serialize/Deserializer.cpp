#include <algorithm>

#include "Serialize/Deserializer.h"

#include "Havok/hkArray.h"
#include "Havok/hkClass.h"

std::string nemesis::Deserializer::StringToHex(const std::string& str)
{
    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setfill('0');

    for (unsigned char c : str)
    {
        ss << std::setw(2) << static_cast<unsigned int>(c);
    }

    return "0x" + ss.str();
}

std::string nemesis::Deserializer::GetErrorMessage(int err_code)
{
#ifdef _MSC_VER
    const size_t BUFFER_SIZE = 256;
    char buffer[BUFFER_SIZE];
    errno_t result = strerror_s(buffer, BUFFER_SIZE, err_code);

    if (result == 0) return std::string(buffer);

    return "Unknown error (strerror_s failed for code " + std::to_string(err_code) + ")";
#else
    return std::string(strerror(err_code));
#endif
}

void nemesis::Deserializer::Close() noexcept
{
    if (!Stream) return;

    Stream = nullptr;
}

std::streampos nemesis::Deserializer::GetCurrentPosition() const
{
    return 0;
}

void nemesis::Deserializer::GoToPosition(std::streampos address)
{
}

void nemesis::Deserializer::Pad(unsigned int size)
{
}

void nemesis::Deserializer::PadNode()
{
}

void nemesis::Deserializer::Skip(unsigned int step)
{
}

nemesis::hkCString& nemesis::Deserializer::ReadValue(const std::string& name,
                                                     nemesis::hkCString& cstring,
                                                     unsigned int padding_size)
{
    return ReadValue(name, cstring);
}

nemesis::hkStringPtr& nemesis::Deserializer::ReadValue(const std::string& name,
                                                       nemesis::hkStringPtr& string_ptr,
                                                       unsigned int padding_size)
{
    return ReadValue(name, string_ptr);
}

void nemesis::Deserializer::ReadObject(const std::string& name, void* obj_ptr)
{
    nemesis::HavokObject* hkx_ptr = static_cast<nemesis::HavokObject*>(obj_ptr);
    ReadRefObject(name, &hkx_ptr);
}

nemesis::hkClass& nemesis::Deserializer::ReadClass(nemesis::hkClass& cls)
{
    cls.DeserializeFrom(*this);
    return cls;
}

nemesis::hkSmallArray<nemesis::HavokObject>&
nemesis::Deserializer::ReadObject(const std::string& name, nemesis::hkSmallArray<nemesis::HavokObject>& array)
{
    return static_cast<nemesis::hkSmallArray<nemesis::HavokObject>&>(ReadArrayObject(name, array));
}

nemesis::hkArray<nemesis::HavokObject>&
nemesis::Deserializer::ReadObject(const std::string& name, nemesis::hkArray<nemesis::HavokObject>& array)
{
    return static_cast<nemesis::hkArray<nemesis::HavokObject>&>(
        ReadArrayObject(name, array));
}

nemesis::hkRefPtr<nemesis::HavokObject>&
nemesis::Deserializer::ReadObject(const std::string& name, nemesis::hkRefPtr<nemesis::HavokObject>& ref_obj)
{
    return static_cast<nemesis::hkRefPtr<nemesis::HavokObject>&>(ReadRefObject(name, ref_obj));
}

uint64_t nemesis::Deserializer::ReadUSize()
{
    return 0;
}

void nemesis::Deserializer::ReadFileHeader(nemesis::hkPackfileHeader& header)
{
}

std::string nemesis::Deserializer::AssertString(std::initializer_list<std::string> options)
{
    return std::string();
}

std::string nemesis::Deserializer::AssertString(unsigned int size, std::initializer_list<std::string> options)
{
    return std::string();
}

uint64_t nemesis::Deserializer::AssertUSize(std::initializer_list<uint64_t> options)
{
    return 0;
}

unsigned int nemesis::Deserializer::GetPointerSize() const noexcept
{
    return PointerSize;
}

unsigned int nemesis::Deserializer::GetEndian() const noexcept
{
    return Endian;
}

unsigned int nemesis::Deserializer::GetPaddingOption() const noexcept
{
    return PaddingOption;
}

const std::string& nemesis::Deserializer::GetContentsVersionString() const noexcept
{
    return ContentsVersionString;
}

nemesis::HavokVersion nemesis::Deserializer::GetContentsVersion() const noexcept
{
    return ContentsVersion;
}
