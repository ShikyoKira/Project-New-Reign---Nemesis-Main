/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKSERIALZE_SERIALIZATION_CHECKING_UTILS_H
#define HKSERIALZE_SERIALIZATION_CHECKING_UTILS_H

class hkClassNameRegistry;
class hkVtableClassRegistry;
class hkOstream;

/// Utility routine to help maintain correct class serialization
/// We use these routines to help ensure we have reasonable definition of class reflection
namespace hkSerializationCheckingUtils
{
		/// Verify that a class reflection is defined as required.
		/// It checks that if the class is virtual and has registered corresponding vtable,
		/// class finish constructor definitions.
		/// It also checks if reflected class members are defined reasonably,
		/// e.g., members of type of a non-serializable class are defined
		/// with '+nosave' or '+serialized(false)' annotations and are not embedded objects.
		/// A human readable report is written to the output stream.
	hkResult HK_CALL verifyClassReflection(const hkClass& klass, const hkClassNameRegistry& classRegistry, hkOstream& output);

		/// Verify all registered class reflections.
		/// A human readable report is written to the output stream.
		/// The check will fail if memoryManagedPrefixes is non-NULL and a class that begins with one of the prefixes isn't properly memory-managed.
	hkResult HK_CALL verifyReflection(const hkClassNameRegistry& classRegistry, hkOstream& output, const char** memoryManagedPrefixes = HK_NULL, int numPrefixes = 0);
}

#endif // HKSERIALZE_SERIALIZATION_CHECKING_UTILS_H

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
