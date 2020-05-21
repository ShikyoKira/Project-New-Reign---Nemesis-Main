// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbCompiledExpressionSet_1.h"

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


static const hkInternalClassMember hkbCompiledExpressionSetClass_Members[] =
{
   { "rpn",&hkbCompiledExpressionSetTokenClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCompiledExpressionSet,m_rpn) /*8*/,HK_NULL},
   { "expressionToRpnIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_INT32,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCompiledExpressionSet,m_expressionToRpnIndex) /*20*/,HK_NULL},
   { "numExpressions",HK_NULL,HK_NULL,hkClassMember::TYPE_INT8,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCompiledExpressionSet,m_numExpressions) /*32*/,HK_NULL},
};

// Signature:  3a7d76cc
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbCompiledExpressionSetClass;
const hkClass hkbCompiledExpressionSetClass(
    "hkbCompiledExpressionSet",
    &hkReferencedObjectClass, // parent
    sizeof(hkbCompiledExpressionSet),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbCompiledExpressionSetClass_Members), HK_COUNT_OF(hkbCompiledExpressionSetClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbCompiledExpressionSet, hkbCompiledExpressionSet);

