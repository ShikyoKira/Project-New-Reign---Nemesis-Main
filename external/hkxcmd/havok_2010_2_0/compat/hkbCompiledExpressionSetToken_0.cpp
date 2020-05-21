// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbCompiledExpressionSetToken_0.h"

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


static const hkInternalClassEnumItem TokenTypeEnumItems[] =
{
   {0, "TOKEN_TYPE_NONE"},
   {1, "TOKEN_TYPE_OPERATOR"},
   {2, "TOKEN_TYPE_NUMBER"},
   {3, "TOKEN_TYPE_VARIABLE_INDEX"},
   {4, "TOKEN_TYPE_OPENING_PAREN"},
   {5, "TOKEN_TYPE_CLOSING_PAREN"},
   {6, "TOKEN_TYPE_COMMA"},
   {7, "TOKEN_TYPE_CHARACTER_PROPERTY_INDEX"},
};

static const hkInternalClassEnumItem OperatorEnumItems[] =
{
   {0, "OP_NOP"},
   {1, "OP_RAND01"},
   {2, "OP_LOGICAL_NOT"},
   {3, "OP_UNARY_MINUS"},
   {4, "OP_UNARY_PLUS"},
   {5, "OP_SIN"},
   {6, "OP_COS"},
   {7, "OP_ASIN"},
   {8, "OP_ACOS"},
   {9, "OP_SQRT"},
   {10, "OP_FABS"},
   {11, "OP_CEIL"},
   {12, "OP_FLOOR"},
   {13, "OP_SQRTINV"},
   {14, "OP_MUL"},
   {15, "OP_DIV"},
   {16, "OP_ADD"},
   {17, "OP_SUB"},
   {18, "OP_LOGICAL_OR"},
   {19, "OP_LOGICAL_AND"},
   {20, "OP_EQ"},
   {21, "OP_NEQ"},
   {22, "OP_LT"},
   {23, "OP_GT"},
   {24, "OP_LEQ"},
   {25, "OP_GEQ"},
   {26, "OP_POW"},
   {27, "OP_MAX2"},
   {28, "OP_MIN2"},
   {29, "OP_RANDRANGE"},
   {30, "OP_ATAN2APPROX"},
   {31, "OP_CLAMP"},
   {32, "OP_MOD"},
   {33, "OP_DEG2RAD"},
   {34, "OP_RAD2DEG"},
   {35, "OP_COSD"},
   {36, "OP_SIND"},
   {37, "OP_ACOSD"},
   {38, "OP_ASIND"},
   {39, "OP_ATAN2APPROXD"},
   {40, "OP_SIGN"},
   {41, "OP_LERP"},
   {42, "OP_CLERP"},
   {43, "OP_COND"},
};

static const hkInternalClassEnum hkbCompiledExpressionSetTokenClass_Enums[] = {
   {"TokenType", TokenTypeEnumItems, _countof(TokenTypeEnumItems), HK_NULL, 0 },
   {"Operator", OperatorEnumItems, _countof(OperatorEnumItems), HK_NULL, 0 },
};
const hkClassEnum* TokenTypeEnum = reinterpret_cast<const hkClassEnum*>(&hkbCompiledExpressionSetTokenClass_Enums[0]);
const hkClassEnum* OperatorEnum = reinterpret_cast<const hkClassEnum*>(&hkbCompiledExpressionSetTokenClass_Enums[1]);

static const hkInternalClassMember hkbCompiledExpressionSetTokenClass_Members[] =
{
   { "data",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCompiledExpressionSetToken,m_data) /*0*/,HK_NULL},
   { "type",HK_NULL,TokenTypeEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCompiledExpressionSetToken,m_type) /*4*/,HK_NULL},
   { "operator",HK_NULL,OperatorEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCompiledExpressionSetToken,m_operator) /*5*/,HK_NULL},
};

// Signature:  c6aaccc8
extern const hkClass hkbCompiledExpressionSetTokenClass;
const hkClass hkbCompiledExpressionSetTokenClass(
    "hkbCompiledExpressionSetToken",
    HK_NULL, // parent
    sizeof(hkbCompiledExpressionSetToken),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbCompiledExpressionSetTokenClass_Enums), HK_COUNT_OF(hkbCompiledExpressionSetTokenClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbCompiledExpressionSetTokenClass_Members), HK_COUNT_OF(hkbCompiledExpressionSetTokenClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(hkbCompiledExpressionSetToken, hkbCompiledExpressionSetToken);

