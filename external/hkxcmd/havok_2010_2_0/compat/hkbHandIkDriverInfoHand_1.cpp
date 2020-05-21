// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbHandIkDriverInfoHand_1.h"

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


static const hkInternalClassMember hkbHandIkDriverInfoHandClass_Members[] =
{
   { "elbowAxisLS",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkDriverInfoHand,m_elbowAxisLS) /*0*/,HK_NULL},
   { "backHandNormalLS",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkDriverInfoHand,m_backHandNormalLS) /*16*/,HK_NULL},
   { "handOffsetLS",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkDriverInfoHand,m_handOffsetLS) /*32*/,HK_NULL},
   { "handOrienationOffsetLS",HK_NULL,HK_NULL,hkClassMember::TYPE_QUATERNION,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkDriverInfoHand,m_handOrienationOffsetLS) /*48*/,HK_NULL},
   { "maxElbowAngleDegrees",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkDriverInfoHand,m_maxElbowAngleDegrees) /*64*/,HK_NULL},
   { "minElbowAngleDegrees",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkDriverInfoHand,m_minElbowAngleDegrees) /*68*/,HK_NULL},
   { "shoulderIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkDriverInfoHand,m_shoulderIndex) /*72*/,HK_NULL},
   { "shoulderSiblingIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkDriverInfoHand,m_shoulderSiblingIndex) /*74*/,HK_NULL},
   { "elbowIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkDriverInfoHand,m_elbowIndex) /*76*/,HK_NULL},
   { "elbowSiblingIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkDriverInfoHand,m_elbowSiblingIndex) /*78*/,HK_NULL},
   { "wristIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkDriverInfoHand,m_wristIndex) /*80*/,HK_NULL},
   { "enforceEndPosition",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkDriverInfoHand,m_enforceEndPosition) /*82*/,HK_NULL},
   { "enforceEndRotation",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkDriverInfoHand,m_enforceEndRotation) /*83*/,HK_NULL},
   { "localFrameName",HK_NULL,HK_NULL,hkClassMember::TYPE_STRINGPTR,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkDriverInfoHand,m_localFrameName) /*84*/,HK_NULL},
};

// Signature:  14dfe1dd
extern const hkClass hkbHandIkDriverInfoHandClass;
const hkClass hkbHandIkDriverInfoHandClass(
    "hkbHandIkDriverInfoHand",
    HK_NULL, // parent
    sizeof(hkbHandIkDriverInfoHand),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbHandIkDriverInfoHandClass_Members), HK_COUNT_OF(hkbHandIkDriverInfoHandClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(hkbHandIkDriverInfoHand, hkbHandIkDriverInfoHand);

