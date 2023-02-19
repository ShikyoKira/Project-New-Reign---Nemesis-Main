#include "animsetdata/animsetdata-crc32.h"

nemesis::animsetdata::CRC32::CRC32(const VecNstr& lines)
{
}

nemesis::animsetdata::CRC32::CRC32(const CRC32& crc32) noexcept
    : filepath(crc32.filepath)
    , filename(crc32.filename)
    , fileformat(crc32.fileformat)
{
}

VecNstr nemesis::animsetdata::CRC32::GetLines() const
{
    VecNstr lines;
    filepath->GetRawData(lines);
    filename->GetRawData(lines);
    fileformat->GetRawData(lines);
    return lines;
}

VecNstr nemesis::animsetdata::CRC32::GetCompiledLines(nemesis::ScopeInfo& scopeinfo) const
{
    VecNstr lines;
    lines.emplace_back(filepath->GetProcessedLines(scopeinfo).front());
    lines.emplace_back(filename->GetProcessedLines(scopeinfo).front());
    lines.emplace_back(fileformat->GetProcessedLines(scopeinfo).front());
    return lines;
}
