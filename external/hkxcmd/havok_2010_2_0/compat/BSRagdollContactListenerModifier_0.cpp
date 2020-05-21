// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "BSRagdollContactListenerModifier_0.h"

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

static const hkClass* BSRagdollContactListenerModifierClass_IFaces[] =
{
   HK_NULL,
};


static const hkInternalClassMember BSRagdollContactListenerModifierClass_Members[] =
{
   { "contactEvent",&hkbEventPropertyClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSRagdollContactListenerModifier,m_contactEvent) /*48*/,HK_NULL},
   { "bones",&hkbBoneIndexArrayClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSRagdollContactListenerModifier,m_bones) /*56*/,HK_NULL},
   { "throwEvent",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSRagdollContactListenerModifier,m_throwEvent) /*60*/,HK_NULL},
   { "ragdollRigidBodies",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_POINTER,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSRagdollContactListenerModifier,m_ragdollRigidBodies) /*64*/,HK_NULL},
};

// Signature:  8003d8ce
extern const hkClass hkbModifierClass;
extern const hkClass BSRagdollContactListenerModifierClass;
const hkClass BSRagdollContactListenerModifierClass(
    "BSRagdollContactListenerModifier",
    &hkbModifierClass, // parent
    sizeof(BSRagdollContactListenerModifier),
    reinterpret_cast<const hkClass**>(BSRagdollContactListenerModifierClass_IFaces), HK_COUNT_OF(BSRagdollContactListenerModifierClass_IFaces),
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(BSRagdollContactListenerModifierClass_Members), HK_COUNT_OF(BSRagdollContactListenerModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(BSRagdollContactListenerModifier, BSRagdollContactListenerModifier);

