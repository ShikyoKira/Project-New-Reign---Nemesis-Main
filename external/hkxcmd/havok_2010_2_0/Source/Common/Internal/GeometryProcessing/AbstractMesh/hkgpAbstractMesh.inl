/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

template <typename T,typename BASE,typename ALLOCATOR>
inline						hkgpAbstractMeshDefinitions::List<T,BASE,ALLOCATOR>::~List()
{
	reset();
}

//
template <typename T,typename BASE,typename ALLOCATOR>
inline void					hkgpAbstractMeshDefinitions::List<T,BASE,ALLOCATOR>::reset()
{
	if(m_allocator.needRelease())
	{
		while(m_used) release(m_used);
	}
	else
	{
		m_allocator.clear();
		m_used=0;
		m_numUsed=0;
	}
	
}
//
template <typename T,typename BASE,typename ALLOCATOR>
inline	T*					hkgpAbstractMeshDefinitions::List<T,BASE,ALLOCATOR>::allocate(const T& value)
{
	T*	item=m_allocator.allocate(value);
	if(item)
	{
		item->prev()=0;
		item->next()=m_used;
		if(m_used) m_used->prev()=item;
		m_used=item;
		m_numUsed++;
	}	
	return(item);
}
//
template <typename T,typename BASE,typename ALLOCATOR>
inline void					hkgpAbstractMeshDefinitions::List<T,BASE,ALLOCATOR>::release(T* item)
{
	T*	n=item->next();
	T*	p=item->prev();
	if(n) n->prev()=p;
	if(p) p->next()=n; else m_used=n;
	m_numUsed--;
	m_allocator.release(item);
}
//
template <typename T,typename BASE,typename ALLOCATOR>
inline void					hkgpAbstractMeshDefinitions::List<T,BASE,ALLOCATOR>::moveUp(T* i)
{
	if(i->prev())
	{
		T*	p=i->prev();
		T*	g=p->prev();
		T*	n=i->next();
		if(n) { n->prev()=p;p->next()=n; } else { p->next()=0; }
		if(g) { g->next()=i;i->prev()=g; } else { m_used=i;i->prev()=0; }
		p->prev()=i;
		i->next()=p;
	}	
}

//
template <typename T,typename BASE,typename ALLOCATOR>
inline void					hkgpAbstractMeshDefinitions::List<T,BASE,ALLOCATOR>::moveDown(T* item)
{
	if(item->next()) moveUp(item->next());
}

//
ABSTRACTMESH_HDR
inline void					ABSTRACTMESH_TYP::EdgeIterator::next()
{
	for(;m_edge.isValid();)
	{
		do
		{
			m_edge=m_edge.next();
		} while(m_edge.index() && !m_edge.isMaster());
		if(m_edge.index()==0 || !m_edge.isMaster())
		{
			m_edge=Edge(m_edge.triangle()->next(),0);
			if(m_edge.isValid() && m_edge.isMaster()) break;
		} else break;
	}
}

//
ABSTRACTMESH_HDR
inline						ABSTRACTMESH_TYP::~hkgpAbstractMesh()
{
#if defined(HK_DEBUG)
	reset();
#endif
}

//
ABSTRACTMESH_HDR
inline	void				ABSTRACTMESH_TYP::reset()
{
#if defined(HK_DEBUG)
	for(TRIANGLE* t=m_triangles.getFirst();t;t=t->next())
	{
		t->link(0)=t->link(1)=t->link(2)=0;
	}
#endif
	m_vertices.reset();
	m_triangles.reset();
}

//
ABSTRACTMESH_HDR
inline bool					ABSTRACTMESH_TYP::isValid() const
{
	return(checkTopology()==hkgpAbstractMeshDefinitions::VALID);
}

