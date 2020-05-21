// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbBlenderGeneratorChildInternalState_0.h"

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


static const hkInternalClassMember hkbBlenderGeneratorChildInternalStateClass_Members[] =
{
   { "isActive",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlenderGeneratorChildInternalState,m_isActive) /*0*/,HK_NULL},
   { "syncNextFrame",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlenderGeneratorChildInternalState,m_syncNextFrame) /*1*/,HK_NULL},
};

// Signature:  ff7327c0
extern const hkClass hkbBlenderGeneratorChildInternalStateClass;
const hkClass hkbBlenderGeneratorChildInternalStateClass(
    "hkbBlenderGeneratorChildInternalState",
    HK_NULL, // parent
    sizeof(hkbBlenderGeneratorChildInternalState),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbBlenderGeneratorChildInternalStateClass_Members), HK_COUNT_OF(hkbBlenderGeneratorChildInternalStateClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(hkbBlenderGeneratorChildInternalState, hkbBlenderGeneratorChildInternalState);

