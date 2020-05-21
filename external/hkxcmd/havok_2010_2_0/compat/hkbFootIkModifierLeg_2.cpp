// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbFootIkModifierLeg_2.h"

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


static const hkInternalClassMember hkbFootIkModifierLegClass_Members[] =
{
   { "originalAnkleTransformMS",HK_NULL,HK_NULL,hkClassMember::TYPE_QSTRANSFORM,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifierLeg,m_originalAnkleTransformMS) /*0*/,HK_NULL},
   { "prevAnkleRotLS",HK_NULL,HK_NULL,hkClassMember::TYPE_QUATERNION,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbFootIkModifierLeg,m_prevAnkleRotLS) /*48*/,HK_NULL},
   { "kneeAxisLS",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifierLeg,m_kneeAxisLS) /*64*/,HK_NULL},
   { "footEndLS",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifierLeg,m_footEndLS) /*80*/,HK_NULL},
   { "ungroundedEvent",&hkbEventPropertyClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifierLeg,m_ungroundedEvent) /*96*/,HK_NULL},
   { "footPlantedAnkleHeightMS",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifierLeg,m_footPlantedAnkleHeightMS) /*104*/,HK_NULL},
   { "footRaisedAnkleHeightMS",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifierLeg,m_footRaisedAnkleHeightMS) /*108*/,HK_NULL},
   { "maxAnkleHeightMS",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifierLeg,m_maxAnkleHeightMS) /*112*/,HK_NULL},
   { "minAnkleHeightMS",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifierLeg,m_minAnkleHeightMS) /*116*/,HK_NULL},
   { "maxKneeAngleDegrees",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifierLeg,m_maxKneeAngleDegrees) /*120*/,HK_NULL},
   { "minKneeAngleDegrees",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifierLeg,m_minKneeAngleDegrees) /*124*/,HK_NULL},
   { "verticalError",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifierLeg,m_verticalError) /*128*/,HK_NULL},
   { "maxAnkleAngleDegrees",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifierLeg,m_maxAnkleAngleDegrees) /*132*/,HK_NULL},
   { "hipIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifierLeg,m_hipIndex) /*136*/,HK_NULL},
   { "kneeIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifierLeg,m_kneeIndex) /*138*/,HK_NULL},
   { "ankleIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifierLeg,m_ankleIndex) /*140*/,HK_NULL},
   { "hitSomething",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifierLeg,m_hitSomething) /*142*/,HK_NULL},
   { "isPlantedMS",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifierLeg,m_isPlantedMS) /*143*/,HK_NULL},
   { "isOriginalAnkleTransformMSSet",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkModifierLeg,m_isOriginalAnkleTransformMSSet) /*144*/,HK_NULL},
};

// Signature:  9f3e3a04
extern const hkClass hkbFootIkModifierLegClass;
const hkClass hkbFootIkModifierLegClass(
    "hkbFootIkModifierLeg",
    HK_NULL, // parent
    sizeof(hkbFootIkModifierLeg),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbFootIkModifierLegClass_Members), HK_COUNT_OF(hkbFootIkModifierLegClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    2 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(hkbFootIkModifierLeg, hkbFootIkModifierLeg);

