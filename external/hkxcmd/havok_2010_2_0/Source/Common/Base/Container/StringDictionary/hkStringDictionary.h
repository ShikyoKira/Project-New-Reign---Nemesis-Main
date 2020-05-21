/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_STRING_DICTIONARY
#define HK_STRING_DICTIONARY

	/// Class providing utilities for building and searching string dictionaries.
class hkStringDictionary
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE, hkStringDictionary);

			/// Initializes the dictionary
		hkStringDictionary(const char** strings, int numStrings);

			/// Destructs the dictionary
		~hkStringDictionary();

			/// Inserts a new word into the dictionary.
		void insert( const char* word );

			/// Removes a given word from the dictionary, returning true if the word was in the dictionary.
		bool remove( const char* word );
		
			/// Remove all words from the dictionary
		void clear();

			/// Returns true if the dictionary contains the word, false otherwise. Does a linear search through
			/// all dictionary entries.
		bool containsWord( const char* word ) const;

			/// Find strings in the dictionary that contain the given pattern, with a wildcard character accepting one or more characters
			/// in its place. The given array is filled using the given allocator.
		void findMatches( const char* pattern, hkArray<const char*>& resultsOut, hkMemoryAllocator& resultsAllocator, const char wildcard = 0, const bool caseSensitive = true ) const;
			/// Find strings in the dictionary that contain the given pattern, with a wildcard character accepting one or more characters
			/// in its place.
		void findMatches( const char* pattern, hkArray<const char*>& resultsOut, const char wildcard = '\0', const bool caseSensitive = true ) const;
		
			/// Copies all words from the dictionary into the given array.
		void copyWordsFromDictionary( hkArray<const char*>& wordsOut ) const;

			/// Return the number of strings in the dictionary.
		int getSize() const;
			
	protected:			
			/// String array containing the words in the dictionary
		hkArray<const char*> m_dictionary;
			
			/// Return the character index of the first occurrence of the pattern, with wildcard, in the given text.
		int naiveStringSearch(  const char* text, const char* pattern, const char wildcard = 0, const bool caseSensitive = true ) const;

			/// Compare the two characters following the case-sensitivity rules for this dictionary.
			/// Returns true if they match following those rules, false otherwise.
		bool charCmpFollowingCaseRules( const char a, const char b, bool caseSensitive ) const;

};

#include <Common/Base/Container/StringDictionary/hkStringDictionary.inl>

#endif // HK_STRING_DICTIONARY

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
