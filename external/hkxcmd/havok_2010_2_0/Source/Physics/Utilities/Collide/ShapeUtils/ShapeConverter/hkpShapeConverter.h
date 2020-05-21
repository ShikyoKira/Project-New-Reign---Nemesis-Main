/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKP_SHAPE_CONVERTER_H
#define HKP_SHAPE_CONVERTER_H


class hkpShape;
struct hkGeometry;

/// Converter class to turn a physics shape into a single piece of simple geometry
class hkpShapeConverter
{
public:
        /// Append add onto geom, transform add by trans
	static void HK_CALL appendTransform(hkGeometry* geom, const hkGeometry* add, const hkTransform& trans);

        /// Append add onto geom
	static void HK_CALL append(hkGeometry* geom, const hkGeometry* add);

        /// Convert a shape into a single piece of geometry
	static hkGeometry* HK_CALL toSingleGeometry(const hkpShape* shape);
      
        /// Returns true if consistent
    static hkBool HK_CALL checkConsistency(const hkGeometry* geom);

        /// Make a new copy
	static hkGeometry* HK_CALL clone(const hkGeometry& geom);
};

#endif // HKP_SHAPE_CONVERTER_H

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
