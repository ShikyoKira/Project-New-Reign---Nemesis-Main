#pragma once

#include "Havok/hkPackfileHeader.h"

#include "Serialize/Serializer.h"

#include "Utilities/OnScopeEnds.h"
#include "Utilities/VectorizedMap.h"

namespace nemesis
{
    struct hkbBehaviorGraphStringData;

    struct XmlSerializer : nemesis::Serializer
    {
    private:
        unsigned int Indent    = 0;
        unsigned int IdCounter = 50;
        UMap<const nemesis::HavokObject*, std::string> ObjectIdMap;
        USet<const nemesis::HavokObject*> CompletedObject;

        nemesis::hkPackfileHeader Header;
        const nemesis::hkbBehaviorGraphStringData* CurrentStringData = nullptr;

        UPtr<Deq<std::function<void()>>> LocalQueue = std::make_unique<Deq<std::function<void()>>>();
        nemesis::VectorizedMap<const nemesis::HavokObject*, std::function<void()>> NodeQueue;
        std::ostringstream XmlStream;

        template <typename Container>
        static std::string StringJoin(const std::string& separator, const Container& items)
        {
            if (items.empty()) return "";

            std::string result;
            size_t total_length = 0;

            for (const auto& item : items)
            {
                total_length += item.length();
            }

            total_length += (items.size() > 1 ? (items.size() - 1) * separator.length() : 0);
            result.reserve(total_length);

            auto it = items.begin();
            result += *it;
            ++it;

            for (; it != items.end(); ++it)
            {
                result += separator;
                result += *it;
            }

            return result;
        }

        template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
        static std::string ToString(T val, size_t precision)
        {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(precision) << val;
            return oss.str();
        }

        static std::string ToString(const nemesis::hkVector4 vec4, bool skip_last = false);
        static std::string ToString(const nemesis::hkQuaternion quaternion);
        static std::string ToString(const nemesis::hkMatrix3 matrix3);
        static std::string ToString(const nemesis::hkMatrix4 matrix4);
        static std::string ToString(const nemesis::hkTransform transform);

        static std::string EncodeXmlValue(const std::string& val);

        std::string GetId(const nemesis::HavokObject* hkx_ptr);

        void WriteXmlDeclaration();
        void WriteHkxParam(const std::string& name, const std::string& val, bool encode_xml = true);

        void WriteSerializeIgnoredXml(const std::string& name);
        void WriteXmlElement(const std::string& name);
        void WriteXmlElement(const std::string& name, const std::string& val);

        template <size_t N>
        void WriteXmlElement(const std::string& name,
                             std::array<Pair<std::string, std::string>, N> xml_attributes)
        {
            if constexpr (N == 0)
            {
                WriteXmlElement(name);
                return;
            }

            (*Stream) << std::string(Indent, '\t') << "<" << name << " ";

            VecStr joined_xml_attr_list;

            for (const Pair<std::string, std::string>& xml_attr : xml_attributes)
            {
                joined_xml_attr_list.emplace_back(xml_attr.first + "=\"" + xml_attr.second + "\"");
            }

            (*Stream) << StringJoin(" ", joined_xml_attr_list) << "/>\n";
        }

        template <size_t N>
        void WriteXmlElement(const std::string& name,
                             std::array<Pair<std::string, std::string>, N> xml_attributes,
                             const std::string& val,
                             bool encode_xml = true)
        {
            if constexpr (N == 0)
            {
                WriteXmlElement(name, val);
                return;
            }

            (*Stream) << std::string(Indent, '\t') << "<" << name << " ";

            VecStr joined_xml_attr_list;

            for (const Pair<std::string, std::string>& xml_attr : xml_attributes)
            {
                joined_xml_attr_list.emplace_back(xml_attr.first + "=\"" + xml_attr.second + "\"");
            }

            (*Stream) << StringJoin(" ", joined_xml_attr_list) << ">"
                      << (encode_xml ? EncodeXmlValue(val) : val) << "</" << name << ">\n";
        }

        void OpenXmlTag(const std::string& name);

        template <size_t N>
        void OpenXmlTag(const std::string& name, std::array<Pair<std::string, std::string>, N> xml_attributes)
        {
            if constexpr (N == 0)
            {
                OpenXmlTag(name);
                return;
            }

            (*Stream) << std::string(Indent++, '\t') << "<" << name << " ";

            VecStr joined_xml_attr_list;

            for (const Pair<std::string, std::string>& xml_attr : xml_attributes)
            {
                joined_xml_attr_list.emplace_back(xml_attr.first + "=\"" + xml_attr.second + "\"");
            }

            (*Stream) << StringJoin(" ", joined_xml_attr_list) << ">\n";
        }

        void CloseXmlTag(const std::string& name);

        void RunNodeQueue();
        void RunLocalQueue();

