// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbBehaviorGraphInternalStateInfo_1.h"

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


static const hkInternalClassMember hkbBehaviorGraphInternalStateInfoClass_Members[] =
{
   { "characterId",HK_NULL,HK_NULL,hkClassMember::TYPE_UINT64,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBehaviorGraphInternalStateInfo,m_characterId) /*8*/,HK_NULL},
   { "internalState",&hkbBehaviorGraphInternalStateClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBehaviorGraphInternalStateInfo,m_internalState) /*16*/,HK_NULL},
   { "auxiliaryNodeInfo",&hkbAuxiliaryNodeInfoClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_POINTER,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBehaviorGraphInternalStateInfo,m_auxiliaryNodeInfo) /*20*/,HK_NULL},
   { "activeEventIds",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_INT16,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBehaviorGraphInternalStateInfo,m_activeEventIds) /*32*/,HK_NULL},
   { "activeVariableIds",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_INT16,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBehaviorGraphInternalStateInfo,m_activeVariableIds) /*44*/,HK_NULL},
};

// Signature:  645f898b
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbBehaviorGraphInternalStateInfoClass;
const hkClass hkbBehaviorGraphInternalStateInfoClass(
    "hkbBehaviorGraphInternalStateInfo",
    &hkReferencedObjectClass, // parent
    sizeof(hkbBehaviorGraphInternalStateInfo),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbBehaviorGraphInternalStateInfoClass_Members), HK_COUNT_OF(hkbBehaviorGraphInternalStateInfoClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbBehaviorGraphInternalStateInfo, hkbBehaviorGraphInternalStateInfo);

