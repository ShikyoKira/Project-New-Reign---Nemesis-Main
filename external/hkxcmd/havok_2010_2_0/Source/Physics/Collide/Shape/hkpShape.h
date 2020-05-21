/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_SHAPE_H
#define HK_COLLIDE2_SHAPE_H

#include <Common/Base/hkBase.h>
#include <Common/Base/Types/Physics/ContactPoint/hkContactPoint.h>
#include <Physics/Collide/Shape/hkpShapeType.h>
#include <Common/Internal/Collide/Gsk/hkCdVertex.h>
#include <Physics/Collide/Shape/hkpShapeBuffer.h>

class hkAabb;
struct hkpShapeRayCastInput;
struct hkpShapeRayCastOutput;
struct hkpShapeRayBundleCastInput;
struct hkpShapeRayBundleCastOutput;
class hkpRayHitCollector;
class hkpCdBody;
class hkpShapeContainer;
class hkSphere;

typedef hkUint16 hkpVertexId;
typedef hkUint32 hkpShapeKey;

class hkpShapeKeyPair
{
public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CDINFO, hkpShapeKeyPair );

	hkpShapeKey m_shapeKeyA; // only used by nm machine
	hkpShapeKey m_shapeKeyB; // used by 1n & nm machines

	HK_FORCE_INLINE bool operator==(const hkpShapeKeyPair& p1) const { return m_shapeKeyB == p1.m_shapeKeyB && m_shapeKeyA == p1.m_shapeKeyA; }
	HK_FORCE_INLINE bool operator< (const hkpShapeKeyPair& p1) const { return (m_shapeKeyA < p1.m_shapeKeyA) || (m_shapeKeyA == p1.m_shapeKeyA && m_shapeKeyB < p1.m_shapeKeyB); }
};

#define HK_INVALID_SHAPE_KEY 0xffffffff
#define HK_INVALID_VERTEX_ID 0xffff

#	define HKP_SHAPE_VIRTUAL virtual
#	define HKP_SHAPE_VIRTUAL_CONST const
#	define HKP_SHAPE_VIRTUAL_THIS 
#	define HKP_SHAPE_VIRTUAL_THIS2 
#	define HK_GET_THIS_PTR this
#	define getChildShapeImpl getChildShape
#	define getCollisionFilterInfoImpl getCollisionFilterInfo

#define HK_REGISTER_CONVEX_AABB_FUNCS_IN_NARROWPHASE_ELF 0

#define HK_DECLARE_GET_SIZE_FOR_SPU(CLASS) int calcSizeForSpu(const CalcSizeForSpuInput& input, int spuBufferSizeLeft) const { return sizeof(*this); }

# define HK_SHAPE_CONTAINER hkpShapeContainer

extern const hkClass hkpShapeClass;
class hkpConvexShape;

/// The base class for narrowphase collision detection objects.
/// All narrowphase collision detection is performed between pairs of hkpShape objects by creating appropriate hkpCollisionAgent objects.
/// An hkpShape can be a simple shape such as a box or sphere, a shape with additional transform information,
/// or a compound shape made up of simpler hkShapes. hkpShape instances can be shared within or even between
/// rigid bodies. See the hkpShape subclasses for more details.
class hkpShape : public hkReferencedObject
{
	public:

