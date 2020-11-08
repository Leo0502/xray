#pragma once

#include "../xrRender/r__dsgraph_structure.h"
#include "..\xrRender\r__occlusion.h"

#include "r2_types.h"
#include "r2_rendertarget.h"

#include "..\xrRender\hom.h"
#include "DetailManager_R2.hpp"

#include "smap_allocator.h"
#include "Light_DB_R2.hpp"
#include "light_render_direct.h"
#include "LightTrack_R2.hpp"

#include "../irenderable.h"
#include "../fmesh.h"

// definition
class CRender : public R_dsgraph_structure
{
public:
	struct		_options	{
		u32		bug					: 1;

		u32		smapsize			: 16;
		u32		depth16				: 1;
		u32		mrt					: 1;
		u32		mrtmixdepth			: 1;
		u32		fp16_filter			: 1;
		u32		fp16_blend			: 1;
		u32		albedo_wo			: 1;						// work-around albedo on less capable HW
		u32		HW_smap				: 1;
		u32		HW_smap_PCF			: 1;
		u32		HW_smap_FETCH4		: 1;

		u32		HW_smap_FORMAT		: 32;

		u32		nvstencil			: 1;
		u32		nvdbt				: 1;

		u32		nullrt				: 1;

		u32		distortion			: 1;
		u32		distortion_enabled	: 1;
		u32		mblur				: 1;

		u32		sunfilter			: 1;
		u32		sunstatic			: 1;
		u32		sjitter				: 1;
		u32		noshadows			: 1;
		u32		Tshadows			: 1;						// transluent shadows
		u32		disasm				: 1;

		u32		forcegloss			: 1;
		u32		forceskinw			: 1;
		float	forcegloss_v		;
	}			o;
	struct		_stats		{
		u32		l_total,	l_visible;
		u32		l_shadowed,	l_unshadowed;
		s32		s_used,		s_merged,	s_finalclip;
		u32		o_queries,	o_culled;
		u32		ic_total,	ic_culled;
	}			stats;

	// Sector detection and visibility
	Fvector														vLastCameraPos;
	u32															uLastLTRACK;
	xrXRC														Sectors_xrc;
	R_occlusion													HWOCC;

	// Global vertex-buffer container
	xr_vector<VertexDeclarator>									alternativeDCL;
	xr_vector<IDirect3DVertexBuffer9*>							alternativeVertexBuffer;
	xr_vector<IDirect3DIndexBuffer9*>							alternativeIndexBuffer;

    CRenderTarget*												Target;			// Render-target

    xray::CLight_DB_R2											Lights;
	CLight_Compute_XFORM_and_VIS								LR;
    xr_vector<xray::Light_R2*>									Lights_LastFrame;
	SMAP_Allocator												LP_smap_pool;
	xray::Light_Package_R2										LP_normal;
	xray::Light_Package_R2										LP_pending;

	xr_vector<Fbox3,render_alloc<Fbox3> >						main_coarse_structure;

	shared_str													c_sbase			;
	shared_str													c_lmaterial		;
	float														o_hemi			;
	float														o_sun			;
	IDirect3DQuery9*											q_sync_point[2]	;
	u32															q_sync_count	;
private:
	// Loading / Unloading
	void							LoadBuffers					(CStreamReader	*fs,	BOOL	_alternative);
	void							LoadVisuals					(IReader	*fs);
	void							LoadLights					(IReader	*fs);
	void							LoadPortals					(IReader	*fs);
	void							LoadSectors					(IReader	*fs);

	BOOL							add_Dynamic					(IRender_Visual	*pVisual, u32 planes);		// normal processing
	void							add_Static					(IRender_Visual	*pVisual, u32 planes);
	void							add_leafs_Dynamic			(IRender_Visual	*pVisual);					// if detected node's full visibility
	void							add_leafs_Static			(IRender_Visual	*pVisual);					// if detected node's full visibility

public:
	IRender_Sector*					rimp_detectSector			(Fvector& P, Fvector& D);
	void							render_main					(Fmatrix& mCombined, bool _fportals);
	void							render_forward				();
	void							render_smap_direct			(Fmatrix& mCombined);
    void							render_indirect(xray::Light_R2* L);
	void							render_lights				(xray::Light_Package_R2& LP);
	void							render_sun					();
	void							render_sun_near				();
	void							render_sun_filtered			();
	void							render_menu					();
    void r_dsgraph_render_emissive();
    void r_dsgraph_render_wmarks();

	ShaderElement*					rimp_select_sh_static		(IRender_Visual	*pVisual, float cdist_sq);
	ShaderElement*					rimp_select_sh_dynamic		(IRender_Visual	*pVisual, float cdist_sq);
    D3DVERTEXELEMENT9* getAlternativeVertexBufferFormat(int id);
    IDirect3DVertexBuffer9* getAlternativeVertexBuffer(int id);
    IDirect3DIndexBuffer9* getAlternativeIndexBuffer(int id);
	IRender_Sector*					detectSector				(const Fvector& P, Fvector& D);
    virtual IRender_Target*			getTarget					();

