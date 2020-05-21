/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKX_ENVIRONMENT_H
#define HKX_ENVIRONMENT_H


extern const class hkClass hkxEnvironmentVariableClass;

/// hkxEnvironment meta information
extern const class hkClass hkxEnvironmentClass;

/// An hkxEnvironment represent a set of variables+values (string+string) used during processing of assets.
/// It is similar to the concept of the OS shell environment variables. Examples of them
/// would be: the name of the asset, the current configuration being used, etc.
/// NOTE: Variables are considered case-insensitive. Values are returned with the case they were stored
class hkxEnvironment : public hkReferencedObject
{
	public:
		//+version(1)
		//+vtable(true)

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA );
		HK_DECLARE_REFLECTION();

		hkxEnvironment();
		hkxEnvironment(hkFinishLoadedObjectFlag f);

			/// Sets the value of the given variable. If the variable is new, it adds it. If value is HK_NULL, the
			/// variable is removed. Returns HK_FAILURE if trying to remove a non-existent variable.
		hkResult setVariable (const char* name, const char* value);

			/// Returns the stored value for the given variable. Return HK_NULL is the variable is unknown.
		const char* getVariableValue (const char* name) const;

			/// Erases all variables
		void clear ();

			/// Returns a single string representation of the environment, of the form
			/// var=value ; var = value ;.
		void convertToString( hkStringBuf& out ) const;

			/// Interprets a string as a sequence of name=value pairs, separated by semicolons. Quotes can be used on
			/// either side. If no value is provided, the variable will be removed (if present). Returns HK_FAILURE
			/// if the string couldn't be properly parsed.
			/// Example of a string is:
			/// \code var1 = "How are you?"; "var 2"= well ;var_3=; "var 4"= "thank you" \endcode
		hkResult interpretString(const char* str);

			/// Direct access to the variables. Returns the number of variables stored.
		int getNumVariables () const;

			/// Direct access to the variables. Returns the name of the i-th variable
		const char* getVariableName (int i) const;

			/// Direct access to the variables. Returns the value of the i-th variable
		const char* getVariableValue (int i) const;

			// Public for serialization
		struct Variable
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkxEnvironment::Variable );
			HK_DECLARE_REFLECTION();

			Variable() {}
			Variable(hkFinishLoadedObjectFlag f) : m_name(f), m_value(f) {}

			hkStringPtr m_name;
			hkStringPtr m_value;
		};

	private:

		int findVariableByName (const char* name) const;

		hkArray<struct Variable> m_variables;

};

#endif //HKX_ENVIRONMENT_H

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
