#pragma once

#include <optional>
#include <type_traits>

#if defined(__GNUC__) || (defined(__clang__) && !defined(_MSC_VER))
#include <cxxabi.h>
#endif

#include "Serialize/Deserializer.h"

#include "Utilities/OnScopeEnds.h"
#include "Utilities/VectorizedMap.h"

namespace nemesis
{
    struct hkbBehaviorGraph;
    struct hkbBehaviorGraphStringData;

    struct XmlDeserializer : Deserializer
    {
    private:
        struct XmlElementData
        {
            std::string TagName;
            nemesis::VectorizedMap<std::string, std::string> Attributes;

            bool SelfClosing = false;
            bool Closing     = false;
        };

        struct StreamBlock
        {
            std::string Text;
            std::istringstream Stream;

            StreamBlock(const std::string& text);
            StreamBlock(const std::string& text, const std::string& stream_text);
        };

        nemesis::hkPackfile* PackfilePtr    = nullptr;
        nemesis::hkRootLevelContainer* Root = nullptr;
        std::string TopLevelObject;
        size_t CurrentLine = 1;

        UPtr<nemesis::XmlDeserializer::XmlElementData> CurrentElement;
        UPtr<nemesis::XmlDeserializer::StreamBlock> CurrentBlock
            = std::make_unique<nemesis::XmlDeserializer::StreamBlock>("");

        Vec<Pair<std::string, const nemesis::hkbBehaviorGraph*>> BehaviorGraphList;  
        UMap<std::string, nemesis::HavokObject*> HkxObjectMap;
        Vec<std::function<void()>> GlobalQueue;

        UPtr<VecStr> LocalReferences;
        UPtr<Vec<std::function<void(const nemesis::hkbBehaviorGraphStringData&)>>> LocalValidation;
        UMap<std::string, std::move_only_function<void(const nemesis::hkbBehaviorGraphStringData&)>>
            ReferenceValidation;
        USetStr ValidatedReference;

        std::string GetXmlInnerText(bool decode_xml = true);

        template <typename T>
        T GetXmlValue(bool decode_xml = true)
        {
            if constexpr (std::is_same_v<std::string, T>)
            {
                return GetXmlInnerText(decode_xml);
            }
            else
            {
                T val;
                std::istringstream iss(GetXmlInnerText(decode_xml));

                if constexpr (std::is_same_v<bool, T>)
                {
                    iss >> std::boolalpha;
                }

                iss >> val;

                if (iss.fail())
                    throw std::runtime_error("Invalid input type (Value: " + iss.str()
                                             + ", Line: " + std::to_string(CurrentLine) + ")");

                return val;
            }
        }

        static nemesis::VectorizedMap<std::string, std::string> GetXmlAttributes(const std::string& xml_node);
        static std::string TrimXmlString(const std::string& xml_str);
        static std::string DecodeXmlValue(const std::string& val);

        UPtr<nemesis::XmlDeserializer::XmlElementData> CreateXmlTag();
        UPtr<nemesis::XmlDeserializer::XmlElementData> CreateXmlTag(const std::string& name);
        nemesis::XmlDeserializer::XmlElementData& OpenXmlTag(const std::string& name);
        nemesis::XmlDeserializer::XmlElementData& OpenXmlTag(const std::string& name,
                                                             std::initializer_list<std::string> attrs);
        nemesis::XmlDeserializer::XmlElementData&
        OpenXmlTag(const std::string& name, std::initializer_list<Pair<std::string, std::string>> attrs);
        nemesis::XmlDeserializer::XmlElementData& CloseXmlTag(const std::string& name);

        void ReadXmlDeclaration();

        template <typename T>
        bool TryReadHkxParam(const std::string& name, T& val, bool decode_xml = true)
        {
            if (!CurrentElement)
            {
                CurrentElement = CreateXmlTag();
            }

            if (CurrentElement->TagName != "hkparam") return false;

            std::string field;

            if (!CurrentElement->Attributes.try_at("name", field) || field != name) return false;

            T rst = GetXmlValue<T>(decode_xml);
            nemesis::OnScopeEnds on_ends([this]() { CurrentElement = nullptr; });

            if (!CurrentElement->Closing)
            {
                auto close_ele = CreateXmlTag("hkparam");

                if (!close_ele->Closing)
                {
                    throw std::runtime_error("Invalid xml structure: missing closing tag (Line: "
                                             + std::to_string(CurrentLine) + ")");
                }
            }

            val = rst;
            return true;
        }

        template <typename T>
        T& ReadComplexContainer(const std::string& name, T& container)
        {
            if (name.empty()) return ReadValue(*CurrentBlock, container);

            std::string str;

            if (!TryReadHkxParam(name, str)) return container;

            std::string new_str = TrimXmlString(str);
            new_str.erase(std::remove_if(
                              new_str.begin(), new_str.end(), [](char ch) { return ch == '(' || ch == ')'; }),
                          new_str.end());

            nemesis::XmlDeserializer::StreamBlock stream_block(str, new_str);
            return ReadValue(stream_block, container);
        }

        void ParseHkxObjectMap();
        void ValidateStringData();
        void RunGlobalQueue();

        static std::string GetDemangledTypeName(const char* mangled_name)
        {
#if defined(__GNUC__) || (defined(__clang__) && !defined(_MSC_VER))
            int status;
            char* demangled_name = abi::__cxa_demangle(mangled_name, nullptr, nullptr, &status);

            if (status == 0)
            {
                std::string result(demangled_name);
                std::free(demangled_name);
                return result;
            }
            else
            {
                return mangled_name;
            }
#else
            return mangled_name;
#endif
        }

