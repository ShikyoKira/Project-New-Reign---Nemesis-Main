#include <cstring>
#include <iostream>

#include "Serialize/PackfileDeserializer.h"

#include "Havok/hkArray.h"
#include "Havok/hkClass.h"
#include "Havok/hkPackfileSection.h"
#include "Havok/hkPackfileSectionHeader.h"
#include "Havok/hkRootLevelContainer.h"

#include "Utilities/Algorithm.h"

bool nemesis::PackfileDeserializer::IsUSizeLong() const noexcept
{
    return PointerSize == 8;
}

void nemesis::PackfileDeserializer::TryPadToPointerSize()
{
    //if (PaddingOption != 1) return;

    //Pad(PointerSize);
}

void nemesis::PackfileDeserializer::ConsumePointer()
{
    AssertUSize({0});
}

const nemesis::hkClass* nemesis::PackfileDeserializer::GetClassFromAddress(size_t address)
{
    auto itr = VirtualMap.find(address);

    if (itr == VirtualMap.end())
    {
        throw std::runtime_error("Target class (" + NumberToHex(address, true) + ") cannot be found ");
    }

    auto cls_itr = ClassMap.find(itr->second->Destination);

    if (cls_itr == ClassMap.end())
    {
        throw std::runtime_error("Target class (" + NumberToHex(address, true) + ") cannot be found ");
    }

    return cls_itr->second;
}

size_t nemesis::PackfileDeserializer::GetTargetAddress(size_t address)
{
    auto itr = GlobalMap.find(address);

    if (itr != GlobalMap.end()) return itr->second->Destination;

    throw std::runtime_error("Target address (" + NumberToHex(address, true) + ") cannot be found ");
}

void nemesis::PackfileDeserializer::Deserialize(nemesis::hkPackfileSectionHeader& header)
{
    header.DeserializeFrom(*this);

    std::streampos cur_pos = GetCurrentPosition();
    GoToPosition(header.GetAbsoluteDataStart() + header.GetLocalFixupsOffset());
    size_t size = (header.GetGlobalFixupsOffset() - header.GetLocalFixupsOffset()) / 8;
    LocalMap.clear();
    LocalFixups.clear();

    for (size_t i = 0; i < size; ++i)
    {
        unsigned int source = Read<unsigned int>();

        if (source == 0xffffffff) break;

        unsigned int dest = Read<unsigned int>();
        LocalMap.insert({source, &LocalFixups.emplace_back(source, dest)});
    }

    GoToPosition(header.GetAbsoluteDataStart() + header.GetGlobalFixupsOffset());
    size = (header.GetVirtualFixupsOffset() - header.GetGlobalFixupsOffset()) / 12;
    GlobalMap.clear();
    GlobalFixups.clear();

    for (size_t i = 0; i < size; ++i)
    {
        unsigned int source = Read<unsigned int>();

        if (source == 0xffffffff) break;

        unsigned int index = Read<unsigned int>();
        unsigned int dest  = Read<unsigned int>();
        GlobalMap.insert({source, &GlobalFixups.emplace_back(source, index, dest)});
    }

    GoToPosition(header.GetAbsoluteDataStart() + header.GetVirtualFixupsOffset());
    size = (header.GetExportFixupsOffset() - header.GetVirtualFixupsOffset()) / 12;
    VirtualMap.clear();
    VirtualFixups.clear();

    for (size_t i = 0; i < size; ++i)
    {
        unsigned int source = Read<unsigned int>();

        if (source == 0xffffffff) break;

        unsigned int index = Read<unsigned int>();
        unsigned int dest  = Read<unsigned int>();
        VirtualMap.insert({source, &VirtualFixups.emplace_back(source, index, dest)});
    }

    GoToPosition(cur_pos);
    DataAddress = header.GetAbsoluteDataStart();
}

static DeqStr local_log;
static std::string space;

void nemesis::PackfileDeserializer::RunLocalQueue()
{
#ifdef _DEBUG
    space.append("  ");
#endif

    for (auto& queue : *LocalQueue)
    {
#ifdef _DEBUG
        size_t addr      = Stream->tellg();
        std::string& log = local_log.emplace_back(space + "Local Queue : " + NumberToHex(addr, true) + " ("
                                                  + std::to_string(addr) + ")");
#endif
        queue();
#ifdef _DEBUG
        size_t end_addr = Stream->tellg();
        log += " | Size: " + std::to_string(end_addr - addr);
#endif
    }

#ifdef _DEBUG
    space.pop_back();
    space.pop_back();
#endif

    LocalQueue->clear();
    Pad(16);
}

