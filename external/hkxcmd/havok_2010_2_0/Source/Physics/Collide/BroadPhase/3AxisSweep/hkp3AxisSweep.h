/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE_3_AXIS_SWEEP_H
#define HK_COLLIDE_3_AXIS_SWEEP_H

#include <Physics/Collide/BroadPhase/hkpBroadPhase.h>
#include <Common/Base/Container/LocalArray/hkLocalBuffer.h>
#include <Common/Base/Math/Vector/hkVector4Util.h>



// define to build in runtime checks
#	define CHECK_CONSISTENCY()


/// Implementation of an efficient broadphase algorithm. Best used with a scene where the number
/// of entities added and removed during runtime is not excessive. Works on a limited world size.
class hkp3AxisSweep : public hkpBroadPhase
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
#ifndef HK_BROADPHASE_32BIT
	typedef hkUint16 BpInt;
#else
	typedef hkUint32 BpInt;
#endif
	public:

			/// Anything within the min and max bounds will participate in broadphase culling.
		hkp3AxisSweep( const hkVector4& worldMin, const hkVector4& worldMax, int numMarkers);
			/// ###ACCESS_CHECKS###( [this,HK_ACCESS_RW] );
		~hkp3AxisSweep();

		void checkConsistency();

		//
		//	virtual functions implemented
		//

		virtual BroadPhaseType getType() const;
		
			/// ###ACCESS_CHECKS###( [this,HK_ACCESS_RW] );
		virtual void addObject( hkpBroadPhaseHandle* object, const hkAabbUint32& aabb, hkArray<hkpBroadPhaseHandlePair>& newPairs );

		virtual void addObject(    hkpBroadPhaseHandle* object, const hkAabb& aabb, hkArray<hkpBroadPhaseHandlePair>& newPairsOut  );

			/// ###ACCESS_CHECKS###( [this,HK_ACCESS_RW] );
		virtual void addObjectBatch( const hkArrayBase<hkpBroadPhaseHandle*>& addObjectList, const hkArrayBase<hkAabb>& addAabbList, hkArray<hkpBroadPhaseHandlePair>& newPairs );

			/// ###ACCESS_CHECKS###( [this,HK_ACCESS_RW] );
		virtual void removeObject( hkpBroadPhaseHandle* object, hkArray<hkpBroadPhaseHandlePair>& delPairsOut );

			/// ###ACCESS_CHECKS###( [this,HK_ACCESS_RW] );
		virtual void removeObjectBatch( const hkArrayBase<hkpBroadPhaseHandle*>& removeObjectList, hkArray<hkpBroadPhaseHandlePair>& delPairsOut );

			/// This function returns the number of objects excluding the first objects
			/// which refers to the world's extents.
			/// ###ACCESS_CHECKS###( [this,HK_ACCESS_RO] );
		virtual int getNumObjects() const;

		virtual void updateAabbs(  hkpBroadPhaseHandle* objects[], const hkAabb* aabbs, int numObjects, hkArray<hkpBroadPhaseHandlePair>& addedPairsOut, hkArray<hkpBroadPhaseHandlePair>& removedPairsOut );

		virtual void updateAabbsUint32(  hkpBroadPhaseHandle* objects[], const hkAabbUint32* aabbs, int numObjects, hkArray<hkpBroadPhaseHandlePair>& addedPairsOut, hkArray<hkpBroadPhaseHandlePair>& removedPairsOut );

			/// ###ACCESS_CHECKS###( [this,HK_ACCESS_RW] );
		virtual void defragment();

			/// ###ACCESS_CHECKS###( [this,HK_ACCESS_RO] );
		virtual void querySingleAabb( const hkAabb& aabb, hkArray<hkpBroadPhaseHandlePair>& pairs_out) const;

			/// ###ACCESS_CHECKS###( [this,HK_ACCESS_RO] );
		virtual void querySingleAabbWithCollector( const hkAabb& aabb, hkpBroadPhaseCastCollector* collector) const;

			/// ###ACCESS_CHECKS###( [this,HK_ACCESS_RO] );
		virtual void reQuerySingleObject( const hkpBroadPhaseHandle* object, hkArray<hkpBroadPhaseHandlePair>& pairs_out) const;

		virtual bool areAabbsOverlapping( const hkpBroadPhaseHandle* bhA, const hkpBroadPhaseHandle* bhB ) const;

			/// Gets all AABBs of all objects, good for debugging purposes.
			/// Can be used to display the broadphase contents, e.g.
			/// \code
			/// reinterpret_cast<hkp3AxisSweep*>(broadPhase)->getAllAabbs( allAabbs );
			/// displayAabbs(allAabbs);
			/// \endcode
			/// ###ACCESS_CHECKS###( [this,HK_ACCESS_RO] );
		virtual void getAllAabbs( hkArray<hkAabb>& allAabbs ) const;

			/// ###ACCESS_CHECKS###( [this,HK_ACCESS_RO] );
		virtual void getAabb(const hkpBroadPhaseHandle* object, hkAabb& aabb) const;

		virtual void getExtents(hkVector4& worldMinOut, hkVector4& worldMaxOut) const;


			/// Fast Raycast through the broadphase using 3dda.
			/// ###ACCESS_CHECKS###( [this,HK_ACCESS_RO] );
		virtual	void castRay( const hkpCastRayInput& input, hkpBroadPhaseCastCollector* collectorBase, int collectorStriding) const;

			/// ###ACCESS_CHECKS###( [this,HK_ACCESS_RO] );
		virtual void castAabb( const hkpCastAabbInput& input, hkpBroadPhaseCastCollector& collector )    const;

		virtual int getAabbCacheSize() const;

			/// ###ACCESS_CHECKS###( [this,HK_ACCESS_RO] );
		virtual void calcAabbCache( const hkAabb& aabb, hkpBroadPhaseAabbCache* AabbCacheOut) const;

		virtual void calcAabbCache( const hkArrayBase<hkpCollidable*>& overlappingCollidables, hkpBroadPhaseAabbCache* AabbCacheOut) const;
		class hkpBpNode;
	protected:
		void calcAabbCacheInternal( const hkArrayBase<const hkpBpNode*>& overlaps, hkpBroadPhaseAabbCache* AabbCacheOut) const;
	public:


			/// ###ACCESS_CHECKS###( [this,HK_ACCESS_RO] );
		void calcContentStatistics( hkStatisticsCollector* collector, const hkClass* cls) const;

			/// ###ACCESS_CHECKS###( [this,HK_ACCESS_RW] );
		virtual void shiftAllObjects( const hkVector4& shiftDistance, hkVector4& effectiveShiftDistanceOut, hkArray<hkpBroadPhaseHandlePair>& newCollisionPairs );

			/// ###ACCESS_CHECKS###( [this,HK_ACCESS_RW] );
		virtual void shiftBroadPhase( const hkVector4& shiftDistance, hkVector4& effectiveShiftDistanceOut, hkArray<hkpBroadPhaseHandlePair>& newCollisionPairs );

			/// Get the AABB used for creation of this broadphase.
		const hkAabb& getOriginalAabb() const;

		void set32BitOffsetAndScale(const hkVector4& offsetLow, const hkVector4& offsetHigh, const hkVector4& scale);

		enum MarkerHandling
		{
			MARKERS_ADD_NEW_OVERLAPS,
			MARKERS_IGNORE,
			MARKERS_REMOVE_NEW_OVERLAPS,
		};

			/// Helper method to access a fast 1-axis sweep.
		void collide1Axis( const hkpBpNode* pa, int numA, const hkpBpNode* pb, int numB, MarkerHandling markerHandlingForNodesB, hkArray<hkpBroadPhaseHandlePair>& pairsOut);

			/// Helper method to access a fast 1-axis sweep.
		void HK_CALL collide1Axis( const hkpBpNode* pa, int numA, hkArray<hkpBroadPhaseHandlePair>& pairsOut );

	public:

		class hkpBpMarker
		{
			public:
				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BROAD_PHASE, hkpBpMarker);
				BpInt m_nodeIndex;
				BpInt m_value;
				hkArray<BpInt> m_overlappingObjects;
		};

		class hkpBpNode
		{
			public:
				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BROAD_PHASE, hkpBpNode );

				static const int s_memberOffsets[];

				BpInt min_y;
				BpInt min_z;
				BpInt max_y;
				BpInt max_z;
				BpInt min_x;
				BpInt max_x;
				hkpBroadPhaseHandle* m_handle;

				HK_FORCE_INLINE BpInt& getMin(int index)
				{
					if ( index == 0 ) return min_x;
					if ( index == 1 ) return min_y;
					return min_z;
				}

				HK_FORCE_INLINE BpInt& getMax(int index)
				{
					if ( index == 0 ) return max_x;
					if ( index == 1 ) return max_y;
					return max_z;
				}

				template<int index>
				HK_FORCE_INLINE BpInt& _getMin()
				{
					if ( index == 0 ) return min_x;
					if ( index == 1 ) return min_y;
					return min_z;
				}

				template<int index>
				HK_FORCE_INLINE BpInt& _getMax()
				{
					if ( index == 0 ) return max_x;
					if ( index == 1 ) return max_y;
					return max_z;
				}


				HK_FORCE_INLINE void setElem( int axis, int minmax, int value )
				{
#ifndef HK_BROADPHASE_32BIT
					HK_ASSERT(0x5887d49e,  value < 0x10000);
#endif
					BpInt* p = static_cast<BpInt*>(hkAddByteOffset( &min_y, s_memberOffsets[axis * 2 + minmax]));
					p[0] = BpInt(value);
				}

				HK_FORCE_INLINE int isMarker() const
				{
					return int(hkUlong(m_handle))& 1;
				}

				HK_FORCE_INLINE hkpBpMarker& getMarker( hkpBpMarker* markers) const
				{
					// Is 64-bit safe
					return *hkAddByteOffset( markers, int(hkUlong(m_handle)) & ~1 );
				}


				// check whether the objects are disjoint
				HK_FORCE_INLINE hkInt32 xyDisjoint( const hkpBpNode& other ) const;
				HK_FORCE_INLINE hkInt32 xzDisjoint( const hkpBpNode& other ) const;
				HK_FORCE_INLINE hkUint32 yzDisjoint( const hkpBpNode& other ) const;
		};

	public:
		class hkpBpEndPoint
		{
			public:
				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BROAD_PHASE, hkp3AxisSweep::hkpBpEndPoint );

				hkBool32 isMaxPoint() const { return m_value & 1; }
				static hkBool32 HK_CALL isMaxPoint( int value )  { return value & 1; }
				hkBool operator <(const hkpBpEndPoint &b) const 	{	return m_value < b.m_value
#if HK_CONFIG_THREAD == HK_CONFIG_MULTI_THREADED
				 || (m_value == b.m_value && m_nodeIndex < b.m_nodeIndex)
#endif
				 ;	}

			public:
				BpInt m_value;
				BpInt m_nodeIndex;
		};



