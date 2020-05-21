// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbBindable_0.h"

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


static const hkInternalClassMember hkbBindableClass_Members[] =
{
   { "variableBindingSet",&hkbVariableBindingSetClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBindable,m_variableBindingSet) /*8*/,HK_NULL},
   { "cachedBindables",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBindable,m_cachedBindables) /*12*/,HK_NULL},
   { "areBindablesCached",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBindable,m_areBindablesCached) /*24*/,HK_NULL},
};

// Signature:  2c1432d7
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbBindableClass;
const hkClass hkbBindableClass(
    "hkbBindable",
    &hkReferencedObjectClass, // parent
    sizeof(hkbBindable),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbBindableClass_Members), HK_COUNT_OF(hkbBindableClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbBindable, hkbBindable);

