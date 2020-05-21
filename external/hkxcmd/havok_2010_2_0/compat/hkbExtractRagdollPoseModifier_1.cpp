// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbExtractRagdollPoseModifier_1.h"

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


static const hkInternalClassMember hkbExtractRagdollPoseModifierClass_Members[] =
{
   { "poseMatchingBone0",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbExtractRagdollPoseModifier,m_poseMatchingBone0) /*44*/,HK_NULL},
   { "poseMatchingBone1",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbExtractRagdollPoseModifier,m_poseMatchingBone1) /*46*/,HK_NULL},
   { "poseMatchingBone2",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbExtractRagdollPoseModifier,m_poseMatchingBone2) /*48*/,HK_NULL},
   { "enableComputeWorldFromModel",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbExtractRagdollPoseModifier,m_enableComputeWorldFromModel) /*50*/,HK_NULL},
};

// Signature:  804dcbab
extern const hkClass hkbModifierClass;
extern const hkClass hkbExtractRagdollPoseModifierClass;
const hkClass hkbExtractRagdollPoseModifierClass(
    "hkbExtractRagdollPoseModifier",
    &hkbModifierClass, // parent
    sizeof(hkbExtractRagdollPoseModifier),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbExtractRagdollPoseModifierClass_Members), HK_COUNT_OF(hkbExtractRagdollPoseModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbExtractRagdollPoseModifier, hkbExtractRagdollPoseModifier);

