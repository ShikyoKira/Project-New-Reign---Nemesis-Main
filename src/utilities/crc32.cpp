#include "utilities/crc32.h"

nemesis::CRC32::CRC32(void)
{
    memset(&iTable, 0, sizeof(iTable)); 

    // 256 values representing ASCII character codes.
    for (int iCodes = 0; iCodes <= 0xFF; iCodes++)
    {
        iTable[iCodes] = Reflect(iCodes, 8) << 24;

        for (int iPos = 0; iPos < 8; iPos++)
        {
            iTable[iCodes]
                = (iTable[iCodes] << 1) ^ ((iTable[iCodes] & (1 << 31)) ? iPolynomial : 0);
        }

        iTable[iCodes] = Reflect(iTable[iCodes], 32);
    }
}

nemesis::CRC32::~CRC32(void)
{
}

uint nemesis::CRC32::Reflect(uint iReflect, const char cChar)
{
    uint iValue = 0;

    // Swap bit 0 for bit 7, bit 1 For bit 6, etc....
    for (int iPos = 1; iPos < (cChar + 1); iPos++)
    {
        if (iReflect & 1) iValue |= (1 << (cChar - iPos));

        iReflect >>= 1;
    }

    return iValue;
}

void nemesis::CRC32::PartialCRC(uint* iCRC, const unsigned char* sData, size_t iDataLength)
{
    while (iDataLength--)
    {
        *iCRC = (*iCRC >> 8) ^ iTable[(*iCRC & 0xFF) ^ *sData++];
    }
}

void nemesis::CRC32::FullCRC(const unsigned char* sData, size_t iDataLength, uint* iOutCRC)
{
    ((uint) *iOutCRC) = initial;

    PartialCRC(iOutCRC, sData, iDataLength);

    ((uint) *iOutCRC) ^= finalxor;
}

void nemesis::CRC32::FullCRC(const char* sData, size_t iLength, uint* iOutCRC) 
{
    FullCRC((unsigned char*) sData, iLength, iOutCRC);
}

void nemesis::CRC32::FullCRC(const std::string& sData, uint* iOutCRC) 
{
    FullCRC((unsigned char*) sData.c_str(), sData.length());
}

uint nemesis::CRC32::FullCRC(const unsigned char* sData, size_t iDataLength)
{
    uint iCRC = initial;

    PartialCRC(&iCRC, sData, iDataLength);

    return (iCRC ^ finalxor);
}

uint nemesis::CRC32::FullCRC(const char* sData, size_t iDataLength)
{
    return FullCRC((unsigned char*) sData, iDataLength);
}

uint nemesis::CRC32::FullCRC(const std::string& sData)
{
    return FullCRC((unsigned char*) sData.c_str(), sData.length());
}

bool nemesis::CRC32::FileCRC(const char* sFileName, uint* iOutCRC, size_t iBufferSize)
{
    ((uint) *iOutCRC) = initial;

    FILE* fSource       = NULL;
    unsigned char* sBuf = NULL;
    size_t iBytesRead   = 0;

    if ((fopen_s(&fSource, sFileName, "rb")) != 0)
    {
        return false;
    }

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

    ((uint) *iOutCRC) ^= finalxor;

    return true;
}

uint nemesis::CRC32::FileCRC(const char* sFileName)
{
    uint iCRC;

    if (FileCRC(sFileName, &iCRC, 1048576))
    {
        return iCRC;
    }

    return 0xffffffff;
}

uint nemesis::CRC32::FileCRC(const char* sFileName, size_t iBufferSize)
{
    uint iCRC;

    if (FileCRC(sFileName, &iCRC, iBufferSize))
    {
        return iCRC;
    }

    return 0xffffffff;
}

bool nemesis::CRC32::FileCRC(const char* sFileName, uint* iOutCRC)
{
    return FileCRC(sFileName, iOutCRC, 1048576);
}