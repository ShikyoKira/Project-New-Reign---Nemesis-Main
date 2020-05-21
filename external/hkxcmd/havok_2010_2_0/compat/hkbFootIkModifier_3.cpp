// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbFootIkModifier_3.h"

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


static const hkInternalClassEnumItem AlignModeEnumItems[] =
{
   {0, "ALIGN_MODE_FORWARD_RIGHT"},
   {1, "ALIGN_MODE_FORWARD"},
};

static const hkInternalClassEnum hkbFootIkModifierClass_Enums[] = {
   {"AlignMode", AlignModeEnumItems, _countof(AlignModeEnumItems), HK_NULL, 0 },
};
const hkClassEnum* AlignModeEnum = reinterpret_cast<const hkClassEnum*>(&hkbFootIkModifierClass_Enums[0]);

static const hkInternalClassMember hkbFootIkModifierClass_Members[] =
{
   { "gains",&hkbFootIkGainsClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifier,m_gains) /*44*/,HK_NULL},
   { "legs",&hkbFootIkModifierLegClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifier,m_legs) /*92*/,HK_NULL},
   { "raycastDistanceUp",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifier,m_raycastDistanceUp) /*104*/,HK_NULL},
   { "raycastDistanceDown",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifier,m_raycastDistanceDown) /*108*/,HK_NULL},
   { "originalGroundHeightMS",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifier,m_originalGroundHeightMS) /*112*/,HK_NULL},
   { "errorOut",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifier,m_errorOut) /*116*/,HK_NULL},
   { "errorOutTranslation",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifier,m_errorOutTranslation) /*128*/,HK_NULL},
   { "alignWithGroundRotation",HK_NULL,HK_NULL,hkClassMember::TYPE_QUATERNION,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifier,m_alignWithGroundRotation) /*144*/,HK_NULL},
   { "verticalOffset",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifier,m_verticalOffset) /*160*/,HK_NULL},
   { "collisionFilterInfo",HK_NULL,HK_NULL,hkClassMember::TYPE_UINT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifier,m_collisionFilterInfo) /*164*/,HK_NULL},
   { "forwardAlignFraction",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifier,m_forwardAlignFraction) /*168*/,HK_NULL},
   { "sidewaysAlignFraction",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifier,m_sidewaysAlignFraction) /*172*/,HK_NULL},
   { "sidewaysSampleWidth",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifier,m_sidewaysSampleWidth) /*176*/,HK_NULL},
   { "useTrackData",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifier,m_useTrackData) /*180*/,HK_NULL},
   { "lockFeetWhenPlanted",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifier,m_lockFeetWhenPlanted) /*181*/,HK_NULL},
   { "useCharacterUpVector",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifier,m_useCharacterUpVector) /*182*/,HK_NULL},
   { "alignMode",HK_NULL,AlignModeEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifier,m_alignMode) /*183*/,HK_NULL},
   { "internalLegData",&hkbFootIkModifierInternalLegDataClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRUCT,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbFootIkModifier,m_internalLegData) /*184*/,HK_NULL},
   { "prevIsFootIkEnabled",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbFootIkModifier,m_prevIsFootIkEnabled) /*196*/,HK_NULL},
   { "isSetUp",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbFootIkModifier,m_isSetUp) /*200*/,HK_NULL},
   { "isGroundPositionValid",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbFootIkModifier,m_isGroundPositionValid) /*201*/,HK_NULL},
   { "timeStep",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbFootIkModifier,m_timeStep) /*204*/,HK_NULL},
};

// Signature:  ed8966c0
extern const hkClass hkbModifierClass;
extern const hkClass hkbFootIkModifierClass;
const hkClass hkbFootIkModifierClass(
    "hkbFootIkModifier",
    &hkbModifierClass, // parent
    sizeof(hkbFootIkModifier),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbFootIkModifierClass_Enums), HK_COUNT_OF(hkbFootIkModifierClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbFootIkModifierClass_Members), HK_COUNT_OF(hkbFootIkModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    3 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbFootIkModifier, hkbFootIkModifier);

