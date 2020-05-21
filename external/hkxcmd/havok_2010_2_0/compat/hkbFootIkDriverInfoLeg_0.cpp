// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbFootIkDriverInfoLeg_0.h"

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


static const hkInternalClassMember hkbFootIkDriverInfoLegClass_Members[] =
{
   { "prevAnkleRotLS",HK_NULL,HK_NULL,hkClassMember::TYPE_QUATERNION,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbFootIkDriverInfoLeg,m_prevAnkleRotLS) /*0*/,HK_NULL},
   { "kneeAxisLS",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkDriverInfoLeg,m_kneeAxisLS) /*16*/,HK_NULL},
   { "footEndLS",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkDriverInfoLeg,m_footEndLS) /*32*/,HK_NULL},
   { "footPlantedAnkleHeightMS",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkDriverInfoLeg,m_footPlantedAnkleHeightMS) /*48*/,HK_NULL},
   { "footRaisedAnkleHeightMS",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkDriverInfoLeg,m_footRaisedAnkleHeightMS) /*52*/,HK_NULL},
   { "maxAnkleHeightMS",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkDriverInfoLeg,m_maxAnkleHeightMS) /*56*/,HK_NULL},
   { "minAnkleHeightMS",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkDriverInfoLeg,m_minAnkleHeightMS) /*60*/,HK_NULL},
   { "maxKneeAngleDegrees",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkDriverInfoLeg,m_maxKneeAngleDegrees) /*64*/,HK_NULL},
   { "minKneeAngleDegrees",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkDriverInfoLeg,m_minKneeAngleDegrees) /*68*/,HK_NULL},
   { "maxAnkleAngleDegrees",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkDriverInfoLeg,m_maxAnkleAngleDegrees) /*72*/,HK_NULL},
   { "hipIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkDriverInfoLeg,m_hipIndex) /*76*/,HK_NULL},
   { "kneeIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkDriverInfoLeg,m_kneeIndex) /*78*/,HK_NULL},
   { "ankleIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkDriverInfoLeg,m_ankleIndex) /*80*/,HK_NULL},
};

// Signature:  224b18d1
extern const hkClass hkbFootIkDriverInfoLegClass;
const hkClass hkbFootIkDriverInfoLegClass(
    "hkbFootIkDriverInfoLeg",
    HK_NULL, // parent
    sizeof(hkbFootIkDriverInfoLeg),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbFootIkDriverInfoLegClass_Members), HK_COUNT_OF(hkbFootIkDriverInfoLegClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(hkbFootIkDriverInfoLeg, hkbFootIkDriverInfoLeg);

