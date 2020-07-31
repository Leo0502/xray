#ifndef _MPxDragAndDropBehavior
#define _MPxDragAndDropBehavior
//
//-
// ==========================================================================
// Copyright (C) Alias Systems Corp., and/or its licensors ("Alias").  
// All rights reserved.  These coded instructions, statements, computer  
// programs, and/or related material (collectively, the "Material")
// contain unpublished information proprietary to Alias, which is
// protected by Canadian and US federal copyright law and by international
// treaties. This Material may not be disclosed to third parties, or be copied
// or duplicated, in whole or in part, without the prior written consent of
// Alias.  ALIAS HEREBY DISCLAIMS ALL WARRANTIES RELATING TO THE MATERIAL,
// INCLUDING, WITHOUT LIMITATION, ANY AND ALL EXPRESS OR IMPLIED WARRANTIES OF
// NON-INFRINGEMENT, MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
// IN NO EVENT SHALL ALIAS BE LIABLE FOR ANY DAMAGES WHATSOEVER, WHETHER DIRECT,
// INDIRECT, SPECIAL, OR PUNITIVE, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
// OR OTHER TORTIOUS ACTION, OR IN EQUITY, ARISING OUT OF OR RELATED TO THE
// ACCESS TO, USE OF, OR RELIANCE UPON THE MATERIAL.
// ==========================================================================
//+
//
// CLASS:    MPxDragAndDropBehavior
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MPxDragAndDropBehavior)
//
//  Base class for user-defined drag and drop behaviors
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES

#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/MPlug.h>

// *****************************************************************************

// DECLARATIONS

class MString;

// *****************************************************************************

// CLASS DECLARATION (MPxDragAndDropBehavior)

/// Drag and Drop Behavior (OpenMayaUI)
/**

*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAUI_EXPORT MPxDragAndDropBehavior
{
public:
	///
	MPxDragAndDropBehavior();
	///
	virtual ~MPxDragAndDropBehavior();

	///
	MPxDragAndDropBehavior( void * init );

	///
	virtual bool shouldBeUsedFor( MObject &sourceNode, MObject &destinationNode,
								  MPlug &sourcePlug, MPlug &destinationPlug);

	///
	virtual MStatus connectAttrToAttr( MPlug &sourcePlug, MPlug &destinationPlug, bool force );

	///
	virtual MStatus connectAttrToNode( MPlug &sourcePlug, MObject &destinationNode, bool force );

	///
	virtual MStatus connectNodeToAttr( MObject &sourceNode, MPlug &destinationPlug, bool force );

	///
	virtual MStatus connectNodeToNode( MObject &sourceNode, MObject &destinationNode, bool force );

protected:
	
private:
	void   setData( void* );
	virtual const char*	className() const;
	void * 	data;

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MPxDragAndDropBehavior */
