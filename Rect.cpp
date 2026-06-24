#include "Rect.h"

#include <algorithm>
#include <cassert>

//! Checks if this rectangle overlaps with another rectangle.
//! Two rectangles overlap if they share any common area.
//! Due to half-inclusive ranges, rectangles that only touch at edges do not overlap.
//!
//! @param other The rectangle to check for overlap
//! @pre other.valid() - The other rectangle must have non-negative dimensions
//! @pre valid() - This rectangle must have non-negative dimensions
bool Rect::overlaps(Rect const & other) const
{
    assert(other.valid());
    assert(valid());

    int x_off = other.x - x;
    int y_off = other.y - y;

    return !other.empty() &&
           !empty() &&
           -x_off < other.width &&
           -y_off < other.height &&
           x_off < width &&
           y_off < height;
}

//! Checks if this rectangle completely contains another rectangle.
//! A rectangle contains another if the other rectangle is entirely within its bounds.
//!
//! @param other The rectangle to check for containment
//! @pre other.valid() - The other rectangle must have non-negative dimensions
//! @pre valid() - This rectangle must have non-negative dimensions
bool Rect::contains(Rect const & other) const
{
    assert(other.valid());
    assert(valid());

    int x_off = other.x - x;
    int y_off = other.y - y;

    return !other.empty() &&
           x_off >= 0 && width - x_off >= other.width &&
           y_off >= 0 && height - y_off >= other.height;
}

//! Checks if a point is within this rectangle.
//! Uses half-inclusive ranges: [x, x+width) × [y, y+height).
//! Left and top edges are inclusive, right and bottom edges are exclusive.
//!
//! @param px The X coordinate of the point
//! @param py The Y coordinate of the point
//! @pre valid() - This rectangle must have non-negative dimensions
bool Rect::contains(int px, int py) const
{
    assert(valid());

    int x_off = px - x;
    int y_off = py - y;

    return (x_off >= 0) && (x_off < width) &&
           (y_off >= 0) && (y_off < height);
}

//! Expands this rectangle to include another rectangle.
//! Modifies the size and position of this rectangle so that it encompasses
//! both the original rectangle and the other rectangle.
//!
//! @param other The rectangle to include
//! @pre other.valid() - The other rectangle must have non-negative dimensions
//! @pre valid() - This rectangle must have non-negative dimensions
void Rect::include(Rect const & other)
{
    assert(other.valid());
    assert(valid());

    if (other.empty())
        return;

    int x_off = other.x - x;
    int y_off = other.y - y;

    // Include right edge

    if (width < other.width + x_off)
        width = other.width + x_off;

    // Include bottom edge

    if (height < other.height + y_off)
        height = other.height + y_off;

    // Include left edge

    if (x_off < 0)
    {
        x      = other.x;
        width -= x_off;
    }

    // Include top edge

    if (y_off < 0)
    {
        y       = other.y;
        height -= y_off;
    }
}

//! Expands this rectangle to include a point.
//! Modifies the size and position of this rectangle so that it encompasses
//! both the original rectangle and the specified point.
//! Ensures the point falls within the half-inclusive range [x, x+width) × [y, y+height).
//!
//! @param px The X coordinate of the point to include
//! @param py The Y coordinate of the point to include
//! @pre valid() - This rectangle must have non-negative dimensions
void Rect::include(int px, int py)
{
    assert(valid());

    int x_off = px - x;
    int y_off = py - y;

    // Include on the right side

    if (width <= x_off)
        width = x_off + 1;

    // Include on the bottom side

    if (height <= y_off)
        height = y_off + 1;

    // Include on the left side

    if (x_off < 0)
    {
        x      = px;
        width -= x_off;
    }

    // Include on the top side

    if (y_off < 0)
    {
        y       = py;
        height -= y_off;
    }
}

//! Clips this rectangle to the intersection with another rectangle.
//! Modifies this rectangle to contain only the area that overlaps with the other rectangle.
//! If there is no overlap, the result will be an empty rectangle (width and/or height = 0).
//!
//! @param other The rectangle to clip by
//! @pre other.valid() - The other rectangle must have non-negative dimensions
//! @pre valid() - This rectangle must have non-negative dimensions
void Rect::clip(Rect const & other)
{
    assert(other.valid());
    assert(valid());

    int x_off = other.x - x;
    int y_off = other.y - y;

    // Clip right edge

    if (width > x_off + other.width)
        width = x_off + other.width;

    // Clip bottom edge

    if (height > y_off + other.height)
        height = y_off + other.height;

    // Clip left edge

    if (x_off > 0)
    {
        x      = other.x;
        width -= x_off;
    }

    // Clip top edge

    if (y_off > 0)
    {
        y       = other.y;
        height -= y_off;
    }

    // Clamp to non-negative dimensions
    if (width < 0)
        width = 0;
    if (height < 0)
        height = 0;
}

//! Reduces this rectangle to avoid overlapping with another rectangle.
//! If the rectangles overlap, modifies this rectangle by moving its anchor point
//! to just beyond the overlapping region. Prefers moving right or down to maintain
//! positive dimensions. If there is no overlap, the rectangle remains unchanged.
//!
//! @param other The rectangle to exclude from this one
//! @pre other.valid() - The other rectangle must have non-negative dimensions
//! @pre valid() - This rectangle must have non-negative dimensions
void Rect::exclude(Rect const & other)
{
    assert(other.valid());
    assert(valid());

    if (!overlaps(other))
        return;

    // Move the rect's anchor to just beyond the overlapping region
    // Prefer moving right or down to maintain positive dimensions
    int new_x = std::min(std::max(x, other.right()), right());
    int new_y = std::min(std::max(y, other.bottom()), bottom());

    width  = std::max(0, width - (new_x - x));
    height = std::max(0, height - (new_y - y));
    x = new_x;
    y = new_y;
}
