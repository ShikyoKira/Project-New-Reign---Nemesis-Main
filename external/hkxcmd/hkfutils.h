#pragma once
#include "hkxutils.h"

enum hkPackFormat
{
   HKPF_XML,
   HKPF_DEFAULT,
   HKPF_WIN32,
   HKPF_AMD64,
   HKPF_XBOX,
   HKPF_XBOX360,
   HKPF_TAGFILE,
   HKPF_TAGXML,
};

// Custom enumeration reflection data
extern EnumLookupType PackFlags[];

extern hkPackfileWriter::Options GetWriteOptionsFromFormat(hkPackFormat format);
extern hkPackFormat GetFormatFromLayout(const hkStructureLayout::LayoutRules &rules);

extern void HK_CALL errorReport(const char *msg, void *userContext);

extern hkResource* hkSerializeUtilLoad( hkStreamReader* stream
                                , hkSerializeUtil::ErrorDetails* detailsOut=nullptr
                                , const hkClassNameRegistry* classReg=nullptr
                                , hkSerializeUtil::LoadOptions options=hkSerializeUtil::LOAD_DEFAULT );


extern hkResult hkSerializeUtilSave(hkPackFormat pkFormat, const hkVariant &root, hkOstream &stream
                                    , hkSerializeUtil::SaveOptionBits flags
                                    , const hkPackfileWriter::Options& packFileOptions );
extern hkResult hkSerializeLoad(hkStreamReader *reader, hkVariant &root, hkResource *&resource);
extern hkResult LoadDefaultRegistry();
