#include "stdafx.h"
#pragma hdrstop

#include "FVisual_R2.hpp"

void Fvisual_R2::Load(const char* N, IReader *data, u32 dwFlags)
{
    IRender_Visual::Load(N, data, dwFlags);

    D3DVERTEXELEMENT9	dcl[MAX_FVF_DECL_SIZE];
    D3DVERTEXELEMENT9*	vFormat = 0;
    dwPrimitives = 0;
    BOOL				loaded_v = false;

    if (data->find_chunk(OGF_GCONTAINER)) {
#ifndef _EDITOR
        // verts
        u32 ID = data->r_u32();
        vBase = data->r_u32();
        vCount = data->r_u32();

        VERIFY(NULL == p_rm_Vertices);

        p_rm_Vertices = xray::renderBase.getVertexBuffer(ID);
        p_rm_Vertices->AddRef();
        vFormat = xray::renderBase.getVertexBufferFormat(ID);
        loaded_v = true;

        // indices
        ID = data->r_u32();
        iBase = data->r_u32();
        iCount = data->r_u32();
        dwPrimitives = iCount / 3;

        VERIFY(NULL == p_rm_Indices);
        p_rm_Indices = xray::renderBase.getIndexBuffer(ID);
        p_rm_Indices->AddRef();
#endif
        // check for fast-vertices
        if (data->find_chunk(OGF_FASTPATH))		{
            destructor<IReader>	geomdef(data->open_chunk(OGF_FASTPATH));
            destructor<IReader>	def(geomdef().open_chunk(OGF_GCONTAINER));

            // we have fast-mesh
            m_fast = xr_new<IRender_Mesh>();

            // verts
            D3DVERTEXELEMENT9*	fmt = 0;
            ID = def().r_u32();
            m_fast->vBase = def().r_u32();
            m_fast->vCount = def().r_u32();

            VERIFY(NULL == m_fast->p_rm_Vertices);
            m_fast->p_rm_Vertices = RImplementation.getAlternativeVertexBuffer(ID);
            m_fast->p_rm_Vertices->AddRef();
            fmt = RImplementation.getAlternativeVertexBufferFormat(ID);

            // indices
            ID = def().r_u32();
            m_fast->iBase = def().r_u32();
            m_fast->iCount = def().r_u32();
            m_fast->dwPrimitives = iCount / 3;

            VERIFY(NULL == m_fast->p_rm_Indices);
            m_fast->p_rm_Indices = RImplementation.getAlternativeIndexBuffer(ID);
            m_fast->p_rm_Indices->AddRef();

            // geom
            m_fast->rm_geom.create(fmt, m_fast->p_rm_Vertices, m_fast->p_rm_Indices);
        }
    }

    // read vertices
    if (!loaded_v && (dwFlags&VLOAD_NOVERTICES) == 0) {
        if (data->find_chunk(OGF_VCONTAINER)) {
#ifndef _EDITOR
            u32 ID = data->r_u32();
            vBase = data->r_u32();
            vCount = data->r_u32();
            VERIFY(NULL == p_rm_Vertices);
            p_rm_Vertices = xray::renderBase.getVertexBuffer(ID);
            p_rm_Vertices->AddRef();
            vFormat = xray::renderBase.getVertexBufferFormat(ID);
#endif
        }
        else {
            R_ASSERT(data->find_chunk(OGF_VERTICES));
            vBase = 0;
            u32 fvf = data->r_u32();
            CHK_DX(D3DXDeclaratorFromFVF(fvf, dcl));
            vFormat = dcl;
            vCount = data->r_u32();
            u32 vStride = D3DXGetFVFVertexSize(fvf);

            BOOL	bSoft = HW.Caps.geometry.bSoftware || (dwFlags&VLOAD_FORCESOFTWARE);
            u32		dwUsage = D3DUSAGE_WRITEONLY | (bSoft ? D3DUSAGE_SOFTWAREPROCESSING : 0);
            BYTE*	bytes = 0;
            VERIFY(NULL == p_rm_Vertices);
            R_CHK(HW.pDevice->CreateVertexBuffer(vCount*vStride, dwUsage, 0, D3DPOOL_MANAGED, &p_rm_Vertices, 0));
            R_CHK(p_rm_Vertices->Lock(0, 0, (void**)&bytes, 0));
            CopyMemory(bytes, data->pointer(), vCount*vStride);
            p_rm_Vertices->Unlock();
        }
    }

    // indices
    if (!loaded_v && (dwFlags&VLOAD_NOINDICES) == 0) {
        dwPrimitives = 0;
        if (data->find_chunk(OGF_ICONTAINER)) {
#ifndef _EDITOR
            u32 ID = data->r_u32();
            iBase = data->r_u32();
            iCount = data->r_u32();
            dwPrimitives = iCount / 3;
            VERIFY(NULL == p_rm_Indices);
            p_rm_Indices = xray::renderBase.getIndexBuffer(ID);
            p_rm_Indices->AddRef();
#endif
        }
        else {
            R_ASSERT(data->find_chunk(OGF_INDICES));
            iBase = 0;
            iCount = data->r_u32();
            dwPrimitives = iCount / 3;

            BOOL	bSoft = HW.Caps.geometry.bSoftware || (dwFlags&VLOAD_FORCESOFTWARE);
            u32		dwUsage = /*D3DUSAGE_WRITEONLY |*/ (bSoft ? D3DUSAGE_SOFTWAREPROCESSING : 0);	// indices are read in model-wallmarks code
            BYTE*	bytes = 0;

            VERIFY(NULL == p_rm_Indices);
            R_CHK(HW.pDevice->CreateIndexBuffer(iCount * 2, dwUsage, D3DFMT_INDEX16, D3DPOOL_MANAGED, &p_rm_Indices, 0));
            R_CHK(p_rm_Indices->Lock(0, 0, (void**)&bytes, 0));
            CopyMemory(bytes, data->pointer(), iCount * 2);
            p_rm_Indices->Unlock();
        }
    }

    if (dwFlags&VLOAD_NOVERTICES || dwFlags&VLOAD_NOINDICES)	return;
    else
        rm_geom.create(vFormat, p_rm_Vertices, p_rm_Indices);
}

void Fvisual_R2::Render(float)
{
    RCache.set_Geometry(rm_geom);
    RCache.Render(D3DPT_TRIANGLELIST, vBase, 0, vCount, iBase, dwPrimitives);
    RCache.stat.r.s_static.add(vCount);
}