void nemesis::PackfileDeserializer::RunGlobalQueue()
{
    while (!GlobalQueue.empty())
    {
        size_t addr = Stream->tellg();

#ifdef _DEBUG
        std::cout << space << "Global Queue : " << NumberToHex(addr, true) << " (" << addr << ") ";
        space.append("  ");
#endif

        auto itr = GlobalQueue.find(addr);

        if (itr == GlobalQueue.end())
        {
            std::cout << "Global queue address not found: " << NumberToHex(addr, true) << " (" << addr << ")"
                      << std::endl;

            auto cls     = GetClassFromAddress(addr - DataAddress);
            auto obj_itr = ObjectMap.find(addr);

            if (obj_itr != ObjectMap.end()) continue;

            RunScopedQueue(LocalQueue,
                           [addr, cls, this]()
                           {
                               auto obj = PackfilePtr->CreateObject(cls->GetName());
                               ObjectMap.insert({addr, obj});
                               obj->DeserializeFrom(*this);
                               Pad(16);

                               RunLocalQueue();
                           });
            continue;
        }

        for (auto& each : itr->second)
        {
            each();
        }

#ifdef _DEBUG
        for (auto& each : local_log)
        {
            std::cout << each << std::endl;
        }

        local_log.clear();
        space.pop_back();
        space.pop_back();

        size_t end_addr = Stream->tellg();
        std::cout << space << "Last Global Queue Size: " << end_addr - addr << std::endl;
#endif

        Pad(16);
        GlobalQueue.erase(itr);
    }
}

bool nemesis::PackfileDeserializer::CanAssert() const noexcept
{
    return true;
}

void nemesis::PackfileDeserializer::LoadFile(const std::filesystem::path& filepath)
{
    Stream = std::make_unique<std::ifstream>(filepath, std::ios::binary);

    if (static_cast<std::ifstream*>(Stream.get())->is_open()) return;

    std::error_code ec(errno, std::system_category());
    throw std::runtime_error("Failed to open file: \"" + nemesis::to_utf8_string(filepath)
                             + "\"\nMessage: " + ec.message());
}

void nemesis::PackfileDeserializer::LoadData(const Vec<unsigned char>& data)
{
    LoadData(std::string(reinterpret_cast<const char*>(data.data()), data.size()));
}

void nemesis::PackfileDeserializer::LoadData(const std::string& data)
{
    Stream = std::make_unique<std::istringstream>(data);
}

nemesis::hkPackfile nemesis::PackfileDeserializer::Deserialize()
{
    nemesis::hkPackfile packfile;
    PackfilePtr = &packfile;
    packfile.DeserializeFrom(*this);
    return packfile;
}

void nemesis::PackfileDeserializer::Deserialize(nemesis::hkPackfileSection& section_data)
{
    auto& header = section_data.GetHeader();
    Deserialize(header);

    std::streampos cur_pos = GetCurrentPosition();
    GoToPosition(header.GetAbsoluteDataStart());
    section_data.DeserializeFrom(*this);
    GoToPosition(cur_pos);
}

void nemesis::PackfileDeserializer::Deserialize(nemesis::hkRootLevelContainer& root)
{
    root.DeserializeFrom(*this);
    Pad(16);
    RunLocalQueue();
    RunGlobalQueue();
}

const nemesis::hkClass* nemesis::PackfileDeserializer::DeserializeClass()
{
    unsigned int signature;
    char ch;
    std::string name;

    ReadValue("", signature);
    ReadValue("", ch);

    if (ch != 0x09) return nullptr;

    size_t name_pos = Stream->tellg();
    name_pos -= DataAddress;
    ReadValue("", name);

    const nemesis::hkClass* cls = PackfilePtr->GetClassFromName(name);

    if (cls && ClassMap.find(name_pos) == ClassMap.end())
    {
        ClassMap.insert({name_pos, cls});
    }

    return cls;
}

std::streampos nemesis::PackfileDeserializer::GetCurrentPosition() const
{
    if (!Stream) return 0;

    return Stream->tellg();
}

void nemesis::PackfileDeserializer::GoToPosition(std::streampos address)
{
    Stream->seekg(address, std::ios_base::beg);
}

void nemesis::PackfileDeserializer::Pad(unsigned int size)
{
    unsigned int pos = Stream->tellg();

    if (pos % size == 0) return;

    Stream->seekg(pos + size - pos % size, std::ios_base::beg);
}

