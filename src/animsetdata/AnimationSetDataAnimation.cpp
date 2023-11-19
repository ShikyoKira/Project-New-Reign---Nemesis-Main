#include "animsetdata/AnimationSetDataAnimation.h"

#include "core/CompileState.h"
#include "core/NObjectParser.h"

static const std::string bsva2_key = "bsva2 -->";

nemesis::AnimationSetDataAnimation::Parser::Writer::Writer(ParsingObjectType parse_type)
    : ParseType(parse_type)
{
}

void nemesis::AnimationSetDataAnimation::Parser::Writer::Prepare(UPtr<nemesis::NObject>&& object)
{
    StagedObjects.emplace_back(std::move(object));
}

void nemesis::AnimationSetDataAnimation::Parser::Writer::Consume()
{
    Index++;
}

void nemesis::AnimationSetDataAnimation::Parser::Writer::WriteTo(UPtr<nemesis::NObject>& target)
{
    auto& object = StagedObjects[Index++];

    switch (ParseType)
    {
        case nemesis::AnimationSetDataAnimation::Parser::EVENT_NAME:
        {
            ParseType = ParsingObjectType::UNKNOWN;
            target    = std::move(object);
            break;
        }
        case nemesis::AnimationSetDataAnimation::Parser::UNKNOWN:
        {
            ParseType = ParsingObjectType::ANIMATION_LIST_COUNT;
            target    = std::move(object);
            break;
        }
        case nemesis::AnimationSetDataAnimation::Parser::ANIMATION_LIST_COUNT:
        {
            ParseType = ParsingObjectType::ANIMATION_NAME;
            break;
        }
        case nemesis::AnimationSetDataAnimation::Parser::ANIMATION_NAME:
        {
            auto collection = dynamic_cast<nemesis::CollectionObject*>(target.get());

            if (!collection)
            {
                target = std::make_unique<nemesis::CollectionObject>();
                collection = dynamic_cast<nemesis::CollectionObject*>(target.get());
            }

            collection->AddObject(std::move(object));
            break;
        }
    }
}

bool nemesis::AnimationSetDataAnimation::Parser::Writer::IsEnd() const
{
    return Index >= StagedObjects.size();
}

UPtr<nemesis::IfObject>
nemesis::AnimationSetDataAnimation::Parser::ParseIfObject(nemesis::LineStream& stream,
                                                          nemesis::SemanticManager& manager)
{
    return UPtr<nemesis::IfObject>();
}

bool nemesis::AnimationSetDataAnimation::Parser::IsAnimationEnd(nemesis::LineStream& stream)
{
    auto& token = stream.GetToken();

    if (token.Type != nemesis::LineStream::NONE) return false;

    auto& val = token.Value;

    if (isOnlyNumber(val))
    {
        const nemesis::LineStream::Token* token_1;
        size_t step = 1;

        while (token.Type != nemesis::LineStream::NONE)
        {
            token_1 = stream.GetForwardToken(step);
        }

        if (!token_1) return false;

        if (token_1->Value == "V3") return true;

        if (isOnlyNumber(token_1->Value))
        {
            auto token_2 = stream.GetForwardToken(2);

            if (!token_2) return false;

            if (isOnlyNumber(token_2->Value)) return true;

            do
            {
                if (token_2->Value == "V3") return true;
            } while (!isOnlyNumber(token_2->Value));
        }
    }

    return false;
}

void nemesis::AnimationSetDataAnimation::Parser::SetValue(nemesis::LineStream& stream,
                                                          nemesis::SemanticManager& manager)
{
    auto& token      = stream.GetToken();
    auto& parse_type = ParseTypeLayer.back();

    switch (parse_type)
    {
        case nemesis::AnimationSetDataAnimation::Parser::EVENT_NAME:
        {
            parse_type                 = ParsingObjectType::UNKNOWN;
            AnimationObject->EventName = std::make_unique<nemesis::NLine>(token.Value, manager);
            return;
        }
        case nemesis::AnimationSetDataAnimation::Parser::UNKNOWN:
        {
            parse_type               = ParsingObjectType::ANIMATION_LIST_COUNT;
            AnimationObject->Unknown = std::make_unique<nemesis::NLine>(token.Value, manager);
            return;
        }
        case nemesis::AnimationSetDataAnimation::Parser::ANIMATION_LIST_COUNT:
        {
            parse_type = ParsingObjectType::ANIMATION_NAME;
            return;
        }
    }
}

