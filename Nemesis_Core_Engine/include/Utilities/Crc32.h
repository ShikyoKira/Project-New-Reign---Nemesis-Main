#pragma once

#include <string>

namespace nemesis
{
    class CRC32
    {
    public:
        CRC32();
        ~CRC32();

        bool FileCRC(const char* sFileName, unsigned int* iOutCRC);
        bool FileCRC(const char* sFileName, unsigned int* iOutCRC, unsigned int iBufferSize);
        unsigned int FileCRC(const char* sFileName);
        unsigned int FileCRC(const char* sFileName, unsigned int iBufferSize);

        void FullCRC(const unsigned char* sData, unsigned int iLength, unsigned int* iOutCRC);
        void FullCRC(const char* sData, unsigned int iLength, unsigned int* iOutCRC);
        void FullCRC(const std::string& sData, unsigned int* iOutCRC);
        unsigned int FullCRC(const unsigned char* sData, unsigned int iDataLength);
        unsigned int FullCRC(const char* sData, unsigned int iDataLength);
        unsigned int FullCRC(const std::string& sData);

        void PartialCRC(unsigned int* iCRC, const unsigned char* sData, unsigned int iDataLength);

    private:
        unsigned int Reflect(unsigned int iReflect, const char cChar);

        //0x04C11DB7 is the official polynomial used by PKZip, WinZip and Ethernet.
        unsigned int iPolynomial = 0x04C11DB7;
        unsigned int initial  = 0;
        unsigned int finalxor = 0;
        unsigned int iTable[256];
    };
}