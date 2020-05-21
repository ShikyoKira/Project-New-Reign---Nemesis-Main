// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "BSiStateTaggingGenerator_1.h"

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


static const hkInternalClassMember BSiStateTaggingGeneratorClass_Members[] =
{
   { "pDefaultGenerator",&hkbGeneratorClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::ALIGN_16,HK_OFFSET_OF(BSiStateTaggingGenerator,m_pDefaultGenerator) /*48*/,HK_NULL},
   { "iStateToSetAs",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSiStateTaggingGenerator,m_iStateToSetAs) /*52*/,HK_NULL},
   { "iPriority",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSiStateTaggingGenerator,m_iPriority) /*56*/,HK_NULL},
};

// Signature:  f0826fc1
extern const hkClass hkbGeneratorClass;
extern const hkClass BSiStateTaggingGeneratorClass;
const hkClass BSiStateTaggingGeneratorClass(
    "BSiStateTaggingGenerator",
    &hkbGeneratorClass, // parent
    sizeof(BSiStateTaggingGenerator),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(BSiStateTaggingGeneratorClass_Members), HK_COUNT_OF(BSiStateTaggingGeneratorClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(BSiStateTaggingGenerator, BSiStateTaggingGenerator);

