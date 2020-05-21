/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_FIXED_BUFFER_RAY_HIT_COLLECTOR_H
#define HK_FIXED_BUFFER_RAY_HIT_COLLECTOR_H


#include <Physics/Collide/Shape/Query/hkpRayHitCollector.h>


struct hkpWorldRayCastOutput;

#	define hkpWorldRayCastOutput_Storage hkpWorldRayCastOutput 

///
/// This collector class will always collect the n closest points (with n == 'capacity' as supplied in the constructor).
/// For now it is only used with hkCollisionQuery jobs. Its usage is hardcoded into the jobs, so you cannot derive and
/// use your derived class instead, but you can register your own custom addCdPoint() callback function by calling
/// registerFixedBufferCdPointCollectorAddCdPointCallbackFunction().
///
class hkpFixedBufferRayHitCollector : public hkpRayHitCollector
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_AGENT, hkpFixedBufferRayHitCollector);

	public:

			/// The constructor needs to be passed in a pointer to the result array and its maximum capacity.
		HK_FORCE_INLINE hkpFixedBufferRayHitCollector(hkpWorldRayCastOutput_Storage* buffer, int capacity);

			/// This resets the m_earlyOutHitFraction to 1. You should call this if you want to
			/// reuse the class for a new ray cast.
		inline void reset();

			/// Virtual destructor for derived objects
		virtual ~hkpFixedBufferRayHitCollector() { }

			/// Call this static function to register the collector's default addRayHit() callback implementation.
		static void HK_CALL registerDefaultAddRayHitFunction();

	public:

			/// The collector's default addRayHit() implementation.
			///
			/// This implementation will collect up to n closest hits (with n == 'capacity' as supplied in the class constructor).
			/// As long as there's room available in the internal array, new hits will simply be appended.
			/// Once the maximum capacity is reached we will check if the new hit is closer than the already available furthest
			/// hit and replace the latter accordingly.
		static void HK_CALL addRayHitImplementation(const hkpCdBody& cdBody, const hkpShapeRayCastCollectorOutput& hitInfo, hkpFixedBufferRayHitCollector* collector);

	protected:

		virtual HK_FORCE_INLINE void addRayHit(const hkpCdBody& cdBody, const hkpShapeRayCastCollectorOutput& hitInfo);

	public:

		hkPadSpu<hkpWorldRayCastOutput_Storage*>	m_rayCastOutputBase;	// the result array's base address
		hkPadSpu<hkpWorldRayCastOutput_Storage*>	m_nextFreeOutput;		// the next free entry in the result array
		hkPadSpu<int>				m_capacity;						// the maximum number of entries available in the result array
		hkPadSpu<int>				m_numOutputs;					// the current number of entries available in the result array

			/// This is only used when the collector is used on SPU.
			/// For each hit collected we output the rootCollidable pointer, and we must use the permanent pointer in main memory
			/// and not the temporary copy on SPU.
		hkPadSpu<const hkpCollidable*>	m_collidableOnPpu;

};


//
// User callback registering stuff for hkpFixedBufferRayHitCollector::addRayHit()
//

typedef void (HK_CALL *hkFixedBufferRayHitCollectorAddRayHitCallbackFunc)(const hkpCdBody& cdBody, const hkpShapeRayCastCollectorOutput& hitInfo, class hkpFixedBufferRayHitCollector* collector);

extern hkFixedBufferRayHitCollectorAddRayHitCallbackFunc g_FixedBufferRayHitCollectorAddRayHitCallbackFunc;


/// Register your custom hkpFixedBufferRayHitCollector::addCdPoint() implementation with this function.
///
/// Note that when using the hkpFixedBufferRayHitCollector you can only ever have one global, shared addCdPoint() implementation.
HK_FORCE_INLINE void registerFixedBufferRayHitCollectorAddRayHitCallbackFunction(hkFixedBufferRayHitCollectorAddRayHitCallbackFunc func)
{
	g_FixedBufferRayHitCollectorAddRayHitCallbackFunc = func;
}


#include <Physics/Collide/Query/Collector/RayCollector/hkpFixedBufferRayHitCollector.inl>

#endif // HK_FIXED_BUFFER_RAY_HIT_COLLECTOR_H

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
