/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_MESH_VERTEX_BUFFER_H
#define HK_MESH_VERTEX_BUFFER_H


extern const class hkClass hkVertexFormatElementClass;

extern const class hkClass hkVertexFormatClass;

/// The hkVertexFormat defines the 'elements' which make up a vertex.
///
/// An element in this context is a usage and associated type. For example a vertex buffer which has positional
/// and normal information would consist of two elements. An element for position (USAGE_POSITION), of 3 floats (TYPE_FLOAT32, 3 values),
/// and another for the normal (USAGE_NORMAL) or 3 floats (TYPE_FLOAT32, 3 values)
///
/// Each element has a usage, and sub usage. The usage and sub usage values should uniquely identify an element in a vertex
/// format. The sub usage allows you to have more than one element with the same usage. A simple example of this could be
/// with texture coordinates - the sub usage identifies the channel for the texture coordinates.
///
/// You can build a vertex format by adding elements, specifying their usage, types and flags.
/// In order to simplify processing of vertex formats, the elements must always be in a well defined order before they are
/// used for anything. This order is usage / sub usage order. You can either add the elements in this order, or when you've
/// added all of the elements call makeCanonicalOrder to automatically sort the elements into the appropriate order. Using
/// makeCanonicalOrder is the recommended way of producing a valid vertex format, as it is future proof. If elements have
/// been added in order, makeCanonicalOrder operates particularly rapidly.
///
/// The flags deserve particular note in they can have a significant effect the vertex buffers behavior. The flag FLAG_NOT_SHARED
/// is important - as it defines which components of a vertex buffer should be 'per instance'. If the vertex buffer is cloned
/// these elements will be deep copied. Elements without the flag may be shared by both the original vertex buffer and the
/// new vertex buffer.
///
/// NOTE that behavior is undefined if you try to modify a shared component of shared vertex buffer. This gives an implementation
/// of a vertex buffer more flexibility. In particular a simple implementation could deep copy all components if a single
/// component is shared.
///
/// \sa hkMeshVertexBuffer
struct hkVertexFormat
{
	HK_DECLARE_REFLECTION();

	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkVertexFormat );

	hkVertexFormat( class hkFinishLoadedObjectFlag flag ) { }


	enum { MAX_ELEMENTS  = 32 };	///< the maximum number of elements per format

		/// How the data is stored
	enum ComponentType
	{
		// Do not change order unless the s_dataTypeToSize array is altered
		TYPE_NONE = 0,
		TYPE_INT8,
		TYPE_UINT8,
		TYPE_INT16,
		TYPE_UINT16,
		TYPE_INT32,
		TYPE_UINT32,
		TYPE_UINT8_DWORD,					///< Four uint8s packed into a dword (interpreted as integer for setting/getting element arrays). (NOTE! since dword packed byte order is endian dependent)
		TYPE_ARGB32,						///< A color packed into 4 bytes (interpreted as vector4 , for getting/setting element arrays)
		TYPE_FLOAT16,
		TYPE_FLOAT32,
		TYPE_VECTOR4,						///< Havok Vector4 type - 16 byte aligned (useful for memory vertex buffer formats)
		TYPE_LAST
	};

		/// What the data is used for
	enum ComponentUsage
	{
		USAGE_NONE = 0,
		USAGE_POSITION = 1,					///< Position
		USAGE_NORMAL,						///< Normal
		USAGE_COLOR,						///< Generally assumed subusage 0 is diffuse, and 1 is specular
		USAGE_TANGENT,						///< Tangent
		USAGE_BINORMAL,						///< aka BITANGENT
		USAGE_BLEND_MATRIX_INDEX,			///< Indices to blend matrices (typically held in TYPE_UINT8_DWORD)
		USAGE_BLEND_WEIGHTS,				///< Blend weights, associated matrices stored in BLEND_MATRIX_INDEX
		USAGE_BLEND_WEIGHTS_LAST_IMPLIED,	///< Same as above, except the last weight is implied as 1 - sum of previous weights
		USAGE_TEX_COORD,					///< Sub usage specifies texture channel
		USAGE_POINT_SIZE,					///< Point size for point rendering
		USAGE_USER,							///< User data, of user defined usage.

		USAGE_LAST
	};

		/// Extra hints which allow for optimized creation of vertex buffers
	enum HintFlags
	{
		FLAG_READ = 0x1,						///< This element will be read
		FLAG_WRITE = 0x2,						///< This element will be written
		FLAG_DYNAMIC = 0x4,						///< This element will be accessed often
		FLAG_NOT_SHARED = 0x8					///< If set when the vertex buffer is cloned this element is not shared
	};

		/// Sharing hints
	enum SharingType
	{
		SHARING_ALL_SHARED,						///< All elements are shared
		SHARING_ALL_NOT_SHARED,					///< All elements are not shared
		SHARING_MIXTURE							///< There is a mixture of shared and unshared elements
	};

		/// An element describes one component of a vertex
	struct Element
	{
		HK_DECLARE_REFLECTION();

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkVertexFormat::Element );

			/// Returns true if two elements are equal
		HK_FORCE_INLINE bool operator==(const Element& rhs) const;

			/// Returns true if two elements are not equal
		HK_FORCE_INLINE bool operator!=(const Element& rhs) const;

			/// Set up an element
		HK_FORCE_INLINE void set(ComponentUsage usage, ComponentType dataType, int numValues, int subUsage = 0, int flags = 0);

			/// Returns the size of the element in bytes (dword aligned)
		HK_FORCE_INLINE int calculateAlignedSize() const;

			/// Calculates the size of the element in bytes
		HK_FORCE_INLINE int calculateSize() const;

		hkEnum<ComponentType,  hkUint8>     m_dataType;         ///< The data type
		hkUint8                             m_numValues;        ///< The number of values of this data type
		hkEnum<ComponentUsage, hkUint8>     m_usage;            ///< The usage of this entry
		hkUint8                             m_subUsage;         ///< Ordinal to uniquely identify multiple entries with the same usage
		hkFlags<HintFlags, hkUint8>         m_flags;            ///< Flags about how an element will be used
		hkUint8                             m_pad[3];           ///< Pad so aligned
	};


		/// Default constructor
	hkVertexFormat();

		/// Copy ctor
	hkVertexFormat(const hkVertexFormat& rhs);

		/// Dtor -> needed to make hkVertexFormat work in hkArray
	HK_FORCE_INLINE ~hkVertexFormat() {}

		/// Returns true if the vertex formats are equal. They must be in canonical order
	bool operator==(const hkVertexFormat& rhs) const;

		/// Returns true if the vertex formats are not equal. They must be in canonical order.
	HK_FORCE_INLINE bool operator!=(const hkVertexFormat& rhs) const;

		/// Sets to the same format
	void set(const hkVertexFormat& rhs);

		/// Assignment
	void operator=(const hkVertexFormat& rhs);

		/// Removes all of the elements
	HK_FORCE_INLINE void clear();

		/// Vertex formats must be in 'canonical' order in order to be used. The order
		/// is that each element is increasingly large usage type, and if of the same usage, increasing subUsage
		/// It is invalid to have two elements with the same usage/subUsage
		/// The format can either be constructed so that the order is correct, or after all the elements
		/// are added this method can be called.
	void makeCanonicalOrder();

		/// Returns true if the elements are in canonical order. If isCanonicalOrder() fails after makeCanonicalOrder() call
		/// then the format is badly formed (more than one element with the same usage/subUsage for example)
	bool isCanonicalOrder() const;

		/// Returns the element index which has the semantic and subusage passed in. Returns -1 if not found.
	int findElementIndex(ComponentUsage dataUsage, int subUsage) const;

		/// For a given usage, finds the next free subUsage (i.e., one larger than the largest subUsage). Returns 0 if there
		/// are no elements of the usage.
	int findNextSubUsage(ComponentUsage usage) const;

		/// Add an element. The usage subUsage will be the next available subUsage
	void addElement(ComponentUsage usage, ComponentType type, int numValues = 1, int flags = FLAG_READ | FLAG_WRITE);

		/// Add an element
	void addElement(const Element& element);

		/// Finds out the mix of shared/unshared elements
	SharingType calculateSharingType() const;

	//
	// Members
	//

	struct Element m_elements[hkVertexFormat::MAX_ELEMENTS];    ///< The elements

	int m_numElements;                                          ///< Number of elements used

	static const hkUint8 s_dataTypeToSize[];                    ///< Static array to convert from dataType to its size
};

