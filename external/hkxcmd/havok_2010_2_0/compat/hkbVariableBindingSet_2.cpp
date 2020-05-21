// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbVariableBindingSet_2.h"

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


static const hkInternalClassMember hkbVariableBindingSetClass_Members[] =
{
   { "bindings",&hkbVariableBindingSetBindingClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbVariableBindingSet,m_bindings) /*8*/,HK_NULL},
   { "indexOfBindingToEnable",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbVariableBindingSet,m_indexOfBindingToEnable) /*20*/,HK_NULL},
   { "hasOutputBinding",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbVariableBindingSet,m_hasOutputBinding) /*24*/,HK_NULL},
};

// Signature:  338ad4ff
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbVariableBindingSetClass;
const hkClass hkbVariableBindingSetClass(
    "hkbVariableBindingSet",
    &hkReferencedObjectClass, // parent
    sizeof(hkbVariableBindingSet),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbVariableBindingSetClass_Members), HK_COUNT_OF(hkbVariableBindingSetClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    2 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbVariableBindingSet, hkbVariableBindingSet);

