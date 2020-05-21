// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbFootIkControlsModifierLeg_0.h"

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


static const hkInternalClassMember hkbFootIkControlsModifierLegClass_Members[] =
{
   { "groundPosition",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkControlsModifierLeg,m_groundPosition) /*0*/,HK_NULL},
   { "ungroundedEvent",&hkbEventPropertyClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkControlsModifierLeg,m_ungroundedEvent) /*16*/,HK_NULL},
   { "verticalError",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkControlsModifierLeg,m_verticalError) /*24*/,HK_NULL},
   { "hitSomething",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkControlsModifierLeg,m_hitSomething) /*28*/,HK_NULL},
   { "isPlantedMS",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbFootIkControlsModifierLeg,m_isPlantedMS) /*29*/,HK_NULL},
};

// Signature:  9e17091a
extern const hkClass hkbFootIkControlsModifierLegClass;
const hkClass hkbFootIkControlsModifierLegClass(
    "hkbFootIkControlsModifierLeg",
    HK_NULL, // parent
    sizeof(hkbFootIkControlsModifierLeg),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbFootIkControlsModifierLegClass_Members), HK_COUNT_OF(hkbFootIkControlsModifierLegClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(hkbFootIkControlsModifierLeg, hkbFootIkControlsModifierLeg);