#ifndef HK_BROADPHASE_32BIT
		// Note: AABB_MAX_VALUE is the integer max value, AABB_MAX_FVALUE is the one we use for floating point clipping
	enum { AABB_MIN_VALUE = 0, AABB_MAX_VALUE = 0xfffc, AABB_MAX_FVALUE = 0xfffc, HK_BP_NUM_VALUE_BITS = 16 };
#else
		// we need to decrease the fvalue significantly to make sure that our result value is smaller than the AABB_MAX_VALUE
	enum { AABB_MIN_VALUE = 0, AABB_MAX_VALUE = 0x7ffffffe, AABB_MAX_FVALUE = 0x7ffe0000, HK_BP_NUM_VALUE_BITS = 31 };
#endif

	static HK_ALIGN16( hkUint32 OneMask[4] );
	static hkQuadReal MaxVal;

	public:
		class hkpBpAxis
		{
			public:
				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(MEMORY_CLASS_BASE, hkpBpAxis);
				hkpBpAxis(){}
				hkpBpAxis( hkpBpEndPoint *ep, int initialArraySize) : m_endPoints( ep, 0, initialArraySize ) { }

			public:
				hkArray<hkpBpEndPoint> m_endPoints;

				void mergeBatch( hkpBpNode *nodes, int newIdx, int newNum, int axis, hkpBpEndPoint* scratch );

				template<int axis>
				inline void removeBatch( hkpBpNode *nodes, const hkArrayBase<int>& nodeRelocations );
				void insert( hkpBpNode* nodes, int nodeIndex, BpInt min_position, BpInt maxPosition, BpInt& minInsertPositionOut, BpInt&  maxInsertPositionOut );

					// for batch add 
				void remove( int minPosition, int maxPosition);
				int  insertTail( hkpBpNode *nodes, int axis, hkpBpEndPoint* newNodes, int numNewNodes );

				template< int axis > HK_FORCE_INLINE void mergeRest( hkpBpNode *nodes, int startOfTail, hkpBpEndPoint* newEndPoints, int numNewEndPoints );


				///	Finds a point P using the following rules.
				/// \code
				///	P[-1].m_value < value
				///	P[0].m_value >= value
				/// \endcode
				const hkpBpEndPoint* find( const hkpBpEndPoint* start, const hkpBpEndPoint* end, BpInt value) const;
		};

	public:

		static void HK_FAST_CALL beginOverlapCheckMarker(   hkpBpMarker* markers, hkpBpNode& a, int nodeIndexA, hkpBpNode& b, hkArray<hkpBroadPhaseHandlePair>& m_newPairsOut);
		static void HK_FAST_CALL endOverlapCheckMarker  (   hkpBpMarker* markers, hkpBpNode& a, int nodeIndexA, hkpBpNode& b, hkArray<hkpBroadPhaseHandlePair>& m_deletedPairsOut);

	public:
		static void HK_FAST_CALL beginOverlap( hkpBpNode& a, hkpBpNode& b, hkArray<hkpBroadPhaseHandlePair>& m_newPairsOut);
		static void HK_FAST_CALL endOverlap(   hkpBpNode& a, hkpBpNode& b, hkArray<hkpBroadPhaseHandlePair>& m_deletedPairsOut);

		HK_FORCE_INLINE static void HK_CALL _convertAabbToInt( const hkAabb& aabb, const hkVector4& offsetLow, const hkVector4& offsetHigh, const hkVector4& scale, hkAabbUint32& aabbOut );
		HK_FORCE_INLINE void _convertAabbToInt( const hkAabb& aabb, hkAabbUint32& aabbOut ) const;
		HK_FORCE_INLINE void convertVectorToInt( const hkVector4& vec, hkUint32* intsOut) const;

		HK_FORCE_INLINE void convertAabbToBroadPhaseResolution(const hkAabbUint32& aabbIn, hkAabbUint32& aabbOut);

		HK_FORCE_INLINE int getNumMarkers() const;

		HK_FORCE_INLINE void getOffsetLowHigh32bit(hkVector4& offsetLow32bit, hkVector4& offsetHigh32bit) const;

			// recheck the sorting order of the endpoints if the node id of a node has changed to a lower number
			/// ###ACCESS_CHECKS###( [this,HK_ACCESS_RW] );
		void _fixDeterministicOrderAfterNodeIdWasDecreased( int nodeId );

	public:
		enum hkpBpMarkerUse { HK_BP_NO_MARKER, HK_BP_USE_MARKER };
		//HK_FORCE_INLINE	void updateAxis( int axisIndex, hkpBpNode* nodes, hkpBpNode& node, hkUint32 nodeIndex, hkUint32 new_min, hkUint32 new_max, hkpBpMarkerUse marker, hkArray<hkpBroadPhaseHandlePair>& m_newPairsOut, hkArray<hkpBroadPhaseHandlePair>& m_deletedPairsOut);
			/// ###ACCESS_CHECKS###( [this,HK_ACCESS_RW] );
		HK_FORCE_INLINE void updateAabb( hkpBroadPhaseHandle* object, const hkAabbUint32& aabb, hkArray<hkpBroadPhaseHandlePair>& m_newPairsOut, hkArray<hkpBroadPhaseHandlePair>& m_deletedPairsOut);
	protected:
		void updateNodesAfterInsert( hkpBpNode *nodes, int numNodes, hkpBpNode& newNode );
		void updateNodesAfterDelete( hkpBpNode *nodes, int numNodes, hkpBpNode& oldNode );
		void updateNodesAfterBatchTailInsert( hkpBpNode *nodes, int numNodes, int numNewNodes, int* offsets );


		HK_FORCE_INLINE void querySingleAabbAddObject( hkpBroadPhaseHandle* object, int newNodeIndex, const hkUint32 *bitfield, hkpBpNode& refNode, hkArray<hkpBroadPhaseHandlePair>& pairs_out) const;
		HK_FORCE_INLINE void querySingleAabbRemoveObject( hkpBroadPhaseHandle* object, int newNodeIndex, const hkUint32 *bitfield, hkpBpNode& refNode, hkArray<hkpBroadPhaseHandlePair>& pairs_out) const;
		                void setBitsBasedOnXInterval( int numNodes, int x_value, const hkpBpNode& queryNode, BpInt queryNodeIndex, hkUint32* bitField) const;

		enum hkpBpQuerySingleType { HK_BP_REPORT_HANDLES, HK_BP_REPORT_NODES, HK_BP_COLLECTOR };
		HK_FORCE_INLINE void querySingleAabbSub( hkpBroadPhaseHandle* object, const hkUint32 *bitfield, hkpBpNode& refNode, hkpBpQuerySingleType type, hkArray<hkpBroadPhaseHandlePair>* pairs_out, hkArrayBase<const hkpBpNode*>* nodesOut, hkpBroadPhaseCastCollector* collector) const;
		HK_FORCE_INLINE void _querySingleAabb( const hkAabb& aabb, hkpBpQuerySingleType type, hkArray<hkpBroadPhaseHandlePair>* pairs_out, hkArrayBase<const hkpBpNode*>* nodesOut, hkpBroadPhaseCastCollector* collector) const;
		HK_FORCE_INLINE void addNodePair( const hkpBpNode* n0, const hkpBpNode* n1, hkArray<hkpBroadPhaseHandlePair>& pairsOut, hkBool addMode) const;
			// expects these nodes endpoints to be sorted into the axes already
			// for every node we want to query, a bit with the node index in the bitFieldOfQueryNodes has to be set. (see staticFlipBit)
		void queryBatchAabbSub( const hkUint32* bitFieldOfQueryNodes, hkArray<hkpBroadPhaseHandlePair>& pairsOut, hkBool addMode ) const;

			/// ###ACCESS_CHECKS###( [this,HK_ACCESS_RO] );
		void	getAabbFromNode(const hkpBpNode& node, hkAabb & aabb) const;

	public:
		void checkDeterminism();


		//
		//	Internal public section
		//
	public:
			// broadphase object variables
		hkAabb m_aabb;
		hkVector4 m_offsetLow;
		hkVector4 m_offsetHigh;
		hkVector4 m_scale;
		hkVector4 m_offsetLow32bit;
		hkVector4 m_offsetHigh32bit;
		hkVector4 m_scale32bit;

		hkArray<hkpBpNode> m_nodes;
		hkpBpAxis m_axis[3];

		int m_numMarkers;
		int m_ld2NumMarkers;
		hkpBpMarker *m_markers;

		// We currently do not know the rounding mode of our float to int conversion
		// if you use convertAabbToInt( x + m_intToFloatFloorCorrection) you will be sure that
		// the result will be identical to int( floor( x ) )
		hkReal    m_intToFloatFloorCorrection;
};


#include <Physics/Collide/BroadPhase/3AxisSweep/hkp3AxisSweep.inl>


#endif // HK_COLLIDE_3_AXIS_SWEEP_H

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
