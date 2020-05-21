// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbEventRangeData_0.h"

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


static const hkInternalClassEnumItem EventRangeModeEnumItems[] =
{
   {0, "EVENT_MODE_SEND_ON_ENTER_RANGE"},
   {1, "EVENT_MODE_SEND_WHEN_IN_RANGE"},
};

static const hkInternalClassEnum hkbEventRangeDataClass_Enums[] = {
   {"EventRangeMode", EventRangeModeEnumItems, _countof(EventRangeModeEnumItems), HK_NULL, 0 },
};
const hkClassEnum* EventRangeModeEnum = reinterpret_cast<const hkClassEnum*>(&hkbEventRangeDataClass_Enums[0]);

static const hkInternalClassMember hkbEventRangeDataClass_Members[] =
{
   { "upperBound",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbEventRangeData,m_upperBound) /*0*/,HK_NULL},
   { "event",&hkbEventPropertyClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbEventRangeData,m_event) /*4*/,HK_NULL},
   { "eventMode",HK_NULL,EventRangeModeEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbEventRangeData,m_eventMode) /*12*/,HK_NULL},
};

// Signature:  6cb92c76
extern const hkClass hkbEventRangeDataClass;
const hkClass hkbEventRangeDataClass(
    "hkbEventRangeData",
    HK_NULL, // parent
    sizeof(hkbEventRangeData),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbEventRangeDataClass_Enums), HK_COUNT_OF(hkbEventRangeDataClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbEventRangeDataClass_Members), HK_COUNT_OF(hkbEventRangeDataClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(hkbEventRangeData, hkbEventRangeData);

