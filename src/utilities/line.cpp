#include "utilities/line.h"
#include "utilities/linkedvar.h"

using LinkedString = nemesis::LinkedVar<std::string>;
using LinkedWstring = nemesis::LinkedVar<std::wstring>;

nemesis::Line::Line(uint _linenum)
{
    linenum = _linenum;
}

nemesis::Line::Line(const RawType& _ch) noexcept
{
    base = _ch;
}

nemesis::Line::Line(const RawChar* _ch, uint linenum) noexcept
{
    base = _ch;
    this->linenum = linenum;
}

nemesis::Line::Line(const RawType& _ch, uint linenum) noexcept
{
    base = _ch;
    this->linenum = linenum;
}

nemesis::Line::operator RawType &() noexcept
{
    return base;
}

nemesis::Line::operator RawType() const noexcept
{
    return base;
}

nemesis::Line::RawChar& nemesis::Line::operator[](uint index) noexcept
{
    return base[index];
}

const nemesis::Line::RawChar& nemesis::Line::operator[](uint index) const noexcept
{
    return base[index];
}

bool nemesis::Line::operator==(const RawType& str) noexcept
{
    return base == str;
}

bool nemesis::Line::operator==(const RawType& str) const noexcept
{
    return base == str;
}

bool nemesis::Line::operator!=(const RawType& str) noexcept
{
    return base != str;
}

bool nemesis::Line::operator!=(const RawType& str) const noexcept
{
    return base != str;
}

nemesis::Line::RawType nemesis::Line::operator+(const RawChar* str) noexcept
{
    return base + str;
}

nemesis::Line::RawType nemesis::Line::operator+(const RawChar* str) const noexcept
{
    return base + str;
}

nemesis::Line::RawType nemesis::Line::operator+(const RawType& str) noexcept
{
    return base + str;
}

nemesis::Line::RawType nemesis::Line::operator+(const RawType& str) const noexcept
{
    return base + str;
}

nemesis::Line::RawType nemesis::Line::operator+(const nemesis::Line& str) noexcept
{
    return base + str.base;
}

nemesis::Line::RawType nemesis::Line::operator+(const nemesis::Line& str) const noexcept
{
    return base + str.base;
}

nemesis::Line& nemesis::Line::operator+=(const RawChar* str)
{
    return append(str);
}

nemesis::Line& nemesis::Line::operator+=(const RawType& str)
{
    return append(str);
}

nemesis::Line& nemesis::Line::operator+=(const nemesis::Line& str)
{
    return append(str);
}

nemesis::Line& nemesis::Line::operator=(const nemesis::Line& line)
{
    base    = line.ToString();
    linenum = line.linenum;
    return *this;
}

nemesis::Line& nemesis::Line::operator=(const RawType& line)
{
    linenum = 0;
    base    = line;
    return *this;
}

nemesis::Line& nemesis::Line::operator=(const ViewType& sv)
{
    linenum = 0;
    base    = RawType(sv);
    return *this;
}

size_t nemesis::Line::find(const RawChar* _ch, size_t _off) const noexcept
{
    return base.find(_ch, _off);
}

size_t nemesis::Line::find(const RawType& _str, size_t _off) const noexcept
{
    return base.find(_str, _off);
}

size_t nemesis::Line::find(const ViewType& _sv, size_t _off) const noexcept
{
    return base.find(_sv, _off);
}

size_t nemesis::Line::find(const nemesis::Line& _line, size_t _off) const noexcept
{
    return base.find(_line.ToString(), _off);
}

size_t nemesis::Line::rfind(const RawChar* _ch, size_t _off) const noexcept
{
    return base.rfind(_ch, _off);
}

size_t nemesis::Line::rfind(const RawType& _str, size_t _off) const noexcept
{
    return base.rfind(_str, _off);
}

size_t nemesis::Line::rfind(const ViewType& _sv, size_t _off) const noexcept
{
    return base.rfind(_sv, _off);
}

size_t nemesis::Line::rfind(const nemesis::Line& _line, size_t _off) const noexcept
{
    return base.rfind(_line.ToString(), _off);
}

nemesis::Line::RawType::iterator nemesis::Line::begin() noexcept
{
    return base.begin();
}

nemesis::Line::RawType::const_iterator nemesis::Line::begin() const noexcept
{
    return base.begin();
}

