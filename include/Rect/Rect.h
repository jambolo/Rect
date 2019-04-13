#if !defined(RECT_RECT_H)
#define RECT_RECT_H

#pragma once

//! Operations on rectangles.
class Rect
{
public:

    int x;          //!< Anchor X
    int y;          //!< Anchor Y
    int width;      //!< Width (cannot be negative)
    int height;     //!< Hieght (cannot be negative)

    //! Returns the X value of the right edge.
    int right() const { return x + width; }

    //! Returns the Y value of the bottom edge.
    int bottom() const { return y + height; }

    //! Returns the area.
    int area() const { return width * height; }

    //! Returns true if this rect overlaps another rect.
    bool overlaps(Rect const & other) const;

    //! Returns true if this rect completely contains another rect.
    bool contains(Rect const & other) const;

    //! Returns true if the point is within the rect.
    bool contains(int px, int py) const;

    //! Returns true if the width or height are <= 0.
    bool empty() const { return width <= 0 || height <= 0; }

    //! Changes the size and position to include another rect.
    void include(Rect const & other);

    //! Changes the size and position to include a point.
    void include(int px, int py);

    //! Clips this rect by another rect.
    void clip(Rect const & other);

    //! Reduces this rect so that it does not overlap another rect.
    void exclude(Rect const & other);

private:
    bool valid() const { return width >= 0 && height >= 0; }
};

//! operator ==
bool operator ==(Rect const & lhs, Rect const & rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.width == rhs.width && lhs.height == rhs.height;
}

//! operator !=
bool operator !=(Rect const & lhs, Rect const & rhs) { return !operator==(lhs, rhs); }

#endif // !defined(RECT_RECT_H)
