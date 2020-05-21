#pragma once

#include <Common/Base/hkBase.h>

struct hkbRoleAttribute
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbRoleAttribute );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbRoleAttribute(void) {}
   enum Role
   {
      ROLE_DEFAULT = 0,
      ROLE_FILE_NAME = 1,
      ROLE_BONE_INDEX = 2,
      ROLE_BONE_INDEX_MAP = 3,
      ROLE_EVENT_ID = 4,
      ROLE_VARIABLE_INDEX = 5,
      ROLE_ATTRIBUTE_INDEX = 6,
      ROLE_TIME = 7,
   };

   enum RoleFlags
   {
      FLAG_NONE = 0,
      FLAG_RAGDOLL = 1,
      FLAG_NORMALIZED = 2,
      FLAG_NOT_VARIABLE = 4,
      FLAG_HIDDEN = 8,
      FLAG_OUTPUT = 16,
      FLAG_NOT_CHARACTER_PROPERTY = 32,
   };

   // Properties
   hkEnum<Role,hkInt16> m_role;
   hkFlags<RoleFlags,hkInt16> m_flags;
};
extern const hkClass hkbRoleAttributeClass;

