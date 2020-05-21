// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "BSLookAtModifierBoneData_0.h"

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


static const hkInternalClassMember BSLookAtModifierBoneDataClass_Members[] =
{
   { "index",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSLookAtModifierBoneData,m_index) /*0*/,HK_NULL},
   { "fwdAxisLS",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSLookAtModifierBoneData,m_fwdAxisLS) /*16*/,HK_NULL},
   { "limitAngleDegrees",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSLookAtModifierBoneData,m_limitAngleDegrees) /*32*/,HK_NULL},
   { "onGain",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSLookAtModifierBoneData,m_onGain) /*36*/,HK_NULL},
   { "offGain",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSLookAtModifierBoneData,m_offGain) /*40*/,HK_NULL},
   { "enabled",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSLookAtModifierBoneData,m_enabled) /*44*/,HK_NULL},
   { "currentFwdAxisLS",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSLookAtModifierBoneData,m_currentFwdAxisLS) /*48*/,HK_NULL},
};

// Signature:  29efee59
extern const hkClass BSLookAtModifierBoneDataClass;
const hkClass BSLookAtModifierBoneDataClass(
    "BSLookAtModifierBoneData",
    HK_NULL, // parent
    sizeof(BSLookAtModifierBoneData),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(BSLookAtModifierBoneDataClass_Members), HK_COUNT_OF(BSLookAtModifierBoneDataClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(BSLookAtModifierBoneData, BSLookAtModifierBoneData);

