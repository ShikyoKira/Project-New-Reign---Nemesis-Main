// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "BSEventOnFalseToTrueModifier_1.h"

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


static const hkInternalClassMember BSEventOnFalseToTrueModifierClass_Members[] =
{
   { "bEnableEvent1",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSEventOnFalseToTrueModifier,m_bEnableEvent1) /*44*/,HK_NULL},
   { "bVariableToTest1",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSEventOnFalseToTrueModifier,m_bVariableToTest1) /*45*/,HK_NULL},
   { "EventToSend1",&hkbEventPropertyClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSEventOnFalseToTrueModifier,m_EventToSend1) /*48*/,HK_NULL},
   { "bEnableEvent2",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSEventOnFalseToTrueModifier,m_bEnableEvent2) /*56*/,HK_NULL},
   { "bVariableToTest2",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSEventOnFalseToTrueModifier,m_bVariableToTest2) /*57*/,HK_NULL},
   { "EventToSend2",&hkbEventPropertyClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSEventOnFalseToTrueModifier,m_EventToSend2) /*60*/,HK_NULL},
   { "bEnableEvent3",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSEventOnFalseToTrueModifier,m_bEnableEvent3) /*68*/,HK_NULL},
   { "bVariableToTest3",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSEventOnFalseToTrueModifier,m_bVariableToTest3) /*69*/,HK_NULL},
   { "EventToSend3",&hkbEventPropertyClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSEventOnFalseToTrueModifier,m_EventToSend3) /*72*/,HK_NULL},
   { "bSlot1ActivatedLastFrame",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSEventOnFalseToTrueModifier,m_bSlot1ActivatedLastFrame) /*80*/,HK_NULL},
   { "bSlot2ActivatedLastFrame",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSEventOnFalseToTrueModifier,m_bSlot2ActivatedLastFrame) /*81*/,HK_NULL},
   { "bSlot3ActivatedLastFrame",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSEventOnFalseToTrueModifier,m_bSlot3ActivatedLastFrame) /*82*/,HK_NULL},
};

// Signature:  81d0777a
extern const hkClass hkbModifierClass;
extern const hkClass BSEventOnFalseToTrueModifierClass;
const hkClass BSEventOnFalseToTrueModifierClass(
    "BSEventOnFalseToTrueModifier",
    &hkbModifierClass, // parent
    sizeof(BSEventOnFalseToTrueModifier),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(BSEventOnFalseToTrueModifierClass_Members), HK_COUNT_OF(BSEventOnFalseToTrueModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(BSEventOnFalseToTrueModifier, BSEventOnFalseToTrueModifier);

