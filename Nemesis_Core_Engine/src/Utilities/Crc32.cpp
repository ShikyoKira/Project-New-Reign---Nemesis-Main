#include <cstring>

#include "Utilities/Crc32.h"

nemesis::CRC32::CRC32(void)
{
    std::memset(&iTable, 0, sizeof(iTable)); 

    // 256 values representing ASCII character codes.
    for (size_t iCodes = 0; iCodes <= 0xFF; iCodes++)
    {
        iTable[iCodes] = Reflect(iCodes, 8) << 24;

        for (unsigned int iPos = 0; iPos < 8; iPos++)
        {
            auto cur       = iTable[iCodes];
            auto base      = cur << 1;
            auto bitshift  = 1 << 31;
            auto flag      = iTable[iCodes] & bitshift;
            auto power     = flag ? iPolynomial : 0;
            iTable[iCodes] = base ^ power;
        }

        iTable[iCodes] = Reflect(iTable[iCodes], 32);
    }
}

nemesis::CRC32::~CRC32(void)
{
}

unsigned int nemesis::CRC32::Reflect(unsigned int iReflect, const char cChar)
{
    unsigned int iValue = 0;
    unsigned int size   = static_cast<unsigned int>(cChar + 1);

    // Swap bit 0 for bit 7, bit 1 For bit 6, etc....
    for (unsigned int iPos = 1; iPos < size; iPos++)
    {
        if (iReflect & 1) iValue |= (1 << (cChar - iPos));

        iReflect >>= 1;
    }

    return iValue;
}

void nemesis::CRC32::PartialCRC(unsigned int* iCRC, const unsigned char* sData, unsigned int iDataLength)
{
    while (iDataLength--)
    {
        *iCRC = (*iCRC >> 8) ^ iTable[(*iCRC & 0xFF) ^ *sData++];
    }
}

void nemesis::CRC32::FullCRC(const unsigned char* sData, unsigned int iDataLength, unsigned int* iOutCRC)
{
    *iOutCRC = initial;

    PartialCRC(iOutCRC, sData, iDataLength);

    *iOutCRC ^= finalxor;
}

void nemesis::CRC32::FullCRC(const char* sData, unsigned int iLength, unsigned int* iOutCRC) 
{
    FullCRC((unsigned char*) sData, iLength, iOutCRC);
}

void nemesis::CRC32::FullCRC(const std::string& sData, unsigned int* iOutCRC) 
{
    FullCRC((unsigned char*) sData.c_str(), sData.length());
}

unsigned int nemesis::CRC32::FullCRC(const unsigned char* sData, unsigned int iDataLength)
{
    unsigned int iCRC = initial;

    PartialCRC(&iCRC, sData, iDataLength);

    return (iCRC ^ finalxor);
}

unsigned int nemesis::CRC32::FullCRC(const char* sData, unsigned int iDataLength)
{
    return FullCRC((unsigned char*) sData, iDataLength);
}

unsigned int nemesis::CRC32::FullCRC(const std::string& sData)
{
    return FullCRC((unsigned char*) sData.c_str(), sData.length());
}

bool nemesis::CRC32::FileCRC(const char* sFileName, unsigned int* iOutCRC, unsigned int iBufferSize)
{
    *iOutCRC = initial;

    FILE* fSource       = NULL;
    unsigned char* sBuf = NULL;
    unsigned int iBytesRead   = 0;

#if _WIN32
    if ((fopen_s(&fSource, sFileName, "rb")) != 0) return false;
#else
    if ((fSource = fopen(sFileName, "rb")) == NULL) return false;
#endif

    if (!(sBuf = (unsigned char*) malloc(iBufferSize)))
    {
        fclose(fSource);
        return false; //Out of memory.
    }

    while ((iBytesRead = fread(sBuf, sizeof(char), iBufferSize, fSource)))
    {
        PartialCRC(iOutCRC, sBuf, iBytesRead);
    }

    free(sBuf);
    fclose(fSource);

    *iOutCRC ^= finalxor;

    return true;
}

unsigned int nemesis::CRC32::FileCRC(const char* sFileName)
{
    unsigned int iCRC;

    if (FileCRC(sFileName, &iCRC, 1048576)) return iCRC;

    return 0xffffffff;
}

unsigned int nemesis::CRC32::FileCRC(const char* sFileName, unsigned int iBufferSize)
{
    unsigned int iCRC;

    if (FileCRC(sFileName, &iCRC, iBufferSize)) return iCRC;

    return 0xffffffff;
}

bool nemesis::CRC32::FileCRC(const char* sFileName, unsigned int* iOutCRC)
{
    return FileCRC(sFileName, iOutCRC, 1048576);
}
