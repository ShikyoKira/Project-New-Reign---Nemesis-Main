#include <algorithm>
#include <cctype>
#include <cstring>

#include "Serialize/XmlDeserializer.h"

#include "Havok/hkPackfile.h"
#include "Havok/hkbBehaviorGraph.h"
#include "Havok/hkbBehaviorGraphStringData.h"

#include "Utilities/Algorithm.h"

nemesis::XmlDeserializer::StreamBlock::StreamBlock(const std::string& text)
    : Text(text)
    , Stream(text)
{
}

nemesis::XmlDeserializer::StreamBlock::StreamBlock(const std::string& text, const std::string& stream_text)
    : Text(text)
    , Stream(stream_text)
{
}

std::string nemesis::XmlDeserializer::GetXmlInnerText(bool decode_xml)
{
    std::string content;

    while (!Stream->eof())
    {
        char ch = Stream->get();
        char nx = Stream->peek();

        if (ch == '\n')
        {
            CurrentLine++;
        }

        if (ch == '<')
        {
            if (nx == '!')
            {
                Stream->get();

                if (Stream->eof() || Stream->get() != '-' || Stream->eof() || Stream->get() != '-')
                {
                    throw std::runtime_error("Malformed xml comment");
                }

                std::string comment = "<!--";
                char prev, curr;
                bool valid_comment = false;

                while (Stream->get(curr))
                {
                    if (curr == '\n')
                    {
                        CurrentLine++;
                    }

                    comment.push_back(curr);

                    if (prev == '-' && curr == '-' && !Stream->eof() && Stream->peek() == '>')
                    {
                        valid_comment = true;
                        comment.push_back(Stream->get());
                        break;
                    }

                    prev = curr;
                }

                if (!valid_comment) throw std::runtime_error("Malformed xml comment");

                content.append(comment);
                continue;
            }

            Stream->putback(ch);
            break;
        }

        content.push_back(ch);
    }

    return decode_xml ? DecodeXmlValue(content) : content;
}

nemesis::VectorizedMap<std::string, std::string>
nemesis::XmlDeserializer::GetXmlAttributes(const std::string& attr_segment)
{
    nemesis::VectorizedMap<std::string, std::string> attrs;
    size_t pos = 0;

    while (pos < attr_segment.length())
    {
        pos = attr_segment.find_first_not_of(" \t\n\r\f\v", pos);

        if (pos == NOT_FOUND) break;

        size_t name_start = pos;
        size_t name_end   = attr_segment.find('=', name_start);

        if (name_end == NOT_FOUND)
        {
            throw std::runtime_error("Malformed xml attribute: attribute has no value");
        }

        std::string name   = TrimXmlString(attr_segment.substr(name_start, name_end - name_start));
        size_t value_start = attr_segment.find('"', name_end + 1);

        if (value_start == NOT_FOUND)
        {
            throw std::runtime_error("Malformed xml attribute: missing opening quote for value");
        }

        size_t value_end = attr_segment.find('"', value_start + 1);

        if (value_end == std::string::npos)
        {
            throw std::runtime_error("Malformed xml attribute: missing closing quote");
        }

        std::string value = attr_segment.substr(value_start + 1, value_end - (value_start + 1));
        attrs.emplace_back(name, value);
        pos = value_end + 1;
    }

    return attrs;
}

std::string nemesis::XmlDeserializer::TrimXmlString(const std::string& xml_str)
{
    auto wsfront
        = std::find_if(xml_str.begin(),
                       xml_str.end(),
                       [](unsigned char ch) { return !std::isspace(static_cast<unsigned char>(ch)); });
    auto wsback = std::find_if(xml_str.rbegin(),
                               xml_str.rend(),
                               [](unsigned char ch) { return !std::isspace(static_cast<unsigned char>(ch)); })
                      .base();

    if (wsback <= wsfront) return "";

    return std::string(wsfront, wsback);
}

std::string nemesis::XmlDeserializer::DecodeXmlValue(const std::string& val)
{
    static const UMap<std::string, std::string> xml_entities = {
        {"&amp;", "&"},
        {"&lt;", "<"},
        {"&gt;", ">"},
        {"&quot;", "\""},
        {"&apos;", "'"},
    };

    std::string decoded_text = val;

    for (auto& entity : xml_entities)
    {
        size_t pos = decoded_text.find(entity.first);

        while (pos != NOT_FOUND)
        {
            decoded_text.replace(pos, entity.first.size(), entity.second);
            pos = decoded_text.find(entity.first);
        }
    }

    return decoded_text;
}