        // Template function to get the type name, enabled only for fundamental types
        template <typename T, std::enable_if_t<std::is_fundamental_v<T>, int> = 0>
        static std::string GetPrimitiveTypeName()
        {
            return GetDemangledTypeName(typeid(T).name());
        }

        template <typename T, std::enable_if_t<std::is_fundamental_v<T>, int> = 0>
        T& ReadValue(nemesis::XmlDeserializer::StreamBlock& stream_block, T& val)
        {
            stream_block.Stream >> val;

            if (!stream_block.Stream.fail()) return val;

            throw std::runtime_error("Parsing error: unable to parse string value to "
                                     + GetPrimitiveTypeName<T>() + " (Value: " + stream_block.Text
                                     + ", Line: " + std::to_string(CurrentLine) + ")");
        }

    protected:
        bool CanAssert() const noexcept override;

    public:
        void LoadFile(const std::filesystem::path& filepath);
        void LoadXml(const std::string& xml);

        nemesis::hkPackfile Deserialize() override;
        void Deserialize(nemesis::hkPackfileSection& section_data) override;
        void Deserialize(nemesis::hkRootLevelContainer& root) override;

        std::streampos GetCurrentPosition() const override;
        void GoToPosition(std::streampos address) override;

        void Pad(unsigned int size) override;
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
        ReadValue(const std::string& name, std::string& ref_str, unsigned int size, char pad = 0x00) override;

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
        nemesis::hkVector4& ReadValue(nemesis::XmlDeserializer::StreamBlock& stream_block,
                                      nemesis::hkVector4& vec4);
        nemesis::hkVector4& ReadValue(const std::string& name, nemesis::hkVector4& vec4) override;
        nemesis::hkVector8& ReadValue(nemesis::XmlDeserializer::StreamBlock& stream_block,
                                      nemesis::hkVector8& vec8);
        nemesis::hkVector8& ReadValue(const std::string& name, nemesis::hkVector8& vec8) override;
        nemesis::hkQuaternion& ReadValue(nemesis::XmlDeserializer::StreamBlock& stream_block,
                                         nemesis::hkQuaternion& quaternion);
        nemesis::hkQuaternion& ReadValue(const std::string& name, nemesis::hkQuaternion& quaternion) override;
        nemesis::hkQsTransform& ReadValue(nemesis::XmlDeserializer::StreamBlock& stream_block,
                                          nemesis::hkQsTransform& qs_transform);
        nemesis::hkQsTransform& ReadValue(const std::string& name,
                                          nemesis::hkQsTransform& qs_transform) override;
        nemesis::hkUFloat8& ReadValue(const std::string& name, nemesis::hkUFloat8& ufloat8) override;
        nemesis::hkLong& ReadValue(const std::string& name, nemesis::hkLong& _long) override;
        nemesis::hkUlong& ReadValue(const std::string& name, nemesis::hkUlong& ulong) override;
        nemesis::hkHalf& ReadValue(const std::string& name, nemesis::hkHalf& half) override;
        nemesis::hkTransform& ReadValue(nemesis::XmlDeserializer::StreamBlock& stream_block,
                                        nemesis::hkTransform& transform);
        nemesis::hkTransform& ReadValue(const std::string& name, nemesis::hkTransform& transform) override;
        nemesis::hkMatrix3& ReadValue(nemesis::XmlDeserializer::StreamBlock& stream_block,
                                      nemesis::hkMatrix3& matrix3);
        nemesis::hkMatrix3& ReadValue(const std::string& name, nemesis::hkMatrix3& matrix3) override;
        nemesis::hkMatrix4& ReadValue(nemesis::XmlDeserializer::StreamBlock& stream_block,
                                      nemesis::hkMatrix4& matrix4);
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

        nemesis::HavokObject**
        ReadArrayObject(const std::string& name, nemesis::HavokObject* (&list)[], size_t size) override;

        nemesis::hkSmallArrayBase& ReadArrayObject(const std::string& name,
                                                   nemesis::hkSmallArrayBase& array) override;
        nemesis::hkArrayBase& ReadArrayObject(const std::string& name, nemesis::hkArrayBase& array) override;
        nemesis::hkRefVariant& ReadRefObject(nemesis::XmlDeserializer::StreamBlock& stream_block,
                                             nemesis::hkRefVariant& ref_obj);
        nemesis::hkRefVariant& ReadRefObject(const std::string& name,
                                             nemesis::hkRefVariant& ref_obj) override;
        nemesis::HavokObject** ReadRefObject(nemesis::XmlDeserializer::StreamBlock& stream_block,
                                             nemesis::HavokObject** ptr_ptr_obj);
        nemesis::HavokObject** ReadRefObject(const std::string& name,
                                             nemesis::HavokObject** ptr_ptr_obj) override;

    public:
        nemesis::hkClass& ReadClass(nemesis::hkClass& cls) override;

        nemesis::HavokObject& ReadObject(const std::string& name, nemesis::HavokObject& hk_obj) override;

        std::string ReadString(nemesis::XmlDeserializer::StreamBlock& stream_block);
        std::string ReadString(const std::string& name) override;
        std::string ReadString(const std::string& name, unsigned int size, char pad = 0x00) override;
        nemesis::hkCString ReadCString(const std::string& name) override;
        nemesis::hkStringPtr ReadStringPointer(const std::string& name) override;
    };
}