void nemesis::AnimationSetDataAnimation::Parser::SetModValue(nemesis::LineStream& stream,
                                                             nemesis::SemanticManager& manager)
{
    stream++;
    auto& token  = stream.GetToken();
    auto objects = nemesis::NObjectParser::ParseModObjects(
        stream, manager, [this](nemesis::LineStream& s) { return !IsAnimationEnd(s); });
    auto& parse_type = ParseTypeLayer.back();

    switch (parse_type)
    {
        case nemesis::AnimationSetDataAnimation::Parser::EVENT_NAME:
        {
            switch (objects.size())
            {
                case 2:
                {
                    AnimationObject->Unknown
                        = UPtr<nemesis::NLine>(static_cast<nemesis::NLine*>(objects.back().release()));
                }
                case 1:
                {
                    AnimationObject->EventName
                        = UPtr<nemesis::NLine>(static_cast<nemesis::NLine*>(objects.front().release()));
                    break;
                }
                default:
                {
                    throw std::runtime_error(
                        "Syntax error: incorrect AnimationSetDataAnimation format (Line: "
                        + std::to_string(token.Value.GetLineNumber())
                        + ", File: " + token.Value.GetFilePath().string() + ")");
                }
            }

            parse_type = static_cast<ParsingObjectType>(static_cast<int>(parse_type) + objects.size());
            break;
        }
        case nemesis::AnimationSetDataAnimation::Parser::UNKNOWN:
        {
            if (objects.size() != 1)
            {
                throw std::runtime_error("Syntax error: incorrect AnimationSetDataAnimation format (Line: "
                                         + std::to_string(token.Value.GetLineNumber())
                                         + ", File: " + token.Value.GetFilePath().string() + ")");
            }

            parse_type = ParsingObjectType::ANIMATION_LIST_COUNT;
            AnimationObject->Unknown
                = UPtr<nemesis::NLine>(static_cast<nemesis::NLine*>(objects.back().release()));
            break;
        }
        case nemesis::AnimationSetDataAnimation::Parser::ANIMATION_LIST_COUNT:
        {
            parse_type = ParsingObjectType::ANIMATION_NAME;
            break;
        }
    }
}

