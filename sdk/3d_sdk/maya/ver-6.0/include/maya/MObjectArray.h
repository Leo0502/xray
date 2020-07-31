
#ifndef _MObjectArray
#define _MObjectArray
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
// CLASS:    MObjectArray
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MObjectArray)
//
//  This class implements an array of MObjects.  Common convenience functions
//  are available, and the implementation is compatible with the internal
//  Maya implementation so that it can be passed efficiently between plugins
//  and internal maya data structures.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MObject.h>
#include <maya/MStatus.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MObjectArray)

///  Array of MObjects data type
/**
  Implement an array of MObjects data type.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MObjectArray  
{

public:
    ///
					MObjectArray();
    ///
					MObjectArray( const MObjectArray& other );
    ///
					MObjectArray( const MObject src[], unsigned count );
	///
					MObjectArray( unsigned initialSize, 
								  const MObject &initialValue 
								  = MObject::kNullObj );
    ///
					~MObjectArray();
    ///
 	const MObject&	operator[]( unsigned index ) const;
    ///
 	MObject&		operator[]( unsigned index ); 
    ///
 	MStatus			set( const MObject& element, unsigned index ); 
    ///
	MStatus			setLength( unsigned length );
	///
 	unsigned		length() const;
    ///
 	MStatus			remove( unsigned index );
    ///
 	MStatus			insert( const MObject & element, unsigned index );
    ///
 	MStatus			append( const MObject & element );
    ///
 	MStatus			clear();
    ///
	MStatus			get( MObject array[] ) const;
	///
	void			setSizeIncrement ( unsigned newIncrement );
	///
	unsigned		sizeIncrement () const;

protected:
// No protected members

private:
 	MObjectArray& operator = (const MObjectArray&) const;
 	MObjectArray& operator = (const MObjectArray&);
 	void* fArray;
	static const char* className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MObjectArray */