void nemesis::PackfileDeserializer::PadNode()
{
    if (Endian == 1 && PaddingOption == 1) return;

    Pad(PointerSize);
}

void nemesis::PackfileDeserializer::Skip(unsigned int step)
{
    Stream->seekg(step, std::ios_base::cur);
}

bool& nemesis::PackfileDeserializer::ReadValue(const std::string& name, bool& val)
{
    auto pos = Stream->tellg();
    auto b   = Read<unsigned char>();

    switch (b)
    {
        case 0:
        {
            return val = false;
        }
        case 1:
        {
            return val = true;
        }
        default:
        {
            std::stringstream ss;
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(b);
            std::string msg = "Failed to parse boolean in ReadValue at position (" + std::to_string(pos)
                              + ") with " + ss.str();
            throw std::runtime_error(msg);
        }
    }
}

char& nemesis::PackfileDeserializer::ReadValue(const std::string& name, char& ch)
{
    ReadValueFromBinary(ch);
    return ch;
}

unsigned char& nemesis::PackfileDeserializer::ReadValue(const std::string& name, unsigned char& ch)
{
    ReadValueFromBinary(ch);
    return ch;
}

short& nemesis::PackfileDeserializer::ReadValue(const std::string& name, short& byte)
{
    ReadValueFromBinary(byte);
    return byte;
}

unsigned short& nemesis::PackfileDeserializer::ReadValue(const std::string& name, unsigned short& byte)
{
    ReadValueFromBinary(byte);
    return byte;
}

int& nemesis::PackfileDeserializer::ReadValue(const std::string& name, int& val)
{
    ReadValueFromBinary(val);
    return val;
}

unsigned int& nemesis::PackfileDeserializer::ReadValue(const std::string& name, unsigned int& val)
{
    ReadValueFromBinary(val);
    return val;
}

long& nemesis::PackfileDeserializer::ReadValue(const std::string& name, long& val)
{
    ReadValueFromBinary(val);
    return val;
}

unsigned long& nemesis::PackfileDeserializer::ReadValue(const std::string& name, unsigned long& val)
{
    ReadValueFromBinary(val);
    return val;
}

long long& nemesis::PackfileDeserializer::ReadValue(const std::string& name, long long& val)
{
    ReadValueFromBinary(val);
    return val;
}

unsigned long long& nemesis::PackfileDeserializer::ReadValue(const std::string& name, unsigned long long& val)
{
    ReadValueFromBinary(val);
    return val;
}

Float16& nemesis::PackfileDeserializer::ReadValue(const std::string& name, Float16& val)
{
    ReadValueFromBinary(val);
    return val;
}

float& nemesis::PackfileDeserializer::ReadValue(const std::string& name, float& val)
{
    ReadValueFromBinary(val);
    return val;
}

double& nemesis::PackfileDeserializer::ReadValue(const std::string& name, double& val)
{
    ReadValueFromBinary(val);
    return val;
}

std::string& nemesis::PackfileDeserializer::ReadValue(const std::string& name, std::string& ref_str)
{
    return ref_str = std::string(ReadString(name).data());
}

std::string& nemesis::PackfileDeserializer::ReadValue(const std::string& name,
                                                      std::string& ref_str,
                                                      unsigned int size,
                                                      char pad)
{
    return ref_str = std::string(ReadString(name, size, pad).data());
}

nemesis::hkEventId& nemesis::PackfileDeserializer::ReadValue(const std::string& name,
                                                             nemesis::hkEventId& event_id)
{
    int val;
    ReadValue(name, val);
    event_id = val;
    return event_id;
}

nemesis::hkAttributeId& nemesis::PackfileDeserializer::ReadValue(const std::string& name,
                                                                 nemesis::hkAttributeId& attr_id)
{
    int val;
    ReadValue(name, val);
    attr_id = val;
    return attr_id;
}

nemesis::hkVariableId& nemesis::PackfileDeserializer::ReadValue(const std::string& name,
                                                                nemesis::hkVariableId& var_id)
{
    int val;
    ReadValue(name, val);
    var_id = val;
    return var_id;
}

nemesis::hkCharacterPropertyId&
nemesis::PackfileDeserializer::ReadValue(const std::string& name, nemesis::hkCharacterPropertyId& prop_id)
{
    int val;
    ReadValue(name, val);
    prop_id = val;
    return prop_id;
}

nemesis::hkCString& nemesis::PackfileDeserializer::ReadValue(const std::string& name,
                                                             nemesis::hkCString& cstring)
{
    return ReadValue(name, cstring, 16);
}

