// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbDampingModifierInternalState_0.h"

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


static const hkInternalClassMember hkbDampingModifierInternalStateClass_Members[] =
{
   { "dampedVector",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbDampingModifierInternalState,m_dampedVector) /*16*/,HK_NULL},
   { "vecErrorSum",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbDampingModifierInternalState,m_vecErrorSum) /*32*/,HK_NULL},
   { "vecPreviousError",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbDampingModifierInternalState,m_vecPreviousError) /*48*/,HK_NULL},
   { "dampedValue",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbDampingModifierInternalState,m_dampedValue) /*64*/,HK_NULL},
   { "errorSum",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbDampingModifierInternalState,m_errorSum) /*68*/,HK_NULL},
   { "previousError",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbDampingModifierInternalState,m_previousError) /*72*/,HK_NULL},
};

// Signature:  508d3b36
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbDampingModifierInternalStateClass;
const hkClass hkbDampingModifierInternalStateClass(
    "hkbDampingModifierInternalState",
    &hkReferencedObjectClass, // parent
    sizeof(hkbDampingModifierInternalState),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbDampingModifierInternalStateClass_Members), HK_COUNT_OF(hkbDampingModifierInternalStateClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbDampingModifierInternalState, hkbDampingModifierInternalState);

