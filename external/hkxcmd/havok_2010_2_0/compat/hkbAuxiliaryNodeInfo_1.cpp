// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbAuxiliaryNodeInfo_1.h"

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


static const hkInternalClassEnumItem NodeTypeEnumItems[] =
{
   {0, "NODE_TYPE_UNKNOWN"},
   {1, "NODE_TYPE_NODE"},
   {2, "NODE_TYPE_TRANSITION"},
   {3, "NODE_TYPE_WILDCARD_TRANSITION"},
   {4, "NODE_TYPE_STATE"},
   {5, "NODE_TYPE_STATE_MACHINE"},
   {6, "NODE_TYPE_MODIFIER_GENERATOR"},
   {7, "NODE_TYPE_MODIFIER"},
   {8, "NODE_TYPE_CLIP"},
   {9, "NODE_TYPE_BLEND"},
   {10, "NODE_TYPE_TRANSITION_EFFECT"},
};

static const hkInternalClassEnum hkbAuxiliaryNodeInfoClass_Enums[] = {
   {"NodeType", NodeTypeEnumItems, _countof(NodeTypeEnumItems), HK_NULL, 0 },
};
const hkClassEnum* NodeTypeEnum = reinterpret_cast<const hkClassEnum*>(&hkbAuxiliaryNodeInfoClass_Enums[0]);

static const hkInternalClassMember hkbAuxiliaryNodeInfoClass_Members[] =
{
   { "type",HK_NULL,NodeTypeEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_UINT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbAuxiliaryNodeInfo,m_type) /*8*/,HK_NULL},
   { "depth",HK_NULL,HK_NULL,hkClassMember::TYPE_UINT8,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbAuxiliaryNodeInfo,m_depth) /*9*/,HK_NULL},
   { "referenceBehaviorName",HK_NULL,HK_NULL,hkClassMember::TYPE_STRINGPTR,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbAuxiliaryNodeInfo,m_referenceBehaviorName) /*12*/,HK_NULL},
   { "selfTransitionNames",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRINGPTR,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbAuxiliaryNodeInfo,m_selfTransitionNames) /*16*/,HK_NULL},
};

// Signature:  ca0888ca
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbAuxiliaryNodeInfoClass;
const hkClass hkbAuxiliaryNodeInfoClass(
    "hkbAuxiliaryNodeInfo",
    &hkReferencedObjectClass, // parent
    sizeof(hkbAuxiliaryNodeInfo),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbAuxiliaryNodeInfoClass_Enums), HK_COUNT_OF(hkbAuxiliaryNodeInfoClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbAuxiliaryNodeInfoClass_Members), HK_COUNT_OF(hkbAuxiliaryNodeInfoClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbAuxiliaryNodeInfo, hkbAuxiliaryNodeInfo);

