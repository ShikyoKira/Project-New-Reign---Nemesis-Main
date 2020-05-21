/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

//
template <typename VALUE,typename ALLOCATOR>
HKGP_FORCE_INLINE void			hkGeometryProcessing::HashTable<VALUE,ALLOCATOR>::Iterator::next()
{
	++m_item;
	while(m_bucket<m_table->m_buckets.getSize() && (m_bucket==-1 || m_table->m_buckets[m_bucket].getSize()<=m_item))
	{
		++m_bucket;
		m_item=0;
	}
}

//
template <typename VALUE,typename ALLOCATOR>
HKGP_FORCE_INLINE	void			hkGeometryProcessing::HashTable<VALUE,ALLOCATOR>::setBucketsCount(int size)
{
	clear();
	m_buckets.setSize(size);
	for(int i=0;i<size;++i)
	{
		m_buckets[i].clear();
	}
}
//
template <typename VALUE,typename ALLOCATOR>
HKGP_FORCE_INLINE	const VALUE*	hkGeometryProcessing::HashTable<VALUE,ALLOCATOR>::find(int hash,const VALUE& value) const
{
	const hkArray<VALUE,ALLOCATOR>&	values=lookupBucket(hash);
	const int						index=values.indexOf(value);
	if(index==-1)
		return(0);
	else
		return(&values[index]);
}
//
template <typename VALUE,typename ALLOCATOR>
HKGP_FORCE_INLINE	VALUE*			hkGeometryProcessing::HashTable<VALUE,ALLOCATOR>::find(int hash,const VALUE& value)
{
	hkArray<VALUE,ALLOCATOR>&	values=lookupBucket(hash);
	const int					index=values.indexOf(value);
	if(index==-1)
		return(0);
	else
		return(&values[index]);
}
//
template <typename VALUE,typename ALLOCATOR>
HKGP_FORCE_INLINE	VALUE*			hkGeometryProcessing::HashTable<VALUE,ALLOCATOR>::insert(int hash,const VALUE& value)
{
	++m_numItems;
	hkArray<VALUE,ALLOCATOR>&	values=lookupBucket(hash);
	values.expandOne()=value;
	return(&values[values.getSize()-1]);
}
//
template <typename VALUE,typename ALLOCATOR>
HKGP_FORCE_INLINE void			hkGeometryProcessing::HashTable<VALUE,ALLOCATOR>::remove(int hash,const VALUE& value)
{
	--m_numItems;
	hkArray<VALUE,ALLOCATOR>&	values=lookupBucket(hash);
	const int					index=values.indexOf(value);
	if(index>=0) values.removeAt(values.indexOf(value));
}

//
template <typename VALUE,typename ALLOCATOR>
inline void						hkGeometryProcessing::HashTable<VALUE,ALLOCATOR>::append(const HashTable& other)
{
	if(other.m_buckets.getSize()!=m_buckets.getSize())
	{	
		/* Rehash	*/ 
		for(int i=0;i<other.m_buckets.getSize();++i)
		{
			for(int j=0;j<other.m_buckets[i].getSize();++j)
			{
				insert(other.m_buckets[i][j]);
			}
		}
	}	
	else
	{
		/* Append	*/ 
		for(int i=0;i<m_buckets.getSize();++i)
		{
			m_buckets[i].append( other.m_buckets[i].begin(), other.m_buckets[i].getSize() );
		}
	}
}

//
template <typename T,const int N,typename ALLOCATOR>
inline							hkGeometryProcessing::PoolAllocator<T,N,ALLOCATOR>::~PoolAllocator()
{
	clear();
}

//
template <typename T,const int N,typename ALLOCATOR>
inline typename hkGeometryProcessing::PoolAllocator<T,N,ALLOCATOR>::Item*	hkGeometryProcessing::PoolAllocator<T,N,ALLOCATOR>::allocateItem()
{
	Pool*	pool	=	(m_firstPool && m_firstPool->m_free)?m_firstPool:allocateNewPool();
	Item*	item	=	pool->m_free;
	pool->m_free	=	*(Item**)item;
	item->m_pool	=	pool;
	++pool->m_used;
	return(item);
}

