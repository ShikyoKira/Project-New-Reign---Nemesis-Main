#include "utilities/line.h"
#include "utilities/linkedvar.h"

using LinkedString = nemesis::LinkedVar<std::string>;
using LinkedWstring = nemesis::LinkedVar<std::wstring>;

nemesis::SharableWrapper<std::filesystem::path>* nemesis::Line::GetFilePathPtr() const noexcept
{
    return r_path ? r_path : s_path.get();
}

nemesis::Line::Line() noexcept
{
}

nemesis::Line::Line(size_t _linenum)
{
    linenum = _linenum;
}

nemesis::Line::Line(const RawType& _ch) noexcept
{
    base = _ch;
}

nemesis::Line::Line(const RawChar* _ch) noexcept
{
    base = _ch;
}

nemesis::Line::Line(const QType _ch) noexcept
{
    base = _ch.toStdString();
}

nemesis::Line::Line(const RawChar* _ch,
                    size_t linenum,
                    SPtr<nemesis::SharableWrapper<std::filesystem::path>> path_ptr) noexcept
{
    base = _ch;
    this->linenum = linenum;
    s_path        = path_ptr;
}

nemesis::Line::Line(const RawType& _ch,
                    size_t linenum,
                    SPtr<nemesis::SharableWrapper<std::filesystem::path>> path_ptr) noexcept
{
    base = _ch;
    this->linenum = linenum;
    s_path        = path_ptr;
}

nemesis::Line::Line(const QType& _ch,
                    size_t linenum,
                    SPtr<nemesis::SharableWrapper<std::filesystem::path>> path_ptr) noexcept
{
    base          = _ch.toStdString();
    this->linenum = linenum;
    s_path        = path_ptr;
}

nemesis::Line::Line(const RawChar* _ch,
                    size_t linenum,
                    nemesis::SharableWrapper<std::filesystem::path>* path_ptr) noexcept
{
    base          = _ch;
    this->linenum = linenum;
    r_path        = path_ptr;
}

nemesis::Line::Line(const RawType& _ch,
                    size_t linenum,
                    nemesis::SharableWrapper<std::filesystem::path>* path_ptr) noexcept
{
    base          = _ch;
    this->linenum = linenum;
    r_path        = path_ptr;
}

nemesis::Line::Line(const QType& _ch,
                    size_t linenum,
                    nemesis::SharableWrapper<std::filesystem::path>* path_ptr) noexcept
{
    base          = _ch.toStdString();
    this->linenum = linenum;
    r_path        = path_ptr;
}

nemesis::Line::Line(const RawType& _ch, const std::filesystem::path& filepath) noexcept
{
    base    = _ch;
    s_path  = std::make_shared<nemesis::SharableWrapper<std::filesystem::path>>(filepath);
}

nemesis::Line::Line(const RawType& _ch, size_t linenum, const std::filesystem::path& filepath) noexcept
{
    base    = _ch;
    this->linenum = linenum;
    s_path = std::make_shared<nemesis::SharableWrapper<std::filesystem::path>>(filepath);
}

nemesis::Line::operator RawType &() noexcept
{
    return base;
}

nemesis::Line::operator RawType() const noexcept
{
    return base;
}

nemesis::Line::RawChar& nemesis::Line::operator[](size_t index) noexcept
{
    return base[index];
}

const nemesis::Line::RawChar& nemesis::Line::operator[](size_t index) const noexcept
{
    return base[index];
}