UPtr<nemesis::XmlDeserializer::XmlElementData> nemesis::XmlDeserializer::CreateXmlTag()
{
    auto ele_data = std::make_unique<nemesis::XmlDeserializer::XmlElementData>();
    std::string content;
    bool has_opening = false;

    while (!Stream->eof())
    {
        char ch = Stream->get();
        char nx = Stream->peek();

        if (ch == '\n')
        {
            CurrentLine++;
        }

        if (ch == '<' && nx == '!')
        {
            Stream->get();

            if (Stream->eof() || Stream->get() != '-' || Stream->eof() || Stream->get() != '-')
            {
                throw std::runtime_error("Malformed xml comment (Line: " + std::to_string(CurrentLine) + ")");
            }

            char prev, curr;
            bool valid_comment = false;

            while (Stream->get(curr))
            {
                if (curr == '\n')
                {
                    CurrentLine++;
                }

                if (prev == '-' && curr == '-' && !Stream->eof() && Stream->peek() == '>')
                {
                    valid_comment = true;
                    Stream->get();
                    break;
                }

                prev = curr;
            }

            if (!valid_comment)
            {
                throw std::runtime_error("Malformed xml comment (Line: " + std::to_string(CurrentLine) + ")");
            }

            continue;
        }

        content.push_back(ch);

        if (ch == '>')
        {
            content = TrimXmlString(content);
            break;
        }

        if (ch == '<')
        {
            if (has_opening)
            {
                throw std::runtime_error("Invalid xml element found (Line: " + std::to_string(CurrentLine)
                                         + ")");
            }

            has_opening = true;
        }
    }

    if (content.size() < 3 || content.front() != '<' || content.back() != '>')
    {
        throw std::runtime_error("Invalid xml element found (Line: " + std::to_string(CurrentLine) + ")");
    }

    if (content[1] == '/')
    {
        ele_data->Closing = true;
    }

    ele_data->SelfClosing = *(content.end() - 2) == '/';
    auto tag_end          = ele_data->SelfClosing
                                ? content.end() - 2
                                : std::find_if(content.begin(),
                                      content.end(),
                                      [](unsigned char ch) { return std::isspace(ch) || ch == '>'; });
    ele_data->TagName     = std::string(content.begin() + (content[1] == '/' ? 2 : 1), tag_end);

    if (ele_data->Closing)
    {
        if (!ele_data->SelfClosing) return ele_data;

        throw std::runtime_error(
            "Malformed xml element: double closing tags found (Line: " + std::to_string(CurrentLine) + ")");
    }

    ele_data->Attributes
        = tag_end == content.end() - (ele_data->SelfClosing ? 2 : 1)
              ? nemesis::VectorizedMap<std::string, std::string>()
              : GetXmlAttributes(std::string(tag_end, content.end() - (ele_data->SelfClosing ? 2 : 1)));
    return ele_data;
}

UPtr<nemesis::XmlDeserializer::XmlElementData> nemesis::XmlDeserializer::CreateXmlTag(const std::string& name)
{
    auto ele_data = CreateXmlTag();

    if (ele_data->TagName == name) return ele_data;

    throw std::runtime_error("Xml tag validation failed (Expected tag: " + name + ", Detected tag: "
                             + ele_data->TagName + ", Line: " + std::to_string(CurrentLine) + ")");
}

nemesis::XmlDeserializer::XmlElementData& nemesis::XmlDeserializer::OpenXmlTag(const std::string& name)
{
    return OpenXmlTag(name, std::initializer_list<std::string>{});
}

nemesis::XmlDeserializer::XmlElementData&
nemesis::XmlDeserializer::OpenXmlTag(const std::string& name, std::initializer_list<std::string> attrs)
{
    if (!CurrentElement)
    {
        CurrentElement = CreateXmlTag();
    }

    if (CurrentElement->TagName != name)
    {
        throw std::runtime_error("Xml tag validation failed (Expected tag: " + name + ", Detected tag: "
                                 + CurrentElement->TagName + ", Line: " + std::to_string(CurrentLine) + ")");
    }

    if (CurrentElement->Closing)
    {
        throw std::runtime_error("Invalid xml structure: unexpected closing tag (Tag: "
                                 + CurrentElement->TagName + ", Line: " + std::to_string(CurrentLine) + ")");
    }

    for (auto& each : attrs)
    {
        std::string attr_val;

        if (CurrentElement->Attributes.try_at(each, attr_val)) continue;

        throw std::runtime_error("Xml attribute validation failed: missing \"" + each
                                 + "\" attribute (Line: " + std::to_string(CurrentLine) + ")");
    }

    return *CurrentElement;
}

nemesis::XmlDeserializer::XmlElementData&
nemesis::XmlDeserializer::OpenXmlTag(const std::string& name,
                                     std::initializer_list<Pair<std::string, std::string>> attrs)
{
    if (!CurrentElement)
    {
        CurrentElement = CreateXmlTag();
    }

    if (CurrentElement->TagName != name)
    {
        throw std::runtime_error("Xml tag validation failed (Expected tag: " + name + ", Detected tag: "
                                 + CurrentElement->TagName + ", Line: " + std::to_string(CurrentLine) + ")");
    }

    if (CurrentElement->Closing)
    {
        throw std::runtime_error("Invalid xml structure: unexpected closing tag (Tag: "
                                 + CurrentElement->TagName + ", Line: " + std::to_string(CurrentLine) + ")");
    }

    for (auto& each : attrs)
    {
        std::string attr_val;

        if (CurrentElement->Attributes.try_at(each.first, attr_val) && each.second == attr_val) continue;

        throw std::runtime_error(
            "Xml attribute validation failed: missing or mismatch attribute (Expected attribute: "
            + each.first + "/" + each.second + ", Detected attribute: " + each.first + "/" + attr_val
            + ", Line: " + std::to_string(CurrentLine) + ")");
    }

    return *CurrentElement;
}

nemesis::XmlDeserializer::XmlElementData& nemesis::XmlDeserializer::CloseXmlTag(const std::string& name)
{
    if (!CurrentElement)
    {
        CurrentElement = CreateXmlTag();
    }

    if (CurrentElement->TagName != name)
    {
        throw std::runtime_error("Xml tag validation failed (Expected tag: " + name + ", Detected tag: "
                                 + CurrentElement->TagName + ", Line: " + std::to_string(CurrentLine) + ")");
    }

    if (CurrentElement->Closing) return *CurrentElement;

    throw std::runtime_error("Invalid xml structure: unexpected opening tag (Tag: " + CurrentElement->TagName
                             + ", Line: " + std::to_string(CurrentLine) + ")");
}

