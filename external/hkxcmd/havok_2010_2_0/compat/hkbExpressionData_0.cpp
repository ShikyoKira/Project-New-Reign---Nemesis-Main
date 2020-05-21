// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbExpressionData_0.h"

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


static const hkInternalClassEnumItem ExpressionEventModeEnumItems[] =
{
   {0, "EVENT_MODE_SEND_ONCE"},
   {1, "EVENT_MODE_SEND_ON_TRUE"},
   {2, "EVENT_MODE_SEND_ON_FALSE_TO_TRUE"},
   {3, "EVENT_MODE_SEND_EVERY_FRAME_ONCE_TRUE"},
};

static const hkInternalClassEnum hkbExpressionDataClass_Enums[] = {
   {"ExpressionEventMode", ExpressionEventModeEnumItems, _countof(ExpressionEventModeEnumItems), HK_NULL, 0 },
};
const hkClassEnum* ExpressionEventModeEnum = reinterpret_cast<const hkClassEnum*>(&hkbExpressionDataClass_Enums[0]);

static const hkInternalClassMember hkbExpressionDataClass_Members[] =
{
   { "expression",HK_NULL,HK_NULL,hkClassMember::TYPE_STRINGPTR,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbExpressionData,m_expression) /*0*/,HK_NULL},
   { "assignmentVariableIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbExpressionData,m_assignmentVariableIndex) /*4*/,HK_NULL},
   { "assignmentEventIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbExpressionData,m_assignmentEventIndex) /*8*/,HK_NULL},
   { "eventMode",HK_NULL,ExpressionEventModeEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbExpressionData,m_eventMode) /*12*/,HK_NULL},
   { "raisedEvent",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbExpressionData,m_raisedEvent) /*13*/,HK_NULL},
   { "wasTrueInPreviousFrame",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbExpressionData,m_wasTrueInPreviousFrame) /*14*/,HK_NULL},
};

// Signature:  6740042a
extern const hkClass hkbExpressionDataClass;
const hkClass hkbExpressionDataClass(
    "hkbExpressionData",
    HK_NULL, // parent
    sizeof(hkbExpressionData),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbExpressionDataClass_Enums), HK_COUNT_OF(hkbExpressionDataClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbExpressionDataClass_Members), HK_COUNT_OF(hkbExpressionDataClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(hkbExpressionData, hkbExpressionData);

