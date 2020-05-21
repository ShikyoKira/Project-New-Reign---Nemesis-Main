// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "BSDirectAtModifier_0.h"

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


static const hkInternalClassMember BSDirectAtModifierClass_Members[] =
{
   { "directAtTarget",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSDirectAtModifier,m_directAtTarget) /*44*/,HK_NULL},
   { "sourceBoneIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSDirectAtModifier,m_sourceBoneIndex) /*46*/,HK_NULL},
   { "startBoneIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSDirectAtModifier,m_startBoneIndex) /*48*/,HK_NULL},
   { "endBoneIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSDirectAtModifier,m_endBoneIndex) /*50*/,HK_NULL},
   { "limitHeadingDegrees",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSDirectAtModifier,m_limitHeadingDegrees) /*52*/,HK_NULL},
   { "limitPitchDegrees",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSDirectAtModifier,m_limitPitchDegrees) /*56*/,HK_NULL},
   { "offsetHeadingDegrees",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSDirectAtModifier,m_offsetHeadingDegrees) /*60*/,HK_NULL},
   { "offsetPitchDegrees",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSDirectAtModifier,m_offsetPitchDegrees) /*64*/,HK_NULL},
   { "onGain",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSDirectAtModifier,m_onGain) /*68*/,HK_NULL},
   { "offGain",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSDirectAtModifier,m_offGain) /*72*/,HK_NULL},
   { "targetLocation",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSDirectAtModifier,m_targetLocation) /*80*/,HK_NULL},
   { "userInfo",HK_NULL,HK_NULL,hkClassMember::TYPE_UINT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSDirectAtModifier,m_userInfo) /*96*/,HK_NULL},
   { "directAtCamera",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSDirectAtModifier,m_directAtCamera) /*100*/,HK_NULL},
   { "directAtCameraX",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSDirectAtModifier,m_directAtCameraX) /*104*/,HK_NULL},
   { "directAtCameraY",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSDirectAtModifier,m_directAtCameraY) /*108*/,HK_NULL},
   { "directAtCameraZ",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSDirectAtModifier,m_directAtCameraZ) /*112*/,HK_NULL},
   { "active",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSDirectAtModifier,m_active) /*116*/,HK_NULL},
   { "currentHeadingOffset",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSDirectAtModifier,m_currentHeadingOffset) /*120*/,HK_NULL},
   { "currentPitchOffset",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSDirectAtModifier,m_currentPitchOffset) /*124*/,HK_NULL},
   { "timeStep",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSDirectAtModifier,m_timeStep) /*128*/,HK_NULL},
   { "pSkeletonMemory",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSDirectAtModifier,m_pSkeletonMemory) /*132*/,HK_NULL},
   { "hasTarget",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSDirectAtModifier,m_hasTarget) /*136*/,HK_NULL},
   { "directAtTargetLocation",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSDirectAtModifier,m_directAtTargetLocation) /*144*/,HK_NULL},
   { "boneChainIndices",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSDirectAtModifier,m_boneChainIndices) /*160*/,HK_NULL},
};

// Signature:  19a005c0
extern const hkClass hkbModifierClass;
extern const hkClass BSDirectAtModifierClass;
const hkClass BSDirectAtModifierClass(
    "BSDirectAtModifier",
    &hkbModifierClass, // parent
    sizeof(BSDirectAtModifier),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(BSDirectAtModifierClass_Members), HK_COUNT_OF(BSDirectAtModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(BSDirectAtModifier, BSDirectAtModifier);

