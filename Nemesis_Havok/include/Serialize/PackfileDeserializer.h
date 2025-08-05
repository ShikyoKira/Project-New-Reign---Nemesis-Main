#pragma once

#include <filesystem>

#include "Serialize/Deserializer.h"
#include "Serialize/Fixup.h"

#include "Havok/hkPackfile.h"

#include "Utilities/Binary.h"
#include "Utilities/OnScopeEnds.h"

namespace nemesis
{
    struct PackfileDeserializer : Deserializer
    {
    private:
        Deq<nemesis::LocalFixup> LocalFixups;
        Deq<nemesis::GlobalFixup> GlobalFixups;
        Deq<nemesis::VirtualFixup> VirtualFixups;

        UMap<size_t, nemesis::LocalFixup*> LocalMap;
        UMap<size_t, nemesis::GlobalFixup*> GlobalMap;
        UMap<size_t, nemesis::VirtualFixup*> VirtualMap;

        UMap<size_t, const nemesis::hkClass*> ClassMap;
        UMap<size_t, nemesis::HavokObject*> ObjectMap;
        Map<size_t, Vec<std::function<void()>>> GlobalQueue;
        UPtr<Vec<std::function<void()>>> LocalQueue = std::make_unique<Vec<std::function<void()>>>();

        nemesis::hkPackfile* PackfilePtr = nullptr;

        unsigned int DataAddress;

        bool IsUSizeLong() const noexcept;
        void TryPadToPointerSize();
        void ConsumePointer();

        template <typename T, typename UInt>
        inline void ReverseReadValueFromBinaryImpl(T& val)
        {
            UInt rb;
            Stream->read(reinterpret_cast<char*>(&rb), sizeof(rb));
            rb = BinaryReverse(rb);

            union
            {
                UInt* b;
                T* v;
            } b2v;

            b2v.b = &rb;
            val   = *b2v.v;
        }

        template <typename T>
        inline void ReverseReadValueFromBinary(T& val)
        {
            if constexpr (sizeof(T) == 1)
            {
                Stream->read(reinterpret_cast<char*>(&val), sizeof(val));
            }
            else if constexpr (sizeof(T) == 2)
            {
                ReverseReadValueFromBinaryImpl<T, uint16_t>(val);
            }
            else if constexpr (sizeof(T) == 4)
            {
                ReverseReadValueFromBinaryImpl<T, uint32_t>(val);
            }
            else if constexpr (sizeof(T) == 8)
            {
                ReverseReadValueFromBinaryImpl<T, uint64_t>(val);
            }
        }

        template <typename T>
        inline void ReadValueFromBinary(T& val)
        {
            static_assert(std::is_arithmetic_v<T> || std::is_same_v<Float16, T>,
                          "Only primitive numeric types are allowed");

            if (Endian != 0)
            {
                Stream->read(reinterpret_cast<char*>(&val), sizeof(T));
                return;
            }

            ReverseReadValueFromBinary(val);
        }

        const nemesis::hkClass* GetClassFromAddress(size_t address);
        size_t GetTargetAddress(size_t address);

        void Deserialize(nemesis::hkPackfileSectionHeader& header);

        void RunLocalQueue();
        void RunGlobalQueue();

        template <typename Func>
        static void RunScopedQueue(UPtr<Vec<std::function<void()>>>& queue, Func func)
        {
            auto* old_queue = queue.release();
            nemesis::OnScopeEnds on_ends([old_queue, &queue]() { queue.reset(old_queue); });
            queue = std::make_unique<Vec<std::function<void()>>>();
            func();
        }

    protected:
        bool CanAssert() const noexcept override;

    public:
        void LoadFile(const std::filesystem::path& filepath);
        void LoadData(const Vec<unsigned char>& data);
        void LoadData(const std::string& data);

        nemesis::hkPackfile Deserialize() override;
        void Deserialize(nemesis::hkPackfileSection& section_data) override;
        void Deserialize(nemesis::hkRootLevelContainer& root) override;
        const nemesis::hkClass* DeserializeClass();

