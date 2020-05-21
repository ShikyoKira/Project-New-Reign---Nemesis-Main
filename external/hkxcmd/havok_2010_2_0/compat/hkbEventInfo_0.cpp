// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbEventInfo_0.h"

#include <Common/Serialize/hkSerialize.h>
#include <Common/Serialize/Util/hkSerializeUtil.h>
#include <Common/Serialize/Version/hkVersionPatchManager.h>
#include <Common/Serialize/Data/Dict/hkDataObjectDict.h>
#include <Common/Serialize/Data/Native/hkDataObjectNative.h>
#include <Common/Serialize/Data/Util/hkDataObjectUtil.h>
#include <Common/Base/Reflection/Registry/hkDynamicClassNameRegistry.h>
#include <Common/Base/Reflection/Registry/hkVtableClassRegistry.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/hkInternalClassMember.h>
#include <Common/Serialize/Util/hkSerializationCheckingUtils.h>
#include <Common/Serialize/Util/hkVersionCheckingUtils.h>


static const hkInternalClassEnumItem FlagsEnumItems[] =
{
   {1, "FLAG_SILENT"},
   {2, "FLAG_SYNC_POINT"},
};

static const hkInternalClassEnum hkbEventInfoClass_Enums[] = {
   {"Flags", FlagsEnumItems, _countof(FlagsEnumItems), HK_NULL, 0 },
};
const hkClassEnum* FlagsEnum = reinterpret_cast<const hkClassEnum*>(&hkbEventInfoClass_Enums[0]);

static const hkInternalClassMember hkbEventInfoClass_Members[] =
{
   { "flags",HK_NULL,FlagsEnum,hkClassMember::TYPE_FLAGS,hkClassMember::TYPE_UINT32,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbEventInfo,m_flags) /*0*/,HK_NULL},
};

// Signature:  5874eed4
extern const hkClass hkbEventInfoClass;
const hkClass hkbEventInfoClass(
    "hkbEventInfo",
    HK_NULL, // parent
    sizeof(hkbEventInfo),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbEventInfoClass_Enums), HK_COUNT_OF(hkbEventInfoClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbEventInfoClass_Members), HK_COUNT_OF(hkbEventInfoClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(hkbEventInfo, hkbEventInfo);