        template <typename T>
        void WriteArrayObject(const std::string& name, const T& array)
        {
            std::array<Pair<std::string, std::string>, 2> xml_attrs{
                std::make_pair("name", name), std::make_pair("numelements", std::to_string(array.GetSize()))};

            if (array.GetSize() == 0)
            {
                WriteXmlElement("hkparam", xml_attrs, "");
                return;
            }

            OpenXmlTag("hkparam", xml_attrs);
            array.SerializeTo(*this);
            CloseXmlTag("hkparam");
        }

        template <typename Container, typename Func>
        static void RunScopedContainer(UPtr<Container>& container, Func func)
        {
            Container* old_cont = container.release();
            nemesis::OnScopeEnds on_ends([old_cont, &container]() { container.reset(old_cont); });
            container = std::make_unique<Container>();
            func();
        }

        template <typename Container, typename Func>
        static UPtr<Container> RunScopedContainer(Container*& container, Func func)
        {
            return RunScopedContainer<Container, Container>(container, func);
        }

        template <typename NewContainer, typename Container, typename Func>
        static UPtr<NewContainer> RunScopedContainer(Container*& container, Func func)
        {
            Container* old_cont = container;
            nemesis::OnScopeEnds on_ends([old_cont, &container]() { container = old_cont; });
            auto new_cont = std::make_unique<NewContainer>();
            container     = new_cont.get();
            func();
            return new_cont;
        }

        static void StringReplace(std::string& _ch, const std::string& oldvalue, const std::string& newvalue)
        {
            if (oldvalue.empty()) return;

            size_t position = _ch.find(oldvalue);

            while (position != NOT_FOUND)
            {
                _ch.replace(position, oldvalue.length(), newvalue);
                position = _ch.find(oldvalue, position + newvalue.length());
            }
        }

        template <typename T>
        void WriteArrayValueImplt(const std::string& name, const T* (&list)[], size_t size)
        {
            std::ostream* cur_stream = Stream;
            std::stringstream ss;
            Stream = &ss;
            std::string str;

            {
                nemesis::OnScopeEnds on_ends([&, cur_indent = Indent]() { Indent = cur_indent; });
                Indent = 0;

                for (size_t i = 0; i < size; i++)
                {
                    auto& each = *list[i];
                    WriteValue("", each);
                }

                str = ss.str();
                StringReplace(str, "\n", " ");
            }

            while (!str.empty() && str.back() == ' ')
            {
                str.pop_back();
            }

            Stream = cur_stream;
            WriteHkxParam(name, str);
        }

        template <typename T>
        void WriteArrayObjectImplt(const std::string& name, const T* (&list)[], size_t size)
        {
            std::ostream* cur_stream = Stream;
            std::stringstream ss;
            Stream = &ss;
            std::string str;

            {
                nemesis::OnScopeEnds on_ends([&, cur_indent = Indent]() { Indent = cur_indent; });
                Indent = 0;

                for (size_t i = 0; i < size; i++)
                {
                    auto& each = *list[i];
                    WriteObject("", each);
                }

                str = ss.str();
                StringReplace(str, "\n", " ");
            }

            while (!str.empty() && str.back() == ' ')
            {
                str.pop_back();
            }

            Stream = cur_stream;
            WriteHkxParam(name, str);
        }

    public:
        XmlSerializer(nemesis::HavokVersion version);

        void WriteFileHeader() override;

        void Serialize(const nemesis::hkPackfile& packfile) override;
        void Serialize(const nemesis::hkPackfileSection& section_data) override;
        void Serialize(const nemesis::hkRootLevelContainer& root) override;

        void Save(const std::filesystem::path& filepath) override;
        std::string RawData() const override;

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
        void WriteValue(const std::string& name, const nemesis::hkStringPtr& string_ptr) override;
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

        void WriteSerializeIgnoredValue(const std::string& name, bool val) override;
        void WriteSerializeIgnoredValue(const std::string& name, char val) override;
        void WriteSerializeIgnoredValue(const std::string& name, unsigned char val) override;
        void WriteSerializeIgnoredValue(const std::string& name, short val) override;
        void WriteSerializeIgnoredValue(const std::string& name, unsigned short val) override;
        void WriteSerializeIgnoredValue(const std::string& name, int val) override;
        void WriteSerializeIgnoredValue(const std::string& name, unsigned int val) override;
        void WriteSerializeIgnoredValue(const std::string& name, long val) override;
        void WriteSerializeIgnoredValue(const std::string& name, unsigned long val) override;
        void WriteSerializeIgnoredValue(const std::string& name, long long val) override;
        void WriteSerializeIgnoredValue(const std::string& name, unsigned long long val) override;
        void WriteSerializeIgnoredValue(const std::string& name, Float16 val) override;
        void WriteSerializeIgnoredValue(const std::string& name, float val) override;
        void WriteSerializeIgnoredValue(const std::string& name, double val) override;

        void WriteSerializeIgnoredValue(const std::string& name, const nemesis::hkEventId& event_id) override;
        void WriteSerializeIgnoredValue(const std::string& name,
                                        const nemesis::hkAttributeId& attr_id) override;
        void WriteSerializeIgnoredValue(const std::string& name,
                                        const nemesis::hkVariableId& var_id) override;
        void WriteSerializeIgnoredValue(const std::string& name,
                                        const nemesis::hkCharacterPropertyId& prop_id) override;

