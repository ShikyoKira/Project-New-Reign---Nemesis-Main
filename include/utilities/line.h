#pragma once

#include <string>
#include <QtCore\qvarlengtharray.h>

using uint = unsigned int;

namespace nemesis
{
    struct ConditionInfo;

	struct Line
    {
        using RawChar  = char;
        using RawType  = std::string;
        using ViewType = std::string_view;

    protected:
        RawType base;
        uint linenum = 0;

    public:
        Line() = default;
        Line(uint _linenum);
        Line(const RawType& _ch) noexcept;
        Line(const RawChar* _ch, uint linenum = 0) noexcept;
        Line(const RawType& _ch, uint linenum) noexcept;

        operator RawType &() noexcept;
        operator RawType() const noexcept;
        RawChar& operator[](uint index) noexcept;
        const RawChar& operator[](uint index) const noexcept;
        bool operator==(const RawType& str) noexcept;
        bool operator==(const RawType& str) const noexcept;
        bool operator!=(const RawType& str) noexcept;
        bool operator!=(const RawType& str) const noexcept;
        RawType operator+(const RawChar* str) noexcept;
        RawType operator+(const RawChar* str) const noexcept;
        RawType operator+(const RawType& str) noexcept;
        RawType operator+(const RawType& str) const noexcept;
        RawType operator+(const nemesis::Line& str) noexcept;
        RawType operator+(const nemesis::Line& str) const noexcept;
        nemesis::Line& operator+=(const RawChar* str);
        nemesis::Line& operator+=(const RawType& str);
        nemesis::Line& operator+=(const nemesis::Line& str);
        nemesis::Line& operator=(const nemesis::Line& line);
        nemesis::Line& operator=(const RawType& str);
        nemesis::Line& operator=(const ViewType& sv);

        size_t find(const RawChar* _ch, size_t _off = 0) const noexcept;
        size_t find(const RawType& _str, size_t _off = 0) const noexcept;
        size_t find(const ViewType& _sv, size_t _off = 0) const noexcept;
        size_t find(const nemesis::Line& _line, size_t _off = 0) const noexcept;

        size_t rfind(const RawChar* _ch, size_t _off = 0) const noexcept;
        size_t rfind(const RawType& _str, size_t _off = 0) const noexcept;
        size_t rfind(const ViewType& _sv, size_t _off = 0) const noexcept;
        size_t rfind(const nemesis::Line& _line, size_t _off = 0) const noexcept;

        RawType::iterator begin() noexcept;
        RawType::const_iterator begin() const noexcept;
        RawType::iterator end() noexcept;
        RawType::const_iterator end() const noexcept;

        RawType::reverse_iterator rbegin() noexcept;
        RawType::const_reverse_iterator rbegin() const noexcept;
        RawType::reverse_iterator rend() noexcept;
        RawType::const_reverse_iterator rend() const noexcept;

        nemesis::Line& append(const RawChar* str);
        nemesis::Line& append(const RawType& str);
        nemesis::Line& append(const nemesis::Line& line);

        nemesis::Line substr(uint _off, uint _count = 4294967295U) const;

        RawChar front() const noexcept;
        RawChar back() const noexcept;

        uint length() const noexcept;
        bool empty() const noexcept;
        void clear() noexcept;

        uint GetLineNumber() const noexcept;

        const RawType& ToString() const noexcept;
        const RawChar* c_str() const noexcept;

    private:
        void SetLineNumber(uint linenum);

        friend ConditionInfo;
    };

    struct Wline
    {
        using RawChar = wchar_t;
        using RawType = std::wstring;
        using ViewType = std::wstring_view;

    private:
        RawType base;
        uint linenum;

    public:
        Wline() = default;
        Wline(uint _linenum);
        Wline(const RawType& _wch);
        Wline(const RawChar* _wch, uint _linenum = 0);
        Wline(const RawType& _wch, uint _linenum = 0);

        operator RawType&() noexcept;
        operator RawType() const noexcept;
        RawChar& operator[](uint index) noexcept;
        const RawChar& operator[](uint index) const noexcept;
        bool operator==(const RawType& wstr) noexcept;
        bool operator==(const RawType& wstr) const noexcept;
        bool operator!=(const RawType& wstr) noexcept;
        bool operator!=(const RawType& wstr) const noexcept;
        RawType operator+(const RawChar* wstr) noexcept;
        RawType operator+(const RawChar* wstr) const noexcept;
        RawType operator+(const RawType& wstr) noexcept;
        RawType operator+(const RawType& wstr) const noexcept;
        RawType operator+(const nemesis::Wline& wstr) noexcept;
        RawType operator+(const nemesis::Wline& wstr) const noexcept;
        nemesis::Wline& operator+=(const RawChar* wstr);
        nemesis::Wline& operator+=(const RawType& wstr);
        nemesis::Wline& operator+=(const nemesis::Wline& wstr);
        nemesis::Wline& operator=(const nemesis::Wline& line);
        nemesis::Wline& operator=(const RawType& str);
        nemesis::Wline& operator=(const ViewType& sv);

        size_t find(const RawChar* _wch, size_t _off = 0) const noexcept;
        size_t find(const RawType& str, size_t _off = 0) const noexcept;
        size_t find(const ViewType& _sv, size_t _off = 0) const noexcept;
        size_t find(const nemesis::Wline& _line, size_t _off = 0) const noexcept;

        size_t rfind(const RawChar* _wch, size_t _off = 0) const noexcept;
        size_t rfind(const RawType& _str, size_t _off = 0) const noexcept;
        size_t rfind(const ViewType& _sv, size_t _off = 0) const noexcept;
        size_t rfind(const nemesis::Wline& _line, size_t _off = 0) const noexcept;

        RawType::iterator begin() noexcept;
        RawType::const_iterator begin() const noexcept;
        RawType::iterator end() noexcept;
        RawType::const_iterator end() const noexcept;

        RawType::reverse_iterator rbegin() noexcept;
        RawType::const_reverse_iterator rbegin() const noexcept;
        RawType::reverse_iterator rend() noexcept;
        RawType::const_reverse_iterator rend() const noexcept;

        nemesis::Wline& append(const RawChar* str);
        nemesis::Wline& append(const RawType& str);
        nemesis::Wline& append(const nemesis::Wline& line);

        nemesis::Wline substr(uint _off, uint _count = 4294967295U) const noexcept;

        RawChar front() const noexcept;
        RawChar back() const noexcept;

        uint length() const noexcept;
        bool empty() const noexcept;
        void clear() noexcept;

        uint GetLineNumber() const noexcept;

        const RawType& ToWstring() const noexcept;
        const RawChar* c_str() const noexcept;

    private:
        void SetLineNumber(uint linenum);
    };
}
