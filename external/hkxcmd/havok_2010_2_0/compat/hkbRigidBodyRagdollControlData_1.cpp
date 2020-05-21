// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbRigidBodyRagdollControlData_1.h"

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


static const hkInternalClassMember hkbRigidBodyRagdollControlDataClass_Members[] =
{
   { "keyFrameHierarchyControlData",&hkaKeyFrameHierarchyUtilityControlDataClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::ALIGN_16,HK_OFFSET_OF(hkbRigidBodyRagdollControlData,m_keyFrameHierarchyControlData) /*0*/,HK_NULL},
   { "durationToBlend",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbRigidBodyRagdollControlData,m_durationToBlend) /*48*/,HK_NULL},
};

// Signature:  1e0bc068
extern const hkClass hkbRigidBodyRagdollControlDataClass;
const hkClass hkbRigidBodyRagdollControlDataClass(
    "hkbRigidBodyRagdollControlData",
    HK_NULL, // parent
    sizeof(hkbRigidBodyRagdollControlData),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbRigidBodyRagdollControlDataClass_Members), HK_COUNT_OF(hkbRigidBodyRagdollControlDataClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(hkbRigidBodyRagdollControlData, hkbRigidBodyRagdollControlData);

