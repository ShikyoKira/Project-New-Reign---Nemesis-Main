// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "BSComputeAddBoneAnimModifier_1.h"

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


static const hkInternalClassMember BSComputeAddBoneAnimModifierClass_Members[] =
{
   { "boneIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSComputeAddBoneAnimModifier,m_boneIndex) /*44*/,HK_NULL},
   { "translationLSOut",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSComputeAddBoneAnimModifier,m_translationLSOut) /*48*/,HK_NULL},
   { "rotationLSOut",HK_NULL,HK_NULL,hkClassMember::TYPE_QUATERNION,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSComputeAddBoneAnimModifier,m_rotationLSOut) /*64*/,HK_NULL},
   { "scaleLSOut",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSComputeAddBoneAnimModifier,m_scaleLSOut) /*80*/,HK_NULL},
   { "pSkeletonMemory",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSComputeAddBoneAnimModifier,m_pSkeletonMemory) /*96*/,HK_NULL},
};

// Signature:  a67f8c46
extern const hkClass hkbModifierClass;
extern const hkClass BSComputeAddBoneAnimModifierClass;
const hkClass BSComputeAddBoneAnimModifierClass(
    "BSComputeAddBoneAnimModifier",
    &hkbModifierClass, // parent
    sizeof(BSComputeAddBoneAnimModifier),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(BSComputeAddBoneAnimModifierClass_Members), HK_COUNT_OF(BSComputeAddBoneAnimModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(BSComputeAddBoneAnimModifier, BSComputeAddBoneAnimModifier);

