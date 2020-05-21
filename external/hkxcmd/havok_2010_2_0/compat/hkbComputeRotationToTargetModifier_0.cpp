// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbComputeRotationToTargetModifier_0.h"

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


static const hkInternalClassMember hkbComputeRotationToTargetModifierClass_Members[] =
{
   { "rotationOut",HK_NULL,HK_NULL,hkClassMember::TYPE_QUATERNION,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbComputeRotationToTargetModifier,m_rotationOut) /*48*/,HK_NULL},
   { "targetPosition",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbComputeRotationToTargetModifier,m_targetPosition) /*64*/,HK_NULL},
   { "currentPosition",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbComputeRotationToTargetModifier,m_currentPosition) /*80*/,HK_NULL},
   { "currentRotation",HK_NULL,HK_NULL,hkClassMember::TYPE_QUATERNION,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbComputeRotationToTargetModifier,m_currentRotation) /*96*/,HK_NULL},
   { "localAxisOfRotation",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbComputeRotationToTargetModifier,m_localAxisOfRotation) /*112*/,HK_NULL},
   { "localFacingDirection",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbComputeRotationToTargetModifier,m_localFacingDirection) /*128*/,HK_NULL},
   { "resultIsDelta",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbComputeRotationToTargetModifier,m_resultIsDelta) /*144*/,HK_NULL},
};

// Signature:  47665f1c
extern const hkClass hkbModifierClass;
extern const hkClass hkbComputeRotationToTargetModifierClass;
const hkClass hkbComputeRotationToTargetModifierClass(
    "hkbComputeRotationToTargetModifier",
    &hkbModifierClass, // parent
    sizeof(hkbComputeRotationToTargetModifier),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbComputeRotationToTargetModifierClass_Members), HK_COUNT_OF(hkbComputeRotationToTargetModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbComputeRotationToTargetModifier, hkbComputeRotationToTargetModifier);

