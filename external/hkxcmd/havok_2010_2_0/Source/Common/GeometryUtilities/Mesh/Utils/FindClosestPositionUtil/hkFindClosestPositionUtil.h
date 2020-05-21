/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_FIND_CLOSEST_POSITION_UTIL_H
#define HK_FIND_CLOSEST_POSITION_UTIL_H

#include <Common/Base/Types/Geometry/Aabb/hkAabb.h>
#include <Common/Base/Memory/Allocator/FreeList/hkFreeList.h>

class hkFindClosestPositionUtil
{
    public:
        HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkFindClosestPositionUtil );

            /// Ctor
        hkFindClosestPositionUtil();

            /// Must be called before any points are added
        void start(const hkAabb& aabb, hkReal threshold = 1e-3f);
            /// Finds the vertex closest to point, returns -1 if not found
        int findClosest(const hkVector4& point) const;
            /// Returns the index of the created point (note that many points in the same position are allowed)
        int addPoint(const hkVector4& point);
			/// Add an array of points
		void addPoints(const hkVector4* points, int numPoints);

            /// Get the point with the position
        const hkVector4& getPoint(int index) const { return m_positions[index]; }
            /// Get the positions
        const hkArray<hkVector4>& getPoints() const { return m_positions; }

			/// Remove the point
		void removePoint( hkVector4Parameter point );

            /// Called when all points are added
        void end();

            /// Searches linearly for the closest (Generally significantly slower)
        int findClosestLinearly(const hkVector4& p) const;

			/// Get the threshold being used
		HK_FORCE_INLINE hkReal getThreshold() const { return m_threshold; }

#ifdef HK_DEBUG
		static hkResult HK_CALL selfCheck();
#endif

    protected:
        hkArray<hkVector4> m_positions;

        struct IntCoord
        {
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, hkFindClosestPositionUtil::IntCoord);

                /// ==
            HK_FORCE_INLINE hkBool32 operator==(const IntCoord& rhs) const { return m_x == rhs.m_x && m_y == rhs.m_y && m_z == rhs.m_z; }
                /// !=
            HK_FORCE_INLINE hkBool32 operator!=(const IntCoord& rhs) const { return !(*this == rhs); }

            HK_FORCE_INLINE hkUint32 calculateHash() const
            {
                hkUint32 hash = hkUint32(m_x) ^ ((hkUint32(m_y) << 16) + (hkUint32(m_y) >> 16)) ^ (hkUint32(m_z) << 8);
                // Make sure cannot be -1
                return hkUint32(hash & ~1);
            }

            int m_x;
            int m_y;
            int m_z;
        };
        struct Box
        {
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, hkFindClosestPositionUtil::Box);

            enum { MAX_INDICES = 8 };
            IntCoord m_coord;
            int m_indices[MAX_INDICES];
            int m_numIndices;
            Box* m_next;
        };

        void _findClosest(const IntCoord& coord, const hkVector4& point, hkReal& closestDistInOut, int& closestIndexInOut) const;
        HK_FORCE_INLINE void _calculateIntCoord(const hkVector4& p, IntCoord& coord) const;

		hkResult _findEntry(hkVector4Parameter point, Box*& boxOut, int& indexOut, int& boxIndexOut);

        hkFreeList m_boxFreeList;					///< Storage for all of the boxes
        hkPointerMap<hkUint32, Box*> m_hashMap;     ///< Maps a hashed coordinate to a singly linked list of boxes

        hkAabb m_aabb;
        hkReal m_threshold;

		hkVector4 m_offset;
		hkVector4 m_scale;
};

#endif // HK_FIND_CLOSEST_POSITION_UTIL_H

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
