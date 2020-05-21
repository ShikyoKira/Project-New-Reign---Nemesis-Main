// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbBehaviorGraphData_2.h"

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


static const hkInternalClassMember hkbBehaviorGraphDataClass_Members[] =
{
   { "attributeDefaults",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_REAL,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBehaviorGraphData,m_attributeDefaults) /*8*/,HK_NULL},
   { "variableInfos",&hkbVariableInfoClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBehaviorGraphData,m_variableInfos) /*20*/,HK_NULL},
   { "characterPropertyInfos",&hkbVariableInfoClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBehaviorGraphData,m_characterPropertyInfos) /*32*/,HK_NULL},
   { "eventInfos",&hkbEventInfoClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBehaviorGraphData,m_eventInfos) /*44*/,HK_NULL},
   { "wordMinVariableValues",&hkbVariableValueClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBehaviorGraphData,m_wordMinVariableValues) /*56*/,HK_NULL},
   { "wordMaxVariableValues",&hkbVariableValueClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBehaviorGraphData,m_wordMaxVariableValues) /*68*/,HK_NULL},
   { "variableInitialValues",&hkbVariableValueSetClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBehaviorGraphData,m_variableInitialValues) /*80*/,HK_NULL},
   { "stringData",&hkbBehaviorGraphStringDataClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBehaviorGraphData,m_stringData) /*84*/,HK_NULL},
};

// Signature:  095aca5d
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbBehaviorGraphDataClass;
const hkClass hkbBehaviorGraphDataClass(
    "hkbBehaviorGraphData",
    &hkReferencedObjectClass, // parent
    sizeof(hkbBehaviorGraphData),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbBehaviorGraphDataClass_Members), HK_COUNT_OF(hkbBehaviorGraphDataClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    2 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbBehaviorGraphData, hkbBehaviorGraphData);

