#include "core/LineStream.h"

#include "core/ModLine.h"
#include "core/IfObject.h"
#include "core/EmptyObject.h"
#include "core/ForEachObject.h"

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
    StringMatcher.Insert("<!-- MOD_CODE_OPEN ~",
                         "~ -->",
                         [](const std::string& body)
                         { return std::make_pair(body, nemesis::LineStream::MOD_OPEN); });

    StringMatcher.Insert(
        "<!-- MOD_CODE_ORIGINAL -->",
        [](const std::string& body)
                         { return std::make_pair("", nemesis::LineStream::MOD_ORG); });

    StringMatcher.Insert("<!-- MOD_CODE_CLOSE -->",
                         [](const std::string& body)
                         { return std::make_pair("", nemesis::LineStream::MOD_CLOSE); });

    StringMatcher.Insert("<!-- IF ^",
                         "^ -->",
                         [](const std::string& body)
                         { return std::make_pair(body, nemesis::LineStream::IF); });

    StringMatcher.Insert("<!-- ELSEIF ^",
                         "^ -->",
                         [](const std::string& body)
                         { return std::make_pair(body, nemesis::LineStream::ELSE_IF); });

    StringMatcher.Insert("<!-- ELSE -->",
                         [](const std::string& body)
                         { return std::make_pair("", nemesis::LineStream::ELSE); });

    StringMatcher.Insert("<!-- ENDIF -->",
                         [](const std::string& body)
                         { return std::make_pair("", nemesis::LineStream::END_IF); });

    StringMatcher.Insert("<!-- FOREACH ^",
                         "^ -->",
                         [](const std::string& body)
                         { return std::make_pair(body, nemesis::LineStream::FOR_EACH); });

    StringMatcher.Insert("<!-- CLOSE -->",
                         [](const std::string& body)
                         { return std::make_pair("", nemesis::LineStream::CLOSE); });
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
    const auto& nline = *Iterator;
    int pos           = Iterator - Start;
    auto itr          = TokenPointers.find(pos);

    if (itr != TokenPointers.end()) return *itr->second;

    static nemesis::LineStream::TokenParser Parser;
    return *(TokenPointers[pos] = Parser.GenerateToken(nline));
}

int nemesis::LineStream::GetPosition() const noexcept
{
    return Iterator - Start;
}
