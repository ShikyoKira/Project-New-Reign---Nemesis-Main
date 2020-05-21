// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbRegisteredGenerator_1.h"

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


static const hkInternalClassMember hkbRegisteredGeneratorClass_Members[] =
{
   { "generator",&hkbGeneratorClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbRegisteredGenerator,m_generator) /*28*/,HK_NULL},
   { "relativePosition",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbRegisteredGenerator,m_relativePosition) /*32*/,HK_NULL},
   { "relativeDirection",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbRegisteredGenerator,m_relativeDirection) /*48*/,HK_NULL},
};

// Signature:  58b1d082
extern const hkClass hkbBindableClass;
extern const hkClass hkbRegisteredGeneratorClass;
const hkClass hkbRegisteredGeneratorClass(
    "hkbRegisteredGenerator",
    &hkbBindableClass, // parent
    sizeof(hkbRegisteredGenerator),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbRegisteredGeneratorClass_Members), HK_COUNT_OF(hkbRegisteredGeneratorClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbRegisteredGenerator, hkbRegisteredGenerator);