nemesis::hkCString& nemesis::PackfileDeserializer::ReadValue(const std::string& name,
                                                             nemesis::hkCString& cstring,
                                                             unsigned int padding_size)
{
    TryPadToPointerSize();

    size_t pos = Stream->tellg();
    auto itr   = LocalMap.find(pos - DataAddress);

    ConsumePointer();

    if (itr == LocalMap.end())
    {
        cstring.Clear();
        return cstring;
    }

    LocalQueue->emplace_back(
        [&cstring, name, padding_size, this]()
        {
            std::string rtn_val;
            char ch = Read<char>(name);

            while (ch != 0)
            {
                rtn_val.push_back(ch);
                ch = Read<char>(name);
            }

            cstring.SetValue(rtn_val);
            Pad(padding_size);
        });
    return cstring;
}

nemesis::hkStringPtr& nemesis::PackfileDeserializer::ReadValue(const std::string& name,
                                                               nemesis::hkStringPtr& string_ptr)
{
    return ReadValue(name, string_ptr, 16);
}

nemesis::hkStringPtr& nemesis::PackfileDeserializer::ReadValue(const std::string& name,
                                                               nemesis::hkStringPtr& string_ptr,
                                                               unsigned int padding_size)
{
    TryPadToPointerSize();

    size_t pos = Stream->tellg();
    auto itr   = LocalMap.find(pos - DataAddress);

    ConsumePointer();

    if (itr == LocalMap.end())
    {
        string_ptr.Clear();
        return string_ptr;
    }

    LocalQueue->emplace_back(
        [&string_ptr, name, padding_size, this]()
        {
            std::string rtn_val;
            char ch = Read<char>(name);

            while (ch != 0)
            {
                rtn_val.push_back(ch);
                ch = Read<char>(name);
            }

            string_ptr.SetValue(rtn_val);
            Pad(padding_size);
        });
    return string_ptr;
}

nemesis::hkEnumBase& nemesis::PackfileDeserializer::ReadValue(const std::string& name,
                                                              nemesis::hkEnumBase& val)
{
    Stream->read(val.bin(), val.size());
    return val;
}

nemesis::hkVector4& nemesis::PackfileDeserializer::ReadValue(const std::string& name, nemesis::hkVector4& vec4)
{
    float x = 0.0, y = 0.0, z = 0.0, w = 0.0;
    ReadValue("", x);
    ReadValue("", y);
    ReadValue("", z);
    ReadValue("", w);

    vec4.SetX(x);
    vec4.SetY(y);
    vec4.SetZ(z);
    vec4.SetW(w);
    return vec4;
}

nemesis::hkVector8& nemesis::PackfileDeserializer::ReadValue(const std::string& name, nemesis::hkVector8& vec8)
{
    ReadValue(name, vec8.GetLin());
    ReadValue(name, vec8.GetAng());
    return vec8;
}

nemesis::hkQuaternion& nemesis::PackfileDeserializer::ReadValue(const std::string& name,
                                                                nemesis::hkQuaternion& quaternion)
{
    float x = 0.0, y = 0.0, z = 0.0, r = 0.0;
    ReadValue("", x);
    ReadValue("", y);
    ReadValue("", z);
    ReadValue("", r);

    quaternion.SetX(x);
    quaternion.SetY(y);
    quaternion.SetZ(z);
    quaternion.SetR(r);
    return quaternion;
}

nemesis::hkQsTransform& nemesis::PackfileDeserializer::ReadValue(const std::string& name,
                                                                 nemesis::hkQsTransform& qs_transform)
{
    ReadValue("", qs_transform.GetTranslation());
    ReadValue("", qs_transform.GetRotation());
    ReadValue("", qs_transform.GetScale());
    return qs_transform;
}

nemesis::hkUFloat8& nemesis::PackfileDeserializer::ReadValue(const std::string& name,
                                                             nemesis::hkUFloat8& ufloat8)
{
    unsigned char byte;
    ReadValue("", byte);
    ufloat8 = byte;
    return ufloat8;
}

nemesis::hkLong& nemesis::PackfileDeserializer::ReadValue(const std::string& name, nemesis::hkLong& _long)
{
    if (PointerSize == 8)
    {
        uint64_t val;
        ReadValue("", val);
        _long = val;
    }
    else
    {
        uint32_t val;
        ReadValue("", val);
        _long = val;
    }

    return _long;
}

nemesis::hkUlong& nemesis::PackfileDeserializer::ReadValue(const std::string& name, nemesis::hkUlong& ulong)
{
    if (PointerSize == 8)
    {
        int64_t val;
        ReadValue("", val);
        ulong = val;
    }
    else
    {
        int32_t val;
        ReadValue("", val);
        ulong = val;
    }

    return ulong;
}