//
template <typename T,const int N,typename ALLOCATOR>
inline typename hkGeometryProcessing::PoolAllocator<T,N,ALLOCATOR>::Pool*	hkGeometryProcessing::PoolAllocator<T,N,ALLOCATOR>::allocateNewPool()
{
	ALLOCATOR			allocatorType;
	hkMemoryAllocator&	allocator(allocatorType.get(0));
	Item*				prev=HK_NULL;
	Pool*				pool=new(allocator.blockAlloc(sizeof(Pool))) Pool;
	for(int i=N-1;i>=0;--i)
	{
		Item**	current=(Item**)&pool->m_items[i];
		*current	=	prev;
		prev		=	(Item*)current;
	}
	pool->m_used		=	0;
	pool->m_free		=	prev;
	pool->m_links[0]	=	HK_NULL;
	pool->m_links[1]	=	m_firstPool;	
	m_firstPool			=	pool;
	if(pool->m_links[1]) pool->m_links[1]->m_links[0]=pool;
	return(pool);
}

//
template <typename T,const int N,typename ALLOCATOR>
inline void						hkGeometryProcessing::PoolAllocator<T,N,ALLOCATOR>::releasePool(Pool* pool)
{
	ALLOCATOR			allocatorType;
	hkMemoryAllocator&	allocator(allocatorType.get(0));
	if(pool->m_links[0]) pool->m_links[0]->m_links[1]=pool->m_links[1]; else m_firstPool=pool->m_links[1];
	if(pool->m_links[1]) pool->m_links[1]->m_links[0]=pool->m_links[0];
	pool->~Pool();
	allocator.blockFree(pool,sizeof(Pool));
}

//
template <typename T,const int N,typename ALLOCATOR>
inline void						hkGeometryProcessing::PoolAllocator<T,N,ALLOCATOR>::release(T* object)
{
	if(object)
	{
		/* Call dtor				*/ 
		object->~T();
		/* Locate and update pool	*/ 
		Item*	item((Item*)object);
		Pool*	pool((Pool*)item->m_pool);
		HK_ASSERT2(0x5E8E620D, (item>=pool->m_items) && (item<=&pool->m_items[N-1]), "Invalid pool pointer");
		if(0==--pool->m_used) releasePool(pool);
	}
}

//
template <typename T,const int N,typename ALLOCATOR>
inline void					hkGeometryProcessing::PoolAllocator<T,N,ALLOCATOR>::clear()
{
	while(m_firstPool) releasePool(m_firstPool);
}

//
template <int BITS,int MARGIN,int DIMS>
HKGP_FORCE_INLINE hkInt32		hkGeometryProcessing::Quantizer<BITS,MARGIN,DIMS>::round(hkReal x)
{
	return (hkInt32) (x>=0? x+0.5f : x-0.5f);
}

//
template <int BITS,int MARGIN,int DIMS>
HKGP_FORCE_INLINE hkInt32		hkGeometryProcessing::Quantizer<BITS,MARGIN,DIMS>::clamp(hkInt32 x)
{
	return x<DOMAIN_MIN_BOUND_WM? DOMAIN_MIN_BOUND_WM : x>DOMAIN_MAX_BOUND_WM? DOMAIN_MAX_BOUND_WM : x;
}

//
template <int BITS,int MARGIN,int DIMS>
inline						hkGeometryProcessing::Quantizer<BITS,MARGIN,DIMS>::Quantizer()
{
	m_worldToLocal	=	hkMatrix4::getIdentity();
	m_localToWorld	=	hkMatrix4::getIdentity();
}

//
template <int BITS,int MARGIN,int DIMS>
void						hkGeometryProcessing::Quantizer<BITS,MARGIN,DIMS>::setWorldToLocalTransform(const hkMatrix4& t)
{
	m_worldToLocal	=	t;
	if(HK_SUCCESS != m_localToWorld.setInverse(t,HK_REAL_EPSILON))
	{
		HK_ERROR(0x0DB78260,"Invalid transform");
	}
}

//
template <int BITS,int MARGIN,int DIMS>
void						hkGeometryProcessing::Quantizer<BITS,MARGIN,DIMS>::setLocalToWorldTransform(const hkMatrix4& t)
{
	m_localToWorld	=	t;
	if(HK_SUCCESS != m_worldToLocal.setInverse(t,HK_REAL_EPSILON))
	{
		HK_ERROR(0x0DB78261,"Invalid transform");
	}
}

//
template <int BITS,int MARGIN,int DIMS>
HKGP_FORCE_INLINE	void		hkGeometryProcessing::Quantizer<BITS,MARGIN,DIMS>::worldToLocal(const hkVector4& position,hkInt32* output) const
{
	hkVector4	k;
	m_worldToLocal.transformPosition(position,k);
	for(int i=0;i<DIMS;++i) output[i]=clamp(round(k(i)));
}