        std::streampos GetCurrentPosition() const override;
        void GoToPosition(std::streampos address) override;

        void Pad(unsigned int size) override;
        void PadNode() override;
        void Skip(unsigned int step) override;

        bool& ReadValue(const std::string& name, bool& val) override;
        char& ReadValue(const std::string& name, char& ch) override;
        unsigned char& ReadValue(const std::string& name, unsigned char& ch) override;
        short& ReadValue(const std::string& name, short& byte) override;
        unsigned short& ReadValue(const std::string& name, unsigned short& byte) override;
        int& ReadValue(const std::string& name, int& val) override;
        unsigned int& ReadValue(const std::string& name, unsigned int& val) override;
        long& ReadValue(const std::string& name, long& val) override;
        unsigned long& ReadValue(const std::string& name, unsigned long& val) override;
        long long& ReadValue(const std::string& name, long long& val) override;
        unsigned long long& ReadValue(const std::string& name, unsigned long long& val) override;
        Float16& ReadValue(const std::string& name, Float16& val) override;
        float& ReadValue(const std::string& name, float& val) override;
        double& ReadValue(const std::string& name, double& val) override;

        std::string& ReadValue(const std::string& name, std::string& ref_str) override;
        std::string&
        ReadValue(const std::string& name, std::string& ref_str, unsigned int size, char pad = 0x00)
            override;

        nemesis::hkEventId& ReadValue(const std::string& name, nemesis::hkEventId& event_id) override;
        nemesis::hkAttributeId& ReadValue(const std::string& name, nemesis::hkAttributeId& attr_id) override;
        nemesis::hkVariableId& ReadValue(const std::string& name, nemesis::hkVariableId& var_id) override;
        nemesis::hkCharacterPropertyId& ReadValue(const std::string& name,
                                                  nemesis::hkCharacterPropertyId& prop_id) override;

        nemesis::hkCString& ReadValue(const std::string& name, nemesis::hkCString& cstring) override;
        nemesis::hkCString&
        ReadValue(const std::string& name, nemesis::hkCString& cstring, unsigned int padding_size) override;
        nemesis::hkStringPtr& ReadValue(const std::string& name, nemesis::hkStringPtr& string_ptr) override;
        nemesis::hkStringPtr& ReadValue(const std::string& name,
                                        nemesis::hkStringPtr& string_ptr,
                                        unsigned int padding_size) override;
        nemesis::hkEnumBase& ReadValue(const std::string& name, nemesis::hkEnumBase& val) override;
        nemesis::hkVector4& ReadValue(const std::string& name, nemesis::hkVector4& vec4) override;
        nemesis::hkVector8& ReadValue(const std::string& name, nemesis::hkVector8& vec8) override;
        nemesis::hkQuaternion& ReadValue(const std::string& name, nemesis::hkQuaternion& quaternion) override;
        nemesis::hkQsTransform& ReadValue(const std::string& name,
                                          nemesis::hkQsTransform& qs_transform) override;
        nemesis::hkUFloat8& ReadValue(const std::string& name, nemesis::hkUFloat8& ufloat8) override;
        nemesis::hkLong& ReadValue(const std::string& name, nemesis::hkLong& _long) override;
        nemesis::hkUlong& ReadValue(const std::string& name, nemesis::hkUlong& ulong) override;
        nemesis::hkHalf& ReadValue(const std::string& name, nemesis::hkHalf& half) override;
        nemesis::hkTransform& ReadValue(const std::string& name, nemesis::hkTransform& transform) override;
        nemesis::hkMatrix3& ReadValue(const std::string& name, nemesis::hkMatrix3& matrix3) override;
        nemesis::hkMatrix4& ReadValue(const std::string& name, nemesis::hkMatrix4& matrix4) override;

