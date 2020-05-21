// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbBehaviorReferenceGenerator_0.h"

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


static const hkInternalClassMember hkbBehaviorReferenceGeneratorClass_Members[] =
{
   { "behaviorName",HK_NULL,HK_NULL,hkClassMember::TYPE_STRINGPTR,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBehaviorReferenceGenerator,m_behaviorName) /*40*/,HK_NULL},
   { "behavior",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBehaviorReferenceGenerator,m_behavior) /*44*/,HK_NULL},
};

// Signature:  0fcb5423
extern const hkClass hkbGeneratorClass;
extern const hkClass hkbBehaviorReferenceGeneratorClass;
const hkClass hkbBehaviorReferenceGeneratorClass(
    "hkbBehaviorReferenceGenerator",
    &hkbGeneratorClass, // parent
    sizeof(hkbBehaviorReferenceGenerator),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbBehaviorReferenceGeneratorClass_Members), HK_COUNT_OF(hkbBehaviorReferenceGeneratorClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbBehaviorReferenceGenerator, hkbBehaviorReferenceGenerator);

