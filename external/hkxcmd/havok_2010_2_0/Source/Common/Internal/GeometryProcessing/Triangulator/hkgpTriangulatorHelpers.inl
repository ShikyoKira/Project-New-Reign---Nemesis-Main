/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

//
TRIANGULATOR_HDR
inline bool						TRIANGULATOR_TYP::check() const
{
	for(Triangle* t=m_mesh.m_triangles.getFirst();t;t=t->next())
	{		
		Vertex*		tv[]={t->vertex(0),t->vertex(1),t->vertex(2)};
		const int	o=orientation(tv[0],tv[1],tv[2]);
		HK_ASSERT2(0x54567,o>0,"Invalid triangle orientation");
		if(o<=0) return(false);
		for(int i=0;i<3;++i)
		{
			const Edge	e(t,i);
			const Edge	l(e.link());
			if(l.isValid())
			{
				const bool mt=(e.start()==l.end())&&(e.end()==l.start());
				HK_ASSERT2(0x53547,mt,"Unmatched edge binding");
				if(!mt) return(false);
			}
			else
			{
				const bool sb=sameBoundary(e.start(),e.end());
				HK_ASSERT2(0x54547,sb,"Naked edge inside triangulation");
				if(!sb) return(false);
			}
			const EdgeData*	edgeData=getEdgeData(e);
			HK_ASSERT2(0xD5822F39,!edgeData || e.isConstrained(),"Unconstrained edges cannot have data");
		}
		for(Vertex* v=m_mesh.m_vertices.getFirst();v;v=v->next())
		{
			if((v!=tv[0])&&(v!=tv[1])&&(v!=tv[2]))
			{
				if(	(orientation(tv[0],tv[1],v)>=0)&&
					(orientation(tv[1],tv[2],v)>=0)&&
					(orientation(tv[2],tv[0],v)>=0))
				{
					HK_ASSERT2(0x73547,false,"Vertex inside triangle");
					return(false);
				}
			}
		}
	}
	return(true);
}

