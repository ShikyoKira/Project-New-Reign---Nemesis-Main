// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbBehaviorInfoIdToNamePair_0.h"

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


static const hkInternalClassEnumItem ToolNodeTypeEnumItems[] =
{
   {0, "NODE_TYPE_UNKNOWN"},
   {1, "NODE_TYPE_STATE_MACHINE"},
   {2, "NODE_TYPE_CLIP"},
   {3, "NODE_TYPE_BLEND"},
   {4, "NODE_TYPE_MODIFIER"},
   {5, "NODE_TYPE_GENERATOR"},
   {6, "NODE_TYPE_MODIFIER_GENERATOR"},
   {7, "NODE_TYPE_TRANSITION_EFFECT"},
   {8, "NODE_TYPE_BEHAVIOR_FILE_REFERENCE"},
};

static const hkInternalClassEnum hkbBehaviorInfoIdToNamePairClass_Enums[] = {
   {"ToolNodeType", ToolNodeTypeEnumItems, _countof(ToolNodeTypeEnumItems), HK_NULL, 0 },
};
const hkClassEnum* ToolNodeTypeEnum = reinterpret_cast<const hkClassEnum*>(&hkbBehaviorInfoIdToNamePairClass_Enums[0]);

static const hkInternalClassMember hkbBehaviorInfoIdToNamePairClass_Members[] =
{
   { "behaviorName",HK_NULL,HK_NULL,hkClassMember::TYPE_STRINGPTR,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBehaviorInfoIdToNamePair,m_behaviorName) /*0*/,HK_NULL},
   { "nodeName",HK_NULL,HK_NULL,hkClassMember::TYPE_STRINGPTR,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBehaviorInfoIdToNamePair,m_nodeName) /*4*/,HK_NULL},
   { "toolType",HK_NULL,ToolNodeTypeEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_UINT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBehaviorInfoIdToNamePair,m_toolType) /*8*/,HK_NULL},
   { "id",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBehaviorInfoIdToNamePair,m_id) /*10*/,HK_NULL},
};

// Signature:  35a0439a
extern const hkClass hkbBehaviorInfoIdToNamePairClass;
const hkClass hkbBehaviorInfoIdToNamePairClass(
    "hkbBehaviorInfoIdToNamePair",
    HK_NULL, // parent
    sizeof(hkbBehaviorInfoIdToNamePair),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbBehaviorInfoIdToNamePairClass_Enums), HK_COUNT_OF(hkbBehaviorInfoIdToNamePairClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbBehaviorInfoIdToNamePairClass_Members), HK_COUNT_OF(hkbBehaviorInfoIdToNamePairClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(hkbBehaviorInfoIdToNamePair, hkbBehaviorInfoIdToNamePair);

