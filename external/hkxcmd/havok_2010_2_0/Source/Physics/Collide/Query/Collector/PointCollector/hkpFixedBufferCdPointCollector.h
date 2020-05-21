/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_FIXED_BUFFER_CD_POINT_COLLECTOR_H
#define HK_FIXED_BUFFER_CD_POINT_COLLECTOR_H


#include <Physics/Collide/Agent/Query/hkpCdPointCollector.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpRootCdPoint.h>



typedef void (HK_CALL *hkFixedBufferCdPointCollectorAddCdPointCallbackFunc)(const hkpCdPoint& event, class hkpFixedBufferCdPointCollector* collector);

///
/// This collector class will always collect the n closest points (with n == 'capacity' as supplied in the constructor).
/// For now it is only used with hkCollisionQuery jobs. Its usage is hardcoded into the jobs, so you cannot derive and
/// use your derived class instead, but you can register your own custom addCdPoint() callback function by calling
/// registerFixedBufferCdPointCollectorAddCdPointCallbackFunction().
///
class hkpFixedBufferCdPointCollector : public hkpCdPointCollector
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_AGENT, hkpFixedBufferCdPointCollector);

	public:

			/// The constructor needs to be passed in a pointer to the result array and its maximum capacity.
		HK_FORCE_INLINE hkpFixedBufferCdPointCollector(hkpRootCdPoint* buffer, int capacity);
		
			/// Resets the early out condition and empties the array of hits.
			/// You must call this function if you want to reuse an object of this class.
		HK_FORCE_INLINE void reset();

		HK_FORCE_INLINE const hkpRootCdPoint* getHits() const;

		HK_FORCE_INLINE int getNumHits() const;

			/// Removes the first hit and copies elements down one slot as in the STL array.
		HK_FORCE_INLINE void removeFirstHitAndCopy();

		HK_FORCE_INLINE void removeHit( int index );

		HK_FORCE_INLINE hkBool hasHit() const;

			/// Sort the hits so that the closest hits are earliest in the hit list
		void sortHits();

			/// Call this static function to register the collector's default addCdPoint() callback implementation.
		static void HK_CALL HK_INIT_FUNCTION( registerDefaultAddCdPointFunction )();
		static void HK_CALL HK_INIT_FUNCTION( registerCustomAddCdPointFunction )(hkFixedBufferCdPointCollectorAddCdPointCallbackFunc func);

	public:

			/// The collector's default addCdPoint() implementation.
			///
			/// This implementation will collect up to n closest points (with n == 'capacity' as supplied in the class constructor).
			/// As long as there's room available in the internal array, new points will simply be appended.
			/// Once the maximum capacity is reached we will check if the new point is closer than the already available furthest
			/// point and replace the latter accordingly.
		static void HK_CALL addCdPointImplementation(const hkpCdPoint& event, hkpFixedBufferCdPointCollector* collector);

	protected:

		virtual HK_FORCE_INLINE void addCdPoint(const hkpCdPoint& event);

	public:

		hkPadSpu<hkpRootCdPoint*>	m_pointsArrayBase;	// the result array's base address
		hkPadSpu<int>				m_capacity;			// the maximum number of entries available in the result array
		hkPadSpu<int>				m_numPoints;		// the current number of entries available in the result array

};


//
// User callback registering stuff for hkpFixedBufferCdPointCollector::addCdPoint()
//
extern hkFixedBufferCdPointCollectorAddCdPointCallbackFunc g_FixedBufferCdPointCollectorAddCdPointCallbackFunc;


/// Register your custom hkpFixedBufferCdPointCollector::addCdPoint() implementation with this function.
///
/// Note that when using the hkpFixedBufferCdPointCollector you can only ever have one global, shared addCdPoint() implementation.
HK_FORCE_INLINE void registerFixedBufferCdPointCollectorAddCdPointCallbackFunction(hkFixedBufferCdPointCollectorAddCdPointCallbackFunc func)
{
	g_FixedBufferCdPointCollectorAddCdPointCallbackFunc = func;
}


#include <Physics/Collide/Query/Collector/PointCollector/hkpFixedBufferCdPointCollector.inl>


#endif // HK_FIXED_BUFFER_CD_POINT_COLLECTOR_H

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
