// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbKeyframeBonesModifier_3.h"

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


static const hkInternalClassMember hkbKeyframeBonesModifierClass_Members[] =
{
   { "keyframeInfo",&hkbKeyframeBonesModifierKeyframeInfoClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbKeyframeBonesModifier,m_keyframeInfo) /*44*/,HK_NULL},
   { "keyframedBonesList",&hkbBoneIndexArrayClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbKeyframeBonesModifier,m_keyframedBonesList) /*56*/,HK_NULL},
};

// Signature:  95f66629
extern const hkClass hkbModifierClass;
extern const hkClass hkbKeyframeBonesModifierClass;
const hkClass hkbKeyframeBonesModifierClass(
    "hkbKeyframeBonesModifier",
    &hkbModifierClass, // parent
    sizeof(hkbKeyframeBonesModifier),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbKeyframeBonesModifierClass_Members), HK_COUNT_OF(hkbKeyframeBonesModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    3 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbKeyframeBonesModifier, hkbKeyframeBonesModifier);

