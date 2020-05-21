// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbEventsFromRangeModifier_0.h"

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


static const hkInternalClassMember hkbEventsFromRangeModifierClass_Members[] =
{
   { "inputValue",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbEventsFromRangeModifier,m_inputValue) /*44*/,HK_NULL},
   { "lowerBound",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbEventsFromRangeModifier,m_lowerBound) /*48*/,HK_NULL},
   { "eventRanges",&hkbEventRangeDataArrayClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbEventsFromRangeModifier,m_eventRanges) /*52*/,HK_NULL},
   { "wasActiveInPreviousFrame",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbEventsFromRangeModifier,m_wasActiveInPreviousFrame) /*56*/,HK_NULL},
};

// Signature:  bc561b6e
extern const hkClass hkbModifierClass;
extern const hkClass hkbEventsFromRangeModifierClass;
const hkClass hkbEventsFromRangeModifierClass(
    "hkbEventsFromRangeModifier",
    &hkbModifierClass, // parent
    sizeof(hkbEventsFromRangeModifier),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbEventsFromRangeModifierClass_Members), HK_COUNT_OF(hkbEventsFromRangeModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbEventsFromRangeModifier, hkbEventsFromRangeModifier);

