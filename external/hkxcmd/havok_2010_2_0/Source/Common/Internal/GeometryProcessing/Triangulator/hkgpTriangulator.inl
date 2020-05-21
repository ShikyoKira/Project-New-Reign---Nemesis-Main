/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE											TRIANGULATOR_TYP::hkgpTriangulatorType()
{
	HK_COMPILE_TIME_ASSERT(ORIENTATION==+1 || ORIENTATION==-1);
	HK_COMPILE_TIME_ASSERT((BITS<30 && BITS>4));

	hkVector4	dmin,dmax;
	dmin.set(-1,-1,0,0);
	dmax.set(+1,+1,0,0);
	setDomain(dmin,dmax,0,true);
}
//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE											TRIANGULATOR_TYP::~hkgpTriangulatorType()
{
	reset();
}

//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE hkBool32								TRIANGULATOR_TYP::Edge::getAndClearConstraint() const
{
	const Edge		e(tBase::master());
	const hkUint32	m(1<<e.index());
	const hkUint32	v(e.triangle()->m_constraints&m);
	e.triangle()->m_constraints &= ~v;
	return v;
}

//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE hkBool32								TRIANGULATOR_TYP::Edge::canFlip() const
{
	if(!tBase::isNaked()&&!isConstrained())
	{
		const Edge	lk=tBase::link();
		const Edge	ep=tBase::prev();
		const Edge	lp=lk.prev();
		return(	(orientation(ep.start(),tBase::start(),lp.start())>0)&&
				(orientation(lp.start(),lk.start(),ep.start())>0));
	}
	return(false);
}
//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE typename TRIANGULATOR_TYP::Edge			TRIANGULATOR_TYP::Edge::flip() const
{
	HK_ASSERT2(0x122374,canFlip(),"Invalid flip");
	const hkBool32	c0=tBase::prev().getAndClearConstraint();
	const hkBool32	c1=tBase::link().prev().getAndClearConstraint();
	Edge			edge=tBase::flip();
	if(c0) edge.link().next().setConstrained();
	if(c1) edge.next().setConstrained();
	return(edge);
}

//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE int										TRIANGULATOR_TYP::Edge::rank() const
{
	Edge	edge=*this;
	int		r=0;
	if(tBase::start()->isBoundary())
	{
		while(!edge.isNaked()) edge.turnCcwInPlace();
		do { ++r;edge.turnCwInPlace(); } while(edge.isValid());
	}
	else
	{
		const Edge base=edge;
		do { ++r;edge.turnCwInPlace(); } while(edge!=base);
	}
	return(r);
}

//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE hkBool32								TRIANGULATOR_TYP::SortByArea_Predicate::operator()(const Triangle* a,const Triangle* b) const
{
	if(cfgIsCCW)
		return orientation(a->vertex(0),a->vertex(1),a->vertex(2)) > orientation(b->vertex(0),b->vertex(1),b->vertex(2));
	else
		return orientation(a->vertex(0),a->vertex(1),a->vertex(2)) < orientation(b->vertex(0),b->vertex(1),b->vertex(2));
}
//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE typename TRIANGULATOR_TYP::tSets		TRIANGULATOR_TYP::FloodPolicy::getSetsFromEdgeData(const EdgeData* edgeData) const
{
	return (tSets)(1<<edgeData->m_tag);
}
//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE typename TRIANGULATOR_TYP::tExtInt		TRIANGULATOR_TYP::orientation(hkInt32 ax,hkInt32 ay,hkInt32 bx,hkInt32 by,hkInt32 cx,hkInt32 cy)
{
	tExtInt bbx = bx-ax;
	tExtInt bby = by-ay;
	tExtInt ccx = cx-ax;
	tExtInt ccy = cy-ay;
	return(tExtInt(ORIENTATION) * (bby*ccx-bbx*ccy));
}

//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE typename TRIANGULATOR_TYP::tExtInt		TRIANGULATOR_TYP::orientation(const Vertex* a,hkInt32 bx,hkInt32 by,hkInt32 cx,hkInt32 cy)
{
	return(orientation(a->m_x,a->m_y,bx,by,cx,cy));
}

//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE typename TRIANGULATOR_TYP::tExtInt		TRIANGULATOR_TYP::orientation(const Vertex* a,const Vertex* b,hkInt32 x,hkInt32 y)
{
	return(orientation(a->m_x,a->m_y,b->m_x,b->m_y,x,y));
}
//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE typename TRIANGULATOR_TYP::tExtInt		TRIANGULATOR_TYP::orientation(const Vertex* a,const Vertex* b,const Vertex* c)
{
	return(orientation(a->m_x,a->m_y,b->m_x,b->m_y,c->m_x,c->m_y));
}
//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE typename TRIANGULATOR_TYP::tExtInt		TRIANGULATOR_TYP::orientation(const Edge& e,hkInt32 x,hkInt32 y)
{
	return(orientation(e.start(),e.end(),x,y));
}
//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE typename TRIANGULATOR_TYP::tExtInt		TRIANGULATOR_TYP::orientation(const Edge& e,const Vertex* c)
{
	return(orientation(e,c->m_x,c->m_y));
}
//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE hkBool32								TRIANGULATOR_TYP::checkOrientation(hkInt32 x0,hkInt32 y0,hkInt32 x1,hkInt32 y1)
{
	if(cfgIsCW)
	{
		hkAlgorithm::swap(x0,x1);
		hkAlgorithm::swap(y0,y1);
	}
	if(x0==x1)
	{
		if((x0==0) && (y0<y1))					return(false);
		else if((x0==cfgBoundary) && (y0>y1))	return(false);
	}
	if(y0==y1)
	{
		if((y0==0) && (x0>x1))					return(false);
		else if((y0==cfgBoundary) && (x0<x1))	return(false);
	}
	return(true);
}
//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE hkBool32								TRIANGULATOR_TYP::notInCircle(hkInt32 ax,hkInt32 ay,hkInt32 bx,hkInt32 by,hkInt32 cx,hkInt32 cy,hkInt32 dx,hkInt32 dy)
{
	HK_COMPILE_TIME_ASSERT(BITS <= 15);
	ax-=dx;ay-=dy;
	bx-=dx;by-=dy;
	cx-=dx;cy-=dy;
	const tBigInt	d0=(tBigInt)(ax*by-bx*ay);
	const tBigInt	d1=(tBigInt)(bx*cy-cx*by);
	const tBigInt	d2=(tBigInt)(cx*ay-ax*cy);
	const tBigInt	l0=(tBigInt)(ax*ax+ay*ay);
	const tBigInt	l1=(tBigInt)(bx*bx+by*by);
	const tBigInt	l2=(tBigInt)(cx*cx+cy*cy);
	return (hkBool32) (((l0*d1+l1*d2+l2*d0)*(tBigInt)ORIENTATION) >= 0);
}
//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE hkBool32								TRIANGULATOR_TYP::notInCircleFloat(hkInt32 axi,hkInt32 ayi,hkInt32 bxi,hkInt32 byi,hkInt32 cxi,hkInt32 cyi,hkInt32 dxi,hkInt32 dyi)
{
	const hkReal	ax = (hkReal)(axi-dxi), ay = (hkReal)(ayi-dyi);
	const hkReal	bx = (hkReal)(bxi-dxi), by = (hkReal)(byi-dyi);
	const hkReal	cx = (hkReal)(cxi-dxi), cy = (hkReal)(cyi-dyi);
	const hkReal	d0 = ax*by - bx*ay;
	const hkReal	d1 = bx*cy - cx*by;
	const hkReal	d2 = cx*ay - ax*cy;
	const hkReal	l0 = ax*ax + ay*ay;
	const hkReal	l1 = bx*bx + by*by;
	const hkReal	l2 = cx*cx + cy*cy;
	return (hkBool32) (((l0*d1+l1*d2+l2*d0)*(float)ORIENTATION) >= 0);
}
//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE hkInt32									TRIANGULATOR_TYP::clamp(hkInt32 x,hkInt32 min,hkInt32 max)
{
	return(x<min?min:x>max?max:x);
}
//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE hkInt32									TRIANGULATOR_TYP::round(hkReal x)
{
	return((int)(x<0?x-0.5f:x+0.5f));
}
//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE int										TRIANGULATOR_TYP::rootIndex(int x,int y)
{
	x>>=cfgRootShift;
	y>>=cfgRootShift;
	return((y<<cfgRoots)+x);
}
//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE	hkBool32								TRIANGULATOR_TYP::sameBoundary(const Vertex* v0,const Vertex* v1)
{
	if(v0->m_x==v1->m_x)
		if((v0->m_x==0)||(v0->m_x==cfgBoundary)) return(true);
	if(v0->m_y==v1->m_y)
		if((v0->m_y==0)||(v0->m_y==cfgBoundary)) return(true);
	return(false);
}
//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE hkBool32								TRIANGULATOR_TYP::isConform(const Vertex* a,const Vertex* b,const Vertex* c,const Vertex* p)
{
	return ConformPredicate::isConform((hkgpTriangulatorType*)0,a,b,c,p);
}
//
TRIANGULATOR_HDR
inline void												TRIANGULATOR_TYP::removeOrphanVertices()
{
	/* Mark vertices referenced by constrained edges	*/ 
	clearVerticesMarker();
	for(typename Mesh::EdgeIterator e=m_mesh.getEdgeIterator();e;++e)
	{
		Edge	edge(e());
		if(edge.isConstrained())
		{
			edge.start()->m_marker = edge.end()->m_marker = 1;
		}
	}
	/* Remove unmarked vertices							*/ 
	for(Vertex* v=m_mesh.m_vertices.getFirst();v;)
	{
		Vertex* nv=v->next();
		if(!v->m_marker)
		{
			if(v->isCorner())
				v->clrConstrained();
			else
				removeVertex(*v);
		}
		v = nv;
	}
}

