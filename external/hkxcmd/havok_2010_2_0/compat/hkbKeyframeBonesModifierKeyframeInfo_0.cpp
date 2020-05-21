// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbKeyframeBonesModifierKeyframeInfo_0.h"

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


static const hkInternalClassMember hkbKeyframeBonesModifierKeyframeInfoClass_Members[] =
{
   { "keyframedPosition",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbKeyframeBonesModifierKeyframeInfo,m_keyframedPosition) /*0*/,HK_NULL},
   { "keyframedRotation",HK_NULL,HK_NULL,hkClassMember::TYPE_QUATERNION,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbKeyframeBonesModifierKeyframeInfo,m_keyframedRotation) /*16*/,HK_NULL},
   { "boneIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbKeyframeBonesModifierKeyframeInfo,m_boneIndex) /*32*/,HK_NULL},
   { "isValid",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbKeyframeBonesModifierKeyframeInfo,m_isValid) /*34*/,HK_NULL},
};

// Signature:  72deb7a6
extern const hkClass hkbKeyframeBonesModifierKeyframeInfoClass;
const hkClass hkbKeyframeBonesModifierKeyframeInfoClass(
    "hkbKeyframeBonesModifierKeyframeInfo",
    HK_NULL, // parent
    sizeof(hkbKeyframeBonesModifierKeyframeInfo),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbKeyframeBonesModifierKeyframeInfoClass_Members), HK_COUNT_OF(hkbKeyframeBonesModifierKeyframeInfoClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(hkbKeyframeBonesModifierKeyframeInfo, hkbKeyframeBonesModifierKeyframeInfo);