void nemesis::XmlDeserializer::ReadXmlDeclaration()
{
    std::string content;
    bool has_opening = false;

    while (!Stream->eof())
    {
        char ch = Stream->get();
        char nx = Stream->peek();

        if (ch == '\n')
        {
            CurrentLine++;
        }

        if (ch == '<' && nx == '!')
        {
            Stream->get();

            if (Stream->eof() || Stream->get() != '-' || Stream->eof() || Stream->get() != '-')
            {
                throw std::runtime_error("Malformed xml comment (Line: " + std::to_string(CurrentLine) + ")");
            }

            char prev, curr;
            bool valid_comment = false;

            while (!Stream->get(curr))
            {
                if (curr == '\n')
                {
                    CurrentLine++;
                }

                if (prev == '-' && curr == '-' && !Stream->eof() && Stream->peek() == '>')
                {
                    valid_comment = true;
                    Stream->get();
                    break;
                }

                prev = curr;
            }

            if (!valid_comment)
            {
                throw std::runtime_error("Malformed xml comment (Line: " + std::to_string(CurrentLine) + ")");
            }

            continue;
        }

        content.push_back(ch);

        if (ch == '>')
        {
            content = TrimXmlString(content);
            break;
        }

        if (ch == '<')
        {
            if (has_opening)
            {
                throw std::runtime_error("Invalid xml declaration (Line: " + std::to_string(CurrentLine)
                                         + ")");
            }

            has_opening = true;
        }
    }

    std::transform(
        content.begin(), content.end(), content.begin(), [](unsigned char ch) { return std::tolower(ch); });

    if (content.empty() || content.front() != '<' || content.find("<?xml ") != 0
        || content.rfind("?>") != content.size() - 2)
    {
        throw std::runtime_error("Invalid xml declaration (Line: " + std::to_string(CurrentLine) + ")");
    }

    auto attrs = GetXmlAttributes(content.substr(6, content.size() - 6 - 2));

    if (attrs["version"] != "1.0")
    {
        throw std::runtime_error("Unsupported xml version: only 1.0 is supported");
    }

    if (attrs["encoding"] == "ascii") return;

    throw std::runtime_error("Unsupported xml encoding: only ascii is supported");
}

void nemesis::XmlDeserializer::ParseHkxObjectMap()
{
    HkxObjectMap.clear();
    ReferenceValidation.clear();
    ValidatedReference.clear();

    auto ele_uptr = CreateXmlTag();
    auto ele_ptr  = ele_uptr.get();
    std::string node_name, cls_name, sig;

    while (ele_ptr->TagName == "hkobject")
    {
        if (ele_ptr->Closing)
        {
            throw std::runtime_error("Invalid xml structure: unexpected closing tag (Tag: " + ele_ptr->TagName
                                     + ", Line: " + std::to_string(CurrentLine) + ")");
        }

        if (!ele_ptr->Attributes.try_at("name", node_name))
        {
            throw std::runtime_error("Malformed hkx node: hkobject name cannot be found (Line: "
                                     + std::to_string(CurrentLine) + ")");
        }

        if (!ele_ptr->Attributes.try_at("class", cls_name))
        {
            throw std::runtime_error("Malformed hkx node: hkobject class cannot be found (Line: "
                                     + std::to_string(CurrentLine) + ")");
        }

        if (!ele_ptr->Attributes.try_at("signature", sig))
        {
            throw std::runtime_error("Malformed hkx node: hkobject signature cannot be found (Line: "
                                     + std::to_string(CurrentLine) + ")");
        }

        LocalReferences = std::make_unique<VecStr>();
        LocalValidation
            = std::make_unique<Vec<std::function<void(const nemesis::hkbBehaviorGraphStringData&)>>>();

        if (TopLevelObject == node_name)
        {
            Root->DeserializeFrom(*this);
            HkxObjectMap.insert({node_name, Root});
        }
        else
        {
            auto hk_ptr = PackfilePtr->CreateObject(cls_name);
            auto* bhv_graph = dynamic_cast<nemesis::hkbBehaviorGraph*>(hk_ptr);
            
            if (bhv_graph)
            {
                BehaviorGraphList.push_back({node_name, bhv_graph});
            }

            hk_ptr->DeserializeFrom(*this);
            HkxObjectMap.insert({node_name, hk_ptr});

            ReferenceValidation.insert({node_name,
                                        [local_refs   = std::move(LocalReferences),
                                         local_valids = std::move(LocalValidation),
                                         this](const nemesis::hkbBehaviorGraphStringData& string_data)
                                        {
                                            for (auto& validation : *local_valids)
                                            {
                                                validation(string_data);
                                            }

                                            for (auto& ref : *local_refs)
                                            {
                                                auto itr = ReferenceValidation.find(ref);

                                                if (itr == ReferenceValidation.end()) continue;

                                                auto itr_vad = ValidatedReference.find(ref);

                                                if (itr_vad != ValidatedReference.end()) continue;

                                                itr->second(string_data);
                                                ValidatedReference.insert(ref);
                                            }
                                        }});
        }

        CloseXmlTag("hkobject");
        CurrentElement = nullptr;

        ele_uptr = CreateXmlTag();
        ele_ptr  = ele_uptr.get();
    }

    if (ele_ptr->Closing)
    {
        Stream->putback('>');

        for (int i = ele_ptr->TagName.size() - 1; i >= 0; i--)
        {
            Stream->putback(ele_ptr->TagName[i]);
        }

        Stream->putback('/');
        Stream->putback('<');
    }
}

void nemesis::XmlDeserializer::ValidateStringData()
{
    for (auto& bhv_graph : BehaviorGraphList)
    {
        auto* data = bhv_graph.second->GetData();

        if (!data) continue;

        auto* string_data = data->GetStringData();

        if (!string_data) continue;

        auto itr = ReferenceValidation.find(bhv_graph.first);

        if (itr == ReferenceValidation.end()) continue;

        itr->second(*string_data);
    }
}

void nemesis::XmlDeserializer::RunGlobalQueue()
{
    for (auto& queue : GlobalQueue)
    {
        queue();
    }
}

bool nemesis::XmlDeserializer::CanAssert() const noexcept
{
    return false;
}