//
TRIANGULATOR_HDR
inline int												TRIANGULATOR_TYP::getOpenBoundaries(hkArray<Edge>* edges) const
{
	/* Backup regions				*/ 
	hkArray<hkUint16>	regionsBackup;
	regionsBackup.reserveExactly(m_mesh.m_triangles.getSize());
	for(Triangle* t=m_mesh.m_triangles.getFirst();t;t=t->next())
	{
		hkUint16 region = t->m_region;
		regionsBackup.pushBack(region);
	}

	/* Partition					*/ 
	partition();

	/* Check for open boundaries	*/ 
	int		numFounds=0;
	for(Triangle* t=m_mesh.m_triangles.getFirst();t;t=t->next())
	{
		for(int i=0;i<3;++i)
		{
			Edge	e(t,i);
			if(e.isBound() && e.isConstrained())
			{
				if(e.triangle()->m_region==e.link().triangle()->m_region)
				{
					++numFounds;
					if(edges) edges->pushBack(e);
				}
			}
		}
	}

	/* Restore regions				*/ 
	const hkUint16*		regionsPointer=&regionsBackup[0];
	for(Triangle* t=m_mesh.m_triangles.getFirst();t;t=t->next())
	{
		t->m_region = *regionsPointer++;
	}

	return numFounds;
}
//
TRIANGULATOR_HDR
inline hkBool32											TRIANGULATOR_TYP::getEdgeDataOrientation() const
{
	int	counts[]={0,0};
	for(Triangle* t=m_mesh.m_triangles.getFirst();t;t=t->next())
	{
		if(t->m_region&1)
		{
			for(int i=0;i<3;++i)
			{
				Edge	e(t,i);
				if(e.isBound())
				{
					const EdgeData* dIn = getEdgeData(e);
					const EdgeData* dOut = getEdgeData(e.link());
					if(dIn) ++counts[0]; else if(dOut) ++counts[1];
				}
				else
				{
					if(getEdgeData(e)) ++counts[0]; else ++counts[1];
				}
			}
		}
	}
	return counts[0]>counts[1];
}
//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE hkBool32								TRIANGULATOR_TYP::isConform(const Edge& edge)
{
	return(edge.isNaked()||edge.isConstrained()||isConform(edge.apex(),edge.start(),edge.end(),edge.link().apex()));
}

//
TRIANGULATOR_HDR
const typename TRIANGULATOR_TYP::EdgeData*				TRIANGULATOR_TYP::getEdgeData(const Edge& edge) const
{
	return m_edgeDataPolicy.getEdgeData(edge);	
}

//
TRIANGULATOR_HDR
const typename TRIANGULATOR_TYP::EdgeData*				TRIANGULATOR_TYP::getAnyEdgeData(const Edge& edge) const
{
	const typename TRIANGULATOR_TYP::EdgeData*	ed;
	ed = getEdgeData(edge);
	if(ed) return ed;
	ed = edge.isBound()?getEdgeData(edge.link()):HK_NULL;
	return ed;
}

//
TRIANGULATOR_HDR
void													TRIANGULATOR_TYP::setEdgeData(const Edge& edge, const EdgeData& data)
{
	HK_ASSERT2(0x52faa065, edge.isConstrained(), "Edge data are only accessible for constrained edges." );
	m_edgeDataPolicy.setEdgeData(edge,data);
}

//
TRIANGULATOR_HDR
void													TRIANGULATOR_TYP::setDualEdgeData(const Edge& edge, const EdgeData& data)
{
	setEdgeData(edge,data);
	if(edge.isBound()) setEdgeData(edge.link(),data);
}

//
TRIANGULATOR_HDR
void													TRIANGULATOR_TYP::clearEdgeData(const Edge& edge)
{
	m_edgeDataPolicy.clearEdgeData(edge);
}
//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE typename TRIANGULATOR_TYP::tExtInt		TRIANGULATOR_TYP::distanceSqrd(const Vertex* a,const Vertex* b)
{
	tExtInt dx=a->m_x-b->m_x;
	tExtInt dy=a->m_y-b->m_y;
	return dx*dx+dy*dy;
}

//
TRIANGULATOR_HDR
inline hkBool32											TRIANGULATOR_TYP::canSplitEdge(int ix,int iy,const Edge& edge)
{
	if(orientation(edge.next(),ix,iy)<=0)									return(false);
	else if(orientation(edge.prev(),ix,iy)<=0)								return(false);
	else if(edge.isBound() && (orientation(edge.link().next(),ix,iy)<=0))	return(false);
	else if(edge.isBound() && (orientation(edge.link().prev(),ix,iy)<=0))	return(false);
	return(true);
}

//
TRIANGULATOR_HDR
inline typename TRIANGULATOR_TYP::Edge					TRIANGULATOR_TYP::flipEdge(const Edge& edge)
{
	HK_ASSERT2(0x190FF717, getEdgeData(edge)==HK_NULL && getEdgeData(edge.link())==HK_NULL, "Cannot flip an edge with attached data");
	return m_edgeDataPolicy.flipEdge(edge);
}

//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE void									TRIANGULATOR_TYP::copyDataset(const Triangle* from,Triangle* to)
{
	to->m_marker	=	from->m_marker;
	to->m_region	=	from->m_region;
	to->m_sets		=	from->m_sets;
}

//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE void									TRIANGULATOR_TYP::setRoot(Triangle* t)
{
	const Vertex*const*	v=&t->vertex(0);
	const hkInt32		cx=(v[0]->m_x*2+v[1]->m_x+v[2]->m_x)>>2;
	const hkInt32		cy=(v[0]->m_y*2+v[1]->m_y+v[2]->m_y)>>2;
	m_roots[rootIndex(cx,cy)]=t;
	t->m_root=1;
}

//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE typename TRIANGULATOR_TYP::Vertex*		TRIANGULATOR_TYP::newVertex(hkInt32 x,hkInt32 y,hkBool32 constraint)
{
	Vertex*		v=m_mesh.m_vertices.allocate();
	if(v)
	{
		v->m_x			=	x;
		v->m_y			=	y;
		v->m_constraint	=	constraint;
		v->m_marker		=	0;
		v->m_data		=	0;
		v->m_data		=	~v->m_data;
	}
	return(v);
}
//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE void									TRIANGULATOR_TYP::releaseVertex(Vertex* v)
{
	m_mesh.m_vertices.release(v);
}
//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE typename TRIANGULATOR_TYP::Triangle*	TRIANGULATOR_TYP::newTriangleNoRoot(Vertex* a,Vertex* b,Vertex* c)
{
	Triangle*	t=m_mesh.m_triangles.allocate();
	if(t)
	{
		t->setVertices(a,b,c);
		t->m_constraints	=	0;
		t->m_root			=	0;
		t->m_marker			=	0;
		t->m_region			=	0;
		t->m_sets			=	0;
	}
	return(t);
}
//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE typename TRIANGULATOR_TYP::Triangle*	TRIANGULATOR_TYP::newTriangle(Vertex* a,Vertex* b,Vertex* c)
{
	Triangle*	t=newTriangleNoRoot(a,b,c);
	if(t) setRoot(t);
	return(t);
}
//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE void									TRIANGULATOR_TYP::releaseTriangle(Triangle* t)
{
	if(t->m_root)
	{
		for(int i=0;i<cfgTotalRoots;++i)
		{
			if(m_roots[i]==t) m_roots[i] = HK_NULL;
		}
	}
	m_mesh.m_triangles.release(t);
}
//
TRIANGULATOR_HDR
HKGP_INLINE void										TRIANGULATOR_TYP::reset(bool createDomainTriangulation)
{
	/* Reset storages	*/ 
	m_mesh.reset();
	/* Clear roots		*/ 
	for(int i=0;i<cfgTotalRoots;++i)
	{
		m_roots[i] = HK_NULL;
	}
	/* Clear edge data	*/ 
	m_edgeDataPolicy.reset();

	/* Create base		*/ 
	if(createDomainTriangulation)
	{
		Vertex*		c00(newVertex(0,0,false));
		Vertex*		c01(newVertex(0,cfgBoundary,false));
		Vertex*		c10(newVertex(cfgBoundary,0,false));
		Vertex*		c11(newVertex(cfgBoundary,cfgBoundary,false));
		if(cfgIsCCW)
		{
			Triangle*	t0(newTriangle(c00,c01,c11));
			Triangle*	t1(newTriangle(c00,c11,c10));
			t0->link(0)=t0->link(1)=t0->link(2)=0;
			t1->link(0)=t1->link(1)=t1->link(2)=0;
			Edge(t0,2).bind(Edge(t1,0));
		}
		else
		{
			Triangle*	t0(newTriangle(c00,c10,c11));
			Triangle*	t1(newTriangle(c00,c11,c01));
			t0->link(0)=t0->link(1)=t0->link(2)=0;
			t1->link(0)=t1->link(1)=t1->link(2)=0;
			Edge(t0,2).bind(Edge(t1,0));
		}
	}	
}
//
TRIANGULATOR_HDR
inline void												TRIANGULATOR_TYP::clone(hkgpTriangulatorType* other) const
{
	other->reset(false);
	other->m_localToWorld	=	m_localToWorld;
	other->m_worldToLocal	=	m_worldToLocal;
	other->m_margin			=	m_margin;
	hkPointerMap<Vertex*,Vertex*>						vmap;
	hkPointerMap<Triangle*,Triangle*>					tmap;
	hkPointerMap<hkgpTopology::Uid,hkgpTopology::Uid>	emap;
	for(Vertex* v=m_mesh.m_vertices.getFirst();v;v=v->next())
	{
		Vertex*		nv=other->m_mesh->m_vertices.allocate(*v);
		vmap.insert(v,nv);
	}
	for(Triangle* t=m_mesh.m_triangles.getFirst();t;t=t->next())
	{
		Triangle*	nt=other->m_mesh->m_triangles.allocate(*t);
		tmap.insert(t,nt);
		for(int i=0;i<3;++i)
		{
			nt->vertex(i)=vmap.getWithDefault(nt->vertex(i),0);
		}
		for(int i=0;i<3;++i)
		{
			Edge	te(t,i);
			Edge	ne(nt,i);
			if(te.isNaked())
			{
				nt->link(i)=0;
			}
			else
			{
				Edge	link(te.link());
				Edge	le(emap.getWithDefault(link.uid,0));
				if(le.isValid())
				{
					ne.bind(le);
					emap.remove(link.uid());
				}
				else
				{
					emap.insert(te.uid(),ne.uid());
				}
			}
		}
	}
	for(int i=0;i<cfgTotalRoots;++i)
	{
		if(m_roots[i])
		{
			other->m_roots[i]=tmap.getWithDefault(m_roots[i],0);
		}
		else
		{
			other->m_roots[i]=0;
		}
	}
}

