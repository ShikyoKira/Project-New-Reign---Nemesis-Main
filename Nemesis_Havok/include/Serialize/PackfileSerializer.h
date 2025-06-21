#pragma once

#include <stdfloat>
#include <filesystem>
#include <sstream>

#include "Havok/hkPackfileHeader.h"

#include "Serialize/Serializer.h"
#include "Serialize/Fixup.h"

#include "Utilities/Binary.h"
#include "Utilities/OnScopeEnds.h"

namespace nemesis
{
    struct hkArrayBase;
    struct hkPackfileSectionHeader;

    struct PackfileSerializer : nemesis::Serializer
    {
    private:
        Vec<UPtr<nemesis::GlobalFixup>> GlobalFixupList;

        nemesis::hkPackfileHeader Header;
        Vec<UPtr<nemesis::hkPackfileSectionHeader>> SectionHeaders;

        struct StreamSection
        {
            std::ostringstream HeaderStream, DataStream, LocalStream, GlobalStream, VirtualStream;
        };

        std::ostringstream MainStream;
        Vec<StreamSection> StreamSections;

        UMap<const nemesis::hkClass*, unsigned int> ClassMap;
        UMap<const nemesis::HavokObject*, unsigned int> GlobalMap;

        UPtr<Vec<std::move_only_function<void()>>> GlobalQueue
            = std::make_unique<Vec<std::move_only_function<void()>>>();
        UPtr<Vec<std::move_only_function<void()>>> LocalQueue
            = std::make_unique<Vec<std::move_only_function<void()>>>();

        nemesis::PlatformType PltfrmType;

        inline bool IsUSizeLong() const noexcept
        {
            return PointerSize == 8;
        }

        template <typename UInt>
        inline void ReserveWriteImpl(const void* val)
        {
            UInt rb;
            std::memcpy(&rb, val, sizeof(UInt));
            rb = BinaryReverse(rb);
            Stream->write(reinterpret_cast<const char*>(&rb), sizeof(UInt));
        }

        template <typename T>
        inline void ReverseWrite(T val)
        {
            if constexpr (sizeof(T) == 1)
            {
                Stream->write(reinterpret_cast<const char*>(&val), sizeof(T));
            }
            else if constexpr (sizeof(T) == 2)
            {
                ReserveWriteImpl<uint16_t>(&val);
            }
            else if constexpr (sizeof(T) == 4)
            {
                ReserveWriteImpl<uint32_t>(&val);
            }
            else if constexpr (sizeof(T) == 8)
            {
                ReserveWriteImpl<uint64_t>(&val);
            }
            else
            {
                throw std::runtime_error("Unsupported data type");
            }
        }

        template <size_t Size>
        inline void ReverseWrite(const void* val)
        {
            if constexpr (Size == 1)
            {
                Stream->write(reinterpret_cast<const char*>(val), Size);
            }
            else if constexpr (Size == 2)
            {
                ReserveWriteImpl<uint16_t>(val);
            }
            else if constexpr (Size == 4)
            {
                ReserveWriteImpl<uint32_t>(val);
            }
            else if constexpr (Size == 8)
            {
                ReserveWriteImpl<uint64_t>(val);
            }
            else
            {
                throw std::runtime_error("Unsupported data size");
            }
        }

        template <typename T>
        inline void WriteValue(T val)
        {
            static_assert(std::is_arithmetic_v<T> || std::is_same_v<Float16, T>,
                          "Only primitive numeric types are allowed");

            switch (PltfrmType)
            {
                case nemesis::PlatformType::PS3:
                case nemesis::PlatformType::XB360:
                    ReverseWrite(val);
                    break;
                default:
                    Stream->write(reinterpret_cast<const char*>(&val), sizeof(T));
                    break;
            }
        }

        template <size_t Size>
        inline void WriteFromPointer(const void* val)
        {
            switch (PltfrmType)
            {
                case nemesis::PlatformType::PS3:
                case nemesis::PlatformType::XB360:
                    ReverseWrite<Size>(val);
                    break;
                default:
                    Stream->write(reinterpret_cast<const char*>(val), Size);
                    break;
            }
        }

