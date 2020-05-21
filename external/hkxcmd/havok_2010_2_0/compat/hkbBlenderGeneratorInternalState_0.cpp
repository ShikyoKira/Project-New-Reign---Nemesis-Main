// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbBlenderGeneratorInternalState_0.h"

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


static const hkInternalClassMember hkbBlenderGeneratorInternalStateClass_Members[] =
{
   { "childrenInternalStates",&hkbBlenderGeneratorChildInternalStateClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlenderGeneratorInternalState,m_childrenInternalStates) /*8*/,HK_NULL},
   { "sortedChildren",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_INT16,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlenderGeneratorInternalState,m_sortedChildren) /*20*/,HK_NULL},
   { "endIntervalWeight",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlenderGeneratorInternalState,m_endIntervalWeight) /*32*/,HK_NULL},
   { "numActiveChildren",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlenderGeneratorInternalState,m_numActiveChildren) /*36*/,HK_NULL},
   { "beginIntervalIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlenderGeneratorInternalState,m_beginIntervalIndex) /*40*/,HK_NULL},
   { "endIntervalIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlenderGeneratorInternalState,m_endIntervalIndex) /*42*/,HK_NULL},
   { "initSync",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlenderGeneratorInternalState,m_initSync) /*44*/,HK_NULL},
   { "doSubtractiveBlend",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlenderGeneratorInternalState,m_doSubtractiveBlend) /*45*/,HK_NULL},
};

// Signature:  84717488
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbBlenderGeneratorInternalStateClass;
const hkClass hkbBlenderGeneratorInternalStateClass(
    "hkbBlenderGeneratorInternalState",
    &hkReferencedObjectClass, // parent
    sizeof(hkbBlenderGeneratorInternalState),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbBlenderGeneratorInternalStateClass_Members), HK_COUNT_OF(hkbBlenderGeneratorInternalStateClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbBlenderGeneratorInternalState, hkbBlenderGeneratorInternalState);

