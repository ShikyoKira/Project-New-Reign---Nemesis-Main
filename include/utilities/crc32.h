#pragma once

#include <string>

using size_t = size_t;

namespace nemesis
{
    class CRC32
    {
    public:
        CRC32();
        ~CRC32();

        bool FileCRC(const char* sFileName, size_t* iOutCRC);
        bool FileCRC(const char* sFileName, size_t* iOutCRC, size_t iBufferSize);
        size_t FileCRC(const char* sFileName);
        size_t FileCRC(const char* sFileName, size_t iBufferSize);

        void FullCRC(const unsigned char* sData, size_t iLength, size_t* iOutCRC);
        void FullCRC(const char* sData, size_t iLength, size_t* iOutCRC);
        void FullCRC(const std::string& sData, size_t* iOutCRC);
        size_t FullCRC(const unsigned char* sData, size_t iDataLength);
        size_t FullCRC(const char* sData, size_t iDataLength);
        size_t FullCRC(const std::string& sData);

        void PartialCRC(size_t* iCRC, const unsigned char* sData, size_t iDataLength);

    private:
        size_t Reflect(size_t iReflect, const char cChar);

        //0x04C11DB7 is the official polynomial used by PKZip, WinZip and Ethernet.
        size_t iPolynomial = 0x04C11DB7;
        size_t initial     = 0;
        size_t finalxor    = 0;
        size_t iTable[256];
    };
}