//
TRIANGULATOR_HDR
inline void												TRIANGULATOR_TYP::setDomain(const hkVector4& dmin,const hkVector4& dmax,hkReal angle,bool margin)
{
	HK_ASSERT2(0x454797,(dmin(0)<dmax(0))&&(dmin(1)<dmax(1)),"Invalid domain");
	m_margin	=	margin?1:0;
	const hkReal	ext(cfgBoundary-m_margin*2.0f);
	hkVector4		scl;
	const hkReal	s=hkMath::sin(angle);
	const hkReal	c=hkMath::cos(angle);
	scl.set((dmax(0)-dmin(0))/ext,(dmax(1)-dmin(1))/ext,0,0);
	m_localToWorld=hkMatrix4::getIdentity();
	m_localToWorld(0,0)=	c*scl(0);	m_localToWorld(0,1)=	s*scl(1);	m_localToWorld(0,3)=	dmin(0);
	m_localToWorld(1,0)=	-s*scl(0);	m_localToWorld(1,1)=	c*scl(1);	m_localToWorld(1,3)=	dmin(1);
	m_worldToLocal.setInverse(m_localToWorld,0);
	reset();
}
//
TRIANGULATOR_HDR
inline void												TRIANGULATOR_TYP::setDomainFromPoints(const hkStridedVertices& points, const hkMatrix4& worldToXYPlane, bool margin, bool keepRatio)
{
	/* Calculate domain bounds			*/ 
	hkVector4	dmin,dmax;
	dmin.set(+HK_REAL_MAX,+HK_REAL_MAX,+HK_REAL_MAX,0);
	dmax.set(-HK_REAL_MAX,-HK_REAL_MAX,-HK_REAL_MAX,0);
	for(int i=0,ni=points.getSize();i<ni;++i)
	{
		hkVector4	tp;
		hkVector4	vt;
		points.getVertex(i,vt);
		worldToXYPlane.transformPosition(vt,tp);
		dmin.setMin4(dmin,tp);
		dmax.setMax4(dmax,tp);
	}
	/* Fixup zero bounds				*/ 
	const hkReal	minExtend=HK_REAL_EPSILON*100;
	if((dmax(0)-dmin(0))<minExtend) { dmin(0)-=minExtend;dmax(0)+=minExtend; }
	if((dmax(1)-dmin(1))<minExtend) { dmin(1)-=minExtend;dmax(1)+=minExtend; }
	/* Keep ratio						*/ 
	hkVector4		extents;extents.setSub4(dmax,dmin);
	if(keepRatio)
	{
		const int		maxExt=extents(0)>extents(1)?0:1;
		const int		minExt=1-maxExt;
		const hkReal	half=(extents(maxExt)-extents(minExt))*0.5f;
		dmin(minExt)	-=	half;
		dmax(minExt)	+=	half;
		extents(minExt)	=	extents(maxExt);
	}
	/* Evaluate transforms				*/ 
	m_margin	=	margin?1:0;
	const hkReal	ext(cfgBoundary-m_margin*2.0f);
	hkMatrix4		trs(hkMatrix4::getIdentity());
	trs(0,0)	=	ext/extents(0);
	trs(1,1)	=	ext/extents(1);
	trs(0,3)	=	-dmin(0)*trs(0,0)+m_margin;
	trs(1,3)	=	-dmin(1)*trs(1,1)+m_margin;
	m_worldToLocal.setMul(trs,worldToXYPlane);
	m_localToWorld.setInverse(m_worldToLocal,0);
	reset();
}
//
TRIANGULATOR_HDR
inline void												TRIANGULATOR_TYP::setDomainFromPoints(const hkStridedVertices& points,bool margin,bool keepRatio)
{
	setDomainFromPoints(points,hkMatrix4::getIdentity(),margin,keepRatio);
}

//
TRIANGULATOR_HDR
inline void												TRIANGULATOR_TYP::setDomainFromPlanarPoints(const hkStridedVertices& points,hkVector4Parameter planeNormal,bool margin,bool keepRatio,bool useMainAxis)
{
	hkMatrix4		mat = hkMatrix4::getIdentity();
	if(useMainAxis)
	{
		int			majorAxis = planeNormal.getMajorAxis3();
		hkAlgorithm::swap(mat.getColumn(majorAxis),mat.getColumn(2));
		if(majorAxis<2 && planeNormal(majorAxis)>0)
		{
			hkVector4	c0 = mat.getColumn(0);
			hkVector4	c1 = mat.getColumn(1);
			mat.getColumn(0).setNeg4(c0);
			mat.getColumn(1).setNeg4(c1);
		}
		else if(majorAxis==2 && planeNormal(majorAxis)<0)
		{
			hkVector4	c0 = mat.getColumn(0);
			mat.getColumn(0).setNeg4(c0);
		}
	}
	else
	{
		hkVector4		xyPlane;xyPlane.set(0,0,1,0);
		hkQuaternion	rot;
		rot.setShortestRotation(planeNormal,xyPlane);
		hkTransform		trs=hkTransform::getIdentity();
		trs.setRotation(rot);
		mat.set(trs);
	}

	setDomainFromPoints(points,mat,margin,keepRatio);
}

//
TRIANGULATOR_HDR
inline hkVector4										TRIANGULATOR_TYP::getEffectiveAccuracy() const
{
	hkVector4	d,td,acc;
	acc.setZero4();
	d.set(1,0,0,0);	m_localToWorld.transformDirection(d,td); acc(0)=td.length3();
	d.set(0,1,0,0);	m_localToWorld.transformDirection(d,td); acc(1)=td.length3();
	return acc;
}

