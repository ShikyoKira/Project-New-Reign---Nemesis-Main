#include "core/LineStream.h"

#include "core/ModLine.h"
#include "core/IfObject.h"
#include "core/EmptyObject.h"
#include "core/ForEachObject.h"

#include "utilities/conditionsyntax.h"

namespace ns = nemesis::syntax;

nemesis::LineStream::Token::Token(const std::string& value,
                                  size_t linenum,
                                  const std::filesystem::path& filepath,
                                  TokenType type) noexcept
    : Value(value, linenum, filepath)
    , Type(type)
{
}

nemesis::LineStream::Token::Token(const nemesis::Line& value, TokenType type) noexcept
    : Value(value)
    , Type(type)
{
}

nemesis::LineStream::TokenParser::TokenParser()
    : StringMatcher([] { return std::make_pair("", nemesis::LineStream::NONE); })
{
    StringMatcher.Insert(ns::ModCode(),
                         ns::EndModCodeSyntax(),
                         [](const std::string& body)
                         { return std::make_pair(body, nemesis::LineStream::MOD_OPEN); });

    StringMatcher.Insert(ns::ModOriginal(),
                         [](const std::string& body)
                         { return std::make_pair("", nemesis::LineStream::MOD_ORG); });

    StringMatcher.Insert(ns::ModClose(),
                         [](const std::string& body)
                         { return std::make_pair("", nemesis::LineStream::MOD_CLOSE); });

    StringMatcher.Insert(ns::If(),
                         ns::EndSyntax(),
                         [](const std::string& body)
                         { return std::make_pair(body, nemesis::LineStream::IF); });

    StringMatcher.Insert(ns::ElseIf(),
                         ns::EndSyntax(),
                         [](const std::string& body)
                         { return std::make_pair(body, nemesis::LineStream::ELSE_IF); });

    StringMatcher.Insert(ns::Else(),
                         [](const std::string& body)
                         { return std::make_pair("", nemesis::LineStream::ELSE); });

    StringMatcher.Insert(ns::EndIf(),
                         [](const std::string& body)
                         { return std::make_pair("", nemesis::LineStream::END_IF); });

    StringMatcher.Insert(ns::ForEach(),
                         ns::EndSyntax(),
                         [](const std::string& body)
                         { return std::make_pair(body, nemesis::LineStream::FOR_EACH); });

    StringMatcher.Insert(ns::Close(),
                         [](const std::string& body)
                         { return std::make_pair("", nemesis::LineStream::CLOSE); });

    StringMatcher.Insert(ns::Break(),
                         ns::EndSyntax(),
                         [](const std::string& body)
                         { return std::make_pair(body, nemesis::LineStream::BREAK); });
}

UPtr<nemesis::LineStream::Token> nemesis::LineStream::TokenParser::GenerateToken(const nemesis::Line& line)
{
    auto pair = StringMatcher.Match(line);

    if (pair.second == nemesis::LineStream::NONE)
    {
        return std::make_unique<nemesis::LineStream::Token>(line, pair.second);
    }

    return std::make_unique<nemesis::LineStream::Token>(pair.first, line.GetLineNumber(), line.GetFilePath(), pair.second);
}

const nemesis::LineStream::Token* nemesis::LineStream::GenerateToken(VecNstr::const_iterator itr)
{
    const auto& nline = *itr;
    int pos           = itr - Start;
    auto citr          = TokenPointers.find(pos);

    if (citr != TokenPointers.end()) return citr->second.get();

    static nemesis::LineStream::TokenParser Parser;
    return (TokenPointers[pos] = Parser.GenerateToken(nline)).get();
}

nemesis::LineStream::LineStream(VecNstr::const_iterator start, VecNstr::const_iterator end) noexcept
    : Start(start)
    , Iterator(start)
    , End(end)
{
}

bool nemesis::LineStream::IsEoF() const noexcept
{
    return Iterator == End;
}

nemesis::LineStream& nemesis::LineStream::operator++() noexcept
{
    Iterator++;
    return *this;
}

nemesis::LineStream& nemesis::LineStream::operator--() noexcept
{
    Iterator--;
    return *this;
}

nemesis::LineStream& nemesis::LineStream::operator+=(int offset) noexcept
{
    Iterator += offset;
    return *this;
}

nemesis::LineStream& nemesis::LineStream::operator-=(int offset) noexcept
{
    Iterator -= offset;
    return *this;
}

const nemesis::Line& nemesis::LineStream::operator*() noexcept
{
    return *Iterator;
}

const nemesis::LineStream::Token& nemesis::LineStream::GetToken()
{
    return *GenerateToken(Iterator);
}

const nemesis::LineStream::Token* nemesis::LineStream::GetForwardToken(size_t step)
{
    auto cur_itr = Iterator + step;

    if (cur_itr >= End) return nullptr;

    return GenerateToken(cur_itr);
}

const nemesis::LineStream::Token* nemesis::LineStream::GetBackwardToken(size_t step)
{
    auto cur_itr = Iterator - step;

    if (cur_itr < Start) return nullptr;

    return GenerateToken(cur_itr);
}

int nemesis::LineStream::GetPosition() const noexcept
{
    return Iterator - Start;
}

size_t nemesis::LineStream::GetSize() const noexcept
{
    return End - Start;
}
