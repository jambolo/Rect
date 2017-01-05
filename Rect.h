/** @file *//********************************************************************************************************

											         Rect.h

										Copyright 2002, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Libraries/Rect/Rect.h#3 $

	$NoKeywords: $

 ********************************************************************************************************************/

#pragma once


#include <list>

/********************************************************************************************************************/

class Rect
{
public:

		// Constructor
	Rect() {}
	Rect( int x, int y, int w, int h );

		// Returns the X value of the right edge of the rect
	int Right() const							{ return m_X + m_Width; }

		// Returns the Y value of the bottom edge of the rect
	int Bottom() const							{ return m_Y + m_Height; }

		// Returns the number of pixels in the rect
	int Area() const							{ return m_Width * m_Height; }

		// Returns true if this rect overlaps the given rect
	bool Overlaps( Rect const & rect ) const;

		// Returns true if this rect completely contains the given rect
	bool Contains( Rect const & rect ) const;

		// Returns true if the point is within the rect
	bool Contains( int x, int y ) const;

		// Returns true if the width or height == 0
	bool IsEmpty() const;

		// Returns true if the width and height >= 0
	bool IsNormal() const;

		// Forces the width and height to be >= 0
	void Normalize();

		// Change size to include the given rect
	void Include( Rect const & rect );

		// Change size to include a point
	void Include( int x, int y );

		// Change size and position to clip to given rect
	void Clip( Rect const & rect );

		// Position and size
	int m_X;
	int	m_Y;
	int	m_Width;
	int	m_Height;
};


/********************************************************************************************************************/

typedef std::list<Rect> RectList;


/********************************************************************************************************************/

inline Rect::Rect( int x, int y, int w, int h )
	: m_X( x ), m_Y( y ), m_Width( w ), m_Height( h )
{
}


/********************************************************************************************************************/

inline bool Rect::IsEmpty() const
{
	return ( m_Width <= 0 || m_Height <= 0 );
}


/********************************************************************************************************************/

inline bool Rect::IsNormal() const
{
	return ( m_Width >= 0 && m_Height >= 0 );
}
