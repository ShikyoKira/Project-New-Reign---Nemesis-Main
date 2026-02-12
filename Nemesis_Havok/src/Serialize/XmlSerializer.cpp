#include "Serialize/XmlSerializer.h"

#include "Havok/hkbBehaviorGraphData.h"
#include "Havok/hkbBehaviorGraphStringData.h"
#include "Havok/hkPackfile.h"

#include "Utilities/Algorithm.h"

std::string nemesis::XmlSerializer::ToString(const nemesis::hkVector4 vec4, bool skip_last)
{
    if (skip_last)
    {
        std::array<std::string, 3> values{
            ToString(vec4.GetX(), 6), ToString(vec4.GetY(), 6), ToString(vec4.GetZ(), 6)};
        return "(" + StringJoin(" ", values) + ")";
    }

    std::array<std::string, 4> values{ToString(vec4.GetX(), 6),
                                      ToString(vec4.GetY(), 6),
                                      ToString(vec4.GetZ(), 6),
                                      ToString(vec4.GetW(), 6)};
    return "(" + StringJoin(" ", values) + ")";
}

std::string nemesis::XmlSerializer::ToString(const nemesis::hkQuaternion quaternion)
{
    std::array<std::string, 4> values{ToString(quaternion.GetX(), 6),
                                      ToString(quaternion.GetY(), 6),
                                      ToString(quaternion.GetZ(), 6),
                                      ToString(quaternion.GetR(), 6)};
    return "(" + StringJoin(" ", values) + ")";
}

std::string nemesis::XmlSerializer::ToString(const nemesis::hkMatrix3 matrix3)
{
    return ToString(matrix3.GetCol0()) + ToString(matrix3.GetCol1()) + ToString(matrix3.GetCol2());
}

std::string nemesis::XmlSerializer::ToString(const nemesis::hkMatrix4 matrix4)
{
    return ToString(matrix4.GetCol0()) + ToString(matrix4.GetCol1()) + ToString(matrix4.GetCol2())
           + ToString(matrix4.GetCol3());
}

std::string nemesis::XmlSerializer::ToString(const nemesis::hkTransform transform)
{
    auto& rotation = transform.GetRotation();
    return ToString(rotation.GetCol0(), true) + ToString(rotation.GetCol1(), true)
           + ToString(rotation.GetCol2(), true) + ToString(transform.GetTranslation(), true);
}

std::string nemesis::XmlSerializer::EncodeXmlValue(const std::string& val)
{
    std::ostringstream oss;

    for (auto& ch : val)
    {
        switch (ch)
        {
            case '&':
                oss << "&amp;";
                continue;
            case '<':
                oss << "&lt;";
                continue;
            case '>':
                oss << "&gt;";
                continue;
            case '"':
                oss << "&quot;";
                continue;
            case '\'':
                oss << "&apos;";
                continue;
            default:
                break;
        }

        if (ch <= 127)
        {
            oss << ch;
            continue;
        }

        oss << "&#" << static_cast<int>(ch) << ";";
    }

    return oss.str();
}

std::string nemesis::XmlSerializer::GetId(const nemesis::HavokObject* hkx_ptr)
{
    if (hkx_ptr == nullptr) return "null";

    auto itr = ObjectIdMap.find(hkx_ptr);

    if (itr != ObjectIdMap.end()) return itr->second;

    std::ostringstream oss;
    oss << std::setw(4) << std::setfill('0') << IdCounter++;
    auto id_itr = ObjectIdMap.insert({hkx_ptr, "#" + oss.str()});
    return id_itr.first->second;
}

void nemesis::XmlSerializer::WriteXmlDeclaration()
{
    (*Stream) << "<?xml version=\"1.0\" encoding=\"ascii\"?>\n";
}

