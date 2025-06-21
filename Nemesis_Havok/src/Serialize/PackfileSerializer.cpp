#include "Serialize/PackfileSerializer.h"

#include "Havok/hkClassMember.h"
#include "Havok/hkClassNamesSection.h"
#include "Havok/hkPackfile.h"
#include "Havok/hkPackfileSection.h"
#include "Havok/hkPackfileSectionHeader.h"
#include "Havok/hkRootLevelContainer.h"

#include <iostream>

void nemesis::PackfileSerializer::PadStream(std::ostream& stream, int pad_align_to)
{
    while (stream.tellp() % 16 != 0)
    {
        stream.put(0xFF);
    }
}

static DeqStr local_log;
static std::string space;

void nemesis::PackfileSerializer::RunLocalQueue()
{
#ifdef _DEBUG
    space.append("  ");
#endif

    for (size_t i = 0; i < LocalQueue->size(); i++)
    {
        auto& queue = LocalQueue->at(i);

#ifdef _DEBUG
        size_t addr = Stream->tellp();
        std::string& log = local_log.emplace_back(space + "Local Queue : " + NumberToHex(addr, true) + " ("
                                                  + std::to_string(addr) + ")");
#endif

        queue();

#ifdef _DEBUG
        size_t end_addr = Stream->tellp();
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

void nemesis::PackfileSerializer::RunGlobalQueue()
{
    for (size_t i = 0; i < GlobalQueue->size(); i++)
    {
        auto& queue = GlobalQueue->at(i);

#ifdef _DEBUG
        size_t addr = Stream->tellp();
        std::cout << space << "Global Queue : " << NumberToHex(addr, true) << " (" << addr << ") ";
        space.append("  ");
#endif

        queue();

#ifdef _DEBUG
        for (auto& each : local_log)
        {
            std::cout << each << std::endl;
        }

        local_log.clear();
        space.pop_back();
        space.pop_back();

        size_t end_addr = Stream->tellp();
        std::cout << space << "Last Global Queue Size: " << end_addr - addr << std::endl;
#endif

        Pad(16);
    }

    GlobalQueue->clear();
}

unsigned int nemesis::PackfileSerializer::TryClass(const nemesis::hkClass* cls)
{
    auto class_itr = ClassMap.find(cls);

    if (class_itr != ClassMap.end()) return class_itr->second;

    unsigned int signature      = cls->GetSignature();
    constexpr unsigned char pad = 0x09;

    WriteToStream(StreamSections.front().DataStream,
                  [signature, &class_itr, cls, this]()
                  {
                      WriteValue("", signature);
                      WriteValue("", pad);
                      class_itr = ClassMap.insert({cls, Stream->tellp()}).first;
                      (*Stream) << cls->GetName();
                      Stream->put(0x00);
                  });
    return class_itr->second;
}

void nemesis::PackfileSerializer::WriteVirtualFixup(unsigned int body_addr, unsigned int cls_addr)
{
    constexpr unsigned int v_index = 0;

    WriteToStream(StreamSections.back().VirtualStream,
                  [body_addr, cls_addr, this]()
                  {
                      WriteValue("", body_addr);
                      WriteValue("", v_index);
                      WriteValue("", cls_addr);
                  });
}

void nemesis::PackfileSerializer::FlushGlobalFixupList()
{
    WriteToStream(StreamSections.back().GlobalStream,
                  [this]()
                  {
                      for (auto& global_fixup : GlobalFixupList)
                      {
                          WriteValue("", global_fixup->Source);
                          WriteValue("", global_fixup->DestinationSectionIndex);
                          WriteValue("", global_fixup->Destination);
                      }
                  });
}

void nemesis::PackfileSerializer::FlushSectionHeaderList()
{
    size_t offset = MainStream.tellp();
    offset += nemesis::hkPackfileSectionHeader::GetHeaderSize(ContentsVersion) * SectionHeaders.size();

    for (size_t i = 0; i < SectionHeaders.size(); i++)
    {
        auto& header  = SectionHeaders[i];
        auto& section = StreamSections[i];
        Stream        = &section.HeaderStream;

        PadStream(section.DataStream, 16);
        PadStream(section.LocalStream, 16);
        PadStream(section.GlobalStream, 16);
        PadStream(section.VirtualStream, 16);

        header->SetAbsoluteDataStart(offset);
        header->SetLocalFixupsOffset(section.DataStream.tellp());
        header->SetGlobalFixupsOffset(header->GetLocalFixupsOffset() + section.LocalStream.tellp());
        header->SetVirtualFixupsOffset(header->GetGlobalFixupsOffset() + section.GlobalStream.tellp());
        header->SetExportFixupsOffset(header->GetVirtualFixupsOffset() + section.VirtualStream.tellp());
        header->SetImportFixupsOffset(header->GetExportFixupsOffset());
        header->SetEndOffset(header->GetImportFixupsOffset());
        header->SerializeTo(*this);
        offset += header->GetEndOffset();
    }
}

nemesis::PackfileSerializer::PackfileSerializer(nemesis::PlatformType platform,
                                                nemesis::HavokVersion version)
    : Header(platform, version)
    , PltfrmType(platform)
{
    ContentsVersion       = version;
    ContentsVersionString = nemesis::GetStringVersion(version);
}

void nemesis::PackfileSerializer::WriteFileHeader()
{
    Header.SerializeTo(*this);
}

void nemesis::PackfileSerializer::Serialize(const nemesis::hkPackfile& packfile)
{
    GlobalFixupList.clear();
    MainStream.clear();
    StreamSections.clear();
    SectionHeaders.clear();
    ClassMap.clear();

    Stream = &MainStream;
    packfile.SerializeTo(*this);

    FlushGlobalFixupList();
    FlushSectionHeaderList();
}

void nemesis::PackfileSerializer::Serialize(const nemesis::hkPackfileSection& section_data)
{
    Stream = &StreamSections.emplace_back().DataStream;

    if (StreamSections.size() == 1)
    {
        TryClass(nemesis::hkClass::GetCurrentClass());
        TryClass(&nemesis::hkClassMember::Class);
        TryClass(&nemesis::hkClassEnum::Class);
        TryClass(&nemesis::hkClassEnumItem::Class);
    }
    else
    {
        section_data.SerializeTo(*this);
        Pad(16);
    }

    SectionHeaders.emplace_back(section_data.NewHeader());
}

void nemesis::PackfileSerializer::Serialize(const nemesis::hkRootLevelContainer& root)
{
    unsigned int body_addr         = Stream->tellp();
    constexpr unsigned int v_index = 0;
    unsigned int cls_addr          = TryClass(&nemesis::hkRootLevelContainer::Class);

    WriteToStream(StreamSections.back().VirtualStream,
                  [body_addr, cls_addr, this]()
                  {
                      WriteValue("", body_addr);
                      WriteValue("", v_index);
                      WriteValue("", cls_addr);
                  });

    root.SerializeTo(*this);
    Pad(16);

    RunLocalQueue();
    RunGlobalQueue();
}

void nemesis::PackfileSerializer::Save(const std::filesystem::path& filepath)
{
    std::filesystem::create_directories(filepath.parent_path());
    std::ofstream file_stream(filepath, std::ios::binary);

    if (!file_stream)
    {
        std::error_code ec(errno, std::system_category());
        throw std::runtime_error("Failed to open file: \"" + filepath.string() + "\"\nMessage: " + ec.message());
    }

    file_stream << MainStream.str();

    for (auto& section : StreamSections)
    {
        file_stream << section.HeaderStream.str();
    }

    for (auto& section : StreamSections)
    {
        file_stream << section.DataStream.str();
        file_stream << section.LocalStream.str();
        file_stream << section.GlobalStream.str();
        file_stream << section.VirtualStream.str();
    }

    file_stream.close();
}

int nemesis::PackfileSerializer::GetCurrentPosition() const
{
    if (!Stream) return 0;

    return Stream->tellp();
}

void nemesis::PackfileSerializer::Pad(unsigned int size)
{
    Pad(size, '\0');
}

void nemesis::PackfileSerializer::Pad(unsigned int size, unsigned char pad_ch)
{
    while (Stream->tellp() % size > 0)
    {
        WriteValue("", pad_ch);
    }
}

void nemesis::PackfileSerializer::PadNode()
{
    if (PltfrmType == nemesis::PlatformType::PS4) return;

    Pad(PointerSize);
}

void nemesis::PackfileSerializer::TryPadToPointerSize()
{
    //if (PaddingOption != 1) return;

    //Pad(PointerSize);
}

void nemesis::PackfileSerializer::Skip(unsigned int step)
{
    if (step == 0) return;

    for (size_t i = 0; i < step; ++i)
    {
        Stream->put(0x00);
    }
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, bool val)
{
    WriteValue(val);
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, char val)
{
    WriteValue(val);
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, unsigned char val)
{
    WriteValue(val);
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, short val)
{
    WriteValue(val);
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, unsigned short val)
{
    WriteValue(val);
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, int val)
{
    WriteValue(val);
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, unsigned int val)
{
    WriteValue(val);
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, long val)
{
    WriteValue(val);
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, unsigned long val)
{
    WriteValue(val);
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, long long val)
{
    WriteValue(val);
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, unsigned long long val)
{
    WriteValue(val);
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, Float16 val)
{
    WriteValue(val);
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, float val)
{
    WriteValue(val);
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, double val)
{
    WriteValue(val);
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, const std::string& val, bool terminate)
{
    std::string text = val;

    if (terminate)
    {
        text.push_back('\0');
    }

    Stream->write(text.data(), text.size());
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name,
                                             const std::string& val,
                                             std::size_t size,
                                             char padding)
{
    std::string replc(val.data(), val.length() + 1);
    std::string text(size, padding);
    text.replace(0, replc.length(), replc);
    Stream->write(text.data(), size);
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, const nemesis::hkEventId& event_id)
{
    WriteValue(name, static_cast<int>(event_id));
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, const nemesis::hkAttributeId& attr_id)
{
    WriteValue(name, static_cast<int>(attr_id));
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, const nemesis::hkVariableId& var_id)
{
    WriteValue(name, static_cast<int>(var_id));
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name,
                                             const nemesis::hkCharacterPropertyId& prop_id)
{
    WriteValue(name, static_cast<int>(prop_id));
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, const nemesis::hkCString& cstring)
{
    WriteValue(name, cstring, 16);
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, const nemesis::hkCString& cstring, unsigned int padding_size)
{
    TryPadToPointerSize();

    unsigned int pos = Stream->tellp();

    WriteUSize(0);

    if (cstring.IsNull()) return;

    auto& val = cstring.GetValue();

    LocalQueue->emplace_back(
        [&val, pos, padding_size, this]()
        {
            unsigned int dest = Stream->tellp();

            WriteToStream(StreamSections.back().LocalStream,
                          [pos, dest, this]()
                          {
                              WriteValue("", pos);
                              WriteValue("", dest);
                          });

            (*Stream) << val;
            Stream->put(0x00);
            Pad(padding_size);
        });
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, const nemesis::hkStringPtr& string_ptr)
{
    WriteValue(name, string_ptr, 16);
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name,
                                             const nemesis::hkStringPtr& string_ptr,
                                             unsigned int padding_size)
{
    TryPadToPointerSize();

    unsigned int pos = Stream->tellp();
    auto& val        = string_ptr.GetValue();

    WriteUSize(0);

    if (string_ptr.IsNull() || val.empty() || val == "\u2400") return;

    LocalQueue->emplace_back(
        [&val, pos, padding_size, this]()
        {
            unsigned int dest = Stream->tellp();

            WriteToStream(StreamSections.back().LocalStream,
                          [pos, dest, this]()
                          {
                              WriteValue("", pos);
                              WriteValue("", dest);
                          });
            (*Stream) << val;
            Stream->put(0x00);
            Pad(padding_size);
        });
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, const nemesis::hkEnumBase& val)
{
    WriteFromPointer(val.bin(), val.size());
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, const nemesis::hkVector4& vec4)
{
    WriteValue(name, vec4.GetX());
    WriteValue(name, vec4.GetY());
    WriteValue(name, vec4.GetZ());
    WriteValue(name, vec4.GetW());
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, const nemesis::hkVector8& vec8)
{
    WriteValue(name, vec8.GetLin());
    WriteValue(name, vec8.GetAng());
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, const nemesis::hkQuaternion& quaternion)
{
    WriteValue(name, quaternion.GetX());
    WriteValue(name, quaternion.GetY());
    WriteValue(name, quaternion.GetZ());
    WriteValue(name, quaternion.GetR());
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name,
                                             const nemesis::hkQsTransform& qs_transform)
{
    WriteValue(name, qs_transform.GetTranslation());
    WriteValue(name, qs_transform.GetRotation());
    WriteValue(name, qs_transform.GetScale());
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, const nemesis::hkUFloat8& ufloat8)
{
    WriteValue(name, ufloat8.AsByte());
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, const nemesis::hkLong& _long)
{
    if (PointerSize == 8)
    {
        WriteValue(name, _long.AsInt64());
        return;
    }

    WriteValue(name, _long.AsInt32());
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, const nemesis::hkUlong& ulong)
{
    if (PointerSize == 8)
    {
        WriteValue(name, ulong.AsUint64());
        return;
    }

    WriteValue(name, ulong.AsUint32());
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, const nemesis::hkHalf& half)
{
    WriteValue(name, half.AsShort());
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, const nemesis::hkTransform& transform)
{
    WriteValue(name, transform.GetRotation());
    WriteValue(name, transform.GetTranslation());
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, const nemesis::hkMatrix3& matrix3)
{
    WriteValue(name, matrix3.GetCol0());
    WriteValue(name, matrix3.GetCol1());
    WriteValue(name, matrix3.GetCol2());
}

void nemesis::PackfileSerializer::WriteValue(const std::string& name, const nemesis::hkMatrix4& matrix4)
{
    WriteValue(name, matrix4.GetCol0());
    WriteValue(name, matrix4.GetCol1());
    WriteValue(name, matrix4.GetCol2());
    WriteValue(name, matrix4.GetCol3());
}

void nemesis::PackfileSerializer::WriteObject(const std::string& name, const nemesis::hkSmallArrayBase& array)
{
    TryPadToPointerSize();

    unsigned short size = array.GetSize();
    unsigned int pos    = Stream->tellp();

    WriteUSize(0);
    WriteValue(name, size);
    WriteValue(name, static_cast<unsigned short>(size | 0x80 << 12));

    if (size <= 0) return;

    LocalQueue->emplace_back(
        [&array, pos, this]()
        {
            unsigned int dest = Stream->tellp();

            WriteToStream(StreamSections.back().LocalStream,
                          [pos, dest, this]()
                          {
                              WriteValue("", pos);
                              WriteValue("", dest);
                          });

            RunScopedQueue(LocalQueue,
                           [&array, this]()
                           {
                               Pad(16);
                               array.SerializeTo(*this);
                               RunLocalQueue();
                           });
        });
}

void nemesis::PackfileSerializer::WriteObject(const std::string& name, const nemesis::hkArrayBase& array)
{
    TryPadToPointerSize();

    unsigned int size = array.GetSize();
    unsigned int pos  = Stream->tellp();

    WriteUSize(0);
    WriteValue(name, size);
    WriteValue(name, size | 0x80 << 24);

    if (size <= 0) return;

    LocalQueue->emplace_back(
        [&array, name, pos, this]()
        {
            unsigned int dest = Stream->tellp();

            WriteToStream(StreamSections.back().LocalStream,
                          [pos, dest, this]()
                          {
                              WriteValue("", pos);
                              WriteValue("", dest);
                          });

            RunScopedQueue(LocalQueue,
                           [&array, this]()
                           {
                               Pad(16);
                               array.SerializeTo(*this);
                               RunLocalQueue();
                           });
        });
}

void nemesis::PackfileSerializer::WriteObject(const std::string& name, const nemesis::hkRefVariant& ref_obj)
{
    TryPadToPointerSize();

    unsigned int pos = Stream->tellp();

    WriteUSize(0);

    if (ref_obj.IsNull()) return;

    auto* ref_ptr     = &ref_obj.GetReference();
    auto global_fixup = std::make_unique<nemesis::GlobalFixup>(pos, 2, 0);
    auto itr          = GlobalMap.find(ref_ptr);

    if (itr != GlobalMap.end())
    {
        global_fixup->Destination = itr->second;
        LocalQueue->emplace_back([fixup_uptr = std::move(global_fixup), this]() mutable
                                 { GlobalFixupList.emplace_back(std::move(fixup_uptr)); });
        return;
    }

    const nemesis::hkClass* cls = ref_obj.GetClass(ContentsVersion);

    if (cls == nullptr) throw std::runtime_error("Referenced Object has null class");

    std::function<void()> to_write = [&ref_obj, global_fixup = global_fixup.get(), ref_ptr, cls, this]()
    {
#ifdef _DEBUG
        std::cout << "===== " << cls->GetName() << " =====" << std::endl;
#endif

        unsigned int cls_addr = TryClass(cls);
        auto itr              = GlobalMap.find(ref_ptr);
        
        if (itr != GlobalMap.end())
        {
            global_fixup->Destination = itr->second;
            return;
        }

        unsigned int dest        = Stream->tellp();
        global_fixup->Destination = dest;
        WriteVirtualFixup(dest, cls_addr);
        GlobalMap.insert({ref_ptr, dest});

        RunScopedQueue(GlobalQueue,
                       [&ref_obj, this]()
                       {
                           RunScopedQueue(LocalQueue,
                                          [&ref_obj, this]()
                                          {
                                              ref_obj.SerializeTo(*this);
                                              Pad(16);
                                              RunLocalQueue();
                                          });
                           RunGlobalQueue();
                       });
    };

    LocalQueue->emplace_back(
        [to_write, fixup_uptr = std::move(global_fixup), this]() mutable
        {
            GlobalFixupList.emplace_back(std::move(fixup_uptr));
            GlobalQueue->emplace_back(to_write);
        });
}

void nemesis::PackfileSerializer::WriteObject(const std::string& name, const nemesis::HavokObject& havok_obj)
{
    havok_obj.SerializeTo(*this);
}

void nemesis::PackfileSerializer::WriteObject(const std::string& name,
                                              const nemesis::HavokObject* const* ptr_obj_ptr)
{
    TryPadToPointerSize();

    unsigned int pos = Stream->tellp();

    WriteUSize(0);

    if (!ptr_obj_ptr || !*ptr_obj_ptr) return;

    auto* ref_ptr     = *ptr_obj_ptr;
    auto global_fixup = std::make_unique<nemesis::GlobalFixup>(pos, 2, 0);
    auto itr          = GlobalMap.find(ref_ptr);

    if (itr != GlobalMap.end())
    {
        global_fixup->Destination = itr->second;
        LocalQueue->emplace_back([fixup_uptr = std::move(global_fixup), this]() mutable
                                 { GlobalFixupList.emplace_back(std::move(fixup_uptr)); });
        return;
    }

    const nemesis::hkClass* cls = ref_ptr->GetClass(ContentsVersion);

    if (cls == nullptr) throw std::runtime_error("Referenced Object has null class");

    std::function<void()> to_write = [ref_ptr, global_fixup = global_fixup.get(), cls, this]()
    {
#ifdef _DEBUG
        std::cout << "===== " << cls->GetName() << " =====" << std::endl;
#endif

        unsigned int cls_addr = TryClass(cls);
        auto itr              = GlobalMap.find(ref_ptr);

        if (itr != GlobalMap.end())
        {
            global_fixup->Destination = itr->second;
            return;
        }

        unsigned int dest         = Stream->tellp();
        global_fixup->Destination = dest;
        WriteVirtualFixup(dest, cls_addr);
        GlobalMap.insert({ref_ptr, dest});

        RunScopedQueue(GlobalQueue,
                       [ref_ptr, this]()
                       {
                           RunScopedQueue(LocalQueue,
                                          [ref_ptr, this]()
                                          {
                                              ref_ptr->SerializeTo(*this);
                                              Pad(16);
                                              RunLocalQueue();
                                          });
                           RunGlobalQueue();
                       });
    };

    LocalQueue->emplace_back(
        [to_write, fixup_uptr = std::move(global_fixup), this]() mutable
        {
            GlobalFixupList.emplace_back(std::move(fixup_uptr));
            GlobalQueue->emplace_back(to_write);
        });
}

void nemesis::PackfileSerializer::WriteNullPointer()
{
    WriteUSize(0);
}

void nemesis::PackfileSerializer::WriteUSize(uint64_t val)
{
    if (IsUSizeLong())
    {
        WriteValue("", val);
        return;
    }

    WriteValue("", static_cast<unsigned int>(val));
}

void nemesis::PackfileSerializer::WriteArrayValue(const std::string& name,
                                                  const void* (&list)[],
                                                  size_t size,
                                                  size_t type_size)
{
    for (size_t i = 0; i < size; ++i)
    {
        WriteFromPointer(list[i], type_size);
    }
}

void nemesis::PackfileSerializer::WriteArrayValue(const std::string& name,
                                                  const nemesis::hkCString* (&list)[],
                                                  size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::PackfileSerializer::WriteArrayValue(const std::string& name,
                                                  const nemesis::hkStringPtr* (&list)[],
                                                  size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::PackfileSerializer::WriteArrayValue(const std::string& name,
                                                  const nemesis::hkEnumBase* (&list)[],
                                                  size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::PackfileSerializer::WriteArrayValue(const std::string& name,
                                                  const nemesis::hkVector4* (&list)[],
                                                  size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::PackfileSerializer::WriteArrayValue(const std::string& name,
                                                  const nemesis::hkVector8* (&list)[],
                                                  size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::PackfileSerializer::WriteArrayValue(const std::string& name,
                                                  const nemesis::hkQuaternion* (&list)[],
                                                  size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::PackfileSerializer::WriteArrayValue(const std::string& name,
                                                  const nemesis::hkQsTransform* (&list)[],
                                                  size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::PackfileSerializer::WriteArrayValue(const std::string& name,
                                                  const nemesis::hkUFloat8* (&list)[],
                                                  size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::PackfileSerializer::WriteArrayValue(const std::string& name,
                                                  const nemesis::hkLong* (&list)[],
                                                  size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::PackfileSerializer::WriteArrayValue(const std::string& name,
                                                  const nemesis::hkUlong* (&list)[],
                                                  size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::PackfileSerializer::WriteArrayValue(const std::string& name,
                                                  const nemesis::hkHalf* (&list)[],
                                                  size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::PackfileSerializer::WriteArrayValue(const std::string& name,
                                                  const nemesis::hkTransform* (&list)[],
                                                  size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::PackfileSerializer::WriteArrayValue(const std::string& name,
                                                  const nemesis::hkMatrix3* (&list)[],
                                                  size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::PackfileSerializer::WriteArrayValue(const std::string& name,
                                                  const nemesis::hkMatrix4* (&list)[],
                                                  size_t size)
{
    WriteArrayValueImplt(name, list, size);
}

void nemesis::PackfileSerializer::WriteArrayObject(const std::string& name,
                                                   const nemesis::HavokObject* (&list)[],
                                                   size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        WriteObject(name, *list[i]);
    }
}