bool nemesis::Line::operator==(const RawType& str) const noexcept
{
    return base == str;
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

nemesis::Line::RawType nemesis::Line::operator+(const QType& str) noexcept
{
    return base + str.toStdString();
}

nemesis::Line::RawType nemesis::Line::operator+(const QType& str) const noexcept
{
    return base + str.toStdString();
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

nemesis::Line& nemesis::Line::operator+=(const QType& str)
{
    return append(str);
}

nemesis::Line& nemesis::Line::operator+=(const nemesis::Line& str)
{
    return append(str);
}

nemesis::Line& nemesis::Line::operator=(const nemesis::Line& line)
{
    base    = line.base;
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

nemesis::Line& nemesis::Line::operator=(const QType& str)
{
    linenum = 0;
    base    = str.toStdString();
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

nemesis::Line nemesis::Line::substr(size_t _off, size_t _count) const
{
    return nemesis::Line(base.substr(_off, _count), linenum, GetFilePathPtr());
}

nemesis::Line::RawChar nemesis::Line::front() const noexcept
{
    return base.front();
}

nemesis::Line::RawChar nemesis::Line::back() const noexcept
{
    return base.back();
}

size_t nemesis::Line::length() const noexcept
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

nemesis::Line& nemesis::Line::append(const QType& str)
{
    base.append(str.toStdString());
    return *this;
}

nemesis::Line& nemesis::Line::append(const nemesis::Line& line)
{
    base.append(line.base);
    return *this;
}

nemesis::Line& nemesis::Line::insert(size_t pos, const RawChar* str)
{
    if (pos == 0)
    {
        base = str + base;
    }
    else if (pos >= base.length())
    {
        base.append(str);
    }
    else
    {
        base = base.substr(0, pos) + str + base.substr(pos);
    }

    return *this;
}

nemesis::Line& nemesis::Line::insert(size_t pos, const RawType& str)
{
    return insert(pos, str.c_str());
}

nemesis::Line& nemesis::Line::insert(size_t pos, const QType& str)
{
    return insert(pos, str.toStdString());
}

nemesis::Line& nemesis::Line::insert(size_t pos, const nemesis::Line& line)
{
    return insert(pos, line.base);
}

nemesis::Line& nemesis::Line::replace(const RawChar* from, const RawChar* to)
{
    size_t pos = base.find(from);

    if (pos == NOT_FOUND) return *this;

    base.replace(pos, strlen(from), to);
    return *this;
}

nemesis::Line& nemesis::Line::replace(const RawType& from, const RawType& to)
{
    size_t pos = base.find(from);

    if (pos == NOT_FOUND) return *this;

    base.replace(pos, from.length(), to);
    return *this;
}

nemesis::Line& nemesis::Line::replace(const QType& from, const QType& to)
{
    size_t pos = base.find(from.toStdString());

    if (pos == NOT_FOUND) return *this;

    base.replace(pos, from.length(), to.toStdString());
    return *this;
}

nemesis::Line& nemesis::Line::replace(const nemesis::Line& from, const nemesis::Line& to)
{
    size_t pos = base.find(from);

    if (pos == NOT_FOUND) return *this;

    base.replace(pos, from.length(), to);
    return *this;
}

void nemesis::Line::pop_back() noexcept
{
    base.pop_back();
}

size_t nemesis::Line::GetLineNumber() const noexcept
{
    return linenum;
}

std::filesystem::path nemesis::Line::GetFilePath() const noexcept
{
    auto ptr = GetFilePathPtr();

    if (ptr) return ptr->Get();

    return "";
}

std::string nemesis::Line::GetClassName() const noexcept
{
    std::string format  = "Base";
    std::string path    = nemesis::to_lower_copy(GetFilePath().string());
    std::string bhv_tmp = "behavior templates\\";
    auto pos             = nemesis::isearch(path, bhv_tmp);

    if (pos != NOT_FOUND)
    {
        format = std::string(nemesis::between(path, bhv_tmp, "\\"));
    }

    return format;
}

nemesis::Line::WRawType nemesis::Line::ToWstring() const noexcept
{
    return nemesis::transform_to(base);
}

const nemesis::Line::RawType& nemesis::Line::ToString() const noexcept
{
    return base;
}

const nemesis::Line::RawChar* nemesis::Line::c_str() const noexcept
{
    return base.c_str();
}

bool nemesis::Line::HasProcess() const noexcept
{
    return false;
}

nemesis::Line nemesis::Line::Process(nemesis::ScopeInfo& scopeinfo) const
{
    return *this;
}

void nemesis::Line::SetLineNumber(size_t linenum)
{
    this->linenum = linenum;
}

nemesis::SharableWrapper<std::filesystem::path>* nemesis::Wline::GetFilePathPtr() const noexcept
{
    return r_path ? r_path : s_path.get();
}

nemesis::Wline::Wline(size_t _linenum) noexcept
{
    linenum = _linenum;
}

nemesis::Wline::Wline(const RawChar* _wch) noexcept
{
    base = _wch;
}

nemesis::Wline::Wline(const RawType& _wch) noexcept
{
    base = _wch;
}

nemesis::Wline::Wline(const QType& _wch) noexcept
{
    base = _wch.toStdWString();
}

nemesis::Wline::Wline(const RawChar* _wch,
                      size_t linenum,
                      SPtr<nemesis::SharableWrapper<std::filesystem::path>> path_ptr) noexcept
{
    base          = _wch;
    this->linenum = linenum;
    s_path        = path_ptr;
}

nemesis::Wline::Wline(const RawType& _wch,
                      size_t linenum,
                      SPtr<nemesis::SharableWrapper<std::filesystem::path>> path_ptr) noexcept
{
    base          = _wch;
    this->linenum = linenum;
    s_path        = path_ptr;
}

nemesis::Wline::Wline(const QType& _wch,
                      size_t linenum,
                      SPtr<nemesis::SharableWrapper<std::filesystem::path>> path_ptr) noexcept
{
    base          = _wch.toStdWString();
    this->linenum = linenum;
    s_path        = path_ptr;
}

nemesis::Wline::Wline(const RawChar* _wch,
                      size_t linenum,
                      nemesis::SharableWrapper<std::filesystem::path>* path_ptr) noexcept
{
    base          = _wch;
    this->linenum = linenum;
    r_path        = path_ptr;
}

nemesis::Wline::Wline(const RawType& _wch,
                      size_t linenum,
                      nemesis::SharableWrapper<std::filesystem::path>* path_ptr) noexcept
{
    base          = _wch;
    this->linenum = linenum;
    r_path        = path_ptr;
}

nemesis::Wline::Wline(const QType& _wch,
                      size_t linenum,
                      nemesis::SharableWrapper<std::filesystem::path>* path_ptr) noexcept
{
    base          = _wch.toStdWString();
    this->linenum = linenum;
    r_path        = path_ptr;
}

nemesis::Wline::Wline(const nemesis::Wline& line) noexcept
{
    base    = line.base;
    linenum = line.linenum;

    if (line.r_path)
    {
        r_path = line.r_path;
        return;
    }

    s_path = line.s_path;
}

nemesis::Wline::operator RawType&() noexcept
{
    return base;
}

nemesis::Wline::operator RawType() const noexcept
{
    return base;
}

nemesis::Wline::RawChar& nemesis::Wline::operator[](size_t index) noexcept
{
    return base[index];
}

const nemesis::Wline::RawChar& nemesis::Wline::operator[](size_t index) const noexcept
{
    return base[index];
}

bool nemesis::Wline::operator==(const RawType& wstr) const noexcept
{
    return base == wstr;
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

nemesis::Wline::RawType nemesis::Wline::operator+(const QType& wstr) noexcept
{
    return base + wstr.toStdWString();
}

nemesis::Wline::RawType nemesis::Wline::operator+(const QType& wstr) const noexcept
{
    return base + wstr.toStdWString();
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

nemesis::Wline& nemesis::Wline::operator+=(const QType& wstr)
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

nemesis::Wline& nemesis::Wline::operator=(const QType& str)
{
    linenum = 0;
    base    = str.toStdWString();
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

nemesis::Wline& nemesis::Wline::append(const QType& str)
{
    base.append(str.toStdWString());
    return *this;
}

nemesis::Wline& nemesis::Wline::append(const nemesis::Wline& line)
{
    base.append(line.base);
    return *this;
}

nemesis::Wline& nemesis::Wline::insert(size_t pos, const RawChar* str)
{
    if (pos == 0)
    {
        base = str + base;
    }
    else if (pos >= base.length())
    {
        base.append(str);
    }
    else
    {
        base = base.substr(0, pos) + str + base.substr(pos);
    }

    return *this;
}

nemesis::Wline& nemesis::Wline::insert(size_t pos, const RawType& str)
{
    return insert(pos, str.c_str());
}

nemesis::Wline& nemesis::Wline::insert(size_t pos, const QType& str)
{
    return insert(pos, str.toStdWString());
}

nemesis::Wline& nemesis::Wline::insert(size_t pos, const nemesis::Wline& line)
{
    return insert(pos, line.base);
}

nemesis::Wline nemesis::Wline::substr(size_t _off, size_t _count) const noexcept
{
    return nemesis::Wline(base.substr(_off, _count), linenum, GetFilePathPtr());
}

nemesis::Wline::RawChar nemesis::Wline::front() const noexcept
{
    return base.front();
}

nemesis::Wline::RawChar nemesis::Wline::back() const noexcept
{
    return base.back();
}

size_t nemesis::Wline::length() const noexcept
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

size_t nemesis::Wline::GetLineNumber() const noexcept
{
    return linenum;
}

std::filesystem::path nemesis::Wline::GetFilePath() const noexcept
{
    auto ptr = GetFilePathPtr();

    if (ptr) return ptr->Get();

    return "";
}

const nemesis::Wline::RawType& nemesis::Wline::ToWstring() const noexcept
{
    return base;
}

nemesis::Wline::ARawType nemesis::Wline::ToString() const noexcept
{
    return nemesis::transform_to(base);
}

const nemesis::Wline::RawChar* nemesis::Wline::c_str() const noexcept
{
    return base.c_str();
}

void nemesis::Wline::SetLineNumber(size_t linenum)
{
    this->linenum = linenum;
}
