// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbComputeRotationFromAxisAngleModifier_0.h"

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


static const hkInternalClassMember hkbComputeRotationFromAxisAngleModifierClass_Members[] =
{
   { "rotationOut",HK_NULL,HK_NULL,hkClassMember::TYPE_QUATERNION,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbComputeRotationFromAxisAngleModifier,m_rotationOut) /*48*/,HK_NULL},
   { "axis",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbComputeRotationFromAxisAngleModifier,m_axis) /*64*/,HK_NULL},
   { "angleDegrees",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbComputeRotationFromAxisAngleModifier,m_angleDegrees) /*80*/,HK_NULL},
};

// Signature:  9b3f6936
extern const hkClass hkbModifierClass;
extern const hkClass hkbComputeRotationFromAxisAngleModifierClass;
const hkClass hkbComputeRotationFromAxisAngleModifierClass(
    "hkbComputeRotationFromAxisAngleModifier",
    &hkbModifierClass, // parent
    sizeof(hkbComputeRotationFromAxisAngleModifier),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbComputeRotationFromAxisAngleModifierClass_Members), HK_COUNT_OF(hkbComputeRotationFromAxisAngleModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbComputeRotationFromAxisAngleModifier, hkbComputeRotationFromAxisAngleModifier);