void nemesis::XmlDeserializer::LoadFile(const std::filesystem::path& filepath)
{
    Stream = std::make_unique<std::ifstream>(filepath);

    if (static_cast<std::ifstream*>(Stream.get())->is_open()) return;

    std::error_code ec(errno, std::system_category());
    throw std::runtime_error("Failed to open file: \"" + nemesis::to_utf8_string(filepath)
                             + "\"\nMessage: " + ec.message());
}

void nemesis::XmlDeserializer::LoadXml(const std::string& xml)
{
    Stream = std::make_unique<std::istringstream>(xml);
}

nemesis::hkPackfile nemesis::XmlDeserializer::Deserialize()
{
    if (!Stream) throw std::runtime_error("Empty stream. No xml is loaded");

    nemesis::hkPackfile packfile;
    PackfilePtr = &packfile;
    ReadXmlDeclaration();

    {
        auto& open_ele = OpenXmlTag("hkpackfile", {"classversion", "contentsversion", "toplevelobject"});
        nemesis::OnScopeEnds on_ends([this]() { CurrentElement = nullptr; });

        if (open_ele.Attributes["classversion"] != "8")
        {
            throw std::runtime_error("Unsupported class version: only version 8 is supported");
        }

        ContentsVersionString = open_ele.Attributes["contentsversion"];
        ContentsVersion       = nemesis::GetEnumVersion(ContentsVersionString);
        TopLevelObject        = open_ele.Attributes["toplevelobject"];
    }

    packfile.DeserializeFrom(*this);

    CloseXmlTag("hkpackfile");
    CurrentElement = nullptr;
    Stream         = nullptr;
    return packfile;
}

void nemesis::XmlDeserializer::Deserialize(nemesis::hkPackfileSection& section_data)
{
    nemesis::hkDataSection* data_section = dynamic_cast<nemesis::hkDataSection*>(&section_data);

    if (data_section == nullptr) return;

    OpenXmlTag("hksection", std::initializer_list<Pair<std::string, std::string>>{{"name", "__data__"}});
    CurrentElement = nullptr;

    data_section->DeserializeFrom(*this);

    CloseXmlTag("hksection");
    CurrentElement = nullptr;
}

void nemesis::XmlDeserializer::Deserialize(nemesis::hkRootLevelContainer& root)
{
    Root = &root;
    ParseHkxObjectMap();

    Root = nullptr;
    RunGlobalQueue();
    ValidateStringData();
}

std::streampos nemesis::XmlDeserializer::GetCurrentPosition() const
{
    return 0;
}

void nemesis::XmlDeserializer::GoToPosition(std::streampos address) {}

void nemesis::XmlDeserializer::Pad(unsigned int size) {}

void nemesis::XmlDeserializer::Skip(unsigned int step) {}

bool& nemesis::XmlDeserializer::ReadValue(const std::string& name, bool& val)
{
    if (name.empty()) return ReadValue(*CurrentBlock, val);

    if (!TryReadHkxParam(name, val)) return val = false;

    return val;
}

char& nemesis::XmlDeserializer::ReadValue(const std::string& name, char& ch)
{
    int i;

    if (name.empty()) return ch = static_cast<char>(ReadValue(*CurrentBlock, i));

    if (!TryReadHkxParam(name, i)) return ch = '\0';

    return ch = static_cast<char>(i);
}

unsigned char& nemesis::XmlDeserializer::ReadValue(const std::string& name, unsigned char& ch)
{
    std::string str;

    if (name.empty())
    {
        str       = ReadString(*CurrentBlock);
        return ch = str.empty() ? '\0' : str.front();
    }

    if (!TryReadHkxParam(name, str) || str.empty()) return ch = '\0';

    return ch = str.front();
}

short& nemesis::XmlDeserializer::ReadValue(const std::string& name, short& byte)
{
    if (name.empty()) return ReadValue(*CurrentBlock, byte);

    if (!TryReadHkxParam(name, byte)) return byte = 0;

    return byte;
}

unsigned short& nemesis::XmlDeserializer::ReadValue(const std::string& name, unsigned short& byte)
{
    if (name.empty()) return ReadValue(*CurrentBlock, byte);

    if (!TryReadHkxParam(name, byte)) return byte = 0;

    return byte;
}

int& nemesis::XmlDeserializer::ReadValue(const std::string& name, int& val)
{
    if (name.empty()) return ReadValue(*CurrentBlock, val);

    if (!TryReadHkxParam(name, val)) return val = 0;

    return val;
}

unsigned int& nemesis::XmlDeserializer::ReadValue(const std::string& name, unsigned int& val)
{
    if (name.empty()) return ReadValue(*CurrentBlock, val);

    if (!TryReadHkxParam(name, val)) return val = 0;

    return val;
}

long& nemesis::XmlDeserializer::ReadValue(const std::string& name, long& val)
{
    if (name.empty()) return ReadValue(*CurrentBlock, val);

    if (!TryReadHkxParam(name, val)) return val = 0;

    return val;
}

unsigned long& nemesis::XmlDeserializer::ReadValue(const std::string& name, unsigned long& val)
{
    if (name.empty()) return ReadValue(*CurrentBlock, val);

    if (!TryReadHkxParam(name, val)) return val = 0;

    return val;
}

long long& nemesis::XmlDeserializer::ReadValue(const std::string& name, long long& val)
{
    if (name.empty()) return ReadValue(*CurrentBlock, val);

    if (!TryReadHkxParam(name, val)) return val = 0;

    return val;
}

unsigned long long& nemesis::XmlDeserializer::ReadValue(const std::string& name, unsigned long long& val)
{
    if (name.empty()) return ReadValue(*CurrentBlock, val);

    if (!TryReadHkxParam(name, val)) return val = 0;

    return val;
}

