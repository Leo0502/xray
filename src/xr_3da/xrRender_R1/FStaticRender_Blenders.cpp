#include "stdafx.h"

#include "blenderdefault.h"
#include "blender_default_aref.h"
#include "blender_vertex.h"
#include "blender_vertex_aref.h"
#include "..\xrRender\blender_screen_set.h"
#include "blender_screen_gray.h"
#include "..\xrRender\blender_editor_wire.h"
#include "..\xrRender\blender_editor_selection.h"
#include "blender_LaEmB.h"
#include "Blender_Lm(EbB)_R1.hpp"
#include "Blender_BmmD_R1.hpp"
#include "blender_shadow_world.h"
#include "blender_blur.h"
#include "blender_model.h"
#include "Blender_Model_EbB_R1.hpp"
#include "Blender_detail_still_R1.hpp"
#include "Blender_tree_R1.hpp"
#include "Blender_Particle_R1.hpp"

IBlender*	CRender::blender_create	(CLASS_ID cls)
{	
	switch (cls)
	{
	case B_DEFAULT:			return xr_new<CBlender_default>			();		
	case B_DEFAULT_AREF:	return xr_new<CBlender_default_aref>	();	
	case B_VERT:			return xr_new<CBlender_Vertex>			();		
	case B_VERT_AREF:		return xr_new<CBlender_Vertex_aref>		();	
	case B_SCREEN_SET:		return xr_new<CBlender_Screen_SET>		();	
	case B_SCREEN_GRAY:		return xr_new<CBlender_Screen_GRAY>		();	
	case B_EDITOR_WIRE:		return xr_new<CBlender_Editor_Wire>		();	
	case B_EDITOR_SEL:		return xr_new<CBlender_Editor_Selection>();
	case B_LaEmB:			return xr_new<CBlender_LaEmB>			();		
	case B_LmEbB:			return xr_new<CBlender_LmEbB_R1>		();		
	case B_BmmD:			return xr_new<CBlender_BmmD_R1>			();			
	case B_SHADOW_WORLD:	return xr_new<CBlender_ShWorld>			();		
	case B_BLUR:			return xr_new<CBlender_Blur>			();			
	case B_MODEL:			return xr_new<CBlender_Model>			();		
	case B_MODEL_EbB:		return xr_new<CBlender_Model_EbB_R1>	();	
	case B_DETAIL:			return xr_new<CBlender_Detail_Still_R1>	();	
	case B_TREE:			return xr_new<CBlender_Tree_R1>			();	
	case B_PARTICLE:		return xr_new<CBlender_Particle_R1>		();
	}
	return 0;
}

void		CRender::blender_destroy(IBlender* &B)
{
	xr_delete(B);
}