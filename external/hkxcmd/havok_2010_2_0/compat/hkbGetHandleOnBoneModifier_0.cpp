// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbGetHandleOnBoneModifier_0.h"

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


static const hkInternalClassMember hkbGetHandleOnBoneModifierClass_Members[] =
{
   { "handleOut",&hkbHandleClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbGetHandleOnBoneModifier,m_handleOut) /*44*/,HK_NULL},
   { "localFrameName",HK_NULL,HK_NULL,hkClassMember::TYPE_STRINGPTR,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbGetHandleOnBoneModifier,m_localFrameName) /*48*/,HK_NULL},
   { "ragdollBoneIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbGetHandleOnBoneModifier,m_ragdollBoneIndex) /*52*/,HK_NULL},
   { "animationBoneIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbGetHandleOnBoneModifier,m_animationBoneIndex) /*54*/,HK_NULL},
};

// Signature:  50c34a17
extern const hkClass hkbModifierClass;
extern const hkClass hkbGetHandleOnBoneModifierClass;
const hkClass hkbGetHandleOnBoneModifierClass(
    "hkbGetHandleOnBoneModifier",
    &hkbModifierClass, // parent
    sizeof(hkbGetHandleOnBoneModifier),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbGetHandleOnBoneModifierClass_Members), HK_COUNT_OF(hkbGetHandleOnBoneModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbGetHandleOnBoneModifier, hkbGetHandleOnBoneModifier);