//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE hkVector4								TRIANGULATOR_TYP::transform(const Vertex* v) const
{
	hkVector4	x;
	hkVector4 vReal; vReal.set( (hkReal)v->m_x, (hkReal)v->m_y, 0);
	m_localToWorld.transformPosition(vReal, x);
	return(x);
}
//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE hkVector4								TRIANGULATOR_TYP::transformUnquantized(const hkVector4& v) const
{
	hkVector4	x;
	m_worldToLocal.transformPosition(v,x);
	return x;
}	
//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE typename TRIANGULATOR_TYP::Vertex		TRIANGULATOR_TYP::quantize(const hkVector4& v) const
{
	const int	bmi=m_margin;
	const int	bmx=cfgBoundary-m_margin;
	Vertex		k;
	k.m_x	=	clamp(round(v(0)),bmi,bmx);
	k.m_y	=	clamp(round(v(1)),bmi,bmx);
	return k;
}
//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE typename TRIANGULATOR_TYP::Vertex		TRIANGULATOR_TYP::transform(const hkVector4& v) const
{
	return quantize(transformUnquantized(v));
}	
//
TRIANGULATOR_HDR
inline hkBool32											TRIANGULATOR_TYP::isConform() const
{
	for(Triangle* t=m_mesh.m_triangles.getFirst();t;t=t->next())
	{
		for(int i=0;i<3;++i)
		{
			if(!isConform(Edge(t,i)))
			{
				return(false);
			}
		}
	}
	return(true);
}
//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE typename TRIANGULATOR_TYP::Edge			TRIANGULATOR_TYP::conformEdge(const Edge& edge)
{
	const Vertex*	v0=edge.start();
	const Vertex*	v1=edge.end();
	m_stack.pushBack(edge.next());
	m_stack.pushBack(edge.prev());
	if(!edge.isNaked())
	{
		m_stack.pushBack(edge.nextCcw());
		m_stack.pushBack(edge.nextCw());
	}
	flushConform();
	return(locateExistingEdge(edge,v0,v1));
}	
//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE typename TRIANGULATOR_TYP::Triangle*	TRIANGULATOR_TYP::getRoot(int x,int y) const
{
	Triangle* root=m_roots[rootIndex(x,y)];
	if(!root) root=m_mesh.m_triangles.getFirst();
	return(root);
}
//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE typename TRIANGULATOR_TYP::Location		TRIANGULATOR_TYP::locateVertex(const Edge& rootArg,int x,int y) const
{
	HK_ASSERT2(0x9845757,(x>=0)&&(x<=cfgBoundary)&&(y>=0)&&(y<=cfgBoundary),"Vertex out of domain");
	Edge	root = rootArg;
	for(Triangle* base=m_mesh.m_triangles.getFirst();base;base=base->next())
	{
		tExtInt	last=0;
		while((last=orientation(root,x,y))<0)
		{
			root.turnCcwInPlace();
		}
		for(int n=m_mesh.m_triangles.getSize();n>0;--n)
		{
			const Edge		es[]={root.next(),root.prev()};
			const tExtInt	dir[]={orientation(es[0],x,y),orientation(es[1],x,y)};
			const tExtInt	moi=dir[0]<dir[1]?0:1;
			const tExtInt	mov=dir[moi];
			const Edge		med=es[moi];
			if(mov<0)
			{
				last	=	-mov;
				root	=	med.link();
			}
			else
			{
				const hkUint32 type=(last==0?1:0)+(dir[0]==0?2:0)+(dir[1]==0?4:0);
				switch(type&7)
				{
				case	0+0+0:	return(Location(Location::IN_TRIANGLE,root));
				case	0+0+1:	return(Location(Location::ON_EDGE,root));
				case	0+2+0:	return(Location(Location::ON_EDGE,es[0]));
				case	4+0+0:	return(Location(Location::ON_EDGE,es[1]));
				case	0+2+1:	return(Location(Location::ON_VERTEX,es[0]));
				case	4+2+0:	return(Location(Location::ON_VERTEX,es[1]));
				case	4+0+1:	return(Location(Location::ON_VERTEX,root));
				default:		return(Location(Location::NOT_FOUND,Edge::null()));
				}
			}
		}
		root=Edge(base,0);
	}
	HK_ERROR(0xB8C66B5F,"Cycle detected during point location");
	return(Location(Location::CYCLE,Edge::null()));
}
//
TRIANGULATOR_HDR
HKGP_INLINE typename TRIANGULATOR_TYP::Location			TRIANGULATOR_TYP::locateVertex(int x,int y) const
{
	return(locateVertex(Edge(getRoot(x,y),0),x,y));
}
//
TRIANGULATOR_HDR
inline typename TRIANGULATOR_TYP::Edge					TRIANGULATOR_TYP::locateExistingEdge(int x0,int y0,int x1,int y1) const
{
	Location	l0=locateVertex(x0,y0);
	Location	l1=locateVertex(x1,y1);
	if(	(l0.m_type==Location::ON_VERTEX)&&
		(l1.m_type==Location::ON_VERTEX))
	{
		return(locateExistingEdge(l0.m_edge,l0.m_edge.start(),l1.m_edge.start()));		
	}
	return(Edge::null());
}
//
TRIANGULATOR_HDR
inline typename TRIANGULATOR_TYP::Edge					TRIANGULATOR_TYP::locateExistingVertex(const Edge& rootArg,const Vertex* v) const
{
	Edge	root = rootArg;
	if(root.start()!=v)
	{
		root=locateVertex(root,v->m_x,v->m_y).m_edge;
		HK_ASSERT2(0x78244,root.start()==v,"Vertex does not exist");
	}
	return(root);
}
//
TRIANGULATOR_HDR
inline typename TRIANGULATOR_TYP::Edge					TRIANGULATOR_TYP::locateExistingEdge(const Edge& rootArg,const Vertex* v0,const Vertex* v1) const
{
	Edge	root = rootArg;
	root=locateExistingVertex(root,v0);
	if(root.end()!=v1)
	{
		if(root.start()->isBoundary())
		{
			while(!root.isNaked()) root.turnCcwInPlace();			
		}
		while(root.end()!=v1) root.turnCwInPlace();
		HK_ASSERT2(0x78754,(root.start()==v0)&&(root.end()==v1),"Edge does not exist");
	}
	return(root);
}
//
TRIANGULATOR_HDR
HKGP_FORCE_INLINE typename TRIANGULATOR_TYP::Edge			TRIANGULATOR_TYP::collapse(const Edge& edge)
{
	Vertex*	org=edge.start();
	Vertex*	end=edge.end();
	Edge	link=edge.link();
	if(link.isValid())
	{
		const hkBool32	c0=edge.next().getAndClearConstraint();
		const hkBool32	c1=link.prev().getAndClearConstraint();
		Edge		fan=edge;
		if(org->isBoundary())
		{
			while(!fan.isNaked()) fan.turnCcwInPlace();
		}
		Edge		base=fan;
		do 
		{
			fan.start()=end;
			fan.turnCwInPlace();
		} while(fan.isValid()&&(fan!=base));
		Edge	ea=edge.prev().link();
		Edge	eb=link.next().link();
		ea.bind(edge.next().link());
		eb.bind(link.prev().link());
		releaseTriangle(edge.triangle());
		releaseTriangle(link.triangle());
		releaseVertex(org);
		if(c0) ea.setConstrained();
		if(c1) eb.setConstrained();		
	}
	else
	{

	}
	return(Edge::null());
}
//
TRIANGULATOR_HDR
inline void								TRIANGULATOR_TYP::clearVerticesMarker()
{
	for(typename Mesh::VertexIterator i=m_mesh.getVertexIterator(); i; ++i) i()->m_marker = 0;
}
//
TRIANGULATOR_HDR
inline void								TRIANGULATOR_TYP::clearTrianglesMarker()
{
	for(typename Mesh::TriangleIterator i=m_mesh.getTriangleIterator(); i; ++i) i()->m_marker = 0;
}
//
TRIANGULATOR_HDR
template <typename T>
inline hkBool32							TRIANGULATOR_TYP::fetchFanBoundary(Edge edge,T& boundaries)
{
	if(edge.start()->isBoundary())
	{
		Edge	last=edge;
		while(edge.isBound()) edge=edge.link().next();
		do
		{
			if(edge.isConstrained()) return(false);
			Boundary&	b	=	boundaries.expandOne();
			b.m_link		=	edge.next().link();
			b.m_vertex		=	edge.end();
			b.m_triangle	=	edge.triangle();
			b.m_constrained	=	edge.next().isConstrained() != 0;
			last			=	edge;
			edge			=	edge.prev().link();
		} while(edge.isValid());
		if(last.prev().isConstrained()) return(false);
		Boundary&	b	=	boundaries.expandOne();
		b.m_link		=	Edge::null();
		b.m_vertex		=	last.apex();
		b.m_triangle	=	HK_NULL;
		b.m_constrained	=	false;
	}
	else
	{
		Edge	first=edge;
		do
		{
			if(edge.isConstrained()) return(false);
			Boundary&	b	=	boundaries.expandOne();
			b.m_link		=	edge.next().link();
			b.m_vertex		=	edge.end();
			b.m_triangle	=	edge.triangle();
			b.m_constrained	=	edge.next().isConstrained() != 0;
			edge			=	edge.prev().link();
		} while(edge!=first);
	}
	return(true);
}
//
TRIANGULATOR_HDR
inline void								TRIANGULATOR_TYP::triangulateBoundary(Boundary* boundary)
{
	for(;;)
	{
		Boundary*	ba=boundary;
		Boundary*	bb=ba->m_next;
		Boundary*	bc=bb->m_next;
		if(bc==ba) break;
		if(orientation(ba->m_vertex,bb->m_vertex,bc->m_vertex)>0)
		{
			Boundary*	bd=bc->m_next;
			while(bd!=ba)
			{
				if(	orientation(ba->m_vertex,bb->m_vertex,bd->m_vertex)>=0	&&
					orientation(bb->m_vertex,bc->m_vertex,bd->m_vertex)>=0	&&
					orientation(bc->m_vertex,ba->m_vertex,bd->m_vertex)>=0)
				{ bd=HK_NULL;break; }
				else
				{ bd=bd->m_next; }
			}
			if(bd!=HK_NULL)
			{
				Triangle*	t=newTriangle(ba->m_vertex,bb->m_vertex,bc->m_vertex);
				Edge(t,0).bind(ba->m_link);if(ba->m_constrained) Edge(t,0).setConstrained();
				Edge(t,1).bind(bb->m_link);if(bb->m_constrained) Edge(t,1).setConstrained();
				t->link(2)			=	0;
				ba->m_link			=	Edge(t,2);
				ba->m_next			=	bc;
				ba->m_constrained	=	false;
				setRoot(t);
			}
		}
		boundary=boundary->m_next;
	}
	Edge	last(m_mesh.m_triangles.getFirst(),2);
	last.bind(boundary->m_link);
	if(boundary->m_constrained) last.setConstrained();
}
//
TRIANGULATOR_HDR
HKGP_INLINE typename TRIANGULATOR_TYP::Insertion	TRIANGULATOR_TYP::insertVertex(int x,int y,bool conform,const Location& location,bool edgeSplit)
{
	Edge		edge(location.m_edge);
	switch(location.m_type)
	{
	case	Location::IN_TRIANGLE:
		{
			Vertex*			k=newVertex(x,y,true);
			if(!k)			return Insertion(Insertion::OUT_OF_MEMORY,Edge::null());

			const hkBool32	c1=edge.next().getAndClearConstraint();
			const hkBool32	c2=edge.prev().getAndClearConstraint();

			Triangle*		t0=newTriangleNoRoot(edge.end(),edge.apex(),k);
			Triangle*		t1=newTriangleNoRoot(edge.apex(),edge.start(),k);
			if(!t0 || !t1)	return Insertion(Insertion::OUT_OF_MEMORY,Edge::null());

			m_edgeDataPolicy.propagateDataOnSplitTriangle(edge,t0,t1);

			edge.apex()=k;
			copyDataset(edge.triangle(), t0);
			copyDataset(edge.triangle(), t1);
			Edge(t0,0).bind(edge.prevCcw());
			Edge(t1,0).bind(edge.prevCw());
			Edge(t0,1).bind(Edge(t1,2));
			Edge(t0,2).bind(edge.next());
			Edge(t1,1).bind(edge.prev());
			if(c1) Edge(t0,0).setConstrained();
			if(c2) Edge(t1,0).setConstrained();
			Edge			e=Edge(t0,2);
			const Vertex*	v=e.start();
			if(conform)
			{
				m_stack.pushBack(Edge(t0,0));
				m_stack.pushBack(Edge(t1,0));
				m_stack.pushBack(edge);
				flushConform();
				e=locateExistingVertex(e,v);
			}
			HK_ASSERT2(0x545451,e.start()->m_x==x && e.start()->m_y==y,"Invalid edge");
			setRoot(e.triangle());
			return(Insertion(Insertion::SUCCESS,e));
		}
		break;
	case	Location::ON_EDGE:
		if(edgeSplit || !edge.isConstrained())
		{
			Vertex*			k=newVertex(x,y,true);
			if(!k)			return Insertion(Insertion::OUT_OF_MEMORY,Edge::null());

			const hkBool32	ce=edge.getAndClearConstraint();
			EdgeDataState	eds; if(ce) m_edgeDataPolicy.backupBothEdgeData(*this,edge,eds);
			if(edge.isBound())
			{
				Edge			link=edge.link();
				const hkBool32	c0=edge.prev().getAndClearConstraint();
				const hkBool32	c1=link.prev().getAndClearConstraint();

				Triangle*	t0=newTriangle(edge.apex(),edge.start(),k);
				Triangle*	t1=newTriangle(link.apex(),link.start(),k);
				if(!t0 || !t1)	return Insertion(Insertion::OUT_OF_MEMORY,Edge::null());

				m_edgeDataPolicy.propagateDataOnSplitBoundEdge(edge,link,t0,t1);

				copyDataset(edge.triangle(),t0);
				copyDataset(link.triangle(),t1);
				edge.start()=link.start()=k;
				Edge(t0,0).bind(edge.prevCw());
				Edge(t1,0).bind(link.prevCw());
				Edge(t0,1).bind(link);
				Edge(t1,1).bind(edge);
				Edge(t0,2).bind(edge.prev());
				Edge(t1,2).bind(link.prev());
				if(c0) Edge(t0,0).setConstrained();
				if(c1) Edge(t1,0).setConstrained();
				if(edgeSplit && ce)
				{
					Edge(t0,1).setConstrained();
					Edge(t1,1).setConstrained();
					m_edgeDataPolicy.restoreBothEdgeData(*this,Edge(t0,1),eds);
					m_edgeDataPolicy.restoreBothEdgeData(*this,Edge(t1,1).link(),eds);
				}
				if(conform)
				{							
					m_stack.pushBack(edge.next());
					m_stack.pushBack(link.next());
					m_stack.pushBack(Edge(t0,0));
					m_stack.pushBack(Edge(t1,0));
				}
			}
			else
			{
				const hkBool32	c0=edge.prev().getAndClearConstraint();
				Triangle*		t0=newTriangle(edge.apex(),edge.start(),k);
				if(!t0)			return Insertion(Insertion::OUT_OF_MEMORY,Edge::null());

				m_edgeDataPolicy.propagateDataOnSplitNakedEdge(edge,t0);

				copyDataset(edge.triangle(),t0);
				edge.start()=k;
				Edge(t0,0).bind(edge.prevCw());
				Edge(t0,1).bind(Edge::null());
				Edge(t0,2).bind(edge.prev());
				if(c0) Edge(t0,0).setConstrained();
				if(edgeSplit && ce)
				{
					Edge(t0,1).setConstrained();
					Edge(t0,2).link().next().setConstrained();
					m_edgeDataPolicy.restoreBothEdgeData(*this,Edge(t0,1),eds);
					m_edgeDataPolicy.restoreBothEdgeData(*this,Edge(t0,2).link(),eds);
				}
				if(conform)
				{
					m_stack.pushBack(edge.next());
					m_stack.pushBack(Edge(t0,0));
				}
			}
			if(conform)
			{
				const Vertex* v=edge.start();
				flushConform();
				edge=locateExistingVertex(edge,v);
			}
			HK_ASSERT2(0x545452,edge.start()->m_x==x && edge.start()->m_y==y,"Invalid edge");
			return(Insertion(Insertion::SUCCESS,edge));
		}
		else
		{
			return(Insertion(Insertion::ON_CONSTRAINED_EDGE,edge));
		}
		break;
	case	Location::ON_VERTEX:
		if(!edge.start()->m_constraint)
		{
			edge.start()->m_constraint=true;
			HK_ASSERT2(0x545453,edge.start()->m_x==x && edge.start()->m_y==y,"Invalid edge");
			return(Insertion(Insertion::SUCCESS,edge));
		}
		else
		{
			HK_ASSERT2(0x545454,edge.start()->m_x==x && edge.start()->m_y==y,"Invalid edge");
			return(Insertion(Insertion::ALREADY_PRESENT,edge));
		}
		break;
	default:
		break;
	}
	return(Insertion(Insertion::FAILED,Edge::null()));
}
//
TRIANGULATOR_HDR
inline typename TRIANGULATOR_TYP::Insertion	TRIANGULATOR_TYP::insertVertex(int x,int y,bool conform)
{
	HK_ASSERT2(0x9345757,(x>=m_margin)&&(x<=(cfgBoundary-m_margin))&&(y>=m_margin)&&(y<=(cfgBoundary-m_margin)),"Vertex out of domain");
	const Location	location(locateVertex(x,y));
	return(insertVertex(x,y,conform,location,false));
}
//
TRIANGULATOR_HDR
inline typename TRIANGULATOR_TYP::Insertion	TRIANGULATOR_TYP::insertVertex(const Vertex& v,bool conform)
{
	return(insertVertex(v.m_x,v.m_y,conform));
}
//
TRIANGULATOR_HDR
template <typename EDGECROSSING>
inline typename TRIANGULATOR_TYP::Insertion	TRIANGULATOR_TYP::insertVertex(int x,int y,bool conform,const EDGECROSSING& icrossing)
{
	Insertion	ins=insertVertex(x,y,conform);
	if(ins.m_type==Insertion::ON_CONSTRAINED_EDGE)
	{
		icrossing.notifyEdgeCrossing(ins.m_edge);
		Location	location;
		location.m_type	=	Location::ON_EDGE;
		location.m_edge	=	ins.m_edge;
		ins	=	insertVertex(x,y,conform,location,true);
	}
	return(ins);
}
//
TRIANGULATOR_HDR
template <typename EDGECROSSING>
inline typename TRIANGULATOR_TYP::Insertion	TRIANGULATOR_TYP::insertVertex(const Vertex& v,bool conform,const EDGECROSSING& icrossing)
{
	return(insertVertex(v.m_x,v.m_y,conform,icrossing));
}
//
TRIANGULATOR_HDR
inline typename TRIANGULATOR_TYP::Removal			TRIANGULATOR_TYP::removeVertex(int x,int y)
{
	Location	location=locateVertex(x,y);
	if(location.m_type==Location::ON_VERTEX)
	{
		Vertex*						vertex=location.m_edge.start();
		if(vertex->isCorner())
		{
			vertex->m_constraint	=	false;
			return(Removal(Removal::SUCCESS));
		}
		else
		{
			hkInplaceArray<Boundary,64>	boundaries;
			if(fetchFanBoundary(location.m_edge,boundaries))
			{			
				const int	numBoundaries=boundaries.getSize();
				for(int i=numBoundaries-1,j=0;j<numBoundaries;i=j++)
				{
					Boundary& b(boundaries[j]);
					boundaries[i].m_next=&b;
					if(b.m_triangle)
					{
						Edge(b.m_triangle,0).unbind();
						Edge(b.m_triangle,1).unbind();
						Edge(b.m_triangle,2).unbind();
						releaseTriangle(b.m_triangle);
					}
				}
				releaseVertex(vertex);
				triangulateBoundary(&boundaries[0]);
				return(Removal(Removal::SUCCESS));
			}
			else
			{
				return(Removal(Removal::BELONG_TO_CONSTRAINT));
			}
		}
	}
	return(Removal(Removal::NOT_FOUND));
}
//
TRIANGULATOR_HDR
inline typename TRIANGULATOR_TYP::Removal				TRIANGULATOR_TYP::removeVertex(const Vertex& v)
{
	return removeVertex(v.m_x,v.m_y);
}
//	
TRIANGULATOR_HDR
inline typename TRIANGULATOR_TYP::Insertion				TRIANGULATOR_TYP::insertEdge(int x0,int y0,int x1,int y1,bool conform,const EdgeData* data)
{
	/* Zero length						*/
	if((x0==x1)&&(y0==y1))				return(Insertion(Insertion::EDGE_IS_VERTEX,Edge::null()));
	/* Locate origin					*/ 
	const Location	l0=locateVertex(x0,y0);
	if(l0.m_type!=Location::ON_VERTEX)	return(Insertion(Insertion::CANNOT_LOCATE_ORIGIN,Edge::null()));
	Edge			edge=l0.m_edge;
	const Vertex*	v0=edge.start();
	/* Focus							*/ 
	while(orientation(edge,x1,y1)<0)							edge.turnCcwInPlace();		
	while(edge.isValid()&&orientation(edge.prev(),x1,y1)<=0)	edge.turnCwInPlace();
	if(!edge.isValid())					return(Insertion(Insertion::INVALID_ORIENTATION,Edge::null()));
	Edge			focal=edge.next();
	/* Already inserted					*/ 
	if(edge.end()->isEqual(x1,y1))
	{
		if(edge.isConstrained()) return(Insertion(Insertion::ALREADY_PRESENT,edge));
		edge.setConstrained();
		if(data) setEdgeData(edge,*data);
		if(conform)
		{
			edge=conformEdge(edge);
		}
		return(Insertion(Insertion::SUCCESS,edge));
	}
	/* Check orientation wrt. bounds	*/ 
	if(!checkOrientation(x0,y0,x1,y1))	return(Insertion(Insertion::INVALID_ORIENTATION,Edge::null()));	
	/* Walk to destination				*/ 
	while(orientation(focal,x1,y1)<0)
	{
		if(focal.isConstrained())
		{
			if(orientation(focal.start(),x0,y0,x1,y1)==0)		return(Insertion(Insertion::CROSSING_CONSTRAINED_VERTEX,focal));
			else if(orientation(focal.end(),x0,y0,x1,y1)==0)	return(Insertion(Insertion::CROSSING_CONSTRAINED_VERTEX,focal.next()));
			else												return(Insertion(Insertion::CROSSING_CONSTRAINED_EDGE,focal));
		}
		focal=focal.link();
		const tExtInt	o=orientation(v0,focal.apex(),x1,y1);
		if(o<0)
		{
			focal=focal.next();
		}
		else if(o>0)
		{
			focal=focal.prev();
		}
		else
		{
			if(!focal.apex()->isEqual(x1,y1))	return(Insertion(Insertion::CROSSING_CONSTRAINED_VERTEX,focal.prev()));
			focal=focal.prev();
		}
	}
	const Vertex*	v1=focal.start();
	if(v0==v1)							return(Insertion(Insertion::EDGE_IS_VERTEX,Edge::null()));
	if(orientation(focal,x1,y1)!=0)		return(Insertion(Insertion::CANNOT_LOCATE_DESTINATION,Edge::null()));
	if(!v1->isEqual(x1,y1))				{ HK_ASSERT2(0xF0A7B9C3,locateVertex(x1,y1).m_type!=Location::ON_VERTEX,"Inconsistent location results"); return(Insertion(Insertion::CANNOT_LOCATE_DESTINATION,focal)); }
	if(sameBoundary(v0,v1))				return(Insertion(Insertion::INVALID_ORIENTATION,focal));
	if(orientation(edge,v1)==0)			return(Insertion(Insertion::CROSSING_CONSTRAINED_VERTEX,edge.next()));
	if(orientation(edge,v1)<0)			return(Insertion(Insertion::CROSSING_CONSTRAINED_VERTEX,focal));
	/* Insert							*/ 
	int		skippedA=0;
	while(edge.end()!=v1)
	{
		Edge	root=edge;
		edge=edge.next();
		while(orientation(v0,v1,edge.end())>0) edge.turnCcwInPlace();
		if(orientation(root.start(),edge.start(),edge.end())>0)
		{
			const Vertex*	q=edge.end();
			int				skippedB=0;
			edge=root.next();
			while(edge.end()!=q)
			{					
				if(edge.canFlip())
				{
					edge=flipEdge(edge);
					if(skippedB)
					{
						edge=edge.nextCcw().link();
						--skippedB;
					}
					else
					{
						edge=edge.nextCw();
					}
				}
				else
				{
					++skippedB;
					edge.turnCcwInPlace();						
				}
			}
			edge=edge.prevCcw();
			if(skippedA)
			{
				while(orientation(v0,v1,edge.apex())>0) edge.turnCwInPlace();
				edge=edge.prev();
				--skippedA;
			}
		}
		else
		{
			++skippedA;
		}
	}

	HK_ASSERT2(0x6cc64e18, (edge.start() == v0) && (edge.end()==v1), "Edge insertion internal error");
	edge.setConstrained();
	if(data) setEdgeData(edge,*data);
	if(conform)
	{
		edge=conformEdge(edge);
	}
	setRoot(edge.triangle());
	return(Insertion(Insertion::SUCCESS,edge));
}
//
TRIANGULATOR_HDR
inline typename TRIANGULATOR_TYP::Insertion				TRIANGULATOR_TYP::insertEdge(const Vertex& v0,const Vertex& v1,bool conform,const EdgeData* data)
{
	return(insertEdge(v0.m_x,v0.m_y,v1.m_x,v1.m_y,conform,data));
}
//	
TRIANGULATOR_HDR
inline typename TRIANGULATOR_TYP::Insertion				TRIANGULATOR_TYP::insertEdge(int x0,int y0,int x1,int y1,bool conform,const EdgeData& data)
{
	return(insertEdge(x0,y0,x1,y1,conform,&data));
}
//
TRIANGULATOR_HDR
inline typename TRIANGULATOR_TYP::Insertion				TRIANGULATOR_TYP::insertEdge(const Vertex& v0,const Vertex& v1,bool conform,const EdgeData& data)
{
	return(insertEdge(v0,v1,conform,&data));
}
//	
TRIANGULATOR_HDR
template <typename IEDGECROSSING>
HK_ON_CPU(inline) typename TRIANGULATOR_TYP::Insertion	TRIANGULATOR_TYP::insertCrossingEdge(int x0,int y0,int x1,int y1,bool conform, const IEDGECROSSING& icrossing, const EdgeData* data)
{
#define FIRE_CANNOT_RESOLVE_CROSSING_EDGE_ERROR { HK_ERROR(0x9CABCB60,"Cannot insert crossing edge"); }
	for(;;)
	{
		Insertion	ins=insertEdge(x0,y0,x1,y1,conform,data);
		Edge		edge=ins.m_edge;
		switch(ins.m_type)
		{
			// Existing edge
		case	Insertion::ALREADY_PRESENT:
			if(data)
			{
				setEdgeData(ins.m_edge,*data);
			}
			break;
			// Crossing edge
		case	Insertion::CROSSING_CONSTRAINED_EDGE:
			{
				Vertex*	a		=	edge.start();
				Vertex*	b		=	edge.end();
				icrossing.notifyEdgeCrossing(edge);
				const hkInt32	x2		=	a->m_x;
				const hkInt32	y2		=	a->m_y;
				const hkInt32	x3		=	b->m_x;
				const hkInt32	y3		=	b->m_y;
				const tExtInt	num		=	x3*(tExtInt)(y0-y2)+x0*(tExtInt)(y2-y3)+x2*(tExtInt)(y3-y0);
				const tExtInt	den		=	-(x2-x3)*(tExtInt)(y0-y1)+(x0-x1)*(tExtInt)(y2-y3);
				const hkReal	sol		=	num/(hkReal)den;
				const hkReal	ifx		=	x0+(x1-x0)*sol;
				const hkReal	ify		=	y0+(y1-y0)*sol;
				hkInt32			ix		=	(hkInt32)ifx;
				hkInt32			iy		=	(hkInt32)ify;
				hkBool32		split	=	canSplitEdge(ix,iy,edge);

				/* Try finding an insertion point around ix,iy			*/ 
				if(!split)
				{
					/* Search a valid insertion point using concentric circles	*/ 
					for(int r=1;!split && r<cfgSearchRadius;++r)
					{
						hkInt32 x(r),y(0);
						hkInt32 dx(1-2*r),dy(1),err(-1);
						while (x >= y)
						{
							hkInt32 kx,ky;
							#define CHECK_CAN_SPLIT() split=canSplitEdge(kx,ky,edge); if(split) { ix=kx;iy=ky;break; }
							kx=ix+x;ky=iy+y; CHECK_CAN_SPLIT()
							kx=ix-x;ky=iy+y; CHECK_CAN_SPLIT()
							kx=ix-x;ky=iy-y; CHECK_CAN_SPLIT()
							kx=ix+x;ky=iy-y; CHECK_CAN_SPLIT()
							kx=ix+y;ky=iy+x; CHECK_CAN_SPLIT()
							kx=ix-y;ky=iy+x; CHECK_CAN_SPLIT()
							kx=ix-y;ky=iy-x; CHECK_CAN_SPLIT()
							kx=ix+y;ky=iy-x; CHECK_CAN_SPLIT()
							#undef CHECK_CAN_SPLIT
							y+=1;err+=dy;dy+=2;
							if(2*err+dx > 0) { x-=1;err+=dx;dx+=2; }
						}
					}
				}

				if(!split)
				{
					/* Try finding an insertion point along the edge			*/ 
					hkInt32		dx=x2-x3;dx=dx<0?-dx:dx;
					hkInt32		dy=y2-y3;dy=dy<0?-dy:dy;
					hkInt32		absDx=dx<0?-dx:dx;
					hkInt32		absDy=dy<0?-dy:dy;
					hkVector4	endPoints[2];
					hkVector4	isecPoint;
					hkReal		nearest=HK_REAL_MAX;
					endPoints[0].set((hkReal)x2,(hkReal)y2,0,0);
					endPoints[1].set((hkReal)x3,(hkReal)y3,0,0);
					isecPoint.set(ifx,ify,0,0);
					for(int i=1,ni=hkMath::min2((hkInt32)cfgSearchSteps,(hkInt32)(4+hkMath::max2(absDx,absDy)*8));i<ni;++i)
					{
						const hkReal fraction(i/(hkReal)ni);
						for(int s=0;s<2;++s)
						{
							hkVector4	isec;
							isec.setInterpolate4(isecPoint,endPoints[s],hkSimdReal(fraction));
							hkReal		sqd=isec.distanceToSquared3(isecPoint).getReal();
							if(!split || sqd<nearest)
							{
								hkInt32		tx,ty;
								tx		=	(hkInt32)(isec(0)+0.5f);
								ty		=	(hkInt32)(isec(1)+0.5f);
								if((tx==x2 && ty==y2) || (tx==x3 && ty==y3)) continue;
								split=canSplitEdge(tx,ty,edge);
								if( split )
								{
									nearest	=	sqd;
									ix		=	tx;
									iy		=	ty;
								}
							}
						}
						if(split) break;
					}
				}				
				
				/* Insert at ix,iy if found an insertion point				*/ 
				if(split)
				{
					const hkReal	dx((hkReal)(x3-x2));
					const hkReal	dy((hkReal)(y3-y2));
					hkReal			u(((ix-x2)*dx+(iy-y2)*dy)/(dx*dx+dy*dy));
					if(u<=0)	u=HK_REAL_EPSILON;
					if(u>=1)	u=1-HK_REAL_EPSILON;
					Location		location;
					location.m_edge	=	edge;
					location.m_type	=	Location::ON_EDGE;
					ins				=	insertVertex(ix,iy,false,location,true);
					if(ins.m_type == Insertion::OUT_OF_MEMORY)
						return(ins);
					if(ins.m_type!=Insertion::SUCCESS) FIRE_CANNOT_RESOLVE_CROSSING_EDGE_ERROR
						Edge			insertedEdge;
					icrossing.notifyNewVertex(a,b,u,sol,ins.m_edge.start());
					ins				=	insertCrossingEdge(x0,y0,ix,iy,conform,icrossing,data);
					if(ins.m_type == Insertion::OUT_OF_MEMORY)
						return(ins);
					if(ins.m_type!=Insertion::ALREADY_PRESENT && ins.m_type!=Insertion::SUCCESS)
					{
						FIRE_CANNOT_RESOLVE_CROSSING_EDGE_ERROR
					}
					x0=ix;y0=iy;
				}
				/* Else, snap to the nearest edge's vertex					*/ 
				else
				{
					const tExtInt	o0=orientation(x0,y0,x2,y2,x1,y1);
					const tExtInt	o1=orientation(x1,y1,x3,y3,x0,y0);
					if(o0 < o1)
					{
						if(!checkOrientation(x0,y0,x2,y2)) FIRE_CANNOT_RESOLVE_CROSSING_EDGE_ERROR
							ins=insertCrossingEdge(x0,y0,x2,y2,conform,icrossing,data);
						x0=x2;y0=y2;
					}
					else
					{
						if(!checkOrientation(x0,y0,x3,y3)) FIRE_CANNOT_RESOLVE_CROSSING_EDGE_ERROR
							ins=insertCrossingEdge(x0,y0,x3,y3,conform,icrossing,data);
						x0=x3;y0=y3;
					}
					if(ins.m_type == Insertion::OUT_OF_MEMORY)
						return(ins);
					if(ins.m_type!=Insertion::ALREADY_PRESENT && ins.m_type!=Insertion::SUCCESS)
					{
						FIRE_CANNOT_RESOLVE_CROSSING_EDGE_ERROR
					}
				}
			}
			continue;
			// Crossing vertex
		case	Insertion::CROSSING_CONSTRAINED_VERTEX:
			{
				/* Start over with edge->start() as x0,y0	*/ 
				icrossing.notifyVertexCrossing(edge.start());
				const hkInt32	nx=ins.m_edge.start()->m_x;
				const hkInt32	ny=ins.m_edge.start()->m_y;
				HK_ASSERT2(0x4424,orientation(x0,y0,nx,ny,x1,y1)==0,"Invalid insertion result");
				ins=insertCrossingEdge(x0,y0,nx,ny,conform,icrossing,data);
				if(ins.m_type == Insertion::OUT_OF_MEMORY)
					return(ins);
				if(ins.m_type!=Insertion::ALREADY_PRESENT && ins.m_type!=Insertion::SUCCESS)
				{
					FIRE_CANNOT_RESOLVE_CROSSING_EDGE_ERROR
				}
				x0	=	nx;
				y0	=	ny;
			}
			continue;
		default:
			break;
		}
		return(ins);
	}	
#undef FIRE_CANNOT_RESOLVE_CROSSING_EDGE_ERROR
}
//
TRIANGULATOR_HDR
template <typename IEDGECROSSING>
inline typename TRIANGULATOR_TYP::Insertion				TRIANGULATOR_TYP::insertCrossingEdge(const Vertex& v0,const Vertex& v1,bool conform, const IEDGECROSSING& icrossing, const EdgeData* data)
{
	return(insertCrossingEdge(v0.m_x,v0.m_y,v1.m_x,v1.m_y,conform,icrossing,data));
}
//	
TRIANGULATOR_HDR
template <typename IEDGECROSSING>
inline typename TRIANGULATOR_TYP::Insertion				TRIANGULATOR_TYP::insertCrossingEdge(int x0,int y0,int x1,int y1,bool conform, const IEDGECROSSING& icrossing, const EdgeData& data)
{
	return(insertCrossingEdge(x0,y0,x1,y1,conform,icrossing,&data));
}
//
TRIANGULATOR_HDR
template <typename IEDGECROSSING>
inline typename TRIANGULATOR_TYP::Insertion				TRIANGULATOR_TYP::insertCrossingEdge(const Vertex& v0,const Vertex& v1,bool conform, const IEDGECROSSING& icrossing, const EdgeData& data)
{
	return(insertCrossingEdge(v0.m_x,v0.m_y,v1.m_x,v1.m_y,conform,icrossing,&data));
}

#define CHECK_ARRAY_SIZE_AND_RETURN_IF_OUT_OF_MEM(_arrayName, _returnval)

//
TRIANGULATOR_HDR
inline int												TRIANGULATOR_TYP::partition()
{	
	/* Reset regions	*/ 
	for(Triangle* t=m_mesh.m_triangles.getFirst();t;t=t->next())
	{
		t->m_region=0;--t->m_region;
	}
	const hkUint32	nullregion = m_mesh.m_triangles.getFirst()->m_region;
	/* Flood regions	*/ 
	hkInplaceArray<Edge, cfgStacksize,ALLOCATOR>	stack2;
	hkInplaceArray<Edge, cfgStacksize,ALLOCATOR>*	current=&m_stack;
	hkInplaceArray<Edge, cfgStacksize,ALLOCATOR>*	next=&stack2;
	int												region=-1;
	next->pushBack(locateVertex(0,0).m_edge.uid());
	do
	{
		hkAlgorithm::swap(next,current);
		next->clear();
		++region;
		do 
		{
			Edge		e=current->back();
			Triangle*	t=e.triangle();
			current->popBack();
			if(t->m_region==nullregion)
			{
				t->m_region=region;
				for(int i=0;i<3;++i)
				{
					Edge		ne=Edge(t,i).link();
					Triangle*	nt=ne.triangle();
					if(ne.isValid()&&(nt->m_region==nullregion))
					{
						if(ne.isConstrained())
						{
							// Should never happen. If it does it means we have miscalculated the max number of edges in our initial allocate
							CHECK_ARRAY_SIZE_AND_RETURN_IF_OUT_OF_MEM(*next, -1)
							next->pushBack(ne);
						}
						else
						{
							// Should never happen. If it does it means we have miscalculated the max number of edges in our initial allocate
							CHECK_ARRAY_SIZE_AND_RETURN_IF_OUT_OF_MEM(*current, -1)
							current->pushBack(ne);
						}
					}
				}
			}						
		} while (current->getSize()>0);
	} while(next->getSize()>0);
	return(region+1);
}
//
TRIANGULATOR_HDR
inline void												TRIANGULATOR_TYP::removeNonBoundaryConstrainedEdgeAndData()
{
	for(typename Mesh::EdgeIterator i=m_mesh.getEdgeIterator();i;++i)
	{
		Edge	edge(i());
		if(	edge.isConstrained() &&
			edge.isBound() &&
			edge.triangle()->m_region==edge.link().triangle()->m_region)
		{
			clearEdgeData(edge);
			clearEdgeData(edge.link());
			edge.clrConstrained();
		}
	}
}
//
TRIANGULATOR_HDR
inline void												TRIANGULATOR_TYP::clearTrianglesSets(tSets value)
{
	for(Triangle* t=m_mesh.m_triangles.getFirst();t;t=t->next()) t->m_sets=value;
}

//
TRIANGULATOR_HDR
template <typename FLOODPOLICY>
inline hkBool32												TRIANGULATOR_TYP::floodEdgeSets(const FLOODPOLICY& policy)
{
	/* Partition	*/ 
	int numR = partition();
	if( numR == -1 )
	{
		return false;
	}
	/* Initialize	*/ 
	hkInplaceArray<EdgeSet,cfgStacksize>	stack;
	for(Triangle* t=m_mesh.m_triangles.getFirst();t;t=t->next())
	{
		for(int i=0;i<3;++i)
		{
			Edge	e(t,i);
			if(e.isConstrained())
			{
				const EdgeData*	d(getEdgeData(e));
				if(d)
				{
					const tSets	sets=policy.getSetsFromEdgeData(d);
					if(sets!=0)
					{
						if(e.isNaked() || e.triangle()->m_region>e.link().triangle()->m_region)
						{
							for(int j=0,nj=sizeof(tSets)*8, bit = 1<<0; j<nj; ++j, bit<<=1)
							{
								if(sets&bit) 
								{
									// Should never happen. If it does it means we have miscalculated the max number of edges in our initial allocate
									CHECK_ARRAY_SIZE_AND_RETURN_IF_OUT_OF_MEM(stack, false)
									stack.pushBack(EdgeSet(e,(tSets)bit));
								}
							}
						}
					}					
				}
			}
		}
	}
	/* Flood		*/ 
	while(stack.getSize())
	{
		EdgeSet	es=stack.back();stack.popBack();
		if(!(es.m_edge.triangle()->m_sets&es.m_set))
		{
			es.m_edge.triangle()->m_sets |= es.m_set;
			for(int i=0;i<2;++i)
			{
				es.m_edge=es.m_edge.next();
				if(es.m_edge.isBound())
				{
					hkBool32	allowCrossing=!es.m_edge.isConstrained();
					if(!allowCrossing)
					{
						const EdgeData*	ed=getEdgeData(es.m_edge);
						if(!ed || !(policy.getSetsFromEdgeData(ed)&es.m_set))
						{
							if(es.m_edge.triangle()->m_region<es.m_edge.link().triangle()->m_region)
							{
								allowCrossing=true;
							}
						}
					}
					if(allowCrossing)
					{
						// Should never happen. If it does it means we have miscalculated the max number of edges in our initial allocate
						CHECK_ARRAY_SIZE_AND_RETURN_IF_OUT_OF_MEM(stack, false)
						stack.pushBack(EdgeSet(es.m_edge.link(),es.m_set));
					}
				}
			}
		}
	}
	return true;
}
//
TRIANGULATOR_HDR
inline void												TRIANGULATOR_TYP::conformEdges()
{
	int			found;
	unsigned	maxPasses = 1+m_mesh.m_triangles.getSize()*3;
	do
	{
		found=0;
		for(Triangle* t=m_mesh.m_triangles.getFirst();t;t=t->next())
		{
			for(int i=0;i<3;++i)
			{
				Edge	e(t,i);
				if(!isConform(e) && e.canFlip())
				{
					e=flipEdge(e);
					++found;
				}
			}
		}
	} while(found && (--maxPasses)>0);
	if(maxPasses==0)
	{
		HK_WARN_ALWAYS(0x0D26E67D,"Infinite cycle detected during triangulation");
	}
}
//
TRIANGULATOR_HDR
inline void												TRIANGULATOR_TYP::minimizeEdgeLengths()
{
	int	found;
	do
	{
		found=0;
		for(Triangle* t=m_mesh.m_triangles.getFirst();t;t=t->next())
		{
			for(int i=0;i<3;++i)
			{
				Edge	e(t,i);
				if(e.canFlip())
				{
					if(	distanceSqrd(e.start(),e.end())>
						distanceSqrd(e.apex(),e.link().apex()))
					{
						e=flipEdge(e);
						++found;
					}
				}
			}
		}
	} while(found);
}

