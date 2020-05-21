/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_CHAIN_DATA_H
#define HK_DYNAMICS2_CHAIN_DATA_H

#include <Physics/ConstraintSolver/Solve/hkpSolverResults.h>
#include <Physics/Dynamics/Constraint/hkpConstraintData.h>
#include <Physics/Dynamics/Action/hkpArrayAction.h>

extern const class hkClass hkpConstraintChainDataClass;

	/// Base class for constraint-chain data's.
	/// See hkpConstraintChainInstance for more information.
class hkpConstraintChainData : public hkpConstraintData
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// Default constructor.
		inline hkpConstraintChainData() {}

			/// Returns number of stored ConstraintInfos. hkConstraintChainInstances that use this data may
			/// have up to (getNumConstraintInfos() + 1) bodies. When their number is lesser, the ConstraintInfos at the
			/// end of the list are ignored.
		virtual int getNumConstraintInfos() = 0;

			/// Serialization constructor
		hkpConstraintChainData(hkFinishLoadedObjectFlag f) : hkpConstraintData(f) {}

};




#endif // HK_DYNAMICS2_CHAIN_DATA_H

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
