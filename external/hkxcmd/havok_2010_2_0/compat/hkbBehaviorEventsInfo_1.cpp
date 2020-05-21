// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbBehaviorEventsInfo_1.h"

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


static const hkInternalClassMember hkbBehaviorEventsInfoClass_Members[] =
{
   { "characterId",HK_NULL,HK_NULL,hkClassMember::TYPE_UINT64,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBehaviorEventsInfo,m_characterId) /*8*/,HK_NULL},
   { "externalEventIds",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_INT16,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBehaviorEventsInfo,m_externalEventIds) /*16*/,HK_NULL},
   { "padding",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBehaviorEventsInfo,m_padding) /*28*/,HK_NULL},
};

// Signature:  66840004
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbBehaviorEventsInfoClass;
const hkClass hkbBehaviorEventsInfoClass(
    "hkbBehaviorEventsInfo",
    &hkReferencedObjectClass, // parent
    sizeof(hkbBehaviorEventsInfo),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbBehaviorEventsInfoClass_Members), HK_COUNT_OF(hkbBehaviorEventsInfoClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbBehaviorEventsInfo, hkbBehaviorEventsInfo);

