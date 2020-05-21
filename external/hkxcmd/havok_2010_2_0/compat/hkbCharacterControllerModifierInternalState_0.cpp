// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbCharacterControllerModifierInternalState_0.h"

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


static const hkInternalClassMember hkbCharacterControllerModifierInternalStateClass_Members[] =
{
   { "gravity",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterControllerModifierInternalState,m_gravity) /*16*/,HK_NULL},
   { "timestep",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterControllerModifierInternalState,m_timestep) /*32*/,HK_NULL},
   { "isInitialVelocityAdded",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterControllerModifierInternalState,m_isInitialVelocityAdded) /*36*/,HK_NULL},
   { "isTouchingGround",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterControllerModifierInternalState,m_isTouchingGround) /*37*/,HK_NULL},
};

// Signature:  f8dfec0d
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbCharacterControllerModifierInternalStateClass;
const hkClass hkbCharacterControllerModifierInternalStateClass(
    "hkbCharacterControllerModifierInternalState",
    &hkReferencedObjectClass, // parent
    sizeof(hkbCharacterControllerModifierInternalState),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbCharacterControllerModifierInternalStateClass_Members), HK_COUNT_OF(hkbCharacterControllerModifierInternalStateClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbCharacterControllerModifierInternalState, hkbCharacterControllerModifierInternalState);