nemesis::hkHalf& nemesis::PackfileDeserializer::ReadValue(const std::string& name, nemesis::hkHalf& half)
{
    short i16;
    ReadValue("", i16);
    half = i16;
    return half;
}

nemesis::hkTransform& nemesis::PackfileDeserializer::ReadValue(const std::string& name,
                                                               nemesis::hkTransform& transform)
{
    ReadValue("", transform.GetRotation());
    ReadValue("", transform.GetTranslation());
    return transform;
}

nemesis::hkMatrix3& nemesis::PackfileDeserializer::ReadValue(const std::string& name,
                                                             nemesis::hkMatrix3& matrix3)
{
    ReadValue("", matrix3.GetCol0());
    ReadValue("", matrix3.GetCol1());
    ReadValue("", matrix3.GetCol2());
    return matrix3;
}

nemesis::hkMatrix4& nemesis::PackfileDeserializer::ReadValue(const std::string& name,
                                                             nemesis::hkMatrix4& matrix4)
{
    ReadValue("", matrix4.GetCol0());
    ReadValue("", matrix4.GetCol1());
    ReadValue("", matrix4.GetCol2());
    ReadValue("", matrix4.GetCol3());
    return matrix4;
}

void** nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name,
                                                     void* list[],
                                                     size_t size,
                                                     size_t type_size)
{
    for (size_t i = 0; i < size; ++i)
    {
        Stream->read(reinterpret_cast<char*>(list[i]), type_size);
    }

    return list;
}

bool** nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name, bool* (&list)[], size_t size)
{
    return ReadArrayValueImplt(name, list, size);
}

char** nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name, char* (&list)[], size_t size)
{
    return ReadArrayValueImplt(name, list, size);
}

unsigned char**
nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name, unsigned char* (&list)[], size_t size)
{
    return ReadArrayValueImplt(name, list, size);
}

short**
nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name, short* (&list)[], size_t size)
{
    return ReadArrayValueImplt(name, list, size);
}

unsigned short**
nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name, unsigned short* (&list)[], size_t size)
{
    return ReadArrayValueImplt(name, list, size);
}

int**
nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name, int* (&list)[], size_t size)
{
    return ReadArrayValueImplt(name, list, size);
}

unsigned int**
nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name, unsigned int* (&list)[], size_t size)
{
    return ReadArrayValueImplt(name, list, size);
}

long** nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name, long* (&list)[], size_t size)
{
    return ReadArrayValueImplt(name, list, size);
}

unsigned long**
nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name, unsigned long* (&list)[], size_t size)
{
    return ReadArrayValueImplt(name, list, size);
}

long long**
nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name, long long* (&list)[], size_t size)
{
    return ReadArrayValueImplt(name, list, size);
}

unsigned long long** nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name,
                                                                   unsigned long long* (&list)[],
                                                                   size_t size)
{
    return ReadArrayValueImplt(name, list, size);
}

Float16** nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name, Float16* (&list)[],
                                                                   size_t size)
{
    return ReadArrayValueImplt(name, list, size);
}

float** nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name, float* (&list)[],
                                                                   size_t size)
{
    return ReadArrayValueImplt(name, list, size);
}

double**
nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name, double* (&list)[],
                                                                   size_t size)
{
    return ReadArrayValueImplt(name, list, size);
}

nemesis::hkCString** nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name,
                                                                   nemesis::hkCString* (&list)[],
                                                                   size_t size)
{
    return ReadArrayValueImplt(name, list, size);
}

nemesis::hkStringPtr** nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name,
                                                                     nemesis::hkStringPtr* (&list)[],
                                                                     size_t size)
{
    return ReadArrayValueImplt(name, list, size);
}

nemesis::hkEnumBase** nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name,
                                                                    nemesis::hkEnumBase* (&list)[],
                                                                    size_t size)
{
    return ReadArrayValueImplt(name, list, size);
}

nemesis::hkVector4** nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name,
                                                                   nemesis::hkVector4* (&list)[],
                                                                   size_t size)
{
    return ReadArrayValueImplt(name, list, size);
}

nemesis::hkVector8** nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name,
                                                                   nemesis::hkVector8* (&list)[],
                                                                   size_t size)
{
    return ReadArrayValueImplt(name, list, size);
}

nemesis::hkQuaternion** nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name,
                                                                      nemesis::hkQuaternion* (&list)[],
                                                                      size_t size)
{
    return ReadArrayValueImplt(name, list, size);
}

