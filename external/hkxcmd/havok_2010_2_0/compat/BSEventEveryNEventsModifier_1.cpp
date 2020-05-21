// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "BSEventEveryNEventsModifier_1.h"

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


static const hkInternalClassMember BSEventEveryNEventsModifierClass_Members[] =
{
   { "eventToCheckFor",&hkbEventPropertyClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSEventEveryNEventsModifier,m_eventToCheckFor) /*44*/,HK_NULL},
   { "eventToSend",&hkbEventPropertyClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSEventEveryNEventsModifier,m_eventToSend) /*52*/,HK_NULL},
   { "numberOfEventsBeforeSend",HK_NULL,HK_NULL,hkClassMember::TYPE_INT8,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSEventEveryNEventsModifier,m_numberOfEventsBeforeSend) /*60*/,HK_NULL},
   { "minimumNumberOfEventsBeforeSend",HK_NULL,HK_NULL,hkClassMember::TYPE_INT8,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSEventEveryNEventsModifier,m_minimumNumberOfEventsBeforeSend) /*61*/,HK_NULL},
   { "randomizeNumberOfEvents",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSEventEveryNEventsModifier,m_randomizeNumberOfEvents) /*62*/,HK_NULL},
   { "numberOfEventsSeen",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSEventEveryNEventsModifier,m_numberOfEventsSeen) /*64*/,HK_NULL},
   { "calculatedNumberOfEventsBeforeSend",HK_NULL,HK_NULL,hkClassMember::TYPE_INT8,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSEventEveryNEventsModifier,m_calculatedNumberOfEventsBeforeSend) /*68*/,HK_NULL},
};

// Signature:  6030970c
extern const hkClass hkbModifierClass;
extern const hkClass BSEventEveryNEventsModifierClass;
const hkClass BSEventEveryNEventsModifierClass(
    "BSEventEveryNEventsModifier",
    &hkbModifierClass, // parent
    sizeof(BSEventEveryNEventsModifier),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(BSEventEveryNEventsModifierClass_Members), HK_COUNT_OF(BSEventEveryNEventsModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(BSEventEveryNEventsModifier, BSEventEveryNEventsModifier);

