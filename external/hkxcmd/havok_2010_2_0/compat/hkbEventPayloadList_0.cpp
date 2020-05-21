// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbEventPayloadList_0.h"

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


static const hkInternalClassMember hkbEventPayloadListClass_Members[] =
{
   { "payloads",&hkbEventPayloadClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_POINTER,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbEventPayloadList,m_payloads) /*8*/,HK_NULL},
};

// Signature:  3d2dbd34
extern const hkClass hkbEventPayloadClass;
extern const hkClass hkbEventPayloadListClass;
const hkClass hkbEventPayloadListClass(
    "hkbEventPayloadList",
    &hkbEventPayloadClass, // parent
    sizeof(hkbEventPayloadList),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbEventPayloadListClass_Members), HK_COUNT_OF(hkbEventPayloadListClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbEventPayloadList, hkbEventPayloadList);