        inline void WriteFromPointer(const void* val, size_t size)
        {
            switch (size)
            {
                case 1:
                    WriteFromPointer<1>(val);
                    break;
                case 2:
                    WriteFromPointer<2>(val);
                    break;
                case 4:
                    WriteFromPointer<4>(val);
                    break;
                case 8:
                    WriteFromPointer<8>(val);
                    break;
                default:
                    throw std::runtime_error("Unsupported data size");
            }
        }

        static void PadStream(std::ostream& stream, int pad_align_to);

        void RunLocalQueue();
        void RunGlobalQueue();

        unsigned int TryClass(const nemesis::hkClass* cls);
        void WriteVirtualFixup(unsigned int body_addr, unsigned int cls_addr);
        void FlushGlobalFixupList();
        void FlushSectionHeaderList();

        template <typename Func>
        void WriteToStream(std::ostream& stream, Func func)
        {
            auto old_stream = Stream;
            nemesis::OnScopeEnds on_ends([old_stream, this]() { Stream = old_stream; });
            Stream = &stream;
            func();
        }

        template <typename Func>
        static void RunScopedQueue(UPtr<Vec<std::move_only_function<void()>>>& queue, Func func)
        {
            auto* old_queue = queue.release();
            nemesis::OnScopeEnds on_ends([old_queue, &queue]() { queue.reset(old_queue); });
            queue = std::make_unique<Vec<std::move_only_function<void()>>>();
            func();
        }

    public:
        PackfileSerializer(nemesis::PlatformType platform, nemesis::HavokVersion version);

        void WriteFileHeader() override;

        void Serialize(const nemesis::hkPackfile& packfile) override;
        void Serialize(const nemesis::hkPackfileSection& section_data) override;
        void Serialize(const nemesis::hkRootLevelContainer& root) override;

        void Save(const std::filesystem::path& filepath) override;

        int GetCurrentPosition() const override;
        void Pad(unsigned int size) override;
        void Pad(unsigned int size, unsigned char pad_ch) override;
        void PadNode() override;
        void TryPadToPointerSize() override;

        void Skip(unsigned int step) override;

        void WriteValue(const std::string& name, bool val) override;
        void WriteValue(const std::string& name, char val) override;
        void WriteValue(const std::string& name, unsigned char val) override;
        void WriteValue(const std::string& name, short val) override;
        void WriteValue(const std::string& name, unsigned short val) override;
        void WriteValue(const std::string& name, int val) override;
        void WriteValue(const std::string& name, unsigned int val) override;
        void WriteValue(const std::string& name, long val) override;
        void WriteValue(const std::string& name, unsigned long val) override;
        void WriteValue(const std::string& name, long long val) override;
        void WriteValue(const std::string& name, unsigned long long val) override;
        void WriteValue(const std::string& name, Float16 val) override;
        void WriteValue(const std::string& name, float val) override;
        void WriteValue(const std::string& name, double val) override;

        void WriteValue(const std::string& name, const std::string& val, bool terminate = false) override;
        void WriteValue(const std::string& name,
                        const std::string& val,
                        std::size_t size,
                        char padding = '\0') override;

        void WriteValue(const std::string& name, const nemesis::hkEventId& event_id) override;
        void WriteValue(const std::string& name, const nemesis::hkAttributeId& attr_id) override;
        void WriteValue(const std::string& name, const nemesis::hkVariableId& var_id) override;
        void WriteValue(const std::string& name, const nemesis::hkCharacterPropertyId& prop_id) override;