	// HW-occlusion culling
	IC u32							occq_begin					(u32&	ID		)	{ return HWOCC.occq_begin	(ID);	}
	IC void							occq_end					(u32&	ID		)	{ HWOCC.occq_end	(ID);			}
	IC u32							occq_get					(u32&	ID		)	{ return HWOCC.occq_get		(ID);	}

	ICF void						apply_object				(IRenderable*	O)
	{
		if (0==O)					return;
		if (0==O->renderable_ROS())	return;
		CROS_impl& LT				= *((CROS_impl*)O->renderable_ROS());
		LT.update_smooth			(O)								;
		o_hemi						= 0.75f*LT.get_hemi			()	;
		o_sun						= 0.75f*LT.get_sun			()	;
	}
	IC void							apply_lmaterial				()
	{
		R_constant*		C	= &*RCache.get_c	(c_sbase);		// get sampler
		if (0==C)			return;
		VERIFY				(RC_dest_sampler	== C->destination);
		VERIFY				(RC_sampler			== C->type);
		CTexture*		T	= RCache.get_ActiveTexture	(u32(C->samp.index));
		VERIFY				(T);
		float	mtl			= T->m_material;
#ifdef	DEBUG
		if (ps_r2_ls_flags.test(R2FLAG_GLOBALMATERIAL))	mtl=ps_r2_gmaterial;
#endif
		RCache.set_c		(c_lmaterial,o_hemi,o_sun,0,(mtl+.5f)/4.f);
	}

	// feature level
	virtual	GenerationLevel			get_generation			()	{ return IRender_interface::GENERATION_R2; }

	// Loading / Unloading
	virtual void					create						();
	virtual void					destroy						();
	virtual	void					reset_begin					();
	virtual	void					reset_end					();

	virtual	void					level_Load					(IReader*);
	virtual void					level_Unload				();

	virtual IDirect3DBaseTexture9*	texture_load			(LPCSTR	fname, u32& msize);
	virtual HRESULT					shader_compile			(
		LPCSTR							name,
		LPCSTR                          pSrcData,
		UINT                            SrcDataLen,
		void*							pDefines,
		void*							pInclude,
		LPCSTR                          pFunctionName,
		LPCSTR                          pTarget,
		DWORD                           Flags,
		void*							ppShader,
		void*							ppErrorMsgs,
		void*							ppConstantTable);

	// Information
	virtual void					Statistics					(CGameFont* F);
	virtual LPCSTR					getShaderPath				()									{ return "r2\\";	}
	virtual IRender_Sector*			detectSector				(const Fvector& P);

	// Main 
	virtual void					flush						();
	virtual void					set_Object					(IRenderable*		O	);
	virtual	void					add_Occluder				(Fbox2&	bb_screenspace	);			// mask screen region as oclluded
	virtual void					add_Visual					(IRender_Visual*	V	);			// add visual leaf	(no culling performed at all)
	virtual void					add_Geometry				(IRender_Visual*	V	);			// add visual(s)	(all culling performed)

	//
	virtual IBlender*				blender_create				(CLASS_ID cls);
	virtual void					blender_destroy				(IBlender* &);

	//
	virtual IRender_ObjectSpecific*	ros_create					(IRenderable*		parent);
	virtual void					ros_destroy					(IRender_ObjectSpecific* &);

	// Lighting
	virtual IRender_Light*			light_create				();
	virtual IRender_Glow*			glow_create					();

	// Models
	virtual IRender_Visual*			model_CreateParticles		(LPCSTR name);
	virtual IRender_DetailModel*	model_CreateDM				(IReader* F);
	virtual IRender_Visual*			model_Create				(LPCSTR name, IReader* data=0);
	virtual IRender_Visual*			model_CreateChild			(LPCSTR name, IReader* data);
	virtual IRender_Visual*			model_Duplicate				(IRender_Visual*	V);
	virtual void					model_Delete				(IRender_Visual* &	V, BOOL bDiscard);
	virtual void 					model_Delete				(IRender_DetailModel* & F);
	virtual void					model_Logging				(BOOL bEnable)				{ xray::renderBase.Models->Logging(bEnable); }
	virtual void					models_Prefetch				();
	virtual void					models_Clear				(BOOL b_complete);

	// Main
	virtual void					Calculate					();
	virtual void					Render						();
	virtual void					Screenshot					(ScreenshotMode mode=SM_NORMAL, LPCSTR name = 0);
	virtual void					OnFrame					();

	// Constructor/destructor/loader
	CRender							();
	virtual ~CRender				();
};

extern CRender						RImplementation;
