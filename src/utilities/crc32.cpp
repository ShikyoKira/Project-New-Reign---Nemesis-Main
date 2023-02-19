#include "utilities/crc32.h"

nemesis::CRC32::CRC32(void)
{
    memset(&iTable, 0, sizeof(iTable)); 

    // 256 values representing ASCII character codes.
    for (size_t iCodes = 0; iCodes <= 0xFF; iCodes++)
    {
        iTable[iCodes] = Reflect(iCodes, 8) << 24;

        for (size_t iPos = 0; iPos < 8; iPos++)
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

size_t nemesis::CRC32::Reflect(size_t iReflect, const char cChar)
{
    size_t iValue = 0;
    size_t size   = static_cast<size_t>(cChar + 1);

    // Swap bit 0 for bit 7, bit 1 For bit 6, etc....
    for (size_t iPos = 1; iPos < size; iPos++)
    {
        if (iReflect & 1) iValue |= (1 << (cChar - iPos));

        iReflect >>= 1;
    }

    return iValue;
}

void nemesis::CRC32::PartialCRC(size_t* iCRC, const unsigned char* sData, size_t iDataLength)
{
    while (iDataLength--)
    {
        *iCRC = (*iCRC >> 8) ^ iTable[(*iCRC & 0xFF) ^ *sData++];
    }
}

void nemesis::CRC32::FullCRC(const unsigned char* sData, size_t iDataLength, size_t* iOutCRC)
{
    *iOutCRC = initial;

    PartialCRC(iOutCRC, sData, iDataLength);

    *iOutCRC ^= finalxor;
}

void nemesis::CRC32::FullCRC(const char* sData, size_t iLength, size_t* iOutCRC) 
{
    FullCRC((unsigned char*) sData, iLength, iOutCRC);
}

void nemesis::CRC32::FullCRC(const std::string& sData, size_t* iOutCRC) 
{
    FullCRC((unsigned char*) sData.c_str(), sData.length());
}

size_t nemesis::CRC32::FullCRC(const unsigned char* sData, size_t iDataLength)
{
    size_t iCRC = initial;

    PartialCRC(&iCRC, sData, iDataLength);

    return (iCRC ^ finalxor);
}

size_t nemesis::CRC32::FullCRC(const char* sData, size_t iDataLength)
{
    return FullCRC((unsigned char*) sData, iDataLength);
}

size_t nemesis::CRC32::FullCRC(const std::string& sData)
{
    return FullCRC((unsigned char*) sData.c_str(), sData.length());
}

bool nemesis::CRC32::FileCRC(const char* sFileName, size_t* iOutCRC, size_t iBufferSize)
{
    *iOutCRC = initial;

    FILE* fSource       = NULL;
    unsigned char* sBuf = NULL;
    size_t iBytesRead   = 0;

    if ((fopen_s(&fSource, sFileName, "rb")) != 0) return false;

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

size_t nemesis::CRC32::FileCRC(const char* sFileName)
{
    size_t iCRC;

    if (FileCRC(sFileName, &iCRC, 1048576)) return iCRC;

    return 0xffffffff;
}

size_t nemesis::CRC32::FileCRC(const char* sFileName, size_t iBufferSize)
{
    size_t iCRC;

    if (FileCRC(sFileName, &iCRC, iBufferSize)) return iCRC;

    return 0xffffffff;
}

bool nemesis::CRC32::FileCRC(const char* sFileName, size_t* iOutCRC)
{
    return FileCRC(sFileName, iOutCRC, 1048576);
}
