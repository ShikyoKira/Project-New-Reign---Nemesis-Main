/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKVEHICLE_TYREMARKS_hkTyremarksInfo_XML_H
#define HKVEHICLE_TYREMARKS_hkTyremarksInfo_XML_H


#include <Common/Base/hkBase.h>

extern const class hkClass hkpTyremarksWheelClass;

extern const class hkClass hkpTyremarkPointClass;

extern const class hkClass hkpTyremarksInfoClass;

class hkpVehicleInstance;
class hkpVehicleData;
	
/// A tyremark point is defined by two points (left and right and the strength of
/// the tyremark). Having two points instead of one allows for thickness in
/// tyremarks. The strength is a user value that can be used, for example, to
/// shade tyremarks depending on the amount of skidding
struct hkpTyremarkPoint
{
	public:
	
		HK_DECLARE_REFLECTION();

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_VEHICLE, hkpTyremarkPoint);

			/// Default constructor
		hkpTyremarkPoint();
		
		//
		// Methods
		//
		
			/// The strength of a tyremarkPoint is stored in the w-component
			/// of the vectors. The strength is in the range 0.0f to 255.0f.
		hkReal getTyremarkStrength() const;
		
		//
		// Members
		//
	public:
		
			/// The left position of the tyremark.
		hkVector4 m_pointLeft;
		
			/// The right position of the tyremark.
		hkVector4 m_pointRight;

	public: 
		hkpTyremarkPoint(hkFinishLoadedObjectFlag f) { }

};


/// hkpTyremarksWheel stores a list of tyremarks associated with a particular wheel.
/// This is a circular array, so old tyremarks eventually get replaced by new
/// tyremarks.
class hkpTyremarksWheel : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_VEHICLE);
		HK_DECLARE_REFLECTION();

			/// Default constructor
		hkpTyremarksWheel();

		virtual ~hkpTyremarksWheel() {}
		
		//
		// Methods
		//
		
			/// Sets the number of tyre mark points that can be stored.
		void setNumPoints(int num_points);
		
			/// Add a tyreMarkPoint to the array.
		void addTyremarkPoint( hkpTyremarkPoint& point);
		
			/// Returns the i-th stored tyremark point in the object.
		const hkpTyremarkPoint& getTyremarkPoint(int point) const;
		
		//
		// Members
		//
	public:
		
			/// Current position in the array of tyremarkPoints.
		int m_currentPosition;
		
			/// The number of points in the array
		int m_numPoints;
		
			/// Circular array of tyreMarkPoints.
		hkArray<struct hkpTyremarkPoint> m_tyremarkPoints;

	public: 
		hkpTyremarksWheel(hkFinishLoadedObjectFlag f) :
			hkReferencedObject(f), m_tyremarkPoints(f) { }

};


/// hkpTyremarksInfo stores a list of hkpTyremarksWheel for a particular vehicle
class hkpTyremarksInfo : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_VEHICLE);
		HK_DECLARE_REFLECTION();

		//
		// Methods
		//
		
			/// Updates Tyremark information
		virtual void updateTyremarksInfo(hkReal timestep, const hkpVehicleInstance* vehicle);
		
			/// Retrieves the Tyremark information in the form of a strip
		virtual void getWheelTyremarksStrips(const hkpVehicleInstance* vehicle, int wheel, hkVector4* strips_out) const;
		
			/// The constructor takes a hkpVehicleData object and the number of skidmark
			/// points to store.
		hkpTyremarksInfo(const hkpVehicleData& data, int num_points);
		
		virtual ~hkpTyremarksInfo();
		//
		// Members
		//
	public:
		
			/// The minimum energy a tyremark should have. The actual strength of a point will
			/// be scaled to be between 0.0f and 255.0f.
		hkReal m_minTyremarkEnergy;
		
			/// The maximum energy a tyremark should have. The actual strength of a point will
			/// be scaled to be between 0.0f and 255.0f.
		hkReal m_maxTyremarkEnergy;
		
			/// There is a hkpTyremarksWheel for each wheel.
		hkArray<class hkpTyremarksWheel*> m_tyremarksWheel;

	public: 
		void calcContentStatistics( hkStatisticsCollector* collector, const hkClass* cls) const;

		hkpTyremarksInfo(hkFinishLoadedObjectFlag f) :
			hkReferencedObject(f), m_tyremarksWheel(f) { }

};

#endif // HKVEHICLE_TYREMARKS_hkTyremarksInfo_XML_H

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
