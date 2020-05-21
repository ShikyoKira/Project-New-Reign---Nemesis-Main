// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbRotateCharacterModifierInternalState_0.h"

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


static const hkInternalClassMember hkbRotateCharacterModifierInternalStateClass_Members[] =
{
   { "angle",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbRotateCharacterModifierInternalState,m_angle) /*8*/,HK_NULL},
};

// Signature:  dc40bf4a
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbRotateCharacterModifierInternalStateClass;
const hkClass hkbRotateCharacterModifierInternalStateClass(
    "hkbRotateCharacterModifierInternalState",
    &hkReferencedObjectClass, // parent
    sizeof(hkbRotateCharacterModifierInternalState),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbRotateCharacterModifierInternalStateClass_Members), HK_COUNT_OF(hkbRotateCharacterModifierInternalStateClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbRotateCharacterModifierInternalState, hkbRotateCharacterModifierInternalState);

