// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbCombineTransformsModifier_0.h"

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


static const hkInternalClassMember hkbCombineTransformsModifierClass_Members[] =
{
   { "translationOut",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCombineTransformsModifier,m_translationOut) /*48*/,HK_NULL},
   { "rotationOut",HK_NULL,HK_NULL,hkClassMember::TYPE_QUATERNION,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCombineTransformsModifier,m_rotationOut) /*64*/,HK_NULL},
   { "leftTranslation",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCombineTransformsModifier,m_leftTranslation) /*80*/,HK_NULL},
   { "leftRotation",HK_NULL,HK_NULL,hkClassMember::TYPE_QUATERNION,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCombineTransformsModifier,m_leftRotation) /*96*/,HK_NULL},
   { "rightTranslation",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCombineTransformsModifier,m_rightTranslation) /*112*/,HK_NULL},
   { "rightRotation",HK_NULL,HK_NULL,hkClassMember::TYPE_QUATERNION,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCombineTransformsModifier,m_rightRotation) /*128*/,HK_NULL},
   { "invertLeftTransform",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCombineTransformsModifier,m_invertLeftTransform) /*144*/,HK_NULL},
   { "invertRightTransform",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCombineTransformsModifier,m_invertRightTransform) /*145*/,HK_NULL},
   { "invertResult",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCombineTransformsModifier,m_invertResult) /*146*/,HK_NULL},
};

// Signature:  fd1f0b79
extern const hkClass hkbModifierClass;
extern const hkClass hkbCombineTransformsModifierClass;
const hkClass hkbCombineTransformsModifierClass(
    "hkbCombineTransformsModifier",
    &hkbModifierClass, // parent
    sizeof(hkbCombineTransformsModifier),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbCombineTransformsModifierClass_Members), HK_COUNT_OF(hkbCombineTransformsModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbCombineTransformsModifier, hkbCombineTransformsModifier);