nemesis::Line::RawType::iterator nemesis::Line::end() noexcept
{
    return base.end();
}

nemesis::Line::RawType::const_iterator nemesis::Line::end() const noexcept
{
    return base.end();
}

nemesis::Line::RawType::reverse_iterator nemesis::Line::rbegin() noexcept
{
    return base.rbegin();
}

nemesis::Line::RawType::const_reverse_iterator nemesis::Line::rbegin() const noexcept
{
    return base.rbegin();
}

nemesis::Line::RawType::reverse_iterator nemesis::Line::rend() noexcept
{
    return base.rend();
}

nemesis::Line::RawType::const_reverse_iterator nemesis::Line::rend() const noexcept
{
    return base.rend();
}

nemesis::Line nemesis::Line::substr(uint _off, uint _count) const
{
    return nemesis::Line(base.substr(_off, _count), linenum);
}

nemesis::Line::RawChar nemesis::Line::front() const noexcept
{
    return base.front();
}

nemesis::Line::RawChar nemesis::Line::back() const noexcept
{
    return base.back();
}

uint nemesis::Line::length() const noexcept
{
    return base.length();
}

bool nemesis::Line::empty() const noexcept
{
    return base.empty();
}

void nemesis::Line::clear() noexcept
{
    base.clear();
}

nemesis::Line& nemesis::Line::append(const RawChar* str)
{
    base.append(str);
    return *this;
}

nemesis::Line& nemesis::Line::append(const RawType& str)
{
    base.append(str);
    return *this;
}

nemesis::Line& nemesis::Line::append(const nemesis::Line& line)
{
    base.append(line.base);
    return *this;
}

uint nemesis::Line::GetLineNumber() const noexcept
{
    return linenum;
}

const nemesis::Line::RawType& nemesis::Line::ToString() const noexcept
{
    return base;
}

const nemesis::Line::RawChar* nemesis::Line::c_str() const noexcept
{
    return base.c_str();
}

void nemesis::Line::SetLineNumber(uint linenum)
{
    this->linenum = linenum;
}

nemesis::Wline::Wline(uint _linenum)
{
    linenum = _linenum;
}

nemesis::Wline::Wline(const RawType& _wch)
{
    base = _wch;
}

nemesis::Wline::Wline(const RawChar* _wch, uint _linenum)
{
    base          = _wch;
    this->linenum = linenum;
}

nemesis::Wline::Wline(const RawType& _wch, uint linenum)
{
    base          = _wch;
    this->linenum = linenum;
}

nemesis::Wline::operator RawType&() noexcept
{
    return base;
}

nemesis::Wline::operator RawType() const noexcept
{
    return base;
}

nemesis::Wline::RawChar& nemesis::Wline::operator[](uint index) noexcept
{
    return base[index];
}

const nemesis::Wline::RawChar& nemesis::Wline::operator[](uint index) const noexcept
{
    return base[index];
}

bool nemesis::Wline::operator==(const RawType& wstr) noexcept
{
    return base == wstr;
}

bool nemesis::Wline::operator==(const RawType& wstr) const noexcept
{
    return base == wstr;
}

bool nemesis::Wline::operator!=(const RawType& wstr) noexcept
{
    return base != wstr;
}

bool nemesis::Wline::operator!=(const RawType& wstr) const noexcept
{
    return base != wstr;
}

nemesis::Wline::RawType nemesis::Wline::operator+(const RawChar* wstr) noexcept
{
    return base + wstr;
}

nemesis::Wline::RawType nemesis::Wline::operator+(const RawChar* wstr) const noexcept
{
    return base + wstr;
}

nemesis::Wline::RawType nemesis::Wline::operator+(const RawType& wstr) noexcept
{
    return base + wstr;
}

nemesis::Wline::RawType nemesis::Wline::operator+(const RawType& wstr) const noexcept
{
    return base + wstr;
}

nemesis::Wline::RawType nemesis::Wline::operator+(const nemesis::Wline& wstr) noexcept
{
    return base + wstr.base;
}

nemesis::Wline::RawType nemesis::Wline::operator+(const nemesis::Wline& wstr) const noexcept
{
    return base + wstr.base;
}

nemesis::Wline& nemesis::Wline::operator+=(const RawChar* wstr)
{
    return append(wstr);
}

