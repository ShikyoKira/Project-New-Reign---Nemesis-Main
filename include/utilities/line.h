#pragma once

#include <filesystem>
#include <QtCore\qvarlengtharray.h>

#include "types.h"

#include "base/sharablewrapper.h"

namespace nemesis
{
    struct ScopeInfo;
    struct ConditionInfo;

	struct Line
    {
        using RawChar  = char;
        using WRawType  = std::wstring;
        using RawType  = std::string;
        using ViewType = std::string_view;
        using QType = QString;

    protected:
        RawType base;
        size_t linenum = 0;

        SPtr<nemesis::SharableWrapper<std::filesystem::path>> s_path;
        nemesis::SharableWrapper<std::filesystem::path>* r_path = nullptr;

        nemesis::SharableWrapper<std::filesystem::path>* GetFilePathPtr() const noexcept;

    public:
        Line() = default;
        Line(size_t _linenum);
        Line(const RawType& _ch) noexcept;
        Line(const RawChar* _ch) noexcept;
        Line(const QType _ch) noexcept;
        Line(const RawChar* _ch,
             size_t linenum,
             SPtr<nemesis::SharableWrapper<std::filesystem::path>> path_ptr) noexcept;
        Line(const RawType& _ch,
             size_t linenum,
             SPtr<nemesis::SharableWrapper<std::filesystem::path>> path_ptr) noexcept;
        Line(const QType& _ch,
             size_t linenum,
             SPtr<nemesis::SharableWrapper<std::filesystem::path>> path_ptr) noexcept;
        Line(const RawChar* _ch,
             size_t linenum,
             nemesis::SharableWrapper<std::filesystem::path>* path_ptr = nullptr) noexcept;
        Line(const RawType& _ch,
             size_t linenum,
             nemesis::SharableWrapper<std::filesystem::path>* path_ptr = nullptr) noexcept;
        Line(const QType& _ch,
             size_t linenum,
             nemesis::SharableWrapper<std::filesystem::path>* path_ptr = nullptr) noexcept;
        Line(const RawType& _ch, const std::filesystem::path& filepath) noexcept;
        Line(const RawType& _ch, size_t linenum, const std::filesystem::path& filepath) noexcept;

        operator RawType &() noexcept;
        operator RawType() const noexcept;
        RawChar& operator[](size_t index) noexcept;
        const RawChar& operator[](size_t index) const noexcept;
        bool operator==(const RawType& str) const noexcept;
        bool operator!=(const RawType& str) const noexcept;
        RawType operator+(const RawChar* str) noexcept;
        RawType operator+(const RawChar* str) const noexcept;
        RawType operator+(const RawType& str) noexcept;
        RawType operator+(const RawType& str) const noexcept;
        RawType operator+(const QType& str) noexcept;
        RawType operator+(const QType& str) const noexcept;
        RawType operator+(const nemesis::Line& str) noexcept;
        RawType operator+(const nemesis::Line& str) const noexcept;
        nemesis::Line& operator+=(const RawChar* str);
        nemesis::Line& operator+=(const RawType& str);
        nemesis::Line& operator+=(const QType& str);
        nemesis::Line& operator+=(const nemesis::Line& str);
        nemesis::Line& operator=(const nemesis::Line& line);
        nemesis::Line& operator=(const RawType& str);
        nemesis::Line& operator=(const ViewType& sv);
        nemesis::Line& operator=(const QType& str);

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
        nemesis::Line& append(const QType& str);
        nemesis::Line& append(const nemesis::Line& line);
        
        nemesis::Line& insert(size_t pos, const RawChar* str);
        nemesis::Line& insert(size_t pos, const RawType& str);
        nemesis::Line& insert(size_t pos, const QType& str);
        nemesis::Line& insert(size_t pos, const nemesis::Line& line);

        nemesis::Line& replace(const RawChar* from, const RawChar* to);
        nemesis::Line& replace(const RawType& from, const RawType& to);
        nemesis::Line& replace(const QType& from, const QType& to);
        nemesis::Line& replace(const nemesis::Line& from, const nemesis::Line& to);

        void pop_back() noexcept;

        nemesis::Line substr(size_t _off, size_t _count = 4294967295U) const;

        RawChar front() const noexcept;
        RawChar back() const noexcept;

        size_t length() const noexcept;
        bool empty() const noexcept;
        void clear() noexcept;

        size_t GetLineNumber() const noexcept;
        std::filesystem::path GetFilePath() const noexcept;
        std::string GetClassName() const noexcept;