		HK_DECLARE_REFLECTION();
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CDINFO);

			/// Sets the user data to 0
			/// Sets the type to the specified type
		HK_FORCE_INLINE hkpShape( hkpShapeType type );

			/// Gets the hkpShape type. This is used by the collision dispatcher to dispatch between pairs of shapes
		HK_FORCE_INLINE hkpShapeType getType() const;

			/// Gets the AABB for the hkpShape given a local to world transform and an extra tolerance.
			/// 
			/// Forwards to getAabbImpl()
		HK_FORCE_INLINE void getAabb( const hkTransform& localToWorld, hkReal tolerance, hkAabb& out ) const;

			/// Support for creating bounding volume hierarchies of shapes.
			/// This function returns the maximal extent of a shape along a given direction.
			/// It is not the same as hkpConvexShape::getSupportingVertex, because the extent does not have to be exact, it just has to at least
			/// contain the shape. It is for the purposes of creating bounding volumes around the shape ( mid-phase ) rather than exact collision
			/// detection (narrow-phase).
			/// The default implementation of this function uses the AABB of the shape. For custom shapes, you can get a better fit.
		virtual hkReal getMaximumProjection( const hkVector4& direction ) const;

			/// Get the user data for the shape (initialized to 0)
		inline hkUlong getUserData() const;

			/// Set the user data of the shape: This is a real user data and not used by the engine otherwise.
			/// If you are interested in triangle indices, you can retrieve this information from the hkpCdBody
			/// during most callbacks.
		inline void setUserData( hkUlong data );
		
			/// Finds the closest intersection between the shape and a ray defined in the shape's local space, starting at fromLocal, ending at toLocal.
			/// This is data driven, and places the results in hkpShapeRayCastOutput
			/// Implementation notes: For all convex shapes except hkSphere and hkCapsule the radius of the shape will be ignored.
		HK_FORCE_INLINE hkBool castRay( const hkpShapeRayCastInput& input, hkpShapeRayCastOutput& output ) const;

			/// Casts a bundle of rays against the shape
		HK_FORCE_INLINE hkVector4Comparison castRayBundle( const hkpShapeRayBundleCastInput& input, hkpShapeRayBundleCastOutput& output) const;
		HK_FORCE_INLINE hkVector4Comparison castRayBundle( const hkpShapeRayBundleCastInput& input, hkpShapeRayBundleCastOutput& output,  hkVector4ComparisonParameter mask ) const;

			/// Finds the closest intersection between the shape and a ray defined in the shape's local space, starting at fromLocal, ending at toLocal.
			/// This is a callback driven raycast. For each hit found, the hkpRayHitCollector receives a callback with the hit info.
			/// Implementation notes: For all convex shapes except hkSphere and hkCapsule the radius of the shape will be ignored.
		HK_FORCE_INLINE void castRayWithCollector( const hkpShapeRayCastInput& input, const hkpCdBody& cdBody, hkpRayHitCollector& collector ) const;

			/// Query if the shape supports the container interface.
			/// Returns a pointer to the interface if the shape has one or more child shapes.
			/// Otherwise returns null.
		virtual const hkpShapeContainer* getContainer() const { return HK_NULL; }

			/// Returns true if the shape is a convex shape
		virtual bool isConvex() const { return false; }


			// Returns the size of the shape class. The default implementation returns -1 which will force the shape onto the PPU.
		struct CalcSizeForSpuInput
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpShape::CalcSizeForSpuInput );

			bool m_midphaseAgent3Registered;
			bool m_isFixedOrKeyframed;
			bool m_hasDynamicMotionSaved;
		};

		virtual int calcSizeForSpu(const CalcSizeForSpuInput& input, int spuBufferSizeLeft) const;

	protected:	


			/// Gets the AABB for the hkpShape given a local to world transform and an extra tolerance.
		HKP_SHAPE_VIRTUAL void getAabbImpl( HKP_SHAPE_VIRTUAL_THIS const hkTransform& localToWorld, hkReal tolerance, hkAabb& out ) HKP_SHAPE_VIRTUAL_CONST = 0;

			/// hkpShape::castRay() interface implementation.
		HKP_SHAPE_VIRTUAL hkBool castRayImpl( HKP_SHAPE_VIRTUAL_THIS const hkpShapeRayCastInput& input, hkpShapeRayCastOutput& output ) HKP_SHAPE_VIRTUAL_CONST = 0;

			/// hkpShape::castRayWithCollector() interface implementation.
		HKP_SHAPE_VIRTUAL void castRayWithCollectorImpl( HKP_SHAPE_VIRTUAL_THIS const hkpShapeRayCastInput& input, const hkpCdBody& cdBody, hkpRayHitCollector& collector ) HKP_SHAPE_VIRTUAL_CONST = 0;


		HKP_SHAPE_VIRTUAL hkVector4Comparison castRayBundleImpl( HKP_SHAPE_VIRTUAL_THIS const hkpShapeRayBundleCastInput& input, hkpShapeRayBundleCastOutput& output, hkVector4ComparisonParameter mask  ) HKP_SHAPE_VIRTUAL_CONST;

	
	public:

			// These only need to be called in debug; the table is always fully written.
			static void HK_CALL resetShapeFunctions();
			static void HK_CALL verifyShapeFunctions();

			//
			//	A hand coded vtable replacement, needed to run code on the spu
			//

			typedef void (HK_CALL *GetSupportingVertexFunc)        (const void* thisObj, hkVector4Parameter direction, hkpCdVertex &supportVertex );
			typedef void (HK_CALL *ConvertVertexIdsToVerticesFunc)	(const void* thisObj, const hkpVertexId* ids, int numIds, class hkpCdVertex* verticesOut );
			typedef int (HK_CALL *WeldContactPointFunc)	(const void* thisObj, hkpVertexId* featurePoints, hkUint8& numFeaturePoints, hkVector4& contactPointWs, const hkTransform* thisObjTransform, const hkpConvexShape* collidingShape, const hkTransform* collidingTransform, hkVector4& separatingNormalInOut );

			typedef void (HK_CALL *GetCentreFunc)	(const void* thisObj, hkVector4& centreOut );

			typedef int  (HK_CALL *GetNumCollisionSpheresFunc)		(const void* thisObj );
			typedef const hkSphere* (HK_CALL *GetCollisionSpheresFunc)(const void* thisObj, hkSphere* sphereBuffer );

			typedef void (HK_CALL *GetAabbFunc)	(const void* thisObj, const hkTransform& localToWorld, hkReal tolerance, hkAabb& out );
			typedef hkBool (HK_CALL *CastRayFunc) (const void* thisObj, const hkpShapeRayCastInput& input, hkpShapeRayCastOutput& output );
			typedef void (HK_CALL *CastRayWithCollectorFunc) (const void* thisObj, const hkpShapeRayCastInput& input, const hkpCdBody& cdBody, hkpRayHitCollector& collector );
			typedef hkVector4Comparison (HK_CALL *CastRayBundleFunc) (const void* thisObj, const hkpShapeRayBundleCastInput& input, hkpShapeRayBundleCastOutput& output, hkVector4ComparisonParameter mask );

			typedef const hkpShape* (HK_CALL *GetChildShapeFunc) (const void* thisObj, hkpShapeKey key, hkpShapeBuffer& buffer );
			typedef hkUint32 (HK_CALL *GetCollisionFilterInfoFunc) (const void* thisObj, hkpShapeKey key );

			// Structure to store shape functions.  Effectively a shape's v-table.  Each function is padded to a 16 byte address.
			struct ShapeFuncs
			{
				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpShape::ShapeFuncs );

				hkPadSpu<GetSupportingVertexFunc>			m_getSupportingVertexFunc;
				hkPadSpu<ConvertVertexIdsToVerticesFunc>	m_convertVertexIdsToVertices;
				hkPadSpu<WeldContactPointFunc>				m_weldContactPointFunc;
				hkPadSpu<GetCentreFunc>						m_getCentreFunc;
				hkPadSpu<GetNumCollisionSpheresFunc>		m_getNumCollisionSpheresFunc;
				hkPadSpu<GetCollisionSpheresFunc>			m_getCollisionSpheresFunc;
				hkPadSpu<GetAabbFunc>						m_getAabbFunc;
				hkPadSpu<CastRayFunc>						m_castRay;
				hkPadSpu<CastRayWithCollectorFunc>			m_castRayWithCollector;
				hkPadSpu<CastRayBundleFunc>					m_castRayBundle;
				hkPadSpu<GetChildShapeFunc>					m_getChildShapeFunc;
				hkPadSpu<GetCollisionFilterInfoFunc>		m_getCollisionFilterInfoFunc;

				void HK_INIT_FUNCTION(reset)();
			};

			// helper functions to be used with the hkpCollisionDispatcher
			struct ShapeFuncs2
			{
				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpShape::ShapeFuncs2 );

				HK_ALIGN( GetSupportingVertexFunc	m_getSupportingVertexFunc, 64);
				ConvertVertexIdsToVerticesFunc		m_convertVertexIdsToVertices;
				WeldContactPointFunc				m_weldContactPointFunc;
				GetCentreFunc						m_getCentreFunc;
				GetNumCollisionSpheresFunc			m_getNumCollisionSpheresFunc;
				GetCollisionSpheresFunc				m_getCollisionSpheresFunc;
				GetAabbFunc							m_getAabbFunc;
				CastRayFunc							m_castRay;
				CastRayWithCollectorFunc			m_castRayWithCollector;
				CastRayBundleFunc					m_castRayBundle;
				GetChildShapeFunc					m_getChildShapeFunc;
				GetCollisionFilterInfoFunc			m_getCollisionFilterInfoFunc;
			};

			typedef void (HK_CALL *RegisterFunc)(ShapeFuncs& sf);

	public:

	public:

		hkUlong m_userData;

	public:
		hkEnum< ::hkpShapeType, hkUint32> m_type; //+nosave

	public:

		hkpShape( class hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag)
		{
			if( flag.m_finishing )
			{
				m_type = HK_SHAPE_INVALID;
			}
		}
};

/// Interface for an object capable of modifying a shape's internal data (i.e., alter the vertices of a convex vertices shape)
class hkpShapeModifier : public hkReferencedObject
{
	public:

		HK_DECLARE_REFLECTION();
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CDINFO);

	public:

		hkpShapeModifier()
		:	hkReferencedObject()
		{}

		hkpShapeModifier(class hkFinishLoadedObjectFlag flag)
		:	hkReferencedObject(flag)
		{}

		virtual ~hkpShapeModifier()
		{}

	public:

		/// This function will be called from hkWorldOperation::WORLD_OBJECT_UPDATE_SHAPE if the shape to be updated has a valid modifier
		virtual void modifyShape(hkpShape* shapeInOut) = 0;
};


#include <Physics/Collide/Shape/hkpShape.inl>

#endif // HK_COLLIDE2_SHAPE_H

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
