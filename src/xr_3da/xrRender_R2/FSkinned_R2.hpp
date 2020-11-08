// SkeletonX.h: interface for the CSkeletonX class.
//
//////////////////////////////////////////////////////////////////////

#pragma		once

#include "FVisual_R2.hpp"
#include "FProgressive_R2.hpp"
#ifdef _EDITOR
#	include	"SkeletonX.h"
#else
#	include	"..\SkeletonX.h"
#endif
struct	SEnumVerticesCallback;
class		CSkeletonX_ext : public CSkeletonX	// shared code for SkeletonX derivates
{
protected:
    virtual void			_Load_hw(Fvisual& V, void *		data);
    virtual void			_CollectBoneFaces(Fvisual* V, u32 iBase, u32 iCount);
    void			_EnumBoneVertices(SEnumVerticesCallback &C, Fvisual* V, u16 bone_id, u32 iBase, u32 iCount) const;
    virtual void			_FillVerticesHW1W(const Fmatrix& view, CSkeletonWallmark& wm, const Fvector& normal, float size, Fvisual* V, u16* indices, CBoneData::FacesVec& faces);
    virtual void			_FillVerticesHW2W(const Fmatrix& view, CSkeletonWallmark& wm, const Fvector& normal, float size, Fvisual* V, u16* indices, CBoneData::FacesVec& faces);
    virtual void			_FillVertices(const Fmatrix& view, CSkeletonWallmark& wm, const Fvector& normal, float size, Fvisual* V, u16 bone_id, u32 iBase, u32 iCount);

    virtual BOOL			_PickBoneHW1W(Fvector& normal, float& range, const Fvector& S, const Fvector& D, Fvisual* V, u16* indices, CBoneData::FacesVec& faces);
    virtual BOOL			_PickBoneHW2W(Fvector& normal, float& range, const Fvector& S, const Fvector& D, Fvisual* V, u16* indices, CBoneData::FacesVec& faces);
    virtual BOOL			_PickBone(Fvector& normal, float& range, const Fvector& S, const Fvector& D, Fvisual* V, u16 bone_id, u32 iBase, u32 iCount);
public:
};

class CSkeletonX_ST : public Fvisual_R2, public CSkeletonX_ext
{
private:
    typedef Fvisual_R2 inherited1;
    typedef CSkeletonX_ext inherited2;
public:
    CSkeletonX_ST()	{}
    virtual					~CSkeletonX_ST()	{}
    virtual void			Render(float LOD);
    virtual void			Load(const char* N, IReader *data, u32 dwFlags);
    virtual void			Copy(IRender_Visual *pFrom);
    virtual void			Release();
    virtual void			AfterLoad(CKinematics* parent, u16 child_idx);
    virtual void			EnumBoneVertices(SEnumVerticesCallback &C, u16 bone_id);
    virtual BOOL			PickBone(Fvector& normal, float& dist, const Fvector& start, const Fvector& dir, u16 bone_id);
    virtual void			FillVertices(const Fmatrix& view, CSkeletonWallmark& wm, const Fvector& normal, float size, u16 bone_id);
private:
    CSkeletonX_ST(const CSkeletonX_ST& other);
    void	operator=			(const CSkeletonX_ST& other);
};

class CSkeletonX_PM : public FProgressive_R2, public CSkeletonX_ext
{
private:
    typedef FProgressive_R2 inherited1;
    typedef CSkeletonX_ext inherited2;
public:
    CSkeletonX_PM()	{}
    virtual					~CSkeletonX_PM()	{}
    virtual void			Render(float LOD);
    virtual void			Load(const char* N, IReader *data, u32 dwFlags);
    virtual void			Copy(IRender_Visual *pFrom);
    virtual void			Release();
    virtual void			AfterLoad(CKinematics* parent, u16 child_idx);
    virtual void			EnumBoneVertices(SEnumVerticesCallback &C, u16 bone_id);
    virtual BOOL			PickBone(Fvector& normal, float& dist, const Fvector& start, const Fvector& dir, u16 bone_id);
    virtual void			FillVertices(const Fmatrix& view, CSkeletonWallmark& wm, const Fvector& normal, float size, u16 bone_id);
private:
    CSkeletonX_PM(const CSkeletonX_PM& other);
    void	operator=			(const CSkeletonX_PM& other);
};