//
TRIANGULATOR_HDR
inline int												TRIANGULATOR_TYP::decompose(hkArray<hkArray<Edge,ALLOCATOR>,ALLOCATOR>& pieces)
{
	hkArray<Triangle*>		triangles;
	triangles.reserve(m_mesh.m_triangles.getSize());
	for(Triangle* t=m_mesh.m_triangles.getFirst();t;t=t->next())
	{
		triangles.pushBackUnchecked(t);
	}
	hkAlgorithm::quickSort(&triangles[0],triangles.getSize(),SortByArea_Predicate());

	clearTrianglesMarker();
	for(int triangleIndex=0;triangleIndex<triangles.getSize();++triangleIndex)
	{
		Triangle*	t=triangles[triangleIndex];
		if(!t->m_marker)
		{
			const hkUint32	region=t->m_region;
			hkArray<Edge,ALLOCATOR>&	edges=pieces.expandOne();
			edges.pushBack(Edge(t,0));
			edges.pushBack(Edge(t,1));
			edges.pushBack(Edge(t,2));
			t->m_marker=1;
			for(;;)
			{
				int		bestEdge=-1;
				tExtInt	bestLength=0;
				for(int i=0,ni=edges.getSize();i<ni;++i)
				{
					Edge		l=edges[i].link();
					Triangle*	lt=l.triangle();
					if(l.isValid() && lt->m_region==region && !lt->m_marker)
					{
						const tExtInt	dst=distanceSqrd(l.start(),l.end());
						if(bestEdge==-1 || dst>bestLength)
						{
							Edge	p=edges[(i+ni-1)%ni];
							Edge	n=edges[(i+1)%ni];
							if(	(orientation(p,l.apex())>=0) &&
								(orientation(n,l.apex())>=0))
							{
								bestEdge	=	i;
								bestLength	=	dst;
							}
						}
					}
				}
				if(bestEdge>=0)
				{
					Edge	l=edges[bestEdge].link();
					edges[bestEdge]=l.prev();
					edges.insertAt(bestEdge,l.next());
					l.triangle()->m_marker=1;
				} else break;
			}
		}
	}
	return(pieces.getSize());
}

//
TRIANGULATOR_HDR
inline int												TRIANGULATOR_TYP::decomposeFast(hkArray<hkArray<Edge,ALLOCATOR>,ALLOCATOR>& pieces)
{
	clearTrianglesMarker();
	for(Triangle* t=m_mesh.m_triangles.getFirst();t;t=t->next())
	{
		if(!t->m_marker)
		{
			const hkUint32				region=t->m_region;
			hkArray<Edge,ALLOCATOR>&	edges=pieces.expandOne();
			int							changed=1;
			{
				Edge* edgePtr = edges.expandBy(3);
				new (edgePtr+0) Edge(t,0);
				new (edgePtr+1) Edge(t,1);
				new (edgePtr+2) Edge(t,2);
			}
			t->m_marker=1;
			do 
			{
				changed=0;
				for(int i=0,ni=edges.getSize();i<ni;++i)
				{
					Edge		l=edges[i].link();
					Triangle*	lt=l.triangle();
					if(l.isValid() && lt->m_region==region && !lt->m_marker)
					{						
						Edge	p=edges[(i+ni-1)%ni];
						Edge	n=edges[(i+1)%ni];
						if(	(orientation(p,l.apex())>=0) &&
							(orientation(n,l.apex())>=0))
						{
							++ni;
							++changed;
							edges[i]=l.prev();
							edges.insertAt(i--,l.next());
							lt->m_marker=1;
						}
					}
				}
			} while (changed);
		}
	}
	return(pieces.getSize());
}

