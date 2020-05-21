#pragma once

#include <Common/Base/hkBase.h>

struct hkbWorldFromModelModeData
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbWorldFromModelModeData );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbWorldFromModelModeData(void) {}
   enum WorldFromModelMode
   {
      WORLD_FROM_MODEL_MODE_USE_OLD = 0,
      WORLD_FROM_MODEL_MODE_USE_INPUT = 1,
      WORLD_FROM_MODEL_MODE_COMPUTE = 2,
      WORLD_FROM_MODEL_MODE_NONE = 3,
      WORLD_FROM_MODEL_MODE_RAGDOLL = 4,
   };

   // Properties
   hkInt16 m_poseMatchingBone0;
   hkInt16 m_poseMatchingBone1;
   hkInt16 m_poseMatchingBone2;
   hkEnum<WorldFromModelMode,hkInt8> m_mode;
};
extern const hkClass hkbWorldFromModelModeDataClass;