//
#if defined(HKGP_TRIANGULATOR_ENABLE_DRAW)
#include <Common/Base/Types/Color/hkColor.h>
#include <Common/Visualize/hkDebugDisplay.h>
TRIANGULATOR_HDR
inline void						TRIANGULATOR_TYP::draw(const IDrawing* idrawing) const
{
	static const IDrawing	idefault;
	if(!idrawing) idrawing=&idefault;
	if(m_mesh.m_triangles.getSize()>0)
	{		
		hkVector4	minBounds=transform(m_mesh.m_vertices.getFirst());
		hkVector4	maxBounds=minBounds;
		for(Vertex* v=m_mesh.m_vertices.getFirst();v;v=v->next())
		{
			const hkVector4	x=transform(v);
			minBounds.setMin4(minBounds,x);
			maxBounds.setMax4(maxBounds,x);
		}
		hkVector4	center;
		center.setAdd4(minBounds,maxBounds);
		center.mul4(0.5f);
		hkVector4	axis;
		hkVector4	up;
		axis.set(0,0,1,0);
		m_localToWorld.transformDirection(axis,up);
		axis.set(0,0,1,0);
		hkQuaternion	rot;
		rot.setShortestRotation(up,axis);
		hkArray<hkVector4>			positions;
		hkPointerMap<Vertex*,int>	indices;
		positions.reserve(m_mesh.m_vertices.getSize());
		indices.reserve(m_mesh.m_vertices.getSize());
		for(Vertex* v=m_mesh.m_vertices.getFirst();v;v=v->next())
		{
			indices.insert(v,positions.getSize());
			hkVector4&	pos=positions.expandOne();
			pos=transform(v);
			pos.sub4(center);
			pos.setRotatedDir(rot,pos);
			hkStringBuf name;
			idrawing->getVertexString(*this,v,name);
			if(name.getLength())
			{
				HK_DISPLAY_3D_TEXT(name.cString(),pos,hkColor::WHITE);
			}
		}
		for(Triangle* t=m_mesh.m_triangles.getFirst();t;t=t->next())
		{		
			for(int i=0;i<3;++i)
			{
				const Edge	e(t,i);
				if(e.isMaster())
				{
					hkColor::ExtendedColors	color=(hkColor::ExtendedColors)hkColor::rgbFromFloats(0.25f,0.25f,0.25f,1);
					if(e.isNaked())			color=hkColor::DARKORANGE;
					if(e.isConstrained())	color=hkColor::WHITE;
					const hkVector4&		a(positions[indices.getWithDefault(e.start(),-1)]);
					const hkVector4&		b(positions[indices.getWithDefault(e.end(),-1)]);
					HK_DISPLAY_LINE(a,b,color);
					hkStringBuf				name;
					idrawing->getEdgeString(*this,e,name);
					if(name.getLength())
					{
						hkVector4	middle;
						middle.setInterpolate4(a,b,0.5f);
						HK_DISPLAY_3D_TEXT(name.cString(),middle,hkColor::LIGHTGREEN);
					}
				}
				hkStringBuf				name;
				idrawing->getHalfEdgeString(*this,e,name);
				if(name.getLength())
				{
					const hkVector4&		a(positions[indices.getWithDefault(e.start(),-1)]);
					const hkVector4&		b(positions[indices.getWithDefault(e.end(),-1)]);
					const hkVector4&		c(positions[indices.getWithDefault(e.apex(),-1)]);
					hkVector4	middle;
					middle.setInterpolate4(a,b,0.5f);
					middle.setInterpolate4(middle,c,0.2f);
					HK_DISPLAY_3D_TEXT(name.cString(),middle,hkColor::CYAN);
				}
			}
			if(t->m_region&1)
			{
				hkVector4		p[]={positions[indices.getWithDefault(t->vertex(0),-1)],positions[indices.getWithDefault(t->vertex(1),-1)],positions[indices.getWithDefault(t->vertex(2),-1)]};
				const hkReal	o=0.00001f;
				p[0](2)-=o;
				p[1](2)-=o;
				p[2](2)-=o;
				if(cfgIsCCW)
					HK_DISPLAY_TRIANGLE(p[0],p[2],p[1],hkColor::DARKBLUE);
				else
					HK_DISPLAY_TRIANGLE(p[0],p[1],p[2],hkColor::DARKBLUE);
			}
			hkStringBuf	name;
			idrawing->getTriangleString(*this,t,name);
			if(name.getLength())
			{
				const hkVector4&	a(positions[indices.getWithDefault(t->vertex(0),-1)]);
				const hkVector4&	b(positions[indices.getWithDefault(t->vertex(1),-1)]);
				const hkVector4&	c(positions[indices.getWithDefault(t->vertex(2),-1)]);
				hkVector4			ctr;
				ctr.setAdd4(a,b);ctr.add4(c);ctr.mul4(1/3.0f);
				HK_DISPLAY_3D_TEXT(name.cString(),ctr,hkColor::YELLOW);
			}
		}	
	}	
}
#endif

//
#if defined(HKGP_TRIANGULATOR_ENABLE_READ_WRITE)
#include <Common/Base/System/Io/IArchive/hkIArchive.h>
#include <Common/Base/System/Io/OArchive/hkOArchive.h>

//
template <typename ALLOCATOR>
inline void						hkgpTriangulatorBase::DefaultEdgeData<ALLOCATOR>::read(class hkIArchive& file)
{
	m_tags.clear();
	const int	count=(int)file.read32();
	for(int i=0;i<count;++i) m_tags.pushBack((hkUlong)file.read64u());
}

//
template <typename ALLOCATOR>
inline void						hkgpTriangulatorBase::DefaultEdgeData<ALLOCATOR>::write(class hkOArchive& file) const
{
	file.write32((hkInt32)m_tags.getSize());
	for(int i=0;i<m_tags.getSize();++i)
	{
		file.write64u((hkUint64)m_tags[i]);
	}
}