//
ABSTRACTMESH_HDR
inline typename hkgpAbstractMeshDefinitions::Error	ABSTRACTMESH_TYP::checkTopology(bool checkForOrphanVertices) const
{
	hkPointerMap<VERTEX*,VERTEX*>		vmap;
	hkPointerMap<TRIANGLE*,TRIANGLE*>	tmap;
	for(TRIANGLE* t=m_triangles.getFirst();t;t=t->next())
	{
		for(int i=0;i<3;++i)
		{
			EDGE	e(t,i);
			EDGE	l(e.link());
			if(!vmap.hasKey(e.start()))
			{
				bool	found=false;
				VERTEX*	sv=e.start();
				for(VERTEX* v=m_vertices.getFirst();v;v=v->next())
				{
					if(sv==v) { found=true;break; }
				}
				if(!found)
				{
					return(hkgpAbstractMeshDefinitions::ZOMBIE_VERTEX);
				}
				vmap.insert(sv,sv);
			}
			if(e.start()==e.end())
			{
				return(hkgpAbstractMeshDefinitions::NULL_EDGE);
			}
			if(l.isValid())
			{
				if(e.triangle()==l.triangle())
				{
					return(hkgpAbstractMeshDefinitions::SELF_EDGE);
				}
				if(!tmap.hasKey(l.triangle()))
				{
					bool	found=false;
					for(TRIANGLE* lt=m_triangles.getFirst();lt;lt=lt->next())
					{
						if(lt==l.triangle()) { found=true;break; }
					}
					if(!found) return(hkgpAbstractMeshDefinitions::ZOMBIE_TRIANGLE);
					tmap.insert(l.triangle(),l.triangle());
				}
				if((e.start()!=l.end())||(l.start()!=e.end()))
				{
					return(hkgpAbstractMeshDefinitions::INVALID_BIND);
				}
				if(e.apex()==l.apex())
				{
					return(hkgpAbstractMeshDefinitions::VOID_TRIANGLE);
				}
			}
		}
	}
	if(checkForOrphanVertices&& (vmap.getSize()!=m_vertices.getSize()))
	{
		return(hkgpAbstractMeshDefinitions::ORPHAN_VERTEX);
	}
	return(hkgpAbstractMeshDefinitions::VALID);
}
//
ABSTRACTMESH_HDR
inline bool					ABSTRACTMESH_TYP::hasReferenceTo(const VERTEX* vertex) const
{
	for(TRIANGLE* t=m_triangles.getFirst();t;t=t->next())
	{
		if(t->vertex(0)==vertex) return(true);
		if(t->vertex(1)==vertex) return(true);
		if(t->vertex(2)==vertex) return(true);
	}
	return(false);
}
//
ABSTRACTMESH_HDR
inline bool					ABSTRACTMESH_TYP::isClosedManifold() const
{
	for(TRIANGLE* t=m_triangles.getFirst();t;t=t->next())
	{
		if(EDGE(t,0).isNaked()||EDGE(t,1).isNaked()||EDGE(t,2).isNaked()) return(false);
	}
	return(true);
}

//
ABSTRACTMESH_HDR
inline bool					ABSTRACTMESH_TYP::bindEdge(Edge edge) const
{
	for(TRIANGLE* t=m_triangles.getFirst();t;t=t->next())
	{
		for(int i=0;i<3;++i)
		{
			if(EDGE(t,i).tryBind(edge)) return true;
		}
	}
	return false;
}

//
ABSTRACTMESH_HDR
inline EDGE					ABSTRACTMESH_TYP::findEdgeOrdered(const VERTEX* a,const VERTEX* b) const
{
	for(TRIANGLE* t=m_triangles.getFirst();t;t=t->next())
	{
		if(EDGE(t,0).start() == a && EDGE(t,0).end() == b) return EDGE(t,0);
		if(EDGE(t,1).start() == a && EDGE(t,2).end() == b) return EDGE(t,1);
		if(EDGE(t,2).start() == a && EDGE(t,0).end() == b) return EDGE(t,2);
	}
	return EDGE::null();
}

//
ABSTRACTMESH_HDR
inline EDGE					ABSTRACTMESH_TYP::findEdgeUnordered(const VERTEX* a,const VERTEX* b) const
{
	for(TRIANGLE* t=m_triangles.getFirst();t;t=t->next())
	{
		for(int i=0;i<3;++i)
		{
			if(EDGE(t,i).start() == a && EDGE(t,i).end() == b) return EDGE(t,i);
			if(EDGE(t,i).start() == b && EDGE(t,i).end() == a) return EDGE(t,i);
		}
	}
	return EDGE::null();
}

//
ABSTRACTMESH_HDR
inline TRIANGLE*			ABSTRACTMESH_TYP::findTriangleUnordered(const VERTEX* a,const VERTEX* b,const VERTEX* c) const
{
	for(TRIANGLE* t=m_triangles.getFirst();t;t=t->next())
	{
		int	mask=0;
		for(int i=0;i<3;++i)
		{
			if(t->vertex(i) == a || t->vertex(i) == b || t->vertex(i) == c) mask |= 1<<i;
		}
		if(mask == 7) return t;
	}
	return HK_NULL;
}

//
template <typename EDGE,typename VERTEX,typename TRIANGLE,typename TETRAHEDRON,typename ALLOCATOR>
inline	void				hkgpAbstractTetraMesh<EDGE,VERTEX,TRIANGLE,TETRAHEDRON,ALLOCATOR>::reset()
{
	Parent::reset();
	m_tetrahedra.reset();	
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
