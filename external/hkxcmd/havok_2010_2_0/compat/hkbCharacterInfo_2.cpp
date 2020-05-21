// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbCharacterInfo_2.h"

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


static const hkInternalClassEnumItem EventEnumItems[] =
{
   {0, "REMOVED_FROM_WORLD"},
   {1, "SHOWN"},
   {2, "HIDDEN"},
};

static const hkInternalClassEnum hkbCharacterInfoClass_Enums[] = {
   {"Event", EventEnumItems, _countof(EventEnumItems), HK_NULL, 0 },
};
const hkClassEnum* EventEnum = reinterpret_cast<const hkClassEnum*>(&hkbCharacterInfoClass_Enums[0]);

static const hkInternalClassMember hkbCharacterInfoClass_Members[] =
{
   { "characterId",HK_NULL,HK_NULL,hkClassMember::TYPE_UINT64,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterInfo,m_characterId) /*8*/,HK_NULL},
   { "event",HK_NULL,EventEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_UINT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterInfo,m_event) /*16*/,HK_NULL},
   { "padding",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterInfo,m_padding) /*20*/,HK_NULL},
};

// Signature:  d9709ff2
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbCharacterInfoClass;
const hkClass hkbCharacterInfoClass(
    "hkbCharacterInfo",
    &hkReferencedObjectClass, // parent
    sizeof(hkbCharacterInfo),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbCharacterInfoClass_Enums), HK_COUNT_OF(hkbCharacterInfoClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbCharacterInfoClass_Members), HK_COUNT_OF(hkbCharacterInfoClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    2 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbCharacterInfo, hkbCharacterInfo);

