/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// YOU CANNOT INCLUDE THIS FILE DIRECTLY 
//HK_REFLECTION_PARSER_EXCLUDE_FILE

HK_MEMORY_CLASS_DEFINITION_START

	//the format for these macros is HK_MEMORY_CLASS([name], parent)   
	//ROOT should never be used as a memory class in your code, it simply accumulates all results
	//IMPORTANT NOTE: a memory class is a "category" under which the memory used by objects is placed.
	//it is not a c++ class

	HK_MEMORY_CLASS(ROOT,		HK_MEMORY_CLASS_ROOT)

	//these are for the Base library
	HK_MEMORY_CLASS(BASE,		HK_MEMORY_CLASS_ROOT)

	//these are for the collide lib
	HK_MEMORY_CLASS(COLLIDE,	HK_MEMORY_CLASS_ROOT)

	//these are for the constraint solver
	HK_MEMORY_CLASS(CONSTRAINT_SOLVER,	HK_MEMORY_CLASS_ROOT)

	//these are for the dynamics library
	HK_MEMORY_CLASS(DYNAMICS,	HK_MEMORY_CLASS_ROOT)

	//these are for the export library
	HK_MEMORY_CLASS(EXPORT,		HK_MEMORY_CLASS_ROOT)

	//these are for the serialize library
	HK_MEMORY_CLASS(SERIALIZE,	HK_MEMORY_CLASS_ROOT)

	//these are for the geometry library
	HK_MEMORY_CLASS(GEOMETRY,	HK_MEMORY_CLASS_ROOT)
	
	//these are for the graphics library
	HK_MEMORY_CLASS(DISPLAY,	HK_MEMORY_CLASS_ROOT)

	//these are for the maths library
	HK_MEMORY_CLASS(MATH,		HK_MEMORY_CLASS_ROOT)

	//these are for the scene data library
	HK_MEMORY_CLASS(SCENE_DATA,	HK_MEMORY_CLASS_ROOT)

	//these are for the vehicle library
	HK_MEMORY_CLASS(VEHICLE,	HK_MEMORY_CLASS_ROOT)

	//these are for the destruction library
	HK_MEMORY_CLASS(DESTRUCTION,HK_MEMORY_CLASS_ROOT)

	//these are for the utilities library
	HK_MEMORY_CLASS(UTILITIES,	HK_MEMORY_CLASS_ROOT)

	//these are for the animation
	HK_MEMORY_CLASS(ANIMATION,	HK_MEMORY_CLASS_ROOT)

	//these are for the behavior library
	HK_MEMORY_CLASS(BEHAVIOR,	HK_MEMORY_CLASS_ROOT)

	//these are for the cloth library
	HK_MEMORY_CLASS(CLOTH,		HK_MEMORY_CLASS_ROOT)

	//these are for the ai library
	HK_MEMORY_CLASS(AI,			HK_MEMORY_CLASS_ROOT)

	//these are for use by the tools supplied with the havok SDK
	HK_MEMORY_CLASS(TOOLS,		HK_MEMORY_CLASS_ROOT)
	
	//these are for use by the demo framework
	HK_MEMORY_CLASS(DEMO_FRAMEWORK,	HK_MEMORY_CLASS_ROOT)

	//these are for use by the demos
	HK_MEMORY_CLASS(DEMO,			HK_MEMORY_CLASS_ROOT)

	//these are for use by users
	HK_MEMORY_CLASS(USER,			HK_MEMORY_CLASS_ROOT)

	//these are the sub-classes used for hkbase
	HK_MEMORY_CLASS(BASE_CLASS,		HK_MEMORY_CLASS_BASE)
	HK_MEMORY_CLASS(STRING,			HK_MEMORY_CLASS_BASE)
	HK_MEMORY_CLASS(ARRAY,			HK_MEMORY_CLASS_BASE)
	HK_MEMORY_CLASS(SINGLETON,		HK_MEMORY_CLASS_BASE)
	HK_MEMORY_CLASS(SCRATCHPAD,		HK_MEMORY_CLASS_BASE)
	HK_MEMORY_CLASS(STREAM,			HK_MEMORY_CLASS_BASE)
	HK_MEMORY_CLASS(MONITOR,		HK_MEMORY_CLASS_BASE)
	HK_MEMORY_CLASS(MAP,			HK_MEMORY_CLASS_BASE)
	HK_MEMORY_CLASS(TREE,			HK_MEMORY_CLASS_BASE)
	HK_MEMORY_CLASS(HKCLASS,		HK_MEMORY_CLASS_BASE)

	//these are the sub-classes used for hkcollide
	HK_MEMORY_CLASS(AGENT,			HK_MEMORY_CLASS_COLLIDE)
	HK_MEMORY_CLASS(ALGORITHM,		HK_MEMORY_CLASS_COLLIDE)
	HK_MEMORY_CLASS(BROAD_PHASE,	HK_MEMORY_CLASS_COLLIDE)
	HK_MEMORY_CLASS(CONTACT,		HK_MEMORY_CLASS_COLLIDE)
	HK_MEMORY_CLASS(DISPATCH,		HK_MEMORY_CLASS_COLLIDE)
	HK_MEMORY_CLASS(MANIFOLD,		HK_MEMORY_CLASS_COLLIDE)
	HK_MEMORY_CLASS(SHAPE,			HK_MEMORY_CLASS_COLLIDE)
	HK_MEMORY_CLASS(COLLIDABLE,		HK_MEMORY_CLASS_COLLIDE)
	//these two may need to be removed
	HK_MEMORY_CLASS(CDINFO,			HK_MEMORY_CLASS_COLLIDE)

	//MOPP is a sub-class of algorithm in collide
	HK_MEMORY_CLASS(MOPP,			HK_MEMORY_CLASS_ALGORITHM)

	//these are sub-classes of dynamics
	HK_MEMORY_CLASS(ACTION,		HK_MEMORY_CLASS_DYNAMICS)
	HK_MEMORY_CLASS(DYNCOLLIDE,	HK_MEMORY_CLASS_DYNAMICS)
	HK_MEMORY_CLASS(CONSTRAINT,	HK_MEMORY_CLASS_DYNAMICS)
	HK_MEMORY_CLASS(ENTITY,		HK_MEMORY_CLASS_DYNAMICS)
	HK_MEMORY_CLASS(MOTION,		HK_MEMORY_CLASS_DYNAMICS)
	HK_MEMORY_CLASS(WORLD,		HK_MEMORY_CLASS_DYNAMICS)
	HK_MEMORY_CLASS(RIGID_BODY,	HK_MEMORY_CLASS_DYNAMICS)
	HK_MEMORY_CLASS(PHANTOM,	HK_MEMORY_CLASS_DYNAMICS)
	HK_MEMORY_CLASS(SIMISLAND,	HK_MEMORY_CLASS_DYNAMICS)

	// these are sub-classes of utilities
	HK_MEMORY_CLASS(CAMERA,		HK_MEMORY_CLASS_UTILITIES)
	HK_MEMORY_CLASS(CHARACTER,	HK_MEMORY_CLASS_UTILITIES)
	HK_MEMORY_CLASS(VDB,		HK_MEMORY_CLASS_UTILITIES)
	HK_MEMORY_CLASS(INERTIA,	HK_MEMORY_CLASS_UTILITIES)

	// these are sub-classes of animation
	HK_MEMORY_CLASS(ANIM_UNCOMPRESSED,	HK_MEMORY_CLASS_ANIMATION)
	HK_MEMORY_CLASS(ANIM_COMPRESSED,	HK_MEMORY_CLASS_ANIMATION)
	HK_MEMORY_CLASS(ANIM_DATA,			HK_MEMORY_CLASS_ANIMATION)
	HK_MEMORY_CLASS(ANIM_MOTION,		HK_MEMORY_CLASS_ANIMATION)
	HK_MEMORY_CLASS(ANIM_RIG,			HK_MEMORY_CLASS_ANIMATION)
	HK_MEMORY_CLASS(ANIM_CONTROL,		HK_MEMORY_CLASS_ANIMATION)
	HK_MEMORY_CLASS(ANIM_RUNTIME,		HK_MEMORY_CLASS_ANIMATION)
	HK_MEMORY_CLASS(ANIM_CACHE,			HK_MEMORY_CLASS_ANIMATION)

	// these are sub-classes of behavior
	HK_MEMORY_CLASS(BEHAVIOR_RUNTIME,	HK_MEMORY_CLASS_BEHAVIOR)
	HK_MEMORY_CLASS(BEHAVIOR_DEMOS,		HK_MEMORY_CLASS_BEHAVIOR)
	
	// these are sub-classes of cloth
	HK_MEMORY_CLASS(CLOTH_RUNTIME,		HK_MEMORY_CLASS_CLOTH)
	HK_MEMORY_CLASS(CLOTH_DATA,			HK_MEMORY_CLASS_CLOTH)
	HK_MEMORY_CLASS(CLOTH_SETUP,		HK_MEMORY_CLASS_CLOTH)

	// these are sub-classes of ai
	HK_MEMORY_CLASS(AI_NAVMESH,			HK_MEMORY_CLASS_AI)
	HK_MEMORY_CLASS(AI_ASTAR,			HK_MEMORY_CLASS_AI)
	HK_MEMORY_CLASS(AI_STEERING,		HK_MEMORY_CLASS_AI)

	// Sub classes of user memory classes
	HK_MEMORY_CLASS(USER_1,	HK_MEMORY_CLASS_USER)
	HK_MEMORY_CLASS(USER_2,	HK_MEMORY_CLASS_USER)
	HK_MEMORY_CLASS(USER_3,	HK_MEMORY_CLASS_USER)
	HK_MEMORY_CLASS(USER_4,	HK_MEMORY_CLASS_USER)
	HK_MEMORY_CLASS(PRE_ALLOCATED, HK_MEMORY_CLASS_USER)

HK_MEMORY_CLASS_DEFINITION_END

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