//
template <int BITS,int MARGIN,int DIMS>
HKGP_FORCE_INLINE	void		hkGeometryProcessing::Quantizer<BITS,MARGIN,DIMS>::localToWorld(const hkInt32* position,hkVector4& output) const
{
	hkVector4	k(hkVector4::getZero());
	for(int i=0;i<DIMS;++i) k(i)=(hkReal)position[i];
	m_localToWorld.transformPosition(k,output);
}

//
template <typename T>
HKGP_FORCE_INLINE void		hkGeometryProcessing::Span<T>::setFromVertices(const hkVector4& direction, const hkVector4* vertices,int numVertices)
{
	m_bounds[0] = m_bounds[1] = (T)direction.dot3(vertices[0]);
	for(int i=1;i<3;++i)
	{
		const T	d = (T)direction.dot3(vertices[i]);
		if(d<m_bounds[0])
			m_bounds[0]=d;
		else if(d>m_bounds[1])
			m_bounds[1]=d;
	}
}

//
template <typename T>
HKGP_FORCE_INLINE hkBool32	hkGeometryProcessing::Span<T>::intersect(const Span& a,const Span& b)
{
	if(a.m_bounds[0] > b.m_bounds[1]) return false;
	if(a.m_bounds[1] < b.m_bounds[0]) return false;
	return true;
}

//
HKGP_FORCE_INLINE hkVector4	hkGeometryProcessing::crossOffset(const hkVector4& o,const hkVector4& a,const hkVector4& b)
{
	hkVector4	oa;
	oa.setSub4(a,o);
	hkVector4	ob;
	ob.setSub4(b,o);
	hkVector4	ret;
	ret.setCross(oa,ob);
	return(ret);
}

//
HKGP_FORCE_INLINE hkVector4	hkGeometryProcessing::crossEdges(const hkVector4& a0, const hkVector4& a1, const hkVector4& b0, const hkVector4& b1)
{
	hkVector4	a; a.setSub4(a1,a0);
	hkVector4	b; b.setSub4(b1,b0);
	hkVector4	c; c.setCross(a,b);
	return c;
}

//
HKGP_FORCE_INLINE hkVector4	hkGeometryProcessing::planeEquation(const hkVector4& a,const hkVector4& b,const hkVector4& c,bool normalized)
{
	hkVector4	eq=crossOffset(a,b,c);
	if(normalized) eq.normalize3();
	eq(3)=-eq.dot3(a).getReal();
	return(eq);
}

//
HKGP_FORCE_INLINE bool		hkGeometryProcessing::comparePlanes(const hkVector4& a,const hkVector4& b,hkReal maxDistance,hkReal minCosAngle)
{
	if(hkMath::fabs(a(3)-b(3))<=maxDistance)
	{
		if(a.dot3(b).getReal()>=minCosAngle)
		{
			return(true);
		}
	}
	return(false);
}

//
HKGP_FORCE_INLINE hkReal		hkGeometryProcessing::decomposeVector(const hkVector4& vector,const hkVector4& axis,hkVector4& para,hkVector4& perp)
{
	const hkSimdReal	dot=vector.dot3(axis);
	para.setMul4(dot,axis);
	perp.setAddMul4(vector,axis,-dot);
	return(dot.getReal());
}

//
HKGP_FORCE_INLINE hkVector4	hkGeometryProcessing::projectOnPlane(const hkVector4& vector,const hkVector4& axis)
{
	hkVector4	pa,pe;
	decomposeVector(vector,axis,pa,pe);
	return(pe);
}

//
HKGP_FORCE_INLINE hkVector4	hkGeometryProcessing::projectOnAxis(const hkVector4& vector,const hkVector4& axis)
{
	hkVector4	pa,pe;
	decomposeVector(vector,axis,pa,pe);
	return(pa);
}

//
HKGP_FORCE_INLINE hkReal		hkGeometryProcessing::triangleArea2Squared(const hkVector4& a,const hkVector4& b)
{
	hkVector4	c;
	c.setCross(a,b);
	return(c.lengthSquared3().getReal());
}

//
HKGP_FORCE_INLINE hkReal		hkGeometryProcessing::triangleArea2(const hkVector4& a,const hkVector4& b,const hkVector4& c)
{
	return(crossOffset(a,b,c).length3().getReal());
}

//
HKGP_FORCE_INLINE hkReal		hkGeometryProcessing::tetrahedronVolume6(const hkVector4& a,const hkVector4& b,const hkVector4& c,const hkVector4& d)
{
	hkMatrix3	m;
	m.getColumn(0).setSub4(b,a);
	m.getColumn(1).setSub4(c,a);
	m.getColumn(2).setSub4(d,a);
	return(m.getDeterminant().getReal());
}