    protected:
        void** ReadArrayValue(const std::string& name, void* list[], size_t size, size_t type_size) override;
        nemesis::hkCString**
        ReadArrayValue(const std::string& name, nemesis::hkCString* (&list)[], size_t size) override;
        nemesis::hkStringPtr**
        ReadArrayValue(const std::string& name, nemesis::hkStringPtr* (&list)[], size_t size) override;
        nemesis::hkEnumBase**
        ReadArrayValue(const std::string& name, nemesis::hkEnumBase* (&list)[], size_t size) override;
        nemesis::hkVector4**
        ReadArrayValue(const std::string& name, nemesis::hkVector4* (&list)[], size_t size) override;
        nemesis::hkVector8**
        ReadArrayValue(const std::string& name, nemesis::hkVector8* (&list)[], size_t size) override;
        nemesis::hkQuaternion**
        ReadArrayValue(const std::string& name, nemesis::hkQuaternion* (&list)[], size_t size) override;
        nemesis::hkQsTransform**
        ReadArrayValue(const std::string& name, nemesis::hkQsTransform* (&list)[], size_t size) override;
        nemesis::hkUFloat8**
        ReadArrayValue(const std::string& name, nemesis::hkUFloat8* (&list)[], size_t size) override;
        nemesis::hkLong**
        ReadArrayValue(const std::string& name, nemesis::hkLong* (&list)[], size_t size) override;
        nemesis::hkUlong**
        ReadArrayValue(const std::string& name, nemesis::hkUlong* (&list)[], size_t size) override;
        nemesis::hkHalf**
        ReadArrayValue(const std::string& name, nemesis::hkHalf* (&list)[], size_t size) override;
        nemesis::hkTransform**
        ReadArrayValue(const std::string& name, nemesis::hkTransform* (&list)[], size_t size) override;
        nemesis::hkMatrix3**
        ReadArrayValue(const std::string& name, nemesis::hkMatrix3* (&list)[], size_t size) override;
        nemesis::hkMatrix4**
        ReadArrayValue(const std::string& name, nemesis::hkMatrix4* (&list)[], size_t size) override;

        template <typename T>
        T** ReadArrayValueImplt(const std::string& name, T* (&list)[], size_t size)
        {
            for (size_t i = 0; i < size; ++i)
            {
                ReadValue(name, *list[i]);
            }

            return list;
        }

        nemesis::HavokObject**
        ReadArrayObject(const std::string& name, nemesis::HavokObject* (&list)[], size_t size) override;

        nemesis::hkSmallArrayBase& ReadArrayObject(const std::string& name,
                                                   nemesis::hkSmallArrayBase& array) override;
        nemesis::hkArrayBase& ReadArrayObject(const std::string& name, nemesis::hkArrayBase& array) override;
        nemesis::hkRefVariant& ReadRefObject(const std::string& name,
                                             nemesis::hkRefVariant& ref_obj) override;
        nemesis::HavokObject** ReadRefObject(const std::string& name,
                                             nemesis::HavokObject** ptr_ptr_obj) override;

    public:
        nemesis::hkClass& ReadClass(nemesis::hkClass& cls) override;

        nemesis::HavokObject& ReadObject(const std::string& name, nemesis::HavokObject& hk_obj) override;

        unsigned int ReadArraySize(const std::string& name);
        unsigned short ReadSmallArraySize(const std::string& name);

        std::string ReadString(const std::string& name) override;
        std::string ReadString(const std::string& name, unsigned int size, char pad = 0x00) override;
        nemesis::hkCString ReadCString(const std::string& name) override;
        nemesis::hkStringPtr ReadStringPointer(const std::string& name) override;
        
        uint64_t ReadUSize() noexcept override;
        void ReadFileHeader(nemesis::hkPackfileHeader& header) override;

        std::string AssertString(std::initializer_list<std::string> options) override;
        std::string AssertString(unsigned int size, std::initializer_list<std::string> options) override;
        uint64_t AssertUSize(std::initializer_list<uint64_t> options) override;
    };
}