nemesis::Wline& nemesis::Wline::operator+=(const RawType& wstr)
{
    return append(wstr);
}

nemesis::Wline& nemesis::Wline::operator+=(const nemesis::Wline& wstr)
{
    return append(wstr);
}

nemesis::Wline& nemesis::Wline::operator=(const nemesis::Wline& line)
{
    base    = line.ToWstring();
    linenum = line.linenum;
    return *this;
}

nemesis::Wline& nemesis::Wline::operator=(const RawType& line)
{
    linenum = 0;
    base    = line;
    return *this;
}

nemesis::Wline& nemesis::Wline::operator=(const ViewType& sv)
{
    linenum = 0;
    base    = RawType(sv);
    return *this;
}

size_t nemesis::Wline::find(const RawChar* _wch, size_t _off) const noexcept
{
    return base.find(_wch, _off);
}

size_t nemesis::Wline::find(const RawType& _str, size_t _off) const noexcept
{
    return base.find(_str, _off);
}

size_t nemesis::Wline::find(const ViewType& _sv, size_t _off) const noexcept
{
    return base.find(_sv, _off);
}

size_t nemesis::Wline::find(const nemesis::Wline& _line, size_t _off) const noexcept
{
    return base.find(_line.ToWstring(), _off);
}

size_t nemesis::Wline::rfind(const RawChar* _wch, size_t _off) const noexcept
{
    return base.rfind(_wch, _off);
}

size_t nemesis::Wline::rfind(const RawType& _str, size_t _off) const noexcept
{
    return base.rfind(_str, _off);
}

size_t nemesis::Wline::rfind(const ViewType& _sv, size_t _off) const noexcept
{
    return base.rfind(_sv, _off);
}

size_t nemesis::Wline::rfind(const nemesis::Wline& _line, size_t _off) const noexcept
{
    return base.rfind(_line.ToWstring(), _off);
}

nemesis::Wline::RawType::iterator nemesis::Wline::begin() noexcept
{
    return base.begin();
}

nemesis::Wline::RawType::const_iterator nemesis::Wline::begin() const noexcept
{
    return base.begin();
}

nemesis::Wline::RawType::iterator nemesis::Wline::end() noexcept
{
    return base.end();
}

nemesis::Wline::RawType::const_iterator nemesis::Wline::end() const noexcept
{
    return base.end();
}

nemesis::Wline::RawType::reverse_iterator nemesis::Wline::rbegin() noexcept
{
    return base.rbegin();
}

nemesis::Wline::RawType::const_reverse_iterator nemesis::Wline::rbegin() const noexcept
{
    return base.rbegin();
}

nemesis::Wline::RawType::reverse_iterator nemesis::Wline::rend() noexcept
{
    return base.rend();
}

nemesis::Wline::RawType::const_reverse_iterator nemesis::Wline::rend() const noexcept
{
    return base.rend();
}

nemesis::Wline& nemesis::Wline::append(const RawChar* str)
{
    base.append(str);
    return *this;
}

nemesis::Wline& nemesis::Wline::append(const RawType& str)
{
    base.append(str);
    return *this;
}

nemesis::Wline& nemesis::Wline::append(const nemesis::Wline& line)
{
    base.append(line.base);
    return *this;
}

nemesis::Wline nemesis::Wline::substr(uint _off, uint _count) const noexcept
{
    return nemesis::Wline(base.substr(_off, _count), linenum);
}

nemesis::Wline::RawChar nemesis::Wline::front() const noexcept
{
    return base.front();
}

nemesis::Wline::RawChar nemesis::Wline::back() const noexcept
{
    return base.back();
}

uint nemesis::Wline::length() const noexcept
{
    return base.length();
}

bool nemesis::Wline::empty() const noexcept
{
    return base.empty();
}

void nemesis::Wline::clear() noexcept
{
    base.clear();
}

uint nemesis::Wline::GetLineNumber() const noexcept
{
    return linenum;
}

const nemesis::Wline::RawType& nemesis::Wline::ToWstring() const noexcept
{
    return base;
}

const nemesis::Wline::RawChar* nemesis::Wline::c_str() const noexcept
{
    return base.c_str();
}

void nemesis::Wline::SetLineNumber(uint linenum)
{
    this->linenum = linenum;
}