void nemesis::XmlSerializer::WriteHkxParam(const std::string& name, const std::string& val, bool encode_xml)
{
    if (name.empty())
    {
        (*Stream) << std::string(Indent, '\t') << (encode_xml ? EncodeXmlValue(val) : val) << "\n";
        return;
    }

    WriteXmlElement("hkparam",
                    std::array<Pair<std::string, std::string>, 1>{std::make_pair("name", name)},
                    val,
                    encode_xml);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredXml(const std::string& name)
{
    (*Stream) << std::string(Indent, '\t') << "<!-- " << name << " SERIALIZE_IGNORED -->\n";
}

void nemesis::XmlSerializer::WriteXmlElement(const std::string& name)
{
    (*Stream) << std::string(Indent, '\t') << "<" << name << "/>\n";
}

void nemesis::XmlSerializer::WriteXmlElement(const std::string& name, const std::string& val)
{
    (*Stream) << std::string(Indent, '\t') << "<" << name << ">" << EncodeXmlValue(val) << "</" << name
              << ">\n";
}

void nemesis::XmlSerializer::OpenXmlTag(const std::string& name)
{
    (*Stream) << std::string(Indent++, '\t') << "<" << name << ">\n";
}

void nemesis::XmlSerializer::CloseXmlTag(const std::string& name)
{
    (*Stream) << std::string(--Indent, '\t') << "</" << name << ">\n";
}

void nemesis::XmlSerializer::RunNodeQueue()
{
    NodeQueue.for_each([](const nemesis::HavokObject*, std::function<void()> func) { func(); });
}

void nemesis::XmlSerializer::RunLocalQueue()
{
    for (size_t i = 0; i < LocalQueue->size(); i++)
    {
        (*LocalQueue)[i]();
    }

    LocalQueue->clear();
}

nemesis::XmlSerializer::XmlSerializer(nemesis::HavokVersion version)
    : Header(nemesis::PlatformType::WIN32, version)
{
    ContentsVersion       = version;
    PointerSize           = Header.GetPointerSize();
    Endian                = Header.GetEndian();
    PaddingOption         = Header.GetPaddingOption();
    ContentsVersionString = Header.GetContentsVersionString();
}

void nemesis::XmlSerializer::WriteFileHeader()
{
}

void nemesis::XmlSerializer::Serialize(const nemesis::hkPackfile& packfile)
{
    Indent    = 0;
    IdCounter = 50;
    ObjectIdMap.clear();
    LocalQueue->clear();
    NodeQueue.clear();
    XmlStream.clear();

    Stream = &XmlStream;
    WriteXmlDeclaration();

    OpenXmlTag("hkpackfile",
               std::array<Pair<std::string, std::string>, 3>{
                   std::make_pair("classversion", std::to_string(Header.GetFileVersion())),
                   {"contentsversion", Header.GetContentsVersionString()},
                   {"toplevelobject", "#00" + std::to_string(IdCounter)}});
    (*Stream) << "\n";

    packfile.SerializeTo(*this);

    (*Stream) << "\n";
    CloseXmlTag("hkpackfile");
}

void nemesis::XmlSerializer::Serialize(const nemesis::hkPackfileSection& section_data)
{
    const nemesis::hkDataSection* data_section = dynamic_cast<const nemesis::hkDataSection*>(&section_data);

    if (data_section == nullptr) return;

    OpenXmlTag("hksection",
               std::array<Pair<std::string, std::string>, 1>{
                   std::make_pair("name", data_section->GetHeader().GetSectionTag())});
    (*Stream) << "\n";
    data_section->SerializeTo(*this);
    CloseXmlTag("hksection");
}

void nemesis::XmlSerializer::Serialize(const nemesis::hkRootLevelContainer& root)
{
    auto stream_uptr = std::make_unique<std::ostringstream>();
    auto old_stream  = Stream;
    Stream           = stream_uptr.get();
    auto* cls_ptr    = root.GetClass(ContentsVersion);

    OpenXmlTag(
        "hkobject",
        std::array<Pair<std::string, std::string>, 3>{std::make_pair("name", GetId(&root)),
                                                      {"class", cls_ptr->GetName()},
                                                      {"signature", NumberToHex(cls_ptr->GetSignature())}});
    root.SerializeTo(*this);
    CloseXmlTag("hkobject");
    (*Stream) << "\n";

    RunLocalQueue();

    NodeQueue.emplace_back(&root, [str = stream_uptr->str(), this] { XmlStream << str; });
    RunNodeQueue();

    Stream = old_stream;
}

void nemesis::XmlSerializer::Save(const std::filesystem::path& filepath)
{
    if (filepath.has_parent_path())
    {
        std::filesystem::create_directories(filepath.parent_path());
    }

    std::ofstream file_stream(filepath);

    if (!file_stream)
    {
        std::error_code ec(errno, std::system_category());
        throw std::runtime_error("Failed to open file: \"" + nemesis::to_utf8_string(filepath)
                                 + "\"\nMessage: " + ec.message());
    }

    file_stream << XmlStream.str();
    file_stream.close();
}

std::string nemesis::XmlSerializer::RawData() const
{
    return XmlStream.str();
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, bool val)
{
    static std::string true_val("true");
    static std::string false_val("false");
    WriteHkxParam(name, val ? true_val : false_val, false);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, char val)
{
    WriteHkxParam(name, std::to_string(static_cast<int>(val)), false);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, unsigned char val)
{
    WriteHkxParam(name, std::to_string(val), false);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, short val)
{
    WriteHkxParam(name, std::to_string(val), false);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, unsigned short val)
{
    WriteHkxParam(name, std::to_string(val), false);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, int val)
{
    WriteHkxParam(name, std::to_string(val), false);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, unsigned int val)
{
    WriteHkxParam(name, std::to_string(val), false);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, long val)
{
    WriteHkxParam(name, std::to_string(val), false);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, unsigned long val)
{
    WriteHkxParam(name, std::to_string(val), false);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, long long val)
{
    WriteHkxParam(name, std::to_string(val), false);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, unsigned long long val)
{
    WriteHkxParam(name, std::to_string(val), false);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, Float16 val)
{
    float f = val;
    WriteHkxParam(name, ToString(f, 6), false);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, float val)
{
    WriteHkxParam(name, ToString(val, 6), false);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, double val)
{
    WriteHkxParam(name, ToString(val, 6), false);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, const std::string& val, bool terminate)
{
    WriteHkxParam(name, val);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name,
                                        const std::string& val,
                                        std::size_t size,
                                        char padding)
{
    WriteHkxParam(name, val);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, const nemesis::hkEventId& event_id)
{
    int id   = event_id;
    int size = CurrentStringData->GetEventNames().GetSize();

    if (id < -1 || size <= id)
    {
        throw std::runtime_error("Event id out of bounds (" + std::to_string(id) + ")");
    }

    WriteValue(name, id);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, const nemesis::hkAttributeId& attr_id)
{
    int id   = attr_id;
    int size = CurrentStringData->GetAttributeNames().GetSize();

    if (id < -1 || size <= id)
    {
        throw std::runtime_error("Attribute id out of bounds (" + std::to_string(id) + ")");
    }

    WriteValue(name, id);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, const nemesis::hkVariableId& var_id)
{
    int id   = var_id;
    int size = CurrentStringData->GetVariableNames().GetSize();

    if (id < -1 || size <= id)
    {
        throw std::runtime_error("Variable id out of bounds (" + std::to_string(id) + ")");
    }

    WriteValue(name, id);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name,
                                        const nemesis::hkCharacterPropertyId& prop_id)
{
    int id   = prop_id;
    int size = CurrentStringData->GetCharacterPropertyNames().GetSize();

    if (id < -1 || size <= id)
    {
        throw std::runtime_error("Character property id out of bounds (" + std::to_string(id) + ")");
    }

    WriteValue(name, id);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, const nemesis::hkCString& cstring)
{
    if (name.empty())
    {
        WriteXmlElement("hkcstring", cstring.GetValue());
        return;
    }

    if (cstring.IsNull())
    {
        WriteHkxParam(name, "\u2400");
        return;
    }

    WriteHkxParam(name, cstring.GetValue());
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, const nemesis::hkStringPtr& string_ptr)
{
    if (string_ptr.IsNull())
    {
        WriteHkxParam(name, "\u2400");
        return;
    }

    WriteHkxParam(name, string_ptr.GetValue());
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, const nemesis::hkEnumBase& val)
{
    WriteHkxParam(name, val.str(), false);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, const nemesis::hkVector4& vec4)
{
    WriteHkxParam(name, ToString(vec4), false);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, const nemesis::hkVector8& vec8)
{
    WriteHkxParam(name, ToString(vec8.GetLin()) + ToString(vec8.GetAng()), false);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, const nemesis::hkQuaternion& quaternion)
{
    WriteHkxParam(name, ToString(quaternion), false);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, const nemesis::hkQsTransform& qs_transform)
{
    WriteHkxParam(name,
                  ToString(qs_transform.GetTranslation(), true) + ToString(qs_transform.GetRotation())
                      + ToString(qs_transform.GetScale(), true),
                  false);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, const nemesis::hkUFloat8& ufloat8)
{
    WriteHkxParam(name, ToString(ufloat8.AsFloat(), 6), false);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, const nemesis::hkLong& _long)
{
    WriteHkxParam(name, std::to_string(_long.AsInt64()), false);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, const nemesis::hkUlong& ulong)
{
    WriteHkxParam(name, std::to_string(ulong.AsUint64()), false);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, const nemesis::hkHalf& half)
{
    WriteHkxParam(name, std::to_string(half.AsFloat()), false);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, const nemesis::hkTransform& transform)
{
    WriteHkxParam(name, ToString(transform), false);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, const nemesis::hkMatrix3& matrix3)
{
    WriteHkxParam(name, ToString(matrix3), false);
}

void nemesis::XmlSerializer::WriteValue(const std::string& name, const nemesis::hkMatrix4& matrix4)
{
    WriteHkxParam(name, ToString(matrix4), false);
}

void nemesis::XmlSerializer::WriteObject(const std::string& name, const nemesis::hkSmallArrayBase& array)
{
    WriteArrayObject(name, array);
}

void nemesis::XmlSerializer::WriteObject(const std::string& name, const nemesis::hkArrayBase& array)
{
    WriteArrayObject(name, array);
}

void nemesis::XmlSerializer::WriteObject(const std::string& name, const nemesis::hkRefVariant& ref_obj)
{
    if (ref_obj.IsNull())
    {
        WriteHkxParam(name, "null", false);
        return;
    }

    auto& hkx_obj = ref_obj.GetReference();
    WriteHkxParam(name, GetId(&hkx_obj), false);

    LocalQueue->emplace_front(
        [&ref_obj, &hkx_obj, this]()
        {
            if (NodeQueue.find(&hkx_obj)) return;
            
            UPtr<std::ostringstream> stream_uptr;
            RunScopedContainer(LocalQueue,
                               [&ref_obj, &stream_uptr, this]()
                               {
                                   stream_uptr = RunScopedContainer<std::ostringstream>(
                                       Stream,
                                       [&ref_obj, this]()
                                       {
                                           auto& hkx_obj = ref_obj.GetReference();
                                           auto* bhv_graph_data
                                               = dynamic_cast<const nemesis::hkbBehaviorGraphData*>(&hkx_obj);

                                           if (bhv_graph_data)
                                           {
                                               CurrentStringData = bhv_graph_data->GetStringData();
                                           }

                                           auto* cls_ptr = hkx_obj.GetClass(ContentsVersion);

                                           OpenXmlTag(
                                               "hkobject",
                                               std::array<Pair<std::string, std::string>, 3>{
                                                   std::make_pair("name", GetId(&hkx_obj)),
                                                   {"class", cls_ptr->GetName()},
                                                   {"signature", NumberToHex(cls_ptr->GetSignature())}});
                                           ref_obj.SerializeTo(*this);
                                           CloseXmlTag("hkobject");
                                           (*Stream) << "\n";
                                       });
                                   RunLocalQueue();
                               });
            NodeQueue.emplace_back(&hkx_obj, [str = stream_uptr->str(), this] { XmlStream << str; });
        });
}

void nemesis::XmlSerializer::WriteObject(const std::string& name, const nemesis::HavokObject& havok_obj)
{
    if (!name.empty())
    {
        OpenXmlTag("hkparam", std::array<Pair<std::string, std::string>, 1>{std::make_pair("name", name)});
    }

    OpenXmlTag("hkobject");
    havok_obj.SerializeTo(*this);
    CloseXmlTag("hkobject");

    if (!name.empty())
    {
        CloseXmlTag("hkparam");
    }
}

void nemesis::XmlSerializer::WriteObject(const std::string& name,
                                         const nemesis::HavokObject* const* ptr_obj_ptr)
{
    if (!ptr_obj_ptr || !(*ptr_obj_ptr))
    {
        WriteHkxParam(name, "null", false);
        return;
    }

    auto* hkx_ptr = *ptr_obj_ptr;
    WriteHkxParam(name, GetId(hkx_ptr));

    LocalQueue->emplace_front(
        [hkx_ptr, this]()
        {
            if (NodeQueue.find(hkx_ptr)) return;

            UPtr<std::ostringstream> stream_uptr;
            RunScopedContainer(LocalQueue,
                               [hkx_ptr, &stream_uptr, this]()
                               {
                                   stream_uptr = RunScopedContainer<std::ostringstream>(
                                       Stream,
                                       [hkx_ptr, this]()
                                       {
                                           auto* cls_ptr = hkx_ptr->GetClass(ContentsVersion);

                                           OpenXmlTag(
                                               "hkobject",
                                               std::array<Pair<std::string, std::string>, 3>{
                                                   std::make_pair("name", GetId(hkx_ptr)),
                                                   {"class", cls_ptr->GetName()},
                                                   {"signature", NumberToHex(cls_ptr->GetSignature())}});
                                           hkx_ptr->SerializeTo(*this);
                                           CloseXmlTag("hkobject");
                                       });
                                   RunLocalQueue();
                               });
            NodeQueue.emplace_back(hkx_ptr, [str = stream_uptr->str(), this] { XmlStream << str; });
        });
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name, bool val)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name, char val)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name, unsigned char val)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name, short val)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name, unsigned short val)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name, int val)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name, unsigned int val)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name, long val)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name, unsigned long val)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name, long long val)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name, unsigned long long val)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name, Float16 val)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name, float val)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name, double val)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name,
                                                        const nemesis::hkEventId& event_id)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name,
                                                        const nemesis::hkAttributeId& attr_id)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name,
                                                        const nemesis::hkVariableId& var_id)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name,
                                                        const nemesis::hkCharacterPropertyId& prop_id)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name,
                                                        const nemesis::hkCString& cstring)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name,
                                                        const nemesis::hkStringPtr& string_ptr)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name,
                                                        const nemesis::hkStringPtr& string_ptr,
                                                        unsigned int padding_size)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name,
                                                        const nemesis::hkEnumBase& val)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name,
                                                        const nemesis::hkVector4& vec4)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name,
                                                        const nemesis::hkVector8& vec8)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name,
                                                        const nemesis::hkQuaternion& quaternion)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name,
                                                        const nemesis::hkQsTransform& qs_transform)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name,
                                                        const nemesis::hkTransform& transform)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name,
                                                        const nemesis::hkUFloat8& ufloat8)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name, const nemesis::hkLong& _long)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name,
                                                        const nemesis::hkUlong& ulong)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredValue(const std::string& name, const nemesis::hkHalf& half)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredObject(const std::string& name,
                                                         const nemesis::hkSmallArrayBase& array)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredObject(const std::string& name,
                                                         const nemesis::hkArrayBase& array)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredObject(const std::string& name,
                                                         const nemesis::hkRefVariant& ref_obj)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredObject(const std::string& name,
                                                         const nemesis::HavokObject& havok_obj)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredObject(const std::string& name,
                                                         const nemesis::HavokObject* const* ptr_obj_ptr)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredObject(const std::string& name, void* obj_ptr)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name,
                                             const void* (&list)[],
                                             size_t size,
                                             size_t type_size)
{
    std::stringstream ss;

    for (size_t i = 0; i < size; ++i)
    {
        ss.write(reinterpret_cast<const char*>(list[i]), type_size);
    }

    WriteHkxParam(name, ss.str());
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name, const bool* (&list)[], size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name, const char* (&list)[], size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name,
                                             const unsigned char* (&list)[],
                                             size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name, const short* (&list)[], size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name,
                                             const unsigned short* (&list)[],
                                             size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name, const int* (&list)[], size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name,
                                             const unsigned int* (&list)[],
                                             size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name, const long* (&list)[], size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name,
                                             const unsigned long* (&list)[],
                                             size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name, const long long* (&list)[], size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name,
                                             const unsigned long long* (&list)[],
                                             size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name, const Float16* (&list)[], size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name, const float* (&list)[], size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name, const double* (&list)[], size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name,
                                             const nemesis::hkCString* (&list)[],
                                             size_t size)
{
    throw std::runtime_error("WriteArrayValue with a list of hkCString is unsupported");
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name,
                                             const nemesis::hkStringPtr* (&list)[],
                                             size_t size)
{
    throw std::runtime_error("WriteArrayValue with a list of hkStringPtr is unsupported");
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name,
                                             const nemesis::hkEnumBase* (&list)[],
                                             size_t size)
{
    throw std::runtime_error("WriteArrayValue with a list of hkEnumBase is unsupported");
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name,
                                             const nemesis::hkVector4* (&list)[],
                                             size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name,
                                             const nemesis::hkVector8* (&list)[],
                                             size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name,
                                             const nemesis::hkQuaternion* (&list)[],
                                             size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name,
                                             const nemesis::hkQsTransform* (&list)[],
                                             size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name,
                                             const nemesis::hkUFloat8* (&list)[],
                                             size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name,
                                             const nemesis::hkLong* (&list)[],
                                             size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name,
                                             const nemesis::hkUlong* (&list)[],
                                             size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name,
                                             const nemesis::hkHalf* (&list)[],
                                             size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name,
                                             const nemesis::hkTransform* (&list)[],
                                             size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name,
                                             const nemesis::hkMatrix3* (&list)[],
                                             size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::XmlSerializer::WriteArrayValue(const std::string& name,
                                             const nemesis::hkMatrix4* (&list)[],
                                             size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::XmlSerializer::WriteArrayObject(const std::string& name,
                                              const nemesis::HavokObject* (&list)[],
                                              size_t size)
{
    WriteArrayObjectImplt(name, list, size);
}

void nemesis::XmlSerializer::WriteArrayRefObject(const std::string& name,
                                                 const nemesis::hkRefVariant* (&list)[],
                                                 size_t size)
{
    WriteArrayObjectImplt(name, list, size);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                             const void* (&list)[],
                                                             size_t size,
                                                             size_t type_size)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                             const nemesis::hkCString* (&list)[],
                                                             size_t size)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                             const nemesis::hkStringPtr* (&list)[],
                                                             size_t size)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                             const nemesis::hkEnumBase* (&list)[],
                                                             size_t size)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                             const nemesis::hkVector4* (&list)[],
                                                             size_t size)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                             const nemesis::hkVector8* (&list)[],
                                                             size_t size)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                             const nemesis::hkQuaternion* (&list)[],
                                                             size_t size)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                             const nemesis::hkQsTransform* (&list)[],
                                                             size_t size)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                             const nemesis::hkUFloat8* (&list)[],
                                                             size_t size)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                             const nemesis::hkLong* (&list)[],
                                                             size_t size)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                             const nemesis::hkUlong* (&list)[],
                                                             size_t size)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                             const nemesis::hkHalf* (&list)[],
                                                             size_t size)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                             const nemesis::hkTransform* (&list)[],
                                                             size_t size)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                             const nemesis::hkMatrix3* (&list)[],
                                                             size_t size)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoredArrayValue(const std::string& name,
                                                             const nemesis::hkMatrix4* (&list)[],
                                                             size_t size)
{
    WriteSerializeIgnoredXml(name);
}

void nemesis::XmlSerializer::WriteSerializeIgnoreArrayObject(const std::string& name,
                                                             const nemesis::HavokObject* (&list)[],
                                                             size_t size)
{
    WriteSerializeIgnoredXml(name);
}
