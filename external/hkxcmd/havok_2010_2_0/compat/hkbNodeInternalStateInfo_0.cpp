// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbNodeInternalStateInfo_0.h"

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


static const hkInternalClassMember hkbNodeInternalStateInfoClass_Members[] =
{
   { "syncInfo",&hkbGeneratorSyncInfoClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbNodeInternalStateInfo,m_syncInfo) /*8*/,HK_NULL},
   { "name",HK_NULL,HK_NULL,hkClassMember::TYPE_STRINGPTR,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbNodeInternalStateInfo,m_name) /*88*/,HK_NULL},
   { "internalState",&hkReferencedObjectClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbNodeInternalStateInfo,m_internalState) /*92*/,HK_NULL},
   { "nodeId",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbNodeInternalStateInfo,m_nodeId) /*96*/,HK_NULL},
   { "hasActivateBeenCalled",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbNodeInternalStateInfo,m_hasActivateBeenCalled) /*98*/,HK_NULL},
};

// Signature:  7db9971d
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbNodeInternalStateInfoClass;
const hkClass hkbNodeInternalStateInfoClass(
    "hkbNodeInternalStateInfo",
    &hkReferencedObjectClass, // parent
    sizeof(hkbNodeInternalStateInfo),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbNodeInternalStateInfoClass_Members), HK_COUNT_OF(hkbNodeInternalStateInfoClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbNodeInternalStateInfo, hkbNodeInternalStateInfo);

