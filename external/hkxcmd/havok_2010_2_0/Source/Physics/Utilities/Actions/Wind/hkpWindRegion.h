/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_WIND_REGION_H
#define HK_WIND_REGION_H

#include <Physics/Dynamics/Phantom/hkpAabbPhantom.h>
#include <Physics/Dynamics/World/Listener/hkpWorldPostSimulationListener.h>

#include <Physics/Utilities/Actions/Wind/hkpWind.h>

/// Given a phantom and a wind, will apply the wind to those rigid bodies intersecting the
/// phantom.
class hkpWindRegion : public hkReferencedObject, public hkpWorldPostSimulationListener
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Constructor.
		hkpWindRegion( hkpAabbPhantom* phantom, const hkpWind* wind, hkReal resistanceFactor, hkReal obbFactor = 0.0f );
			/// Destructor.
		virtual ~hkpWindRegion();

			/// Listener callback.
		virtual void postSimulationCallback( hkpWorld* world );

	private:
			/// The phantom which represents the region.
		hkpAabbPhantom* m_phantom;
			/// The wind that this phantom should apply to intersecting bodies.
		const hkpWind *const m_wind;
			///
		hkReal m_resistanceFactor;
			///
		hkReal m_obbFactor;
};

#endif // HK_WIND_REGION_H

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
