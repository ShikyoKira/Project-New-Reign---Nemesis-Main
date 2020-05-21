// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbEventBase_0.h"

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


static const hkInternalClassEnumItem SystemEventIdsEnumItems[] =
{
   {-1, "EVENT_ID_NULL"},
};

static const hkInternalClassEnum hkbEventBaseClass_Enums[] = {
   {"SystemEventIds", SystemEventIdsEnumItems, _countof(SystemEventIdsEnumItems), HK_NULL, 0 },
};
const hkClassEnum* SystemEventIdsEnum = reinterpret_cast<const hkClassEnum*>(&hkbEventBaseClass_Enums[0]);

static const hkInternalClassMember hkbEventBaseClass_Members[] =
{
   { "id",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbEventBase,m_id) /*0*/,HK_NULL},
   { "payload",&hkbEventPayloadClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbEventBase,m_payload) /*4*/,HK_NULL},
};

// Signature:  76bddb31
extern const hkClass hkbEventBaseClass;
const hkClass hkbEventBaseClass(
    "hkbEventBase",
    HK_NULL, // parent
    sizeof(hkbEventBase),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbEventBaseClass_Enums), HK_COUNT_OF(hkbEventBaseClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbEventBaseClass_Members), HK_COUNT_OF(hkbEventBaseClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(hkbEventBase, hkbEventBase);

