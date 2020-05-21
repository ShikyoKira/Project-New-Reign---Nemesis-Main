// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbSenseHandleModifier_2.h"

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


static const hkInternalClassEnumItem SensingModeEnumItems[] =
{
   {0, "SENSE_IN_NEARBY_RIGID_BODIES"},
   {1, "SENSE_IN_RIGID_BODIES_OUTSIDE_THIS_CHARACTER"},
   {2, "SENSE_IN_OTHER_CHARACTER_RIGID_BODIES"},
   {3, "SENSE_IN_THIS_CHARACTER_RIGID_BODIES"},
   {4, "SENSE_IN_GIVEN_CHARACTER_RIGID_BODIES"},
   {5, "SENSE_IN_GIVEN_RIGID_BODY"},
   {6, "SENSE_IN_OTHER_CHARACTER_SKELETON"},
   {7, "SENSE_IN_THIS_CHARACTER_SKELETON"},
   {8, "SENSE_IN_GIVEN_CHARACTER_SKELETON"},
   {9, "SENSE_IN_GIVEN_LOCAL_FRAME_GROUP"},
};

static const hkInternalClassEnum hkbSenseHandleModifierClass_Enums[] = {
   {"SensingMode", SensingModeEnumItems, _countof(SensingModeEnumItems), HK_NULL, 0 },
};
const hkClassEnum* SensingModeEnum = reinterpret_cast<const hkClassEnum*>(&hkbSenseHandleModifierClass_Enums[0]);

static const hkInternalClassMember hkbSenseHandleModifierClass_Members[] =
{
   { "handle",&hkbHandleClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbSenseHandleModifier,m_handle) /*44*/,HK_NULL},
   { "sensorLocalOffset",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSenseHandleModifier,m_sensorLocalOffset) /*80*/,HK_NULL},
   { "ranges",&hkbSenseHandleModifierRangeClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSenseHandleModifier,m_ranges) /*96*/,HK_NULL},
   { "handleOut",&hkbHandleClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSenseHandleModifier,m_handleOut) /*108*/,HK_NULL},
   { "handleIn",&hkbHandleClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSenseHandleModifier,m_handleIn) /*112*/,HK_NULL},
   { "localFrameName",HK_NULL,HK_NULL,hkClassMember::TYPE_STRINGPTR,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSenseHandleModifier,m_localFrameName) /*116*/,HK_NULL},
   { "sensorLocalFrameName",HK_NULL,HK_NULL,hkClassMember::TYPE_STRINGPTR,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSenseHandleModifier,m_sensorLocalFrameName) /*120*/,HK_NULL},
   { "minDistance",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSenseHandleModifier,m_minDistance) /*124*/,HK_NULL},
   { "maxDistance",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSenseHandleModifier,m_maxDistance) /*128*/,HK_NULL},
   { "distanceOut",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSenseHandleModifier,m_distanceOut) /*132*/,HK_NULL},
   { "collisionFilterInfo",HK_NULL,HK_NULL,hkClassMember::TYPE_UINT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSenseHandleModifier,m_collisionFilterInfo) /*136*/,HK_NULL},
   { "sensorRagdollBoneIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSenseHandleModifier,m_sensorRagdollBoneIndex) /*140*/,HK_NULL},
   { "sensorAnimationBoneIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSenseHandleModifier,m_sensorAnimationBoneIndex) /*142*/,HK_NULL},
   { "sensingMode",HK_NULL,SensingModeEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSenseHandleModifier,m_sensingMode) /*144*/,HK_NULL},
   { "extrapolateSensorPosition",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSenseHandleModifier,m_extrapolateSensorPosition) /*145*/,HK_NULL},
   { "keepFirstSensedHandle",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSenseHandleModifier,m_keepFirstSensedHandle) /*146*/,HK_NULL},
   { "foundHandleOut",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSenseHandleModifier,m_foundHandleOut) /*147*/,HK_NULL},
   { "timeSinceLastModify",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbSenseHandleModifier,m_timeSinceLastModify) /*148*/,HK_NULL},
   { "rangeIndexForEventToSendNextUpdate",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbSenseHandleModifier,m_rangeIndexForEventToSendNextUpdate) /*152*/,HK_NULL},
};

// Signature:  2a064d99
extern const hkClass hkbModifierClass;
extern const hkClass hkbSenseHandleModifierClass;
const hkClass hkbSenseHandleModifierClass(
    "hkbSenseHandleModifier",
    &hkbModifierClass, // parent
    sizeof(hkbSenseHandleModifier),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbSenseHandleModifierClass_Enums), HK_COUNT_OF(hkbSenseHandleModifierClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbSenseHandleModifierClass_Members), HK_COUNT_OF(hkbSenseHandleModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    2 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbSenseHandleModifier, hkbSenseHandleModifier);