//
TRIANGULATOR_HDR
inline int												TRIANGULATOR_TYP::decomposeFastFixedSize(hkArrayBase<hkUint32>& pieceSize, void* _storage, int capacity, int& numEdgesOut, int& maxEdgesPerPieceOut)
{
	maxEdgesPerPieceOut = -1;
	HK_ASSERT(0x6351f3e2, pieceSize.isEmpty());
	Edge* edgeStorage = static_cast<Edge*>(_storage);
	numEdgesOut = 0;

	clearTrianglesMarker();
	for(Triangle* t=m_mesh.m_triangles.getFirst();t;t=t->next())
	{
		if(!t->m_marker && (t->m_region&1 ) )
		{
			const hkUint32				region=t->m_region;
			// Current piece that we're building
			Edge* edges = edgeStorage;
			int numEdges = 0;
			int							changed=1;
			{
				new (edges+0) Edge(t,0);
				new (edges+1) Edge(t,1);
				new (edges+2) Edge(t,2);
				edgeStorage+=3;
				numEdges+=3;
			}
			t->m_marker=1;
			do 
			{
				changed=0;
				for(int i=0,ni=numEdges;i<ni;++i)
				{
					Edge		l=edges[i].link();
					Triangle*	lt=l.triangle();
					if(l.isValid() && lt->m_region==region && !lt->m_marker)
					{						
						Edge	p=edges[(i+ni-1)%ni];
						Edge	n=edges[(i+1)%ni];
						if(	(orientation(p,l.apex())>=0) &&
							(orientation(n,l.apex())>=0))
						{
							++ni;
							++changed;
							edges[i]=l.prev();

							// Cleaner to create an array then duplicate insertAt
							hkArray<Edge, ALLOCATOR> tempEdges(edges, numEdges, numEdges+1);
							tempEdges.insertAt(i--,l.next());
							
							edgeStorage++;
							numEdges++;
							lt->m_marker=1;
						}
					}
				}
			} while (changed);

			pieceSize.pushBackUnchecked(numEdges);
			numEdgesOut += numEdges;
			maxEdgesPerPieceOut = hkMath::max2(maxEdgesPerPieceOut, numEdges);
		}
	}

	HK_ASSERT( 0x215d8b15, edgeStorage == static_cast<Edge*>(_storage) + numEdgesOut);
	HK_ASSERT(0x5c6ac9a0, numEdgesOut <= capacity);
	return(pieceSize.getSize());
}

//
TRIANGULATOR_HDR
HKGP_INLINE void										TRIANGULATOR_TYP::flushConform()
{
	unsigned	maxPasses = 1+m_mesh.m_triangles.getSize()*3;
	do
	{
		Edge edge=m_stack.back();
		m_stack.popBack();
		if(!isConform(edge))
		{
			if(edge.canFlip())
			{
				edge=flipEdge(edge);
				m_stack.pushBack(edge.next().uid());
				m_stack.pushBack(edge.nextCw().uid());
			}
		}
	} while(m_stack.getSize()>0 && (--maxPasses)>0);
	if(maxPasses==0)
	{
		HK_WARN_ALWAYS(0x0D26E67E,"Infinite cycle detected during triangulation");
	}
}

//
// EdgeDataPolicy
//

//
template <typename EDGEDATA>
template <typename TRIANGULATOR, typename EDGE>
inline void	hkgpTriangulatorBase::EdgeDataPolicy<EDGEDATA>::backupBothEdgeData(TRIANGULATOR& triangulator, EDGE edge, EdgeDataState& state)
{
	state.m_values[0].m_hasData	=	false;
	state.m_values[1].m_hasData	=	false;
	for(int i=0;i<2;++i)
	{
		const EDGEDATA*	data=triangulator.getEdgeData(edge);
		if(data)
		{			
			state.m_values[i].m_data	=	*data;
			state.m_values[i].m_hasData	=	true;
			triangulator.clearEdgeData(edge);
		}
		if(edge.isBound()) edge=edge.link(); else break;
	}
}

//
template <typename EDGEDATA>
template <typename TRIANGULATOR, typename EDGE>
inline void	hkgpTriangulatorBase::EdgeDataPolicy<EDGEDATA>::restoreBothEdgeData(TRIANGULATOR& triangulator, EDGE edge, const EdgeDataState& state)
{
	for(int i=0;i<2;++i)
	{
		if(state.m_values[i].m_hasData) triangulator.setEdgeData(edge,state.m_values[i].m_data);
		if(edge.isBound()) edge=edge.link(); else break;
	}
}

//
// SparseEdgeDataPolicy
//

//
template <typename EDGEDATA,typename ALLOCATOR>
hkgpTriangulatorBase::SparseEdgeDataPolicy<EDGEDATA,ALLOCATOR>::~SparseEdgeDataPolicy()
{
	if(m_edgeData)
	{
		ALLOCATOR			allocatorType;
		hkMemoryAllocator&	allocator(allocatorType.get(0));
		m_edgeData->~EdgeDataHashTable();
		allocator.blockFree(m_edgeData,sizeof(EdgeDataHashTable));
		m_edgeData = HK_NULL;
	}
}

//
template <typename EDGEDATA,typename ALLOCATOR>
inline void	hkgpTriangulatorBase::SparseEdgeDataPolicy<EDGEDATA,ALLOCATOR>::createEdgeDataHashTable()
{
	ALLOCATOR			allocatorType;
	hkMemoryAllocator&	allocator(allocatorType.get(0));
	m_edgeData = new(allocator.blockAlloc(sizeof(EdgeDataHashTable))) EdgeDataHashTable(hkGeometryProcessing::NEXT_PRIME_AFTER_1024);
}

//
template <typename EDGEDATA,typename ALLOCATOR>
template <typename EDGE>
inline const EDGEDATA*	hkgpTriangulatorBase::SparseEdgeDataPolicy<EDGEDATA,ALLOCATOR>::getEdgeData(const EDGE& edge) const
{
	if(edge.isValid() && m_edgeData)
	{
		EdgeDataBox box(edge);
		const EdgeDataBox*	item=m_edgeData->find(box);
		return item? item->m_data : HK_NULL;
	}
	return HK_NULL;
}

//
template <typename EDGEDATA,typename ALLOCATOR>
template <typename EDGE>
inline void				hkgpTriangulatorBase::SparseEdgeDataPolicy<EDGEDATA,ALLOCATOR>::setEdgeData(const EDGE& edge, const EDGEDATA& data)
{
	if(!m_edgeData) createEdgeDataHashTable();

	EdgeDataBox box(edge);
	EdgeDataBox*	edgeData=m_edgeData->find(box);

	if(!edgeData)
	{
		edgeData=m_edgeData->insert(box);
		edgeData->m_data=m_edgeDataAllocator.allocate();
	}
	edgeData->m_data->appendOrReplace(data);
}

//
template <typename EDGEDATA,typename ALLOCATOR>
template <typename EDGE>
inline void				hkgpTriangulatorBase::SparseEdgeDataPolicy<EDGEDATA,ALLOCATOR>::clearEdgeData(const EDGE& edge)
{
	if(m_edgeData)
	{
		EdgeDataBox box(edge);
		EdgeDataBox*	edgeData=m_edgeData->find(box);
		if(edgeData)
		{
			m_edgeDataAllocator.release(edgeData->m_data);
			m_edgeData->remove(box);
		}
	}
}

//
// DenseEdgeDataPolicy
//

//
template <typename EDGEDATA,typename ALLOCATOR>
template <typename EDGE>
inline const EDGEDATA*	hkgpTriangulatorBase::DenseEdgeDataPolicy<EDGEDATA,ALLOCATOR>::getEdgeData(const EDGE& edge) const
{
	EdgeDataValue&	edgeData = edge.triangle()->m_edgeData[edge.index()];
	if(edge.isValid() && edgeData.m_hasData) return &edgeData.m_data;
	return HK_NULL;
}

//
template <typename EDGEDATA,typename ALLOCATOR>
template <typename EDGE>
inline void				hkgpTriangulatorBase::DenseEdgeDataPolicy<EDGEDATA,ALLOCATOR>::setEdgeData(const EDGE& edge, const EDGEDATA& data)
{
	EdgeDataValue&	edgeData = edge.triangle()->m_edgeData[edge.index()];
	if(!edgeData.m_hasData)
	{
		edgeData.m_data = EDGEDATA();
		edgeData.m_hasData = true;
	}
	edgeData.m_data.appendOrReplace(data);
}

//
template <typename EDGEDATA,typename ALLOCATOR>
template <typename EDGE>
inline void				hkgpTriangulatorBase::DenseEdgeDataPolicy<EDGEDATA,ALLOCATOR>::clearEdgeData(const EDGE& edge)
{
	edge.triangle()->m_edgeData[edge.index()].m_hasData = false;
}

//
template <typename EDGEDATA,typename ALLOCATOR>
template <typename EDGE>
inline EDGE				hkgpTriangulatorBase::DenseEdgeDataPolicy<EDGEDATA,ALLOCATOR>::flipEdge(const EDGE& edgeArg)
{
	EDGE			edge=edgeArg;
	EDGE			link=edge.link();
	EdgeDataValue	data[4]={	getEdgeDataRef(edge.next()),getEdgeDataRef(edge.prev()),
		getEdgeDataRef(link.next()),getEdgeDataRef(link.prev())};

	edge = edge.flip();
	getEdgeDataRef(edge.next()) =	data[3];
	getEdgeDataRef(edge.prev()) =	data[0];
	getEdgeDataRef(edge)		=	EdgeDataValue();

	link = edge.link();
	getEdgeDataRef(link.next()) =	data[1];
	getEdgeDataRef(link.prev()) =	data[2];
	getEdgeDataRef(link)		=	EdgeDataValue();

	return edge;
}

//
template <typename EDGEDATA,typename ALLOCATOR>
template <typename EDGE,typename TRIANGLE>
inline void				hkgpTriangulatorBase::DenseEdgeDataPolicy<EDGEDATA,ALLOCATOR>::propagateDataOnSplitTriangle(const EDGE& edge,TRIANGLE* t0,TRIANGLE* t1)
{
	EdgeDataValue&	data0	=	getEdgeDataRef(edge.next());
	EdgeDataValue&	data1	=	getEdgeDataRef(edge.prev());
	t0->m_edgeData[0]	=	data0;
	t1->m_edgeData[0]	=	data1;
	data0.m_hasData		=	false;
	data1.m_hasData		=	false;
}

//
template <typename EDGEDATA,typename ALLOCATOR>
template <typename EDGE,typename TRIANGLE>
inline void				hkgpTriangulatorBase::DenseEdgeDataPolicy<EDGEDATA,ALLOCATOR>::propagateDataOnSplitBoundEdge(const EDGE& edge,const EDGE& link,TRIANGLE* t0,TRIANGLE* t1)
{
	EdgeDataValue&	data0	=	getEdgeDataRef(edge.prev());
	EdgeDataValue&	data1	=	getEdgeDataRef(link.prev());
	t0->m_edgeData[0]	=	data0;
	data0.m_hasData		=	false;
	t1->m_edgeData[0]	=	data1;
	data1.m_hasData		=	false;
}

//
template <typename EDGEDATA,typename ALLOCATOR>
template <typename EDGE,typename TRIANGLE>
inline void				hkgpTriangulatorBase::DenseEdgeDataPolicy<EDGEDATA,ALLOCATOR>::propagateDataOnSplitNakedEdge(const EDGE& edge,TRIANGLE* t0)
{
	EdgeDataValue&	data	=	getEdgeDataRef(edge.prev());
	t0->m_edgeData[0]	=	data;
	data.m_hasData		=	false;
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