        void WriteValue(const std::string& name, const nemesis::hkCString& cstring) override;
        void WriteValue(const std::string& name,
                        const nemesis::hkCString& cstring,
                        unsigned int padding_size) override;
        void WriteValue(const std::string& name, const nemesis::hkStringPtr& string_ptr) override;
        void WriteValue(const std::string& name,
                        const nemesis::hkStringPtr& string_ptr,
                        unsigned int padding_size) override;
        void WriteValue(const std::string& name, const nemesis::hkEnumBase& val) override;
        void WriteValue(const std::string& name, const nemesis::hkVector4& vec4) override;
        void WriteValue(const std::string& name, const nemesis::hkVector8& vec8) override;
        void WriteValue(const std::string& name, const nemesis::hkQuaternion& quaternion) override;
        void WriteValue(const std::string& name, const nemesis::hkQsTransform& qs_transform) override;
        void WriteValue(const std::string& name, const nemesis::hkUFloat8& ufloat8) override;
        void WriteValue(const std::string& name, const nemesis::hkLong& _long) override;
        void WriteValue(const std::string& name, const nemesis::hkUlong& ulong) override;
        void WriteValue(const std::string& name, const nemesis::hkHalf& half) override;
        void WriteValue(const std::string& name, const nemesis::hkTransform& transform) override;
        void WriteValue(const std::string& name, const nemesis::hkMatrix3& matrix3) override;
        void WriteValue(const std::string& name, const nemesis::hkMatrix4& matrix4) override;

        void WriteObject(const std::string& name, const nemesis::hkSmallArrayBase& array) override;
        void WriteObject(const std::string& name, const nemesis::hkArrayBase& array) override;
        void WriteObject(const std::string& name, const nemesis::hkRefVariant& ref_obj) override;
        void WriteObject(const std::string& name, const nemesis::HavokObject& havok_obj) override;
        void WriteObject(const std::string& name, const nemesis::HavokObject* const* ptr_obj_ptr) override;

        void WriteNullPointer() override;

        void WriteUSize(uint64_t val) override;

    protected:
        void WriteArrayValue(const std::string& name,
                             const void* (&list)[],
                             size_t size,
                             size_t type_size) override;
        void
        WriteArrayValue(const std::string& name, const nemesis::hkCString* (&list)[], size_t size) override;
        void
        WriteArrayValue(const std::string& name, const nemesis::hkStringPtr* (&list)[], size_t size) override;
        void
        WriteArrayValue(const std::string& name, const nemesis::hkEnumBase* (&list)[], size_t size) override;
        void
        WriteArrayValue(const std::string& name, const nemesis::hkVector4* (&list)[], size_t size) override;
        void
        WriteArrayValue(const std::string& name, const nemesis::hkVector8* (&list)[], size_t size) override;
        void WriteArrayValue(const std::string& name,
                             const nemesis::hkQuaternion* (&list)[],
                             size_t size) override;
        void WriteArrayValue(const std::string& name,
                             const nemesis::hkQsTransform* (&list)[],
                             size_t size) override;
        void
        WriteArrayValue(const std::string& name, const nemesis::hkUFloat8* (&list)[], size_t size) override;
        void WriteArrayValue(const std::string& name, const nemesis::hkLong* (&list)[], size_t size) override;
        void
        WriteArrayValue(const std::string& name, const nemesis::hkUlong* (&list)[], size_t size) override;
        void WriteArrayValue(const std::string& name, const nemesis::hkHalf* (&list)[], size_t size) override;
        void
        WriteArrayValue(const std::string& name, const nemesis::hkTransform* (&list)[], size_t size) override;
        void
        WriteArrayValue(const std::string& name, const nemesis::hkMatrix3* (&list)[], size_t size) override;
        void
        WriteArrayValue(const std::string& name, const nemesis::hkMatrix4* (&list)[], size_t size) override;

        void WriteArrayObject(const std::string& name,
                              const nemesis::HavokObject* (&list)[],
                              size_t size) override;

        template <typename T>
        void WriteArrayValueImplt(const std::string& name, const T* (&list)[], size_t size)
        {
            for (size_t i = 0; i < size; ++i)
            {
                WriteValue(name, *list[i]);
            }
        }
    };
}
