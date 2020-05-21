// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbHandle_1.h"

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


static const hkInternalClassMember hkbHandleClass_Members[] =
{
   { "frame",&hkLocalFrameClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandle,m_frame) /*8*/,HK_NULL},
   { "rigidBody",&hkpRigidBodyClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandle,m_rigidBody) /*12*/,HK_NULL},
   { "character",&hkbCharacterClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandle,m_character) /*16*/,HK_NULL},
   { "animationBoneIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandle,m_animationBoneIndex) /*20*/,HK_NULL},
};

// Signature:  d8b6401c
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbHandleClass;
const hkClass hkbHandleClass(
    "hkbHandle",
    &hkReferencedObjectClass, // parent
    sizeof(hkbHandle),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbHandleClass_Members), HK_COUNT_OF(hkbHandleClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbHandle, hkbHandle);