extern const hkClass hkMeshVertexBufferClass;


/// This is an abstract interface to a vertex buffer
///
/// A vertex buffer contains an array of vertices in the format defined by the hkVertexFormat
///
/// \sa hkMeshShape hkVertexFormat
class hkMeshVertexBuffer: public hkReferencedObject
{
	public:
		HK_DECLARE_REFLECTION();

        HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SCENE_DATA);

			/// This structure holds pointers to the in memory representation of the vertex buffer after locking it
		struct LockedVertices
        {
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkMeshVertexBuffer::LockedVertices);

                /// Finds the buffer index, -1 if not found
            int findBufferIndex(hkVertexFormat::ComponentUsage usage, int subUsage) const;

				/// The buffer structures returned will always be in the same order as the elements
				/// either specified in a partial lock, or in the vertex format
			struct Buffer
			{
				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkMeshVertexBuffer::LockedVertices::Buffer);
					/// Steps start to the next element
				HK_FORCE_INLINE void next();

				void* m_start;									///< The start address of the first element
				int   m_stride;									///< The next element data will be stride bytes from the start
				struct hkVertexFormat::Element m_element;		///< Specifies the type of the element
			};

			struct Buffer m_buffers[ hkVertexFormat::MAX_ELEMENTS ];	///< Holds (in the same order as the vertex format or partial lock) information about each element
			int m_numBuffers;									///< The number of element buffers
			int m_numVertices;									///< The total number of vertices

				/// If true then the data has been returned in an interleaved format,
				/// meaning all the strides are the same, and all of the pointers are offsets into a single vertex.
				/// The data may not be contiguous (there may be gaps)
			hkBool m_isInterleaved;
		};

			/// Extra flags
		enum Flags
		{
            ACCESS_READ = 1,                ///< Data can be read
            ACCESS_WRITE = 2,               ///< Data can be written
			ACCESS_READ_WRITE = 3,          ///< Data can be read and written ( same as ACCESS_READ | ACCESS_WRITE)
			ACCESS_WRITE_DISCARD = 4,		///< All elements will be written (can discard data previously stored)
			ACCESS_ELEMENT_ARRAY = 8 		///< Flag to say the data will _only_ be accessed as an element array (stops needless conversions if only ever accessed that way)
		};

			/// Return values from a lock
		enum LockResult
		{
			RESULT_FAILURE = 0,
			RESULT_SUCCESS = 1,
			RESULT_IN_USE = 2
		};

            /// Input structure for lock() and partialLock()
		struct LockInput
		{
			HK_FORCE_INLINE LockInput();

			int m_startVertex;									///< Start vertex index
            int m_numVertices;                                  ///< Total amount of vertices (-1 means all vertices from the startVertex)
			hkBool m_noWait;									///< If set the lock will not wait if doing so will cause a stall (say because the buffer is in use)
			hkBool m_contiguousAccess;							///< If set access will be contiguous - a hint to the lock as to what memory is most appropriate to return
			int m_lockFlags;									///< Lock flags specifying how data will be accessed (ignored on a partial lock)
		};

			/// Partial Lock input information
		struct PartialLockInput
		{
			HK_FORCE_INLINE PartialLockInput();

			int m_numLockFlags;									///< Total amount of lock flags
			int m_elementIndices[hkVertexFormat::MAX_ELEMENTS];	///< Elements to be locked. The locked vertex buffers will be in this order
			hkUint8 m_lockFlags[hkVertexFormat::MAX_ELEMENTS];	///< Flag combination describing how locking occurs for each element (overrides the LockInputs
		};
		
			/// Default Ctor
		hkMeshVertexBuffer() {}

			/// Specifies the groups that should be shared. An implementation may share or just copy the data.
        virtual hkMeshVertexBuffer* clone() = 0;

            /// Returns true if all of the data is sharable - i.e., if all data is sharable doing a clone will just return a ref count of this object
            /// NOTE! That a format that says all the members are SHARABLE, doesn't mean this will return true - as an underlying implementation
            /// may require per instance data (for example when software skinning)
        virtual bool isSharable() = 0;

			/// Gets the vertex format and stores the result in formatOut
		virtual void getVertexFormat( hkVertexFormat& formatOut ) = 0;

			/// The total number of vertices in the vertex buffer
		virtual int getNumVertices() = 0;

			/// Lock all of the elements in a vertex buffer.
			/// Only one lock can be active on the vertex buffer at any time
		virtual LockResult lock( const LockInput& input, LockedVertices& lockedVerticesOut ) = 0;

			/// Perform a partial lock - locks a subset of elements.
			/// Only one lock can be active on the vertex buffer at any time.
		virtual LockResult partialLock( const LockInput& input, const PartialLockInput& partialInput, LockedVertices& lockedOut) = 0;

			/// Fetches elements into a vector4 array doing conversions as needed. Does not convert integral types.
			/// The elements being extracted must have been previously been locked with lock/partialLock.
		virtual void getElementVectorArray(const LockedVertices& lockedVertices, int elementIndex, hkVector4* compData) = 0;

			/// Sets the elements of an array doing conversions from hkVector4s as needed. Does not convert integral types
			/// The elements being set must have previously have been locked with lock/partialLock.
		virtual void setElementVectorArray(const LockedVertices& lockedVertices, int elementIndex, const hkVector4* compData) = 0;

			/// Sets the elements of an array doing conversions from integers as needed. Converts only integral types.
			/// The elements being extracted must have been previously been locked with lock/partialLock.
		virtual void getElementIntArray(const LockedVertices& lockedVertices, int elementIndex, int* compData) = 0;

			/// Sets the elements of an array doing conversions from integers as needed. Converts only integral types.
			/// The elements being set must have previously have been locked with lock/partialLock.
		virtual void setElementIntArray(const LockedVertices& lockedVertices, int elementIndex, const int* compData) = 0;

			/// Unlock a previously locked vertex buffer. The lockedVertices structure that was set in 'lock' or 'partialLock'
			/// should be passed into the unlock. Undefined behavior results if lockedVertices contains any different values than
			/// were returned from the lock/partialLock call.
		virtual void unlock( const LockedVertices& lockedVertices ) = 0;

			/// Serialization Ctor
		hkMeshVertexBuffer( hkFinishLoadedObjectFlag flag ): hkReferencedObject(flag) {}
};

#include <Common/GeometryUtilities/Mesh/hkMeshVertexBuffer.inl>

#endif	// HK_MESH_VERTEX_BUFFER_H

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
