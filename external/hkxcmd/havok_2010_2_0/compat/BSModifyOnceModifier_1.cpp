// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "BSModifyOnceModifier_1.h"

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


static const hkInternalClassMember BSModifyOnceModifierClass_Members[] =
{
   { "pOnActivateModifier",&hkbModifierClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::ALIGN_16,HK_OFFSET_OF(BSModifyOnceModifier,m_pOnActivateModifier) /*48*/,HK_NULL},
   { "pOnDeactivateModifier",&hkbModifierClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::ALIGN_16,HK_OFFSET_OF(BSModifyOnceModifier,m_pOnDeactivateModifier) /*64*/,HK_NULL},
};

// Signature:  1e20a97a
extern const hkClass hkbModifierClass;
extern const hkClass BSModifyOnceModifierClass;
const hkClass BSModifyOnceModifierClass(
    "BSModifyOnceModifier",
    &hkbModifierClass, // parent
    sizeof(BSModifyOnceModifier),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(BSModifyOnceModifierClass_Members), HK_COUNT_OF(BSModifyOnceModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(BSModifyOnceModifier, BSModifyOnceModifier);