//
HKGP_FORCE_INLINE hkVector4	hkGeometryProcessing::transformPlaneToWorld(const hkTransform& transform,const hkVector4& plane)
{
	hkVector4	worldPlane;
	worldPlane.setRotatedDir(transform.getRotation(),plane);
	worldPlane(3)= (plane.getSimdAt(3)-worldPlane.dot3(transform.getTranslation())).getReal();
	return worldPlane;
}

//
HKGP_FORCE_INLINE hkVector4	hkGeometryProcessing::transformPlaneToLocal(const hkTransform& transform,const hkVector4& plane)
{
	hkVector4	localPlane;
	localPlane.setRotatedInverseDir(transform.getRotation(),plane);
	localPlane(3)= (plane.getSimdAt(3)+plane.dot3(transform.getTranslation())).getReal();
	return localPlane;
}

//
HKGP_FORCE_INLINE void		hkGeometryProcessing::getVertex(const hkAabb& box, int index, hkVector4& vertexOut)
{
	switch(index)
	{
	case	0:	vertexOut.set(box.m_min(0),box.m_min(1),box.m_min(2),0);break;
	case	1:	vertexOut.set(box.m_max(0),box.m_min(1),box.m_min(2),0);break;
	case	2:	vertexOut.set(box.m_max(0),box.m_max(1),box.m_min(2),0);break;
	case	3:	vertexOut.set(box.m_min(0),box.m_max(1),box.m_min(2),0);break;
	case	4:	vertexOut.set(box.m_min(0),box.m_min(1),box.m_max(2),0);break;
	case	5:	vertexOut.set(box.m_max(0),box.m_min(1),box.m_max(2),0);break;
	case	6:	vertexOut.set(box.m_max(0),box.m_max(1),box.m_max(2),0);break;
	case	7:	vertexOut.set(box.m_min(0),box.m_max(1),box.m_max(2),0);break;
	default:	box.getCenter(vertexOut);
	}
}

//
template <typename A,typename B>
HKGP_FORCE_INLINE hkUlong		hkGeometryProcessing::makeHash(A ia,B ib)
{
	HK_COMPILE_TIME_ASSERT(sizeof(ia)<=sizeof(hkUlong) && sizeof(ib)<=sizeof(hkUlong));
	hkUlong			va=3424933;(*(A*)&va)=ia;
	hkUlong			vb=1661161;(*(B*)&vb)=ib;
	return( (va*3424933) ^ (vb*1661161) );
}

//
template <typename A,typename B>
HKGP_FORCE_INLINE hkUlong		hkGeometryProcessing::makeSymmetricHash(A a,B b)
{
	if(a<b)
		return(makeHash(a,b));
	else
		return(makeHash(b,a));
}

//
HKGP_FORCE_INLINE int			hkGeometryProcessing::chooseMN_count(int m,int n)
{
	if(!n || n==m) return(1); else return(chooseMN_count(m-1,n-1)+chooseMN_count(m-1,n));
}

//
inline void					hkGeometryProcessing::chooseMN_pick(int m,int n,int i,int* set)
{
	for(int j=0;n && j<m;++j)
	{
		const int c=chooseMN_count(m-j-1,n-1);
		if(i<c)			{ *set++=j;--n; }
		else if(i>=c)	{ i-=c; }
	}
}

template <typename T, int CAPACITY>
hkGeometryProcessing::LocalArrayAllocator<T, CAPACITY>::LocalArrayAllocator()

{
	m_data = hkAllocateStack<T>(CAPACITY);
	m_next = m_data;
	m_end = m_data + CAPACITY;

}

template <typename T, int CAPACITY>
hkGeometryProcessing::LocalArrayAllocator<T, CAPACITY>::~LocalArrayAllocator()
{
	hkDeallocateStack<T>(m_data);
}

template <typename T, int CAPACITY>
T*	hkGeometryProcessing::LocalArrayAllocator<T, CAPACITY>::allocate(const T& value)
{ 
	// Use >= instead of == here 'just in case'
	if( m_data >= m_end )
	{
		return HK_NULL;
	}

	*m_next = value;
	T* retVal = m_next;
	m_next = m_next + 1;
	return retVal;
}

template <typename T, int CAPACITY>
HK_FORCE_INLINE void hkGeometryProcessing::LocalArrayAllocator<T, CAPACITY>::clear()
{
	// call dtor on all objects
	const int size = (int) (m_next - m_data);
	hkArrayUtil::destruct<T>(m_data, size, typename hkIsPodType<T>::type());
	m_next = m_data;
}

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
