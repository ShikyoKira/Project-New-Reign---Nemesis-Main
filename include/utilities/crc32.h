#pragma once

#include <string>

using uint = unsigned int;

namespace nemesis
{
    class CRC32
    {
    public:
        CRC32();
        ~CRC32();

        bool FileCRC(const char* sFileName, uint* iOutCRC);
        bool FileCRC(const char* sFileName, uint* iOutCRC, size_t iBufferSize);
        uint FileCRC(const char* sFileName);
        uint FileCRC(const char* sFileName, size_t iBufferSize);

        void FullCRC(const unsigned char* sData, size_t iLength, uint* iOutCRC);
        void FullCRC(const char* sData, size_t iLength, uint* iOutCRC);
        void FullCRC(const std::string& sData, uint* iOutCRC);
        uint FullCRC(const unsigned char* sData, size_t iDataLength);
        uint FullCRC(const char* sData, size_t iDataLength);
        uint FullCRC(const std::string& sData);

        void PartialCRC(uint* iCRC, const unsigned char* sData, size_t iDataLength);

    private:
        uint Reflect(uint iReflect, const char cChar);

        //0x04C11DB7 is the official polynomial used by PKZip, WinZip and Ethernet.
        uint iPolynomial = 0x04C11DB7;
        uint initial     = 0;
        uint finalxor    = 0;
        uint iTable[256];
    };
}