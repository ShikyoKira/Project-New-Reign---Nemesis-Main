// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "BSIStateManagerModifier_1.h"

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


static const hkInternalClassMember BSIStateManagerModifierClass_Members[] =
{
   { "iStateVar",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSIStateManagerModifier,m_iStateVar) /*44*/,HK_NULL},
   { "stateData",&BSIStateManagerModifierBSiStateDataClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSIStateManagerModifier,m_stateData) /*48*/,HK_NULL},
   { "myStateListener",&BSIStateManagerModifierBSIStateManagerStateListenerClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSIStateManagerModifier,m_myStateListener) /*60*/,HK_NULL},
};

// Signature:  6cb24f2e
extern const hkClass hkbModifierClass;
extern const hkClass BSIStateManagerModifierClass;
const hkClass BSIStateManagerModifierClass(
    "BSIStateManagerModifier",
    &hkbModifierClass, // parent
    sizeof(BSIStateManagerModifier),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(BSIStateManagerModifierClass_Members), HK_COUNT_OF(BSIStateManagerModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(BSIStateManagerModifier, BSIStateManagerModifier);

