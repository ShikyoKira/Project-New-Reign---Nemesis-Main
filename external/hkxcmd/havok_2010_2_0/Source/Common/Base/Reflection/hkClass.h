/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_BASE_CLASS_H
#define HK_BASE_CLASS_H

#include <Common/Base/Reflection/hkClassMember.h>
#include <Common/Base/Reflection/hkClassEnum.h>

class hkClassEnum;
class hkClassMember;
class hkCustomAttributes;
class hkStreamWriter;
class hkTypedUnion;
template <typename K, typename V, typename Allocator> class hkPointerMap;

extern const hkClass hkClassClass;




/// Reflection information for any reflected type.
/// Reflection is based upon the Java object model where any
/// class has exactly zero or one parents and may implement
/// zero or more interfaces. An interface is a class which
/// has virtual methods but no data members.
class hkClass
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_HKCLASS, hkClass );
		HK_DECLARE_REFLECTION();

	public:

		enum SignatureFlags
		{
			SIGNATURE_LOCAL = 1	// don't include signature of parents
		};

		enum FlagValues
		{
			FLAGS_NONE = 0,
			FLAGS_NOT_SERIALIZABLE = 1
		};
		typedef hkFlags<FlagValues, hkUint32> Flags;

			/// Create a class.
		hkClass(const char* className,
				const hkClass* parentClass,
				int objectSizeInBytes,
				const hkClass** implementedInterfaces,
				int numImplementedInterfaces,
				const hkClassEnum* declaredEnums,
				int numDeclaredEnums,
				const hkClassMember* declaredMembers,
				int numDeclaredMembers,
				const void* defaults = HK_NULL,
				const hkCustomAttributes* attributes = HK_NULL,
				hkUint32 flags = 0,
				hkUint32 version = 0);

			/// Get the name of this class. String is valid for the lifetime of this class.
		const char* getName() const;

		/* class methods */

			/// Return parent of this class or HK_NULL if we are a toplevel class.
		const hkClass* getParent() const;

			/// Return parent of this class or HK_NULL if we are a toplevel class.
		hkClass* getParent();

			/// Return number of classes to the top of inheritance hierarchy.
			/// A class with no parents has a depth of one. Interfaces are not counted.
		int getInheritanceDepth() const;

			/// Are we a parent class of class k.
		hkBool isSuperClass(const hkClass& k) const;

			/// Get the i'th implemented interface (including parents).
		const hkClass* getInterface( int i ) const;

			/// Get the number of interfaces this class implements (including parents).
		int getNumInterfaces() const;

			/// Get the i'th implemented interface. (not including parents)
		const hkClass* getDeclaredInterface( int i ) const;

			/// Get the number of interfaces this class implements. (not including parents)
		int getNumDeclaredInterfaces() const;

			/// Get the i'th enum defined in this class and parents.
		const hkClassEnum& getEnum(int i) const;

			/// Get the enum named 'name' or HK_NULL if not found.
		const hkClassEnum* getEnumByName(const char* name) const;

			/// Get the number of enums defined in this class and parents.
		int getNumEnums() const;

			/// Get the i'th enum defined in this class (not including parent class members).
		const hkClassEnum& getDeclaredEnum(int i) const;

			/// Get the declared enum named 'name' or HK_NULL if not found.
		const hkClassEnum* getDeclaredEnumByName(const char* name) const;

			/// Get the number of enums defined in this class (not including parent class members).
		int getNumDeclaredEnums() const;

			/// Get the i'th member (including parent class members).
		const hkClassMember& getMember(int i) const;

			/// Get the i'th member (including parent class members).
		hkClassMember& getMember(int i);

			/// Get the member named 'name' or HK_NULL if not found.
		const hkClassMember* getMemberByName(const char* name) const;

			/// Get the index of member named 'name' or -1 if not found.
		int getMemberIndexByName(const char* name) const;

			/// Get the number of members in this class (including parent class members).
		int getNumMembers() const;

			/// Get the i'th member declared in this class (not including parent class members).
		const hkClassMember& getDeclaredMember(int i) const;

			/// Get the declared member named 'name' or HK_NULL if not found.
		const hkClassMember* getDeclaredMemberByName(const char* name) const;

			/// Get the index of declared member named 'name' or -1 if not found.
		int getDeclaredMemberIndexByName(const char* name) const;

			/// Get the number of members declared in this class (not including parent class members).
		int getNumDeclaredMembers() const;

			/// Get the size required for instances of this class.
		int getObjectSize() const;

			/// Set the size required for instances of this class.
		void setObjectSize(int size);

			/// Does this class have a vtable?
		hkBool hasVtable() const;

			/// Does the given member have a default?
		hkBool32 hasDefault(int memberIndex) const;

			/// Does the given declared member have a default?
		hkBool32 hasDeclaredDefault(int declaredIndex) const;

			/// Write the default of the given member into w and return HK_SUCCESS.
			/// If there is no default do not write anything and return HK_FAILURE.
		hkResult getDefault(int memberIndex, hkStreamWriter* w) const;

			/// Return a pointer to the default of the given member, or HK_NULL
			/// if there is no default
		const void* getDefault(int memberIndex) const;

			/// Write the default of the given declared member into w and return HK_SUCCESS.
			/// If there is no default do not write anything and return HK_FAILURE.
		hkResult getDeclaredDefault(int declaredIndex, hkStreamWriter* w) const;

			/// Put the default of the given member into value and return HK_SUCCESS.
			/// If there is no default don't do anything and return HK_FAILURE.
		hkResult getDefault(int memberIndex, hkTypedUnion& value) const;

			/// Return a pointer to the default of the given declared member, or HK_NULL
			/// if there is no default
		const void* getDeclaredDefault(int memberIndex) const;

			/// Put the default of the given declared member into value and return HK_SUCCESS.
			/// If there is no default don't do anything and return HK_FAILURE.
		hkResult getDeclaredDefault(int declaredIndex, hkTypedUnion& value) const;

			/// Get the class signature for versioning.
		hkUint32 getSignature(int signatureFlags = 0) const;

			/// Gets the described version of this class
		int getDescribedVersion() const;

			/// Get the class signature for versioning.
		void writeSignature( hkStreamWriter* w ) const;

			/// Query for a class attribute.
		const hkVariant* getAttribute(const char* id) const;

			/// See the Flags enum for a description of flags.
		const Flags& getFlags() const;

			/// See the Flags enum for a description of flags.
		Flags& getFlags();

		typedef hkPointerMap<const hkClass*, hkInt32> UpdateFlagFromClassMap;
		static void HK_CALL updateMetadataInplace( hkClass** c, int sourceVersion );
		static void HK_CALL updateMetadataInplace( hkClass* c, UpdateFlagFromClassMap& updatedAlready, int sourceVersion );

	private:

		hkResult retrieveMember(int memberIndex, const void*& defaultOut, const hkClassMember*& memberOut) const;

	protected:

			/// Name of this type.
		const char* m_name;

			/// Parent class.
		const hkClass* m_parent;

			/// Size of the live instance.
		int m_objectSize;

			/// Interfaces implemented by this class.
		//const hkClass** m_implementedInterfaces;

			/// Number of interfaces implemented by this class.
		int m_numImplementedInterfaces;

			/// Declared enum members.
		const class hkClassEnum* m_declaredEnums;

			/// Number of enums declared in this class.
		int m_numDeclaredEnums;

			/// Declared members.
		const class hkClassMember* m_declaredMembers;

			/// Number of members declared in this class.
		int m_numDeclaredMembers;

			/// Default values for this class.
		const void* m_defaults; //+nosave

			/// Default values for this class.
		const hkCustomAttributes* m_attributes; //+serialized(false)

            /// Flag values.
		Flags m_flags;

			/// Version of described object.
		int m_describedVersion;
};

#endif // HK_BASE_CLASS_H

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