UPtr<nemesis::NObject>
nemesis::AnimationSetDataAnimation::Parser::ParseObject(nemesis::LineStream& stream,
                                                        nemesis::SemanticManager& manager)
{
    auto& token       = stream.GetToken();
    auto& token_value = token.Value;

    AnimationObject   = std::make_unique<nemesis::AnimationSetDataAnimation>();
    AnimationNameList = std::make_unique<nemesis::CollectionObject>();

    auto col_ptr = AnimationNameList.get();

    Vec<Writer> writers;
    auto& parse_type = ParseTypeLayer.emplace_back(ParsingObjectType::EVENT_NAME);
    Writers.emplace_back(parse_type);

    switch (token.Type)
    {
        case nemesis::LineStream::TokenType::IF:
        {
            ++stream;

            for (; ParseTypeLayer.size() != 1 || ParseTypeLayer.back() != ParsingObjectType::ANIMATION_LIST_COUNT; ++stream)
            {
                auto& ntoken = stream.GetToken();

                if (ntoken.Type == nemesis::LineStream::TokenType::MOD_OPEN)
                {
                    //SetModValue(stream, manager);

                    auto objects = nemesis::NObjectParser::ParseModObjects(
                        stream, manager, [this](nemesis::LineStream& s) { return !IsAnimationEnd(s); });

                    for (auto& object : objects)
                    {
                        Writers.back().Prepare(std::move(object));
                    }
                }
                else if (ntoken.Type == nemesis::LineStream::TokenType::END_IF)
                {
                }
                else
                {
                    SetValue(stream, manager);
                }
            }

            auto& ftoken = stream.GetToken();

            if (ftoken.Type == nemesis::LineStream::TokenType::END_IF)
            {
                if (parse_type == ParsingObjectType::ANIMATION_LIST_COUNT)
                { 
                    AnimationObject->EventName
                        = std::make_unique<nemesis::IfObject>(token_value,
                                                              token_value.GetLineNumber(),
                                                              token_value.GetFilePath(),
                                                              manager,
                                                              std::move(AnimationObject->EventName));
                    AnimationObject->Unknown
                        = std::make_unique<nemesis::IfObject>(token_value,
                                                              token_value.GetLineNumber(),
                                                              token_value.GetFilePath(),
                                                              manager,
                                                              std::move(AnimationObject->Unknown));
                }
            }

            switch (parse_type)
            {
                case ParsingObjectType::UNKNOWN:
                {
                    auto if_obj                = std::make_unique<nemesis::IfObject>(token_value,
                                                                      token_value.GetLineNumber(),
                                                                      token_value.GetFilePath(),
                                                                      manager,
                                                                      std::move(AnimationObject->EventName));
                    AnimationObject->EventName = std::move(if_obj);
                    break;
                }
            }

            throw std::runtime_error("Syntax Error: Unclosed If Statement (Line: "
                                     + std::to_string(token_value.GetLineNumber())
                                     + ", File: " + token_value.GetFilePath().string() + ")");
        }
        case nemesis::LineStream::TokenType::NONE:
        {
            for (; parse_type != ParsingObjectType::ANIMATION_LIST_COUNT; ++stream)
            {
                auto& ntoken = stream.GetToken();

                if (ntoken.Type == nemesis::LineStream::TokenType::MOD_OPEN)
                {
                    SetModValue(stream, manager);
                    continue;
                }

                SetValue(stream, manager);
            }

            for (; !IsAnimationEnd(stream); ++stream)
            {
                auto& ntoken = stream.GetToken();

                if (ntoken.Type != nemesis::LineStream::TokenType::MOD_OPEN)
                {
                    auto object = nemesis::NObjectParser::ParseLine(stream, manager);
                    AnimationNameList->AddObject(std::move(object));
                    continue;
                }

                auto objects = nemesis::NObjectParser::ParseModObjects(
                    stream, manager, [this](nemesis::LineStream& s) { return !IsAnimationEnd(s); });

                for (auto& object : objects)
                {
                    AnimationNameList->AddObject(std::move(object));
                }
            }

            AnimationObject->AnimationNameList = std::move(AnimationNameList);
            return std::move(AnimationObject);
        }
        default:
        {
            throw std::runtime_error("Syntax Error: Unsupported syntax (Line: "
                                     + std::to_string(token_value.GetLineNumber())
                                     + ", File: " + token_value.GetFilePath().string() + ")");
        }
    }
}

void nemesis::AnimationSetDataAnimation::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    auto* counter = state.GetCounter(bsva2_key);
    (*counter)++;

    EventName->CompileTo(lines, state);
    Unknown->CompileTo(lines, state);
    
    lines.emplace_back(std::to_string(AnimationNameList->Size()));
    AnimationNameList->CompileTo(lines, state);
}

void nemesis::AnimationSetDataAnimation::SerializeTo(DeqNstr& lines) const
{
    EventName->SerializeTo(lines);
    Unknown->SerializeTo(lines);

    lines.emplace_back(std::to_string(AnimationNameList->Size()));
    AnimationNameList->SerializeTo(lines);
}

