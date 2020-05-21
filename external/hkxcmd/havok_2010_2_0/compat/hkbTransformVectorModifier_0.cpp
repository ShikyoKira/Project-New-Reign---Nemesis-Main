// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbTransformVectorModifier_0.h"

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


static const hkInternalClassMember hkbTransformVectorModifierClass_Members[] =
{
   { "rotation",HK_NULL,HK_NULL,hkClassMember::TYPE_QUATERNION,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbTransformVectorModifier,m_rotation) /*48*/,HK_NULL},
   { "translation",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbTransformVectorModifier,m_translation) /*64*/,HK_NULL},
   { "vectorIn",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbTransformVectorModifier,m_vectorIn) /*80*/,HK_NULL},
   { "vectorOut",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbTransformVectorModifier,m_vectorOut) /*96*/,HK_NULL},
   { "rotateOnly",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbTransformVectorModifier,m_rotateOnly) /*112*/,HK_NULL},
   { "inverse",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbTransformVectorModifier,m_inverse) /*113*/,HK_NULL},
   { "computeOnActivate",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbTransformVectorModifier,m_computeOnActivate) /*114*/,HK_NULL},
   { "computeOnModify",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbTransformVectorModifier,m_computeOnModify) /*115*/,HK_NULL},
};

// Signature:  f93e0e24
extern const hkClass hkbModifierClass;
extern const hkClass hkbTransformVectorModifierClass;
const hkClass hkbTransformVectorModifierClass(
    "hkbTransformVectorModifier",
    &hkbModifierClass, // parent
    sizeof(hkbTransformVectorModifier),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbTransformVectorModifierClass_Members), HK_COUNT_OF(hkbTransformVectorModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbTransformVectorModifier, hkbTransformVectorModifier);

