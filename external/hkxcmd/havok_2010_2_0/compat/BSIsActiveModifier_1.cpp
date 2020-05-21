// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "BSIsActiveModifier_1.h"

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


static const hkInternalClassMember BSIsActiveModifierClass_Members[] =
{
   { "bIsActive0",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSIsActiveModifier,m_bIsActive0) /*44*/,HK_NULL},
   { "bInvertActive0",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSIsActiveModifier,m_bInvertActive0) /*45*/,HK_NULL},
   { "bIsActive1",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSIsActiveModifier,m_bIsActive1) /*46*/,HK_NULL},
   { "bInvertActive1",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSIsActiveModifier,m_bInvertActive1) /*47*/,HK_NULL},
   { "bIsActive2",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSIsActiveModifier,m_bIsActive2) /*48*/,HK_NULL},
   { "bInvertActive2",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSIsActiveModifier,m_bInvertActive2) /*49*/,HK_NULL},
   { "bIsActive3",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSIsActiveModifier,m_bIsActive3) /*50*/,HK_NULL},
   { "bInvertActive3",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSIsActiveModifier,m_bInvertActive3) /*51*/,HK_NULL},
   { "bIsActive4",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSIsActiveModifier,m_bIsActive4) /*52*/,HK_NULL},
   { "bInvertActive4",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSIsActiveModifier,m_bInvertActive4) /*53*/,HK_NULL},
};

// Signature:  b0fde45a
extern const hkClass hkbModifierClass;
extern const hkClass BSIsActiveModifierClass;
const hkClass BSIsActiveModifierClass(
    "BSIsActiveModifier",
    &hkbModifierClass, // parent
    sizeof(BSIsActiveModifier),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(BSIsActiveModifierClass_Members), HK_COUNT_OF(BSIsActiveModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(BSIsActiveModifier, BSIsActiveModifier);

