// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "BSPassByTargetTriggerModifier_1.h"

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


static const hkInternalClassMember BSPassByTargetTriggerModifierClass_Members[] =
{
   { "targetPosition",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSPassByTargetTriggerModifier,m_targetPosition) /*48*/,HK_NULL},
   { "radius",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSPassByTargetTriggerModifier,m_radius) /*64*/,HK_NULL},
   { "movementDirection",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSPassByTargetTriggerModifier,m_movementDirection) /*80*/,HK_NULL},
   { "triggerEvent",&hkbEventPropertyClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSPassByTargetTriggerModifier,m_triggerEvent) /*96*/,HK_NULL},
   { "targetPassed",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSPassByTargetTriggerModifier,m_targetPassed) /*104*/,HK_NULL},
};

// Signature:  703d7b66
extern const hkClass hkbModifierClass;
extern const hkClass BSPassByTargetTriggerModifierClass;
const hkClass BSPassByTargetTriggerModifierClass(
    "BSPassByTargetTriggerModifier",
    &hkbModifierClass, // parent
    sizeof(BSPassByTargetTriggerModifier),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(BSPassByTargetTriggerModifierClass_Members), HK_COUNT_OF(BSPassByTargetTriggerModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(BSPassByTargetTriggerModifier, BSPassByTargetTriggerModifier);