nemesis::hkQsTransform** nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name,
                                                                       nemesis::hkQsTransform* (&list)[],
                                                                       size_t size)
{
    return ReadArrayValueImplt(name, list, size);
}

nemesis::hkUFloat8** nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name,
                                                                   nemesis::hkUFloat8* (&list)[],
                                                                   size_t size)
{
    return ReadArrayValueImplt(name, list, size);
}

nemesis::hkLong** nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name,
                                                                nemesis::hkLong* (&list)[],
                                                                size_t size)
{
    return ReadArrayValueImplt(name, list, size);
}

nemesis::hkUlong** nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name,
                                                                 nemesis::hkUlong* (&list)[],
                                                                 size_t size)
{
    return ReadArrayValueImplt(name, list, size);
}

nemesis::hkHalf** nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name,
                                                                nemesis::hkHalf* (&list)[],
                                                                size_t size)
{
    return ReadArrayValueImplt(name, list, size);
}

nemesis::hkTransform** nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name,
                                                                     nemesis::hkTransform* (&list)[],
                                                                     size_t size)
{
    return ReadArrayValueImplt(name, list, size);
}

nemesis::hkMatrix3** nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name,
                                                                   nemesis::hkMatrix3* (&list)[],
                                                                   size_t size)
{
    return ReadArrayValueImplt(name, list, size);
}

nemesis::hkMatrix4** nemesis::PackfileDeserializer::ReadArrayValue(const std::string& name,
                                                                   nemesis::hkMatrix4* (&list)[],
                                                                   size_t size)
{
    return ReadArrayValueImplt(name, list, size);
}

nemesis::HavokObject** nemesis::PackfileDeserializer::ReadArrayObject(const std::string& name,
                                                                      nemesis::HavokObject* (&list)[],
                                                                      size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        ReadObject(name, *list[i]);
    }

    return list;
}

nemesis::hkRefVariant** nemesis::PackfileDeserializer::ReadArrayRefObject(const std::string& name,
                                                                          nemesis::hkRefVariant* (&list)[],
                                                                          size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        ReadRefObject(name, *list[i]);
    }

    return list;
}

nemesis::hkSmallArrayBase& nemesis::PackfileDeserializer::ReadArrayObject(const std::string& name,
                                                                          nemesis::hkSmallArrayBase& array)
{
    TryPadToPointerSize();

    size_t pos = Stream->tellg();
    auto itr   = LocalMap.find(pos - DataAddress);

    ConsumePointer();
    array.Clear();

    unsigned short size = ReadSmallArraySize(name);
    array.SetSize(size);

    if (itr == LocalMap.end() || size <= 0) return array;

    LocalQueue->emplace_back(
        [&array, name, this]
        {
            RunScopedQueue(LocalQueue,
                           [&array, this]()
                           {
                               Pad(16);
                               array.DeserializeFrom(*this);

                               RunLocalQueue();
                           });
        });

    if (!array.IsVariant())
    {
        LocalQueue->emplace_back([this] { Pad(16); });
    }

    return array;
}

nemesis::hkArrayBase& nemesis::PackfileDeserializer::ReadArrayObject(const std::string& name,
                                                                     nemesis::hkArrayBase& array)
{
    TryPadToPointerSize();

    size_t pos = Stream->tellg();
    auto itr   = LocalMap.find(pos - DataAddress);

    ConsumePointer();
    array.Clear();

    unsigned int size = ReadArraySize(name);
    array.SetSize(size);

    if (itr == LocalMap.end() || size <= 0) return array;

    LocalQueue->emplace_back(
        [&array, name, this]
        {
            RunScopedQueue(LocalQueue,
                           [&array, this]()
                           {
                               Pad(16);
                               array.DeserializeFrom(*this);

                               RunLocalQueue();
                           });
        });

    if (!array.IsVariant())
    {
        LocalQueue->emplace_back([this] { Pad(16); });
    }

    return array;
}

