/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_NULL_ACTION_H
#define HK_DYNAMICS2_NULL_ACTION_H


#include <Common/Base/hkBase.h>
#include <Common/Base/Types/Physics/hkStepInfo.h>
#include <Physics/Dynamics/Entity/hkpEntityListener.h>
#include <Physics/Dynamics/Action/hkpAction.h>
//#include <hkdynamics/action/hkNullActionCinfo.h>

class hkpEntity;
class hkpPhantom;
class hkpSimulationIsland;
class hkStepInfo;
class hkpWorld;

/// This is the base class from which user actions (or controllers) are derived. Actions
/// are the interface between user controllable behavior of the physical simulation and the Havok core.
class hkpNullAction : public hkpAction
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_ACTION);

		inline hkpNullAction() : hkpAction(0) {}

		virtual void applyAction( const hkStepInfo& stepInfo )      {}

		virtual void getEntities( hkArray<hkpEntity*>& entitiesOut ) {}

		virtual void entityRemovedCallback(hkpEntity* entity)        {}

		virtual hkpAction* clone( const hkArray<hkpEntity*>& entitiesIn, const hkArray<hkpPhantom*>& newPhantoms  ) const  { return HK_NULL; }

		static inline hkpNullAction* HK_CALL getNullAction(){ return HK_NULL; }

};

#endif // HK_DYNAMICS2_NULL_ACTION_H

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20101115)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2010
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
* 
* Use of this software for evaluation purposes is subject to and indicates
* acceptance of the End User licence Agreement for this product. A copy of
* the license is included with this software and is also available at www.havok.com/tryhavok.
* 
*/
