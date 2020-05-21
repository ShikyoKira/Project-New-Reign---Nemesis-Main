// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbProjectData_2.h"

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

extern const hkClassEnum* EventModeEnum;

static const hkInternalClassMember hkbProjectDataClass_Members[] =
{
   { "worldUpWS",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProjectData,m_worldUpWS) /*16*/,HK_NULL},
   { "stringData",&hkbProjectStringDataClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProjectData,m_stringData) /*32*/,HK_NULL},
   { "defaultEventMode",HK_NULL,EventModeEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProjectData,m_defaultEventMode) /*36*/,HK_NULL},
};

// Signature:  13a39ba7
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbProjectDataClass;
const hkClass hkbProjectDataClass(
    "hkbProjectData",
    &hkReferencedObjectClass, // parent
    sizeof(hkbProjectData),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbProjectDataClass_Members), HK_COUNT_OF(hkbProjectDataClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    2 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbProjectData, hkbProjectData);