Float16& nemesis::XmlDeserializer::ReadValue(const std::string& name, Float16& val)
{
    float f;

    if (name.empty()) return val = ReadValue(*CurrentBlock, f);

    if (!TryReadHkxParam(name, f)) return val = 0;

    return val = f;
}

float& nemesis::XmlDeserializer::ReadValue(const std::string& name, float& val)
{
    if (name.empty()) return ReadValue(*CurrentBlock, val);

    if (!TryReadHkxParam(name, val)) return val = 0;

    return val;
}

double& nemesis::XmlDeserializer::ReadValue(const std::string& name, double& val)
{
    if (name.empty()) return ReadValue(*CurrentBlock, val);

    if (!TryReadHkxParam(name, val)) return val = 0;

    return val;
}

std::string& nemesis::XmlDeserializer::ReadValue(const std::string& name, std::string& ref_str)
{
    return ref_str = std::string(ReadString(name).data());
}

std::string& nemesis::XmlDeserializer::ReadValue(const std::string& name,
                                                 std::string& ref_str,
                                                 unsigned int size,
                                                 char pad)
{
    return ref_str = std::string(ReadString(name, size, pad).data());
}

nemesis::hkEventId& nemesis::XmlDeserializer::ReadValue(const std::string& name, nemesis::hkEventId& event_id)
{
    int val;
    ReadValue(name, val);
    LocalValidation->emplace_back(
        [val](const nemesis::hkbBehaviorGraphStringData& string_data)
        {
            int size = string_data.GetEventNames().GetSize();

            if (-1 <= val && val < size) return;

            throw std::runtime_error("Event id out of bounds (" + std::to_string(val) + ")");
        });
    return event_id = val;
}

nemesis::hkAttributeId& nemesis::XmlDeserializer::ReadValue(const std::string& name,
                                                            nemesis::hkAttributeId& attr_id)
{
    int val;
    ReadValue(name, val);
    LocalValidation->emplace_back(
        [val](const nemesis::hkbBehaviorGraphStringData& string_data)
        {
            int size = string_data.GetAttributeNames().GetSize();

            if (-1 <= val && val < size) return;

            throw std::runtime_error("Attribute id out of bounds (" + std::to_string(val) + ")");
        });
    return attr_id = val;
}

nemesis::hkVariableId& nemesis::XmlDeserializer::ReadValue(const std::string& name,
                                                           nemesis::hkVariableId& var_id)
{
    int val;
    ReadValue(name, val);
    LocalValidation->emplace_back(
        [val](const nemesis::hkbBehaviorGraphStringData& string_data)
        {
            int size = string_data.GetVariableNames().GetSize();

            if (-1 <= val && val < size) return;

            throw std::runtime_error("Variable id out of bounds (" + std::to_string(val) + ")");
        });
    return var_id = val;
}

nemesis::hkCharacterPropertyId& nemesis::XmlDeserializer::ReadValue(const std::string& name,
                                                                    nemesis::hkCharacterPropertyId& prop_id)
{
    int val;
    ReadValue(name, val);
    LocalValidation->emplace_back(
        [val](const nemesis::hkbBehaviorGraphStringData& string_data)
        {
            int size = string_data.GetCharacterPropertyNames().GetSize();

            if (-1 <= val && val < size) return;

            throw std::runtime_error("Character property id out of bounds (" + std::to_string(val)
                                     + ")");
        });
    return prop_id = val;
}

nemesis::hkCString& nemesis::XmlDeserializer::ReadValue(const std::string& name, nemesis::hkCString& cstring)
{
    return ReadValue(name, cstring, 16);
}

nemesis::hkCString& nemesis::XmlDeserializer::ReadValue(const std::string& name,
                                                        nemesis::hkCString& cstring,
                                                        unsigned int padding_size)
{
    std::string val;

    if (!name.empty())
    {
        TryReadHkxParam(name, val, false);
        val = val == "&#9216;" ? "" : DecodeXmlValue(val);
    }
    else
    {
        if (!CurrentElement)
        {
            CurrentElement = CreateXmlTag();
        }

        if (CurrentElement->TagName != "hkcstring")
        {
            throw std::runtime_error("Xml tag validation failed (Expected tag: " + name
                                     + ", Detected tag: " + CurrentElement->TagName
                                     + ", Line: " + std::to_string(CurrentLine) + ")");
        }

        val = GetXmlInnerText();

        if (!CurrentElement->Closing)
        {
            CurrentElement = nullptr;
            CloseXmlTag("hkcstring");
        }

        CurrentElement = nullptr;
    }

    if (val.empty())
    {
        cstring.Clear();
    }
    else
    {
        cstring.SetValue(val);
    }

    return cstring;
}

nemesis::hkStringPtr& nemesis::XmlDeserializer::ReadValue(const std::string& name,
                                                          nemesis::hkStringPtr& string_ptr)
{
    return ReadValue(name, string_ptr, 16);
}

nemesis::hkStringPtr& nemesis::XmlDeserializer::ReadValue(const std::string& name,
                                                          nemesis::hkStringPtr& string_ptr,
                                                          unsigned int padding_size)
{
    std::string val;

    if (!TryReadHkxParam(name, val)) return string_ptr;

    if (val.empty())
    {
        string_ptr.Clear();
    }
    else
    {
        string_ptr.SetValue(val);
    }

    return string_ptr;
}

nemesis::hkEnumBase& nemesis::XmlDeserializer::ReadValue(const std::string& name, nemesis::hkEnumBase& val)
{
    std::string str;

    if (!TryReadHkxParam(name, str))
    {
        val.reset_to_zero();
        return val;
    }

    val = str;
    return val;
}