//
TRIANGULATOR_HDR
inline bool						TRIANGULATOR_TYP::read(const char* filename)
{
	hkIArchive	file(filename);
	if(file.isOk())
	{
		reset(false);
		file.readRaw(&m_localToWorld,sizeof(m_localToWorld));
		file.readRaw(&m_worldToLocal,sizeof(m_worldToLocal));
		m_margin=file.read32();
		if(file.read32()!=cfgOrientation)
		{
			HK_ERROR(0x980652E6,"Incompatible orientation");
		}

		hkPointerMap<int,Vertex*>	v_indices;
		for(int i=0,ni=file.read32();i<ni;++i)
		{
			hkInt32	x=file.read32();
			hkInt32 y=file.read32();
			hkInt32	c=file.read32();
			Vertex*	vertex=newVertex(x,y,c);
			v_indices.insert(i,vertex);
		}

		hkPointerMap<int,Triangle*>	t_indices;
		for(int i=0,ni=file.read32();i<ni;++i)
		{
			Vertex*		vertices[3]={0,0,0};
			for(int j=0;j<3;++j)
			{
				vertices[j]=v_indices.getWithDefault(file.read32(),0);
			}
			Triangle*	triangle=newTriangle(vertices[0],vertices[1],vertices[2]);
			t_indices.insert(i,triangle);
			for(int j=0;j<3;++j)
			{
				Triangle*	bound=t_indices.getWithDefault(file.read32(),0);
				const int	index=file.read32();
				if(bound) Edge(triangle,j).bind(Edge(bound,index));
			}
			triangle->m_constraints	=	file.read32u();
			triangle->m_region		=	file.read32u();
			triangle->m_sets		=	(tSets)file.read32u();
			/*for(int j=0;j<3;++j)
			{
				if(file.read8u()==1)
				{
					EdgeData	edgeData;
					edgeData.read(file);
					setEdgeData(Edge(triangle,j),edgeData);
				}
			}*/
		}
		return(true);
	}
	return(false);
}

//
TRIANGULATOR_HDR
inline bool						TRIANGULATOR_TYP::write(const char* filename)
{
	hkOArchive	file(filename);
	if(file.isOk())
	{		
		file.writeRaw(&m_localToWorld,sizeof(m_localToWorld));
		file.writeRaw(&m_worldToLocal,sizeof(m_worldToLocal));
		file.write32((hkInt32)m_margin);
		file.write32((hkInt32)cfgOrientation);

		hkPointerMap<Vertex*,int>	v_indices;
		file.write32((hkInt32)m_mesh.m_vertices.getSize());	
		for(Vertex* v=m_mesh.m_vertices.getFirst();v;v=v->next())
		{
			v_indices.insert(v,v_indices.getSize());
			file.write32(v->m_x);
			file.write32(v->m_y);
			file.write32(v->m_constraint);
		}

		hkPointerMap<Triangle*,int>	t_indices;
		file.write32((hkInt32)m_mesh.m_triangles.getSize());
		for(Triangle* t=m_mesh.m_triangles.getFirst();t;t=t->next())
		{
			t_indices.insert(t,t_indices.getSize());
			for(int i=0;i<3;++i)
			{
				file.write32((hkInt32)v_indices.getWithDefault(t->vertex(i),-1));				
			}
			for(int i=0;i<3;++i)
			{
				Edge	edge(t,i);
				Edge	link(edge.link());
				file.write32((hkInt32)t_indices.getWithDefault(link.triangle(),-1));
				file.write32((hkInt32)link.index());
			}
			file.write32u(t->m_constraints);
			file.write32u(t->m_region);
			file.write32u(t->m_sets);
			/*for(int i=0;i<3;++i)
			{
				const EdgeData*	edgeData=getEdgeData(Edge(t,i));
				if(edgeData)
				{ 
					HK_ASSERT2(0x6A254763,Edge(t,i).isConstrained(),"Unconstrained edge have data");
					file.write8u(1);edgeData->write(file); }
				else
				{ file.write8u(0); }
			}*/
		}
		return(true);
	}
	return(false);
}
#endif

//
#if defined(HKGP_TRIANGULATOR_ENABLE_BENCHMARK) && defined(_WIN32)