        void WriteSerializeIgnoredValue(const std::string& name, const nemesis::hkCString& cstring) override;
        void WriteSerializeIgnoredValue(const std::string& name,
                                        const nemesis::hkStringPtr& string_ptr) override;
        void WriteSerializeIgnoredValue(const std::string& name,
                                        const nemesis::hkStringPtr& string_ptr,
                                        unsigned int padding_size) override;
        void WriteSerializeIgnoredValue(const std::string& name, const nemesis::hkEnumBase& val) override;
        void WriteSerializeIgnoredValue(const std::string& name, const nemesis::hkVector4& vec4) override;
        void WriteSerializeIgnoredValue(const std::string& name, const nemesis::hkVector8& vec8) override;
        void WriteSerializeIgnoredValue(const std::string& name,
                                        const nemesis::hkQuaternion& quaternion) override;
        void WriteSerializeIgnoredValue(const std::string& name,
                                        const nemesis::hkQsTransform& qs_transform) override;
        void WriteSerializeIgnoredValue(const std::string& name,
                                        const nemesis::hkTransform& transform) override;
        void WriteSerializeIgnoredValue(const std::string& name, const nemesis::hkUFloat8& ufloat8) override;
        void WriteSerializeIgnoredValue(const std::string& name, const nemesis::hkLong& _long) override;
        void WriteSerializeIgnoredValue(const std::string& name, const nemesis::hkUlong& ulong) override;
        void WriteSerializeIgnoredValue(const std::string& name, const nemesis::hkHalf& half) override;

        void WriteSerializeIgnoredObject(const std::string& name,
                                         const nemesis::hkSmallArrayBase& array) override;
        void WriteSerializeIgnoredObject(const std::string& name, const nemesis::hkArrayBase& array) override;
        void WriteSerializeIgnoredObject(const std::string& name,
                                         const nemesis::hkRefVariant& ref_obj) override;
        void WriteSerializeIgnoredObject(const std::string& name,
                                         const nemesis::HavokObject& havok_obj) override;
        void WriteSerializeIgnoredObject(const std::string& name,
                                         const nemesis::HavokObject* const* ptr_obj_ptr) override;
        void WriteSerializeIgnoredObject(const std::string& name, void* obj_ptr) override;

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

        void WriteArrayRefObject(const std::string& name,
                                 const nemesis::hkRefVariant* (&list)[],
                                 size_t size) override;

        void WriteSerializeIgnoredArrayValue(const std::string& name,
                                             const void* (&list)[],
                                             size_t size,
                                             size_t type_size) override;
        void WriteSerializeIgnoredArrayValue(const std::string& name,
                                             const nemesis::hkCString* (&list)[],
                                             size_t size) override;
        void WriteSerializeIgnoredArrayValue(const std::string& name,
                                             const nemesis::hkStringPtr* (&list)[],
                                             size_t size) override;
        void WriteSerializeIgnoredArrayValue(const std::string& name,
                                             const nemesis::hkEnumBase* (&list)[],
                                             size_t size) override;
        void WriteSerializeIgnoredArrayValue(const std::string& name,
                                             const nemesis::hkVector4* (&list)[],
                                             size_t size) override;
        void WriteSerializeIgnoredArrayValue(const std::string& name,
                                             const nemesis::hkVector8* (&list)[],
                                             size_t size) override;
        void WriteSerializeIgnoredArrayValue(const std::string& name,
                                             const nemesis::hkQuaternion* (&list)[],
                                             size_t size) override;
        void WriteSerializeIgnoredArrayValue(const std::string& name,
                                             const nemesis::hkQsTransform* (&list)[],
                                             size_t size) override;
        void WriteSerializeIgnoredArrayValue(const std::string& name,
                                             const nemesis::hkUFloat8* (&list)[],
                                             size_t size) override;
        void WriteSerializeIgnoredArrayValue(const std::string& name,
                                             const nemesis::hkLong* (&list)[],
                                             size_t size) override;
        void WriteSerializeIgnoredArrayValue(const std::string& name,
                                             const nemesis::hkUlong* (&list)[],
                                             size_t size) override;
        void WriteSerializeIgnoredArrayValue(const std::string& name,
                                             const nemesis::hkHalf* (&list)[],
                                             size_t size) override;
        void WriteSerializeIgnoredArrayValue(const std::string& name,
                                             const nemesis::hkTransform* (&list)[],
                                             size_t size) override;
        void WriteSerializeIgnoredArrayValue(const std::string& name,
                                             const nemesis::hkMatrix3* (&list)[],
                                             size_t size) override;
        void WriteSerializeIgnoredArrayValue(const std::string& name,
                                             const nemesis::hkMatrix4* (&list)[],
                                             size_t size) override;

        void WriteSerializeIgnoreArrayObject(const std::string& name,
                                             const nemesis::HavokObject* (&list)[],
                                             size_t size) override;
    };
}
