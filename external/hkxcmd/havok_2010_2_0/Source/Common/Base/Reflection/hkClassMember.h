/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_BASE_CLASS_MEMBER_H
#define HK_BASE_CLASS_MEMBER_H

class hkClassEnum;
class hkClass;
class hkCustomAttributes;

/// hkClassMember meta information
extern const hkClass hkClassMemberClass;




/// Reflection information for a data member of a type.
class hkClassMember
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_HKCLASS, hkClassMember);
		HK_DECLARE_REFLECTION();

			// Be careful when adding types to this enum.
			// Maintain this order:
			// Sort first by type (integer, float, ...)
			// next by size. (int32, uint32, int64, ...)

			/// An enumeration of all possible member types.
			/// There are three basic categories. Plain old data types
			/// (normal and c-style array variants), enums and hkArrays.
		enum Type
		{
				/// No type
			TYPE_VOID = 0,
				/// hkBool, boolean type
			TYPE_BOOL,
				/// hkChar, signed char type
			TYPE_CHAR,
				/// hkInt8, 8 bit signed integer type
			TYPE_INT8,
				/// hkUint8, 8 bit unsigned integer type
			TYPE_UINT8,
				/// hkInt16, 16 bit signed integer type
			TYPE_INT16,
				/// hkUint16, 16 bit unsigned integer type
			TYPE_UINT16,
				/// hkInt32, 32 bit signed integer type
			TYPE_INT32,
				/// hkUint32, 32 bit unsigned integer type
			TYPE_UINT32,
				/// hkInt64, 64 bit signed integer type
			TYPE_INT64,
				/// hkUint64, 64 bit unsigned integer type
			TYPE_UINT64,
				/// hkReal, float type
			TYPE_REAL,
				/// hkVector4 type
			TYPE_VECTOR4,
				/// hkQuaternion type
			TYPE_QUATERNION,
				/// hkMatrix3 type
			TYPE_MATRIX3,
				/// hkRotation type
			TYPE_ROTATION,
				/// hkQsTransform type
			TYPE_QSTRANSFORM,
				/// hkMatrix4 type
			TYPE_MATRIX4,
				/// hkTransform type
			TYPE_TRANSFORM,
				/// Serialize as zero - deprecated.
			TYPE_ZERO,
				/// Generic pointer, see member flags for more info
			TYPE_POINTER,
				/// Function pointer
			TYPE_FUNCTIONPOINTER,
				/// hkArray<T>, array of items of type T
			TYPE_ARRAY,
				/// hkInplaceArray<T,N> or hkInplaceArrayAligned16<T,N>, array of N items of type T
			TYPE_INPLACEARRAY,
				/// hkEnum<ENUM,STORAGE> - enumerated values
			TYPE_ENUM,
				/// Object
			TYPE_STRUCT,
				/// Simple array (ptr(typed) and size only)
			TYPE_SIMPLEARRAY,
				/// Simple array of homogeneous types, so is a class id followed by a void* ptr and size
			TYPE_HOMOGENEOUSARRAY,
				/// hkVariant (void* and hkClass*) type
			TYPE_VARIANT,
				/// char*, null terminated string
			TYPE_CSTRING,
				/// hkUlong, unsigned long, defined to always be the same size as a pointer
			TYPE_ULONG,
				/// hkFlags<ENUM,STORAGE> - 8,16,32 bits of named values.
			TYPE_FLAGS,
				/// hkHalf, 16-bit float value
			TYPE_HALF,
				/// hkStringPtr, c-string
			TYPE_STRINGPTR,
				/// hkRelArray<>, attached const array values
			TYPE_RELARRAY,
			TYPE_MAX
		};

			/// Special member properties.
		enum FlagValues
		{
			FLAGS_NONE = 0,
				/// Member has forced 8 byte alignment.
			ALIGN_8 = 128,
				/// Member has forced 16 byte alignment.
            ALIGN_16 = 256,
                /// The members memory contents is not owned by this object
            NOT_OWNED = 512,
                /// This member should not be written when serializing
            SERIALIZE_IGNORED = 1024
		};
		typedef hkFlags<FlagValues, hkUint16> Flags;

		enum DeprecatedFlagValues
		{
			DEPRECATED_SIZE_8 = 8,
			DEPRECATED_ENUM_8 = 8,
			DEPRECATED_SIZE_16 = 16,
			DEPRECATED_ENUM_16 = 16,
			DEPRECATED_SIZE_32 = 32,
			DEPRECATED_ENUM_32 = 32
		};

		enum
		{
			HK_CLASS_ZERO_DEFAULT = -2,
		};

			/// Get the name of the member.
		inline const char* getName() const;

			/// Get the primary type of the member.
		inline hkClassMember::Type getType() const;

			/// Set the primary type of the member.
		inline void setType(hkClassMember::Type type);

			/// Get the secondary type of the member.
		inline hkClassMember::Type getSubType() const;

			/// Set the secondary type of the member.
		inline void setSubType(hkClassMember::Type subtype);

		hkClassMember::Type getArrayType() const;

			/// Return the size of this member in bytes.
			/// Note that for c style arrays this the size of
			/// a single item times the number of items.
		int getSizeInBytes() const;

			/// Get the alignment of this type in bytes.
		int getAlignment() const;

            /// Return true if the memory of the member does not belong to this object
        hkBool isNotOwner() const;

			/// Get the full declared type of the member.
			/// e.g., hkArray<hkUint32>. Writes at most bufLen chars
			/// into buf. Returns the length of the name.
		int getTypeName(char* buf, int bufLen) const;

			/// For arrays, the size of the contained elements.
		int getArrayMemberSize() const;

			/// Check if the member has an associated class pointer
		inline hkBool hasClass() const;

			/// For struct types or arrays of structs.
		const hkClass& getStructClass() const;

			/// Optional extra type information for structs, pointers etc.
		const hkClass* getClass() const;

			/// Check if the member has an associated enum class pointer
		inline hkBool hasEnumClass() const;

			///For struct types or arrays of structs.
		const hkClassEnum& getEnumClass() const;

			/// If this member is a c-style array, return its size, otherwise return zero.
			/// This is not valid for hkTypeArray nor hkTypeEnum.
		int getCstyleArraySize() const;

			/// For enum members, access its corresponding hkClassEnum.
		const hkClassEnum& getEnumType() const;

			/// Get the enum value.
		int getEnumValue(const void* memberAddress) const;

			/// Get the enum value.
		void setEnumValue(void* memberAddress, int value) const;

			/// The offset of this member from the start of the object.
		inline int getOffset() const;

			/// The offset of this member from the start of the object.
		inline void setOffset(int offset);

			/// See the Flags enum for a description of flags.
		inline const Flags& getFlags() const;

			/// See the Flags enum for a description of flags.
		inline Flags& getFlags();

			/// Query this member for a named attribute.
		const hkVariant* getAttribute(const char* id) const;

			/// Return the enumerated value of 'name'. e.g. "hkReal" -> hkClassMember::TYPE_REAL
		static hkClassMember::Type HK_CALL getTypeOf( const char* name );

			/// Return the subtype of name 'name'. e.g. "hkArray<hkInt32>" -> hkClassMember::TYPE_INT32
		static hkClassMember::Type HK_CALL getSubtypeOf( const char* name );

			/// Properties of the builtin types.
		struct TypeProperties
		{
				/// The type associated with this
			hkEnum<hkClassMember::Type,hkUint8> m_type;
				/// Zero terminated name
			const char* m_name;
				/// Size of the type in bytes <=0 it is not defined
			short m_size;
				/// Alignment in bytes, if <=0 it is not defined
			short m_align;

			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE_CLASS, hkClassMember::TypeProperties );
		};
            /// Return details about the class member type
        static const struct TypeProperties& HK_CALL getClassMemberTypeProperties(Type type);

	private:

			/// The name of this member.
		const char* m_name;

			///
		const hkClass* m_class; //
		const hkClassEnum* m_enum; // Usually null except for enums
		hkEnum<Type,hkUint8> m_type; // An hkMemberType.
		hkEnum<Type,hkUint8> m_subtype; // An hkMemberType.
		hkInt16 m_cArraySize; // Usually zero, nonzero for cstyle array..
		Flags m_flags; // Pointers:optional, voidstar, rawdata. Enums:sizeinbytes.
		hkUint16 m_offset; // Address offset from start of struct.
		const hkCustomAttributes* m_attributes; //+serialized(false)
};

#include <Common/Base/Reflection/hkClassMember.inl>

#endif // HK_BASE_CLASS_MEMBER_H

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
