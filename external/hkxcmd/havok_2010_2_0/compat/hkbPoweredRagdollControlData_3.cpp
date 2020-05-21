// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbPoweredRagdollControlData_3.h"

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


static const hkInternalClassMember hkbPoweredRagdollControlDataClass_Members[] =
{
   { "maxForce",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::ALIGN_16,HK_OFFSET_OF(hkbPoweredRagdollControlData,m_maxForce) /*0*/,HK_NULL},
   { "tau",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbPoweredRagdollControlData,m_tau) /*4*/,HK_NULL},
   { "damping",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbPoweredRagdollControlData,m_damping) /*8*/,HK_NULL},
   { "proportionalRecoveryVelocity",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbPoweredRagdollControlData,m_proportionalRecoveryVelocity) /*12*/,HK_NULL},
   { "constantRecoveryVelocity",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbPoweredRagdollControlData,m_constantRecoveryVelocity) /*16*/,HK_NULL},
};

// Signature:  0f5ba21b
extern const hkClass hkbPoweredRagdollControlDataClass;
const hkClass hkbPoweredRagdollControlDataClass(
    "hkbPoweredRagdollControlData",
    HK_NULL, // parent
    sizeof(hkbPoweredRagdollControlData),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbPoweredRagdollControlDataClass_Members), HK_COUNT_OF(hkbPoweredRagdollControlDataClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    3 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(hkbPoweredRagdollControlData, hkbPoweredRagdollControlData);