#include <stdio.h>
#include <stdlib.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>
/*
Size of triangulator: 1664 bytes
Size of vertex:       20 bytes
Size of triangle:     36 bytes
Roots:                16^2
Cycles:               200
V Insertions/cycle:   10000
V Insertions total:   2000000
E Insertions/cycle:   1000
E Insertions total:   50000
S Insertions/cycle:   100
S Insertions total:   20000
S Insertions steps:   128
----------------------------
>>Insert V0 :   780031 o/s (2564 ms)
>>Insert V? :   1356852 o/s (1474 ms)
>>Insert V1 :   1105583 o/s (1809 ms)
>>Insert E00 with V0 :  89127 o/s (561 ms)
>>Insert E00 with V1 :  93110 o/s (537 ms)
>>Insert E01 with V0 :  93985 o/s (532 ms)
>>Insert E01 with V1 :  92593 o/s (540 ms)
>>Insert E10 with V0 :  581401 o/s (86 ms)
>>Insert E10 with V1 :  442482 o/s (113 ms)
>>Insert E11 with V0 :  284093 o/s (176 ms)
>>Insert E11 with V1 :  438600 o/s (114 ms)
>>Insert E00 in V0 :    14753 o/s (3389 ms)
>>Insert E00 in V1 :    20284 o/s (2465 ms)
>>Insert E01 in V0 :    13808 o/s (3621 ms)
>>Insert E01 in V1 :    20161 o/s (2480 ms)
>>Insert E10 in V0 :    3936 o/s (12701 ms)
>>Insert E10 in V1 :    196080 o/s (255 ms)
>>Insert E11 in V0 :    3171 o/s (15768 ms)
>>Insert E11 in V1 :    196080 o/s (255 ms)
>>Insert circles E10 with V0 :  8692 o/s (2301 ms)
>>Insert circles E10 with V1 :  5160 o/s (3876 ms)
>>Insert circles E11 with V0 :  6826 o/s (2930 ms)
>>Insert circles E11 with V1 :  4784 o/s (4180 ms)
>>Insert circles E10 in V0 :    5333 o/s (3750 ms)
>>Insert circles E10 in V1 :    6587 o/s (3036 ms)
>>Insert circles E11 in V0 :    4715 o/s (4241 ms)
>>Insert circles E11 in V1 :    6058 o/s (3301 ms)
>>Insert cubic curves E10 with V0 :     8281 o/s (2415 ms)
>>Insert cubic curves E10 with V1 :     5514 o/s (3627 ms)
>>Insert cubic curves E11 with V0 :     6870 o/s (2911 ms)
>>Insert cubic curves E11 with V1 :     5117 o/s (3908 ms)
>>Insert cubic curves E10 in V0 :       3630 o/s (5509 ms)
>>Insert cubic curves E10 in V1 :       6447 o/s (3102 ms)
>>Insert cubic curves E11 in V0 :       5039 o/s (3969 ms)
>>Insert cubic curves E11 in V1 :       5814 o/s (3440 ms)
Conforming stack size: 128
*/
TRIANGULATOR_HDR
TRIANGULATOR_TYP*				TRIANGULATOR_TYP::benchmark()
{
	struct Utils
	{
		HK_FORCE_INLINE static Insertion	insertVertex(TRIANGULATOR_TYP* i,int x,int y,bool cf)
		{
			Insertion	ins=i->insertVertex(x,y,cf);
#if defined(HK_DEBUG)
			Edge		edge=ins.m_edge;
			HK_ASSERT2(0x754541,(!ins.isValid())||edge.start()->isEqual(x,y),"Invalid vertex insertion");
#endif
			return(ins);
		}
		HK_FORCE_INLINE static Insertion	insertEdge(TRIANGULATOR_TYP* i,const Vertex* v0,const Vertex* v1,bool cf)
		{
			Insertion	ins=i->insertEdge(v0->m_x,v0->m_y,v1->m_x,v1->m_y,cf);
#if defined(HK_DEBUG)
			Edge		edge=ins.m_edge;
			HK_ASSERT2(0x759541,(!ins.isValid())||((edge.start()==v0)&&(edge.end()==v1)),"Invalid edge insertion");
#endif
			return(ins);
		}
		HK_FORCE_INLINE static int			randomInt(hkPseudoRandomGenerator& rndgen,int margin)
		{
			return(hkMath::max2<int>(1,hkMath::min2<int>(cfgBoundary-1,(rndgen.getRand32()%(cfgBoundary+margin))-(margin/2))));
		}
		HK_FORCE_INLINE static void			insertRandom(TRIANGULATOR_TYP* instance,int count,int margin,bool conform,hkPseudoRandomGenerator& rndgen)
		{
			for(int i=0;i<nVInsertions;++i)
			{
				int x=randomInt(rndgen,margin);
				int y=randomInt(rndgen,margin);
				insertVertex(instance,x,y,conform);
			}
		}
		HK_FORCE_INLINE static hkVector4	lerp(const hkVector4& a,const hkVector4& b,hkReal t)
		{
			hkVector4	delta;
			delta.setSub4(b,a);
			delta.mul4(t);
			delta.add4(a);
			return(delta);
		}
	};
	struct WallClock
	{
		WallClock()						{ reset(); }
		HK_FORCE_INLINE void reset()
		{
			::QueryPerformanceCounter(&m_base);
		}
		HK_FORCE_INLINE void stop()
		{
			::QueryPerformanceCounter(&m_mark);
		}
		HK_FORCE_INLINE void resume()
		{
			::LARGE_INTEGER delta;
			::QueryPerformanceCounter(&delta);
			delta.QuadPart-=m_mark.QuadPart;
			m_base.QuadPart+=delta.QuadPart;
		}
		HK_FORCE_INLINE int getTimeMilliseconds() const
		{
			::LARGE_INTEGER	current,frequency;
			::QueryPerformanceCounter(&current);
			::QueryPerformanceFrequency(&frequency);
			hkInt64	b(m_base.QuadPart);
			hkInt64	c(current.QuadPart);
			hkInt64	f(frequency.QuadPart);
			return(hkMath::max2<int>(1,(int)(((c-b)*1000)/f)));
		}
		HK_FORCE_INLINE void	print(hkInt64 operations) const
		{
			const int		timems=getTimeMilliseconds();
			printf("\t%.I64u o/s (%u ms)\r\n",(operations*1000+500)/timems,timems);
		}		
		::LARGE_INTEGER	m_base;
		::LARGE_INTEGER m_mark;
	}							clock;
	TRIANGULATOR_TYP*			instance=new TRIANGULATOR_TYP();
	static const int			margin=cfgBoundary/16;
#if defined(HK_DEBUG)
	static const int			nVCycles=10;
	static const int			nECycles=5;
	static const int			nSCycles=20;
#else
	static const int			nVCycles=200;
	static const int			nECycles=50;
	static const int			nSCycles=200;
#endif
	static const int			nVInsertions=10000;
	static const int			nEInsertions=1000;
#if defined(HK_DEBUG)
	static const int			nSInsertions=10;
#else
	static const int			nSInsertions=100;
#endif
	static const int			nShapeSteps=128;
	static const bool			bEnableBenchmarks	=	true;
	static const bool			bEnableBenchark0	=	bEnableBenchmarks;
	static const bool			bEnableBenchark1	=	bEnableBenchmarks;
	static const bool			bEnableBenchark2	=	bEnableBenchmarks;
	static const bool			bEnableBenchark3	=	bEnableBenchmarks;
	static const bool			bEnableBenchark4	=	bEnableBenchmarks;
	static const bool			bEnableBenchark5	=	bEnableBenchmarks;
	static const bool			bEnableBenchark6	=	false;
	static const hkReal			extent=20;
	/* Setup											*/ 
	hkPseudoRandomGenerator		rndgen(180673);
	instance->setDomain(hkVector4(-extent,-extent,0),hkVector4(+extent,+extent,0),0,true);
#if defined(HK_DEBUG)
	Vertex		v00=instance->transform(hkVector4(-extent,-extent,0));
	Vertex		v10=instance->transform(hkVector4(+extent,-extent,0));
	Vertex		v01=instance->transform(hkVector4(-extent,+extent,0));
	Vertex		v11=instance->transform(hkVector4(+extent,+extent,0));
	hkVector4	x00=instance->transform(&v00);
	hkVector4	x10=instance->transform(&v10);
	hkVector4	x01=instance->transform(&v01);
	hkVector4	x11=instance->transform(&v11);
#endif
	/* Infos											*/ 
	printf("Size of triangulator: %u bytes\r\n",sizeof(TRIANGULATOR_TYP));
	printf("Size of vertex:       %u bytes\r\n",sizeof(TRIANGULATOR_TYP::Vertex));
	printf("Size of triangle:     %u bytes\r\n",sizeof(TRIANGULATOR_TYP::Triangle));
	printf("Roots:                %u^2\r\n",TRIANGULATOR_TYP::cfgNumRoots);
	printf("Cycles:               %u\r\n",nVCycles);
	printf("V Insertions/cycle:   %u\r\n",nVInsertions);
	printf("V Insertions total:   %u\r\n",nVCycles*nVInsertions);
	printf("E Insertions/cycle:   %u\r\n",nEInsertions);
	printf("E Insertions total:   %u\r\n",nECycles*nEInsertions);
	printf("S Insertions/cycle:   %u\r\n",nSInsertions);
	printf("S Insertions total:   %u\r\n",nSCycles*nSInsertions);
	printf("S Insertions steps:   %u\r\n",nShapeSteps);
	printf("----------------------------\r\n");
	if(bEnableBenchark0)
	{		
		printf(">>Insert V0 : ");
		rndgen.setSeed(180673);
		clock.reset();
		for(int pass=0;pass<nVCycles;++pass)
		{
			instance->reset();
			Utils::insertRandom(instance,nVInsertions,margin,false,rndgen);
		}
		clock.print(nVCycles*nVInsertions);
	}
	if(bEnableBenchark1)
	{
		printf(">>Insert V? : ");
		rndgen.setSeed(180673);
		clock.reset();
		for(int pass=0;pass<nVCycles;++pass)
		{
			instance->reset();
			for(int i=0;i<nVInsertions;++i)
			{
				int x=Utils::randomInt(rndgen,margin);
				int y=Utils::randomInt(rndgen,margin);
				Utils::insertVertex(instance,x,y,0==(i&1));
			}
		}	
		clock.print(nVCycles*nVInsertions);
	}
	if(bEnableBenchark2)
	{
		printf(">>Insert V1 : ");
		rndgen.setSeed(180673);
		clock.reset();
		for(int pass=0;pass<nVCycles;++pass)
		{
			instance->reset();
			Utils::insertRandom(instance,nVInsertions,margin,true,rndgen);
		}	
		clock.print(nVCycles*nVInsertions);
		if(!instance->isConform()) HK_ERROR(0x45457,"Unconforming triangulation");
	}
	if(bEnableBenchark3)
	{		
		rndgen.setSeed(180673);
		for(int asv=0;asv<2;++asv)
		{
			for(int ecf=0;ecf<2;++ecf)
			{
				for(int vcf=0;vcf<2;++vcf)
				{
					if(asv)
						printf(">>Insert E%u in V%u : ",ecf,vcf);
					else
						printf(">>Insert E%u with V%u : ",ecf,vcf);
					clock.reset();
					for(int pass=0;pass<nECycles;++pass)
					{
						instance->reset();
						if(asv!=0)
						{
							clock.stop();
							Utils::insertRandom(instance,nVInsertions,margin,vcf!=0,rndgen);
							clock.resume();
						}							
						for(int i=0;i<nEInsertions;++i)
						{
							int x0=Utils::randomInt(rndgen,margin);
							int y0=Utils::randomInt(rndgen,margin);
							if(Utils::insertVertex(instance,x0,y0,vcf!=0).isValid())
							{
								int x1=Utils::randomInt(rndgen,margin);
								int y1=Utils::randomInt(rndgen,margin);
								if(Utils::insertVertex(instance,x1,y1,vcf!=0).isValid())
								{
									instance->insertEdge(x0,y0,x1,y1,ecf!=0);
								}
							}
						}
					}	
					clock.print(nECycles*nEInsertions);
				}
			}
		}		
	}
	if(bEnableBenchark4)
	{
		hkArray<Vertex*> vertices;
		vertices.reserve(nShapeSteps);
		rndgen.setSeed(180673);
		for(int asv=0;asv<2;++asv)
		{
			for(int ecf=0;ecf<2;++ecf)
			{
				for(int vcf=0;vcf<2;++vcf)
				{
					if(asv)
						printf(">>Insert circles E%u in V%u : ",ecf,vcf);
					else
						printf(">>Insert circles E%u with V%u : ",ecf,vcf);					
					clock.reset();
					for(int pass=0;pass<nSCycles;++pass)
					{
						instance->reset();
						if(asv)
						{
							clock.stop();
							Utils::insertRandom(instance,nVInsertions,margin,vcf!=0,rndgen);
							clock.resume();
						}
						for(int i=0;i<nSInsertions;++i)
						{
							hkReal cx=rndgen.getRandRange(-extent,+extent);
							hkReal cy=rndgen.getRandRange(-extent,+extent);
							hkReal rd=rndgen.getRandRange(extent/100,extent/2);
							vertices.clear();
							for(int j=0;j<nShapeSteps;++j)
							{
								const hkReal	a=-(j/(hkReal)nShapeSteps)*HK_REAL_PI*2;
								hkVector4		x(cx+hkMath::cos(a)*rd,cy+hkMath::sin(a)*rd,0);
								Vertex			v=instance->transform(x);
								Insertion		ins=Utils::insertVertex(instance,v.m_x,v.m_y,vcf!=0);
								if(ins.isValid())
								{
									vertices.pushBack(ins.m_edge.start());
								}
							}				
							for(int j=0,nj=vertices.getSize();j<nj;++j)
							{
								const int	k=(j+1)%nj;
								Utils::insertEdge(instance,vertices[j],vertices[k],ecf!=0);
							}
						}
					}	
					clock.print(nSCycles*nSInsertions);
				}
			}
		}		
	}
	if(bEnableBenchark5)
	{
		hkArray<Vertex*> vertices;
		vertices.reserve(nShapeSteps);
		rndgen.setSeed(180673);
		for(int asv=0;asv<2;++asv)
		{
			for(int ecf=0;ecf<2;++ecf)
			{
				for(int vcf=0;vcf<2;++vcf)
				{
					if(asv)
						printf(">>Insert cubic curves E%u in V%u : ",ecf,vcf);
					else
						printf(">>Insert cubic curves E%u with V%u : ",ecf,vcf);
					clock.reset();
					for(int pass=0;pass<nSCycles;++pass)
					{
						instance->reset();
						if(asv)
						{
							clock.stop();
							Utils::insertRandom(instance,nVInsertions,margin,vcf!=0,rndgen);
							clock.resume();
						}
						for(int i=0;i<nSInsertions;++i)
						{
							const hkVector4	p0(rndgen.getRandRange(-extent,+extent),rndgen.getRandRange(-extent,+extent),0);
							const hkVector4	p1(rndgen.getRandRange(-extent,+extent),rndgen.getRandRange(-extent,+extent),0);
							const hkVector4	p2(rndgen.getRandRange(-extent,+extent),rndgen.getRandRange(-extent,+extent),0);
							const hkVector4	p3(rndgen.getRandRange(-extent,+extent),rndgen.getRandRange(-extent,+extent),0);
							vertices.clear();
							for(int j=0;j<nShapeSteps;++j)
							{
								const hkReal	t=j/(hkReal)(nShapeSteps-1);
								const hkVector4 x=Utils::lerp(	Utils::lerp(	Utils::lerp(p0,p1,t),
									Utils::lerp(p1,p2,t),t),
									Utils::lerp(	Utils::lerp(p1,p2,t),
									Utils::lerp(p2,p3,t),t),t);
								Vertex			v=instance->transform(x);
								Insertion		ins=Utils::insertVertex(instance,v.m_x,v.m_y,vcf!=0);
								if(ins.isValid())
								{
									vertices.pushBack(ins.m_edge.start());
								}
							}				
							for(int j=0,nj=vertices.getSize();j<(nj-1);++j)
							{
								const int	k=(j+1)%nj;
								Utils::insertEdge(instance,vertices[j],vertices[k],ecf!=0);
							}
						}
					}	
					clock.print(nSCycles*nSInsertions);
				}
			}
		}
	}
	if(bEnableBenchark6)
	{		
		printf(">>Remove V1 : ");
		rndgen.setSeed(180673);
		hkArray<Vertex*>	vertices;
		vertices.reserve(nVInsertions);
		clock.reset();
		for(int pass=0;pass<nVCycles;++pass)
		{
			printf("Cycle: %u\r\n",pass);
			instance->reset();
			vertices.clear();
			clock.stop();
			for(int i=0;i<nVInsertions;++i)
			{
				int			x=Utils::randomInt(rndgen,margin);
				int			y=Utils::randomInt(rndgen,margin);
				Insertion	ins=instance->insertVertex(x,y,true);
				if(ins.isValid())
				{
					vertices.pushBack(ins.m_edge.start());
				}
			}			
			for(int i=0,ni=vertices.getSize();i<ni;++i)
			{
				int		j=rndgen.getRand32()%ni;
				Vertex*	t=vertices[i];
				vertices[i]=vertices[j];
				vertices[j]=t;
			}
			clock.resume();
			for(int i=0,ni=vertices.getSize();i<ni;++i)
			{
				instance->removeVertex(vertices[i]->m_x,vertices[i]->m_y);
			}
		}
		clock.print(nVCycles*nVInsertions);
	}
	printf("Conforming stack size: %u\r\n",instance->m_stack.getCapacity());
	return(instance);
}
#else
TRIANGULATOR_HDR
TRIANGULATOR_TYP*			TRIANGULATOR_TYP::benchmark()
{
	return(0);
}
#endif

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