        WRawType ToWstring() const noexcept;
        const RawType& ToString() const noexcept;
        const RawChar* c_str() const noexcept;

        virtual bool HasProcess() const noexcept;
        virtual nemesis::Line Process(nemesis::ScopeInfo& scopeinfo) const;

    private:
        void SetLineNumber(size_t linenum);

        friend ConditionInfo;
    };

    struct Wline
    {
        using RawChar = wchar_t;
        using RawType = std::wstring;
        using ARawType = std::string;
        using ViewType = std::wstring_view;
        using QType = QString;

    protected:
        RawType base;
        size_t linenum = 0;

        SPtr<nemesis::SharableWrapper<std::filesystem::path>> s_path;
        nemesis::SharableWrapper<std::filesystem::path>* r_path = nullptr;

        nemesis::SharableWrapper<std::filesystem::path>* GetFilePathPtr() const noexcept;

    public:
        Wline() = default;
        Wline(size_t _linenum) noexcept;
        Wline(const RawChar* _wch) noexcept;
        Wline(const RawType& _wch) noexcept;
        Wline(const QType& _wch) noexcept;
        Wline(const RawChar* _wch,
              size_t linenum,
              SPtr<nemesis::SharableWrapper<std::filesystem::path>> path_ptr) noexcept;
        Wline(const RawType& _wch,
              size_t linenum,
              SPtr<nemesis::SharableWrapper<std::filesystem::path>> path_ptr) noexcept;
        Wline(const QType& _wch,
              size_t linenum,
              SPtr<nemesis::SharableWrapper<std::filesystem::path>> path_ptr) noexcept;
        Wline(const RawChar* _wch,
              size_t linenum,
              nemesis::SharableWrapper<std::filesystem::path>* path_ptr = nullptr) noexcept;
        Wline(const RawType& _wch,
              size_t linenum,
              nemesis::SharableWrapper<std::filesystem::path>* path_ptr = nullptr) noexcept;
        Wline(const QType& _wch,
              size_t linenum,
              nemesis::SharableWrapper<std::filesystem::path>* path_ptr = nullptr) noexcept;
        Wline(const nemesis::Wline& line) noexcept;

        operator RawType&() noexcept;
        operator RawType() const noexcept;
        RawChar& operator[](size_t index) noexcept;
        const RawChar& operator[](size_t index) const noexcept;
        bool operator==(const RawType& wstr) const noexcept;
        bool operator!=(const RawType& wstr) const noexcept;
        RawType operator+(const RawChar* wstr) noexcept;
        RawType operator+(const RawChar* wstr) const noexcept;
        RawType operator+(const RawType& wstr) noexcept;
        RawType operator+(const RawType& wstr) const noexcept;
        RawType operator+(const QType& wstr) noexcept;
        RawType operator+(const QType& wstr) const noexcept;
        RawType operator+(const nemesis::Wline& wstr) noexcept;
        RawType operator+(const nemesis::Wline& wstr) const noexcept;
        nemesis::Wline& operator+=(const RawChar* wstr);
        nemesis::Wline& operator+=(const RawType& wstr);
        nemesis::Wline& operator+=(const QType& wstr);
        nemesis::Wline& operator+=(const nemesis::Wline& wstr);
        nemesis::Wline& operator=(const nemesis::Wline& line);
        nemesis::Wline& operator=(const RawType& str);
        nemesis::Wline& operator=(const ViewType& sv);
        nemesis::Wline& operator=(const QType& str);

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
        nemesis::Wline& append(const QType& str);
        nemesis::Wline& append(const nemesis::Wline& line);
        
        nemesis::Wline& insert(size_t pos, const RawChar* str);
        nemesis::Wline& insert(size_t pos, const RawType& str);
        nemesis::Wline& insert(size_t pos, const QType& str);
        nemesis::Wline& insert(size_t pos, const nemesis::Wline& line);

        nemesis::Wline substr(size_t _off, size_t _count = 4294967295U) const noexcept;

        RawChar front() const noexcept;
        RawChar back() const noexcept;

        size_t length() const noexcept;
        bool empty() const noexcept;
        void clear() noexcept;

        size_t GetLineNumber() const noexcept;
        std::filesystem::path GetFilePath() const noexcept;

        const RawType& ToWstring() const noexcept;
        ARawType ToString() const noexcept;
        const RawChar* c_str() const noexcept;

    private:
        void SetLineNumber(size_t linenum);
    };
}