void nemesis::AnimationSetDataAnimation::MatchAndUpdate(nemesis::AnimationSetDataAnimation& animation)
{
    auto event_name = static_cast<nemesis::NLine*>(EventName.get());
    auto event_name_2 = static_cast<nemesis::NLine*>(animation.EventName.get());

    if (event_name && event_name_2)
    {
        event_name->MatchAndUpdate(*event_name_2);
    }

    auto unknown   = static_cast<nemesis::NLine*>(Unknown.get());
    auto unknown_2 = static_cast<nemesis::NLine*>(animation.Unknown.get());

    if (unknown && unknown_2)
    {
        unknown->MatchAndUpdate(*unknown_2);
    }

    AnimationNameList->MatchAndUpdate(*animation.AnimationNameList);
}

UPtr<nemesis::NObject> nemesis::AnimationSetDataAnimation::ParseObject(nemesis::LineStream& stream,
                                                                       nemesis::SemanticManager& manager)
{
    Parser parser;
    return parser.ParseObject(stream, manager);
}

void nemesis::AnimationSetDataAnimationList::CompileTo(DeqNstr& lines, nemesis::CompileState& state) const
{
    SPtr<int> counter = std::make_shared<int>(0);
    auto& header = lines.emplace_back("", lines.size() + 1);

    state.AddCounter(bsva2_key, counter.get());

    for (auto& animation : AnimationList)
    {
        animation->CompileTo(lines, state);
    }

    header = std::to_string(*counter);
}

void nemesis::AnimationSetDataAnimationList::SerializeTo(DeqNstr& lines) const
{
    lines.emplace_back(std::to_string(AnimationList.size()));

    for (auto& animation : AnimationList)
    {
        animation->SerializeTo(lines);
    }
}

void nemesis::AnimationSetDataAnimationList::AddAnimation(UPtr<nemesis::NObject>&& animation)
{
    AnimationList.emplace_back(std::move(animation));
}

void nemesis::AnimationSetDataAnimationList::MatchAndUpdate(
    nemesis::AnimationSetDataAnimationList& animation_list)
{
    if (AnimationList.size() > animation_list.AnimationList.size())
    {
        throw std::runtime_error("Failed to update AnimationSetDataAnimationList. Missing animation object");
    }

    size_t base_index      = 0;
    size_t new_index_begin = 0;

    for (size_t i = 0; i < animation_list.AnimationList.size(); i++)
    {
        auto& anim     = animation_list.AnimationList[i];
        auto* anim_ptr = dynamic_cast<nemesis::AnimationSetDataAnimation*>(anim.get());

        if (!anim_ptr)
        {
            AnimationList.insert(AnimationList.begin() + base_index, std::move(anim));
            continue;
        }

        nemesis::AnimationSetDataAnimation* orig_anim_ptr = nullptr;

        for (size_t i = base_index; i < AnimationList.size(); i++)
        {
            auto* cur_ptr = dynamic_cast<nemesis::AnimationSetDataAnimation*>(AnimationList[i].get());

            if (!cur_ptr) continue;

            base_index    = i + 1;
            orig_anim_ptr = cur_ptr;
        }

        if (!orig_anim_ptr)
        {
            if (i == 0) return;

            new_index_begin = i;
            break;
        }

        orig_anim_ptr->MatchAndUpdate(*anim_ptr);
    }

    if (new_index_begin == 0) return;

    for (size_t i = new_index_begin; i < animation_list.AnimationList.size(); i++)
    {
        auto& anim     = animation_list.AnimationList[i];
        auto* anim_ptr = dynamic_cast<nemesis::AnimationSetDataAnimation*>(anim.get());

        if (!anim_ptr)
        {
            throw std::runtime_error(
                "AnimationSetDataAnimation update animation to animation does not match");
        }

        AnimationList.emplace_back(std::move(anim));
    }
}

UPtr<nemesis::NObject> nemesis::AnimationSetDataAnimationList::ParseObject(nemesis::LineStream& stream,
                                                                           nemesis::SemanticManager& manager)
{
    return UPtr<nemesis::NObject>();
}