nemesis::hkRefVariant& nemesis::PackfileDeserializer::ReadRefObject(const std::string& name,
                                                                    nemesis::hkRefVariant& ref_obj)
{
    TryPadToPointerSize();

    size_t addr = Stream->tellg();
    addr -= DataAddress;
    auto itr = GlobalMap.find(addr);

    ConsumePointer();

    if (itr == GlobalMap.end())
    {
        ref_obj.Clear();
        return ref_obj;
    }

    if (ref_obj.IsFixed())
    {
        GlobalQueue[itr->second->Destination + DataAddress].emplace_back(
            [&ref_obj, this]()
            {
#ifdef _DEBUG
                std::cout << "===== " << ref_obj.GetClass(GetContentsVersion())->GetName()
                          << " =====" << std::endl;
#endif

                RunScopedQueue(LocalQueue,
                               [&ref_obj, this]()
                               {
                                   ref_obj.DeserializeFrom(*this);
                                   Pad(16);

                                   RunLocalQueue();
                               });
            });
        return ref_obj;
    }

    auto obj_itr = ObjectMap.find(itr->second->Destination);

    if (obj_itr != ObjectMap.end())
    {
        ref_obj.ReferenceTo(obj_itr->second);
        return ref_obj;
    }

    auto cls    = GetClassFromAddress(GetTargetAddress(addr));
    auto dest   = itr->second->Destination;
    auto q_addr = dest + DataAddress;
    auto q_itr  = GlobalQueue.find(q_addr);

    if (q_itr != GlobalQueue.end())
    {
        q_itr->second.emplace_back([&ref_obj, dest, this]() { ref_obj.ReferenceTo(ObjectMap[dest]); });
        return ref_obj;
    }

    GlobalQueue[q_addr].emplace_back(
        [&ref_obj, name, cls, dest, this]()
        {
#ifdef _DEBUG
            std::cout << "===== " << cls->GetName() << " =====" << std::endl;
#endif

            auto obj_itr = ObjectMap.find(dest);

            if (obj_itr != ObjectMap.end())
            {
                ref_obj.ReferenceTo(obj_itr->second);
                return;
            }

            RunScopedQueue(LocalQueue,
                           [&ref_obj, cls, dest, this]()
                           {
                               auto obj = PackfilePtr->CreateObject(cls->GetName());
                               ObjectMap.insert({dest, obj});
                               ref_obj.ReferenceTo(obj);
                               ref_obj.DeserializeFrom(*this);
                               Pad(16);

                               RunLocalQueue();
                           });
        });
    return ref_obj;
}

nemesis::HavokObject** nemesis::PackfileDeserializer::ReadRefObject(const std::string& name,
                                                                    nemesis::HavokObject** hkx_obj)
{
    TryPadToPointerSize();

    size_t addr = Stream->tellg();
    addr -= DataAddress;
    auto itr = GlobalMap.find(addr);

    ConsumePointer();

    if (itr == GlobalMap.end())
    {
        (*hkx_obj) = nullptr;
        return hkx_obj;
    }

    auto obj_itr = ObjectMap.find(itr->second->Destination);

    if (obj_itr != ObjectMap.end())
    {
        (*hkx_obj) = obj_itr->second;
        return hkx_obj;
    }

    auto cls    = GetClassFromAddress(GetTargetAddress(addr));
    auto dest   = itr->second->Destination;
    auto q_addr = dest + DataAddress;
    auto q_itr  = GlobalQueue.find(q_addr);

    if (q_itr != GlobalQueue.end())
    {
        q_itr->second.emplace_back([&hkx_obj, dest, this]() { (*hkx_obj) = ObjectMap[dest]; });
        return hkx_obj;
    }

    GlobalQueue[q_addr].emplace_back(
        [&hkx_obj, name, cls, dest, this]()
        {
#ifdef _DEBUG
            std::cout << "===== " << cls->GetName() << " =====" << std::endl;
#endif

            auto obj_itr = ObjectMap.find(dest);

            if (obj_itr != ObjectMap.end())
            {
                (*hkx_obj) = obj_itr->second;
                return;
            }

            RunScopedQueue(LocalQueue,
                           [hkx_obj, cls, dest, this]()
                           {
                               auto obj = PackfilePtr->CreateObject(cls->GetName());
                               ObjectMap.insert({dest, obj});
                               (*hkx_obj) = obj;
                               (*hkx_obj)->DeserializeFrom(*this);
                               Pad(16);

                               RunLocalQueue();
                           });
        });
    return hkx_obj;
}

nemesis::hkClass& nemesis::PackfileDeserializer::ReadClass(nemesis::hkClass& cls)
{
    cls.DeserializeFrom(*this);
    return cls;
}

nemesis::HavokObject& nemesis::PackfileDeserializer::ReadObject(const std::string& name,
                                                                nemesis::HavokObject& hk_obj)
{
    hk_obj.DeserializeFrom(*this);
    return hk_obj;
}