nemesis::hkVector4& nemesis::XmlDeserializer::ReadValue(nemesis::XmlDeserializer::StreamBlock& stream_block,
                                                        nemesis::hkVector4& vec4,
                                                        bool skip_last)
{
    float x, y, z, w = 0.0;

    if (skip_last)
    {
        stream_block.Stream >> x >> y >> z;
    }
    else
    {
        stream_block.Stream >> x >> y >> z >> w;
    }

    vec4.SetX(x);
    vec4.SetY(y);
    vec4.SetZ(z);
    vec4.SetW(w);

    if (!stream_block.Stream.fail()) return vec4;

    throw std::runtime_error("Malformed hkVector4: unable to parse string value (Value: " + stream_block.Text
                             + ", Line: " + std::to_string(CurrentLine) + ")");
}

nemesis::hkVector4& nemesis::XmlDeserializer::ReadValue(const std::string& name, nemesis::hkVector4& vec4)
{
    return ReadComplexContainer(name, vec4);
}

nemesis::hkVector8& nemesis::XmlDeserializer::ReadValue(nemesis::XmlDeserializer::StreamBlock& stream_block,
                                                        nemesis::hkVector8& vec8)
{
    ReadValue(stream_block, vec8.GetLin());
    ReadValue(stream_block, vec8.GetAng());
    return vec8;
}

nemesis::hkVector8& nemesis::XmlDeserializer::ReadValue(const std::string& name, nemesis::hkVector8& vec8)
{
    return ReadComplexContainer(name, vec8);
}

nemesis::hkQuaternion&
nemesis::XmlDeserializer::ReadValue(nemesis::XmlDeserializer::StreamBlock& stream_block,
                                    nemesis::hkQuaternion& quaternion)
{
    float x, y, z, r;
    stream_block.Stream >> x >> y >> z >> r;

    quaternion.SetX(x);
    quaternion.SetY(y);
    quaternion.SetZ(z);
    quaternion.SetR(r);

    if (!stream_block.Stream.fail()) return quaternion;

    throw std::runtime_error("Malformed hkQuaternion: unable to parse string value (Value: " + stream_block.Text
                             + ", Line: " + std::to_string(CurrentLine) + ")");
}

nemesis::hkQuaternion& nemesis::XmlDeserializer::ReadValue(const std::string& name,
                                                           nemesis::hkQuaternion& quaternion)
{
    return ReadComplexContainer(name, quaternion);
}

nemesis::hkQsTransform&
nemesis::XmlDeserializer::ReadValue(nemesis::XmlDeserializer::StreamBlock& stream_block,
                                    nemesis::hkQsTransform& qs_transform)
{
    ReadValue(stream_block, qs_transform.GetTranslation(), true);
    ReadValue(stream_block, qs_transform.GetRotation());
    ReadValue(stream_block, qs_transform.GetScale(), true);
    return qs_transform;
}

nemesis::hkQsTransform& nemesis::XmlDeserializer::ReadValue(const std::string& name,
                                                            nemesis::hkQsTransform& qs_transform)
{
    return ReadComplexContainer(name, qs_transform);
}

nemesis::hkUFloat8& nemesis::XmlDeserializer::ReadValue(const std::string& name, nemesis::hkUFloat8& ufloat8)
{
    unsigned char byte;
    ReadValue(name, byte);
    ufloat8 = byte;
    return ufloat8;
}

nemesis::hkLong& nemesis::XmlDeserializer::ReadValue(const std::string& name, nemesis::hkLong& _long)
{
    int64_t val;
    ReadValue(name, val);
    _long = val;
    return _long;
}

nemesis::hkUlong& nemesis::XmlDeserializer::ReadValue(const std::string& name, nemesis::hkUlong& ulong)
{
    uint64_t val;
    ReadValue(name, val);
    ulong = val;
    return ulong;
}

nemesis::hkHalf& nemesis::XmlDeserializer::ReadValue(const std::string& name, nemesis::hkHalf& half)
{
    float val;
    ReadValue(name, val);
    half = val;
    return half;
}

nemesis::hkTransform& nemesis::XmlDeserializer::ReadValue(nemesis::XmlDeserializer::StreamBlock& stream_block,
                                                          nemesis::hkTransform& transform)
{
    ReadValue(stream_block, transform.GetRotation());
    ReadValue(stream_block, transform.GetTranslation());
    return transform;
}

nemesis::hkTransform& nemesis::XmlDeserializer::ReadValue(const std::string& name,
                                                          nemesis::hkTransform& transform)
{
    return ReadComplexContainer(name, transform);
}

nemesis::hkMatrix3& nemesis::XmlDeserializer::ReadValue(nemesis::XmlDeserializer::StreamBlock& stream_block,
                                                        nemesis::hkMatrix3& matrix3)
{
    ReadValue(stream_block, matrix3.GetCol0());
    ReadValue(stream_block, matrix3.GetCol1());
    ReadValue(stream_block, matrix3.GetCol2());
    return matrix3;
}

nemesis::hkMatrix3& nemesis::XmlDeserializer::ReadValue(const std::string& name, nemesis::hkMatrix3& matrix3)
{
    return ReadComplexContainer(name, matrix3);
}

nemesis::hkMatrix4& nemesis::XmlDeserializer::ReadValue(nemesis::XmlDeserializer::StreamBlock& stream_block,
                                                        nemesis::hkMatrix4& matrix4)
{
    ReadValue(stream_block, matrix4.GetCol0());
    ReadValue(stream_block, matrix4.GetCol1());
    ReadValue(stream_block, matrix4.GetCol2());
    ReadValue(stream_block, matrix4.GetCol3());
    return matrix4;
}

nemesis::hkMatrix4& nemesis::XmlDeserializer::ReadValue(const std::string& name, nemesis::hkMatrix4& matrix4)
{
    return ReadComplexContainer(name, matrix4);
}

void**
nemesis::XmlDeserializer::ReadArrayValue(const std::string& name, void* list[], size_t size, size_t type_size)
{
    for (size_t i = 0; i < size; ++i)
    {
        char empty = 0;
        std::memcpy(reinterpret_cast<char*>(list[i]), &empty, type_size);
    }

    return list;
}

