#if !defined(RECT_RECT_H)
#define RECT_RECT_H

#pragma once

//! Operations on rectangles with integer coordinates.
//! 
//! Rectangles are defined by anchor point (x, y) and dimensions (width, height).
//! 
//! **Range Semantics:**
//! Rectangles use half-inclusive ranges:
//! - Horizontal: [x, x+width) — left edge inclusive, right edge exclusive
//! - Vertical: [y, y+height) — top edge inclusive, bottom edge exclusive
//! 
//! This means a point at (x+width, y) or (x, y+height) is outside the rectangle, and two rectangles touching at edges do not
//! overlap.
class Rect
{
public:

    int x;          //!< Anchor X
    int y;          //!< Anchor Y
    int width;      //!< Width (cannot be negative)
    int height;     //!< Height (cannot be negative)

    //! Returns the X value of the right edge (exclusive).
    int right() const { return x + width; }

    //! Returns the Y value of the bottom edge (exclusive).
    int bottom() const { return y + height; }

    //! Returns the area.
    int area() const { return width * height; }

    //! Returns true if this rect overlaps another rect (edge-touching does not count as overlap).
    bool overlaps(Rect const & other) const;

    //! Returns true if this rect completely contains another rect.
    bool contains(Rect const & other) const;

    //! Returns true if the point is within the rect (half-inclusive: left/top inclusive, right/bottom exclusive).
    bool contains(int px, int py) const;

    //! Returns true if the width or height are <= 0.
    bool empty() const { return width <= 0 || height <= 0; }

    //! Changes the size and position to include another rect.
    void include(Rect const & other);

    //! Changes the size and position to include a point.
    void include(int px, int py);

    //! Clips this rect by another rect.
    void clip(Rect const & other);

private:
    bool valid() const { return width >= 0 && height >= 0; }
};

//! Compares two rectangles for equality.
//!
//! Two rectangles are equal if all their components (x, y, width, height) are equal.
//!
//! @param lhs The left-hand side rectangle
//! @param rhs The right-hand side rectangle
//! @return true if the rectangles are equal, false otherwise
inline bool operator ==(Rect const & lhs, Rect const & rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.width == rhs.width && lhs.height == rhs.height;
}

//! Compares two rectangles for inequality.
//!
//! Two rectangles are not equal if any of their components (x, y, width, height) differ.
//!
//! @param lhs The left-hand side rectangle
//! @param rhs The right-hand side rectangle
//! @return true if the rectangles are not equal, false otherwise
inline bool operator !=(Rect const & lhs, Rect const & rhs) { return !operator==(lhs, rhs); }

#endif // !defined(RECT_RECT_H)