unsigned int nemesis::PackfileDeserializer::ReadArraySize(const std::string& name)
{
    unsigned int m_size = Read<unsigned int>(name);
    unsigned int m_capacityAndFlag;
    AssertValue(m_capacityAndFlag, {m_size | 0x80 << 24});
    return m_size;
}

unsigned short nemesis::PackfileDeserializer::ReadSmallArraySize(const std::string& name)
{
    unsigned short m_size = Read<unsigned short>(name);
    unsigned short m_capacityAndFlag;
    AssertValue(m_capacityAndFlag, {static_cast<unsigned short>(m_size | 0x80 << 8)});
    return m_size;
}

std::string nemesis::PackfileDeserializer::ReadString(const std::string& name)
{
    std::string text({Read<char>("")});

    while (text.back() != '\0')
    {
        text.push_back(Read<char>(""));
    }

    text.pop_back();
    return text;
}

std::string nemesis::PackfileDeserializer::ReadString(const std::string& name, unsigned int size, char pad)
{
    std::string rtn_val(size, pad);
    Stream->read(rtn_val.data(), size);
    return rtn_val;
}

nemesis::hkCString nemesis::PackfileDeserializer::ReadCString(const std::string& name)
{
    nemesis::hkCString cstring;
    ReadValue(name, cstring);
    return cstring;
}

nemesis::hkStringPtr nemesis::PackfileDeserializer::ReadStringPointer(const std::string& name)
{
    nemesis::hkStringPtr string_ptr;
    ReadValue(name, string_ptr);
    return string_ptr;
}

uint64_t nemesis::PackfileDeserializer::ReadUSize() noexcept
{
    if (IsUSizeLong()) return Read<uint64_t>();

    return Read<unsigned int>();
}

void nemesis::PackfileDeserializer::ReadFileHeader(nemesis::hkPackfileHeader& header)
{
    header.DeserializeFrom(*this);

    PointerSize           = header.GetPointerSize();
    Endian                = header.GetEndian();
    PaddingOption         = header.GetPaddingOption();
    ContentsVersionString = header.GetContentsVersionString();
    ContentsVersion       = nemesis::GetEnumVersion(ContentsVersionString);
}

std::string nemesis::PackfileDeserializer::AssertString(std::initializer_list<std::string> options)
{
    size_t pos = Stream->tellg();
    std::string val(ReadString("").data());
    std::string optional_val;

    for (const std::string& option : options)
    {
        if (val == option) return val;

        optional_val.append(option + "(" + StringToHex(option) + ")|");
    }

    optional_val        = optional_val.substr(0, optional_val.size() - 1);
    std::string hex_val = StringToHex(val);
    std::replace(val.begin(), val.end(), '\0', ' ');
    std::replace(optional_val.begin(), optional_val.end(), '\0', ' ');
    std::string msg = "Detected string: " + val + " (" + hex_val + "), Expected string(s): " + optional_val
                      + ", Position: " + std::to_string(pos) + "(" + NumberToHex(pos, true) + ")";
    throw std::runtime_error(msg);
}

std::string nemesis::PackfileDeserializer::AssertString(unsigned int size,
                                                        std::initializer_list<std::string> options)
{
    size_t pos = Stream->tellg();
    std::string val(ReadString("", size).data());
    std::string optional_val;

    for (const std::string& option : options)
    {
        if (val == option) return val;

        optional_val.append(option + "(" + StringToHex(option) + ")|");
    }

    optional_val        = optional_val.substr(0, optional_val.size() - 1);
    std::string hex_val = StringToHex(val);
    std::replace(val.begin(), val.end(), '\0', ' ');
    std::replace(optional_val.begin(), optional_val.end(), '\0', ' ');
    std::string msg = "Detected string: " + val + " (" + hex_val + "), Expected string(s): " + optional_val
                      + ", Position: " + std::to_string(pos) + "(" + NumberToHex(pos, true) + ")";
    throw std::runtime_error(msg);
}

uint64_t nemesis::PackfileDeserializer::AssertUSize(std::initializer_list<uint64_t> options)
{
    size_t pos   = Stream->tellg();
    uint64_t val = ReadUSize();
    std::string optional_val;

    for (auto& option : options)
    {
        if (val == option) return val;

        optional_val.append(std::to_string(option) + "|");
    }

    optional_val    = optional_val.substr(0, optional_val.size() - 1);
    std::string msg = "Detected usize: " + std::to_string(val) + " (" + NumberToHex(val, true)
                      + "), Expected value: " + optional_val + ", Position: " + std::to_string(pos) + "("
                      + NumberToHex(pos, true) + ")";
    throw std::runtime_error(msg);
}