nemesis::hkCString**
nemesis::XmlDeserializer::ReadArrayValue(const std::string& name, nemesis::hkCString* (&list)[], size_t size)
{
    return list;
}

nemesis::hkStringPtr** nemesis::XmlDeserializer::ReadArrayValue(const std::string& name,
                                                                nemesis::hkStringPtr* (&list)[],
                                                                size_t size)
{
    return list;
}

nemesis::hkEnumBase**
nemesis::XmlDeserializer::ReadArrayValue(const std::string& name, nemesis::hkEnumBase* (&list)[], size_t size)
{
    return list;
}

nemesis::hkVector4**
nemesis::XmlDeserializer::ReadArrayValue(const std::string& name, nemesis::hkVector4* (&list)[], size_t size)
{
    return list;
}

nemesis::hkVector8**
nemesis::XmlDeserializer::ReadArrayValue(const std::string& name, nemesis::hkVector8* (&list)[], size_t size)
{
    return list;
}

nemesis::hkQuaternion** nemesis::XmlDeserializer::ReadArrayValue(const std::string& name,
                                                                 nemesis::hkQuaternion* (&list)[],
                                                                 size_t size)
{
    return list;
}

nemesis::hkQsTransform** nemesis::XmlDeserializer::ReadArrayValue(const std::string& name,
                                                                  nemesis::hkQsTransform* (&list)[],
                                                                  size_t size)
{
    return list;
}

nemesis::hkUFloat8**
nemesis::XmlDeserializer::ReadArrayValue(const std::string& name, nemesis::hkUFloat8* (&list)[], size_t size)
{
    return list;
}

nemesis::hkLong**
nemesis::XmlDeserializer::ReadArrayValue(const std::string& name, nemesis::hkLong* (&list)[], size_t size)
{
    return list;
}

nemesis::hkUlong**
nemesis::XmlDeserializer::ReadArrayValue(const std::string& name, nemesis::hkUlong* (&list)[], size_t size)
{
    return list;
}

nemesis::hkHalf**
nemesis::XmlDeserializer::ReadArrayValue(const std::string& name, nemesis::hkHalf* (&list)[], size_t size)
{
    return list;
}

nemesis::hkTransform** nemesis::XmlDeserializer::ReadArrayValue(const std::string& name,
                                                                nemesis::hkTransform* (&list)[],
                                                                size_t size)
{
    return list;
}

nemesis::hkMatrix3**
nemesis::XmlDeserializer::ReadArrayValue(const std::string& name, nemesis::hkMatrix3* (&list)[], size_t size)
{
    return list;
}

nemesis::hkMatrix4**
nemesis::XmlDeserializer::ReadArrayValue(const std::string& name, nemesis::hkMatrix4* (&list)[], size_t size)
{
    return list;
}

nemesis::HavokObject** nemesis::XmlDeserializer::ReadArrayObject(const std::string& name,
                                                                 nemesis::HavokObject* (&list)[],
                                                                 size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        ReadObject(name, *list[i]);
    }

    return list;
}

nemesis::hkSmallArrayBase& nemesis::XmlDeserializer::ReadArrayObject(const std::string& name,
                                                                     nemesis::hkSmallArrayBase& array)
{
    if (!CurrentElement)
    {
        CurrentElement = CreateXmlTag();
    }

    if (CurrentElement->TagName != "hkparam") return array;

    std::string val;

    if (!CurrentElement->Attributes.try_at("name", val) || val != name) return array;

    if (CurrentElement->Closing)
    {
        throw std::runtime_error("Invalid xml structure: unexpected closing tag (Tag: "
                                 + CurrentElement->TagName + ", Line: " + std::to_string(CurrentLine) + ")");
    }

    if (!CurrentElement->Attributes.try_at("numelements", val)) return array;

    CurrentElement      = nullptr;
    unsigned short size = std::stoul(val);
    array.SetSize(size);

    if (array.IsVariant() && !array.IsCString())
    {
        auto block_ptr = CurrentBlock.release();
        nemesis::OnScopeEnds on_ends([block_ptr, this]() { CurrentBlock.reset(block_ptr); });

        std::string str     = GetXmlInnerText();
        std::string new_str = TrimXmlString(str);
        new_str.erase(
            std::remove_if(new_str.begin(), new_str.end(), [](char ch) { return ch == '(' || ch == ')'; }),
            new_str.end());

        CurrentBlock = std::make_unique<nemesis::XmlDeserializer::StreamBlock>(str, new_str);
        array.DeserializeFrom(*this);
    }
    else
    {
        array.DeserializeFrom(*this);
    }

    CloseXmlTag("hkparam");
    CurrentElement = nullptr;
    return array;
}

nemesis::hkArrayBase& nemesis::XmlDeserializer::ReadArrayObject(const std::string& name,
                                                                nemesis::hkArrayBase& array)
{
    if (!CurrentElement)
    {
        CurrentElement = CreateXmlTag();
    }

    if (CurrentElement->TagName != "hkparam") return array;

    std::string val;

    if (!CurrentElement->Attributes.try_at("name", val) || val != name) return array;

    if (CurrentElement->Closing)
    {
        throw std::runtime_error("Invalid xml structure: unexpected closing tag (Tag: "
                                 + CurrentElement->TagName + ", Line: " + std::to_string(CurrentLine) + ")");
    }

    if (!CurrentElement->Attributes.try_at("numelements", val)) return array;

    CurrentElement    = nullptr;
    unsigned int size = std::stoul(val);
    array.SetSize(size);

    if (array.IsVariant() && !array.IsCString())
    {
        auto block_ptr = CurrentBlock.release();
        nemesis::OnScopeEnds on_ends([block_ptr, this]() { CurrentBlock.reset(block_ptr); });

        std::string str     = GetXmlInnerText();
        std::string new_str = TrimXmlString(str);
        new_str.erase(
            std::remove_if(new_str.begin(), new_str.end(), [](char ch) { return ch == '(' || ch == ')'; }),
            new_str.end());

        CurrentBlock = std::make_unique<nemesis::XmlDeserializer::StreamBlock>(str, new_str);
        array.DeserializeFrom(*this);
    }
    else
    {
        array.DeserializeFrom(*this);
    }

    CloseXmlTag("hkparam");
    CurrentElement = nullptr;
    return array;
}

