/****************************************************************************

								  Rect.cpp
						Copyright 2002, John J. Bolton

	----------------------------------------------------------------------

	$Header: //depot/Libraries/Rect/Rect.cpp#2 $

	$NoKeywords: $

 ****************************************************************************/

#include "Rect.h"

#include <cassert>


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

bool Rect::Overlaps( Rect const & rect ) const
{
	assert( rect.IsNormal() );
	assert( IsNormal() );

	int const	x_off	= rect.m_X - m_X;
	int const	y_off	= rect.m_Y - m_Y;

	return ( !rect.IsEmpty() &&
			 !IsEmpty() &&
			 -x_off < rect.m_Width &&
			 -y_off < rect.m_Height &&
			 x_off < m_Width &&
			 y_off < m_Height );
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

bool Rect::Contains( Rect const & rect ) const
{
	assert( rect.IsNormal() );
	assert( IsNormal() );

	int const x_off = rect.m_X - m_X;
	int const y_off = rect.m_Y - m_Y;

	return ( !rect.IsEmpty() &&
			 x_off >= 0 && m_Width - x_off >= rect.m_Width &&
			 y_off >= 0 && m_Height - y_off >= rect.m_Height );
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

bool Rect::Contains( int x, int y ) const
{
	assert( IsNormal() );

	int const x_off = x - m_X;
	int const y_off = y - m_Y;

	return ( ( x_off >= 0 ) && ( x_off < m_Width ) &&
			 ( y_off >= 0 ) && ( y_off < m_Height ) );
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

void Rect::Include( Rect const & rect )
{
	assert( rect.IsNormal() );
	assert( IsNormal() );

	if ( rect.IsEmpty() )
		return;

	int const x_off = rect.m_X - m_X;
	int const y_off = rect.m_Y - m_Y;

	// Include right edge

	if ( m_Width < rect.m_Width + x_off )
		m_Width = rect.m_Width + x_off;

	// Include bottom edge

	if ( m_Height < rect.m_Height + y_off )
		m_Height = rect.m_Height + y_off;

	// Include left edge

	if ( x_off < 0 )
	{
		m_X = rect.m_X;
		m_Width -= x_off;
	}

	// Include top edge

	if ( y_off < 0 )
	{
		m_Y = rect.m_Y;
		m_Height -= y_off;
	}
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

void Rect::Include( int x, int y )
{
	assert( IsNormal() );

	int const x_off = x - m_X;
	int const y_off = y - m_Y;

	// Include on the right side

	if ( m_Width <= x_off )
		m_Width = x_off + 1;

	// Include on the bottom side

	if ( m_Height <= y_off )
		m_Height = y_off + 1;

	// Include on the left side

	if ( x_off < 0 )
	{
		m_X = x;
		m_Width -= x_off;
	}

	// Include on the top side

	if ( y_off < 0 )
	{
		m_Y = y;
		m_Height -= y_off;
	}
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

void Rect::Clip( Rect const & rect )
{
	assert( rect.IsNormal() );
	assert( IsNormal() );

	int const x_off = rect.m_X - m_X;
	int const y_off = rect.m_Y - m_Y;

	// Clip right edge

	if ( m_Width > x_off + rect.m_Width )
		m_Width =  x_off + rect.m_Width;

	// Clip bottom edge

	if ( m_Height > y_off + rect.m_Height )
		m_Height = y_off + rect.m_Height;

	// Clip left edge

	if ( x_off > 0 )
	{
		m_X = rect.m_X;
		m_Width += x_off;
	}

	// Clip top edge

	if ( y_off > 0 )
	{
		m_Y = rect.m_Y;
		m_Height += y_off;
	}
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

void Rect::Normalize()
{
	if ( m_Width < 0 )
	{
		m_X -= m_Width;
		m_Width = -m_Width;
	}

	if ( m_Height < 0 )
	{
		m_Y -= m_Height;
		m_Height = -m_Height;
	}
}