nemesis::hkRefVariant&
nemesis::XmlDeserializer::ReadRefObject(nemesis::XmlDeserializer::StreamBlock& stream_block,
                                        nemesis::hkRefVariant& ref_obj)
{
    std::string str;
    stream_block.Stream >> str;

    if (!stream_block.Stream.fail())
    {
        if (str == "null") return ref_obj;

        GlobalQueue.emplace_back(
            [&ref_obj, str, local_refs = LocalReferences.get(), this]()
            {
                auto itr = HkxObjectMap.find(str);

                if (itr == HkxObjectMap.end()) return;

                ref_obj.ReferenceTo(itr->second);
                local_refs->emplace_back(str);
            });
    }

    return ref_obj;
}

nemesis::hkRefVariant& nemesis::XmlDeserializer::ReadRefObject(const std::string& name,
                                                               nemesis::hkRefVariant& ref_obj)
{
    if (name.empty()) return ReadRefObject(*CurrentBlock, ref_obj);

    std::string str;

    if (!TryReadHkxParam(name, str)) return ref_obj;

    if (str == "null") return ref_obj;

    GlobalQueue.emplace_back(
        [&ref_obj, str, local_refs = LocalReferences.get(), this]()
        {
            auto itr = HkxObjectMap.find(str);

            if (itr == HkxObjectMap.end()) return;

            ref_obj.ReferenceTo(itr->second);
            local_refs->emplace_back(str);
        });
    return ref_obj;
}

nemesis::HavokObject**
nemesis::XmlDeserializer::ReadRefObject(nemesis::XmlDeserializer::StreamBlock& stream_block,
                                        nemesis::HavokObject** ptr_ptr_obj)
{
    std::string str;
    stream_block.Stream >> str;

    if (!stream_block.Stream.fail())
    {
        if (str == "null") return ptr_ptr_obj;

        GlobalQueue.emplace_back(
            [ptr_ptr_obj, str, local_refs = LocalReferences.get(), this]()
            {
                auto itr = HkxObjectMap.find(str);

                if (itr == HkxObjectMap.end()) return;

                *ptr_ptr_obj = itr->second;
                local_refs->emplace_back(str);
            });
    }

    return ptr_ptr_obj;
}

nemesis::HavokObject** nemesis::XmlDeserializer::ReadRefObject(const std::string& name,
                                                               nemesis::HavokObject** ptr_ptr_obj)
{
    if (name.empty()) return ReadRefObject(*CurrentBlock, ptr_ptr_obj);

    std::string str;

    if (!TryReadHkxParam(name, str)) return ptr_ptr_obj;

    if (str == "null") return ptr_ptr_obj;

    GlobalQueue.emplace_back(
        [ptr_ptr_obj, str, local_refs = LocalReferences.get(), this]()
        {
            auto itr = HkxObjectMap.find(str);

            if (itr == HkxObjectMap.end()) return;

            *ptr_ptr_obj = itr->second;
            local_refs->emplace_back(str);
        });
    return ptr_ptr_obj;
}

nemesis::hkClass& nemesis::XmlDeserializer::ReadClass(nemesis::hkClass& cls)
{
    cls.DeserializeFrom(*this);
    return cls;
}

nemesis::HavokObject& nemesis::XmlDeserializer::ReadObject(const std::string& name,
                                                           nemesis::HavokObject& hk_obj)
{
    if (!name.empty())
    {
        if (!CurrentElement)
        {
            CurrentElement = CreateXmlTag();
        }

        if (CurrentElement->TagName != "hkparam") return hk_obj;

        if (CurrentElement->Attributes["name"] != name) return hk_obj;

        CurrentElement = nullptr;
    }

    OpenXmlTag("hkobject");
    CurrentElement = nullptr;
    hk_obj.DeserializeFrom(*this);

    CloseXmlTag("hkobject");
    CurrentElement = nullptr;

    if (!name.empty())
    {
        CloseXmlTag("hkparam");
        CurrentElement = nullptr;
    }

    return hk_obj;
}

std::string nemesis::XmlDeserializer::ReadString(nemesis::XmlDeserializer::StreamBlock& stream_block)
{
    std::string str;
    stream_block.Stream >> str;

    if (!stream_block.Stream.fail()) return str;

    throw std::runtime_error("Parsing error: unable to retrieve string value (Value: " + stream_block.Text
                             + ", Line: " + std::to_string(CurrentLine) + ")");
}

std::string nemesis::XmlDeserializer::ReadString(const std::string& name)
{
    if (name.empty()) return ReadString(*CurrentBlock);

    std::string val;

    if (!TryReadHkxParam(name, val)) return val;

    return val;
}

std::string nemesis::XmlDeserializer::ReadString(const std::string& name, unsigned int size, char pad)
{
    return ReadString(name);
}

nemesis::hkCString nemesis::XmlDeserializer::ReadCString(const std::string& name)
{
    nemesis::hkCString cstring;
    ReadValue(name, cstring);
    return cstring;
}

nemesis::hkStringPtr nemesis::XmlDeserializer::ReadStringPointer(const std::string& name)
{
    nemesis::hkStringPtr string_ptr;
    ReadValue(name, string_ptr);
    return string_ptr;
}
