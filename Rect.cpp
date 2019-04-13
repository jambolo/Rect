#include "Rect.h"

#include <cassert>

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

bool Rect::contains(int px, int py) const
{
    assert(valid());

    int x_off = px - x;
    int y_off = py - y;

    return (x_off >= 0) && (x_off < width) &&
           (y_off >= 0) && (y_off < height);
}

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

void Rect::clip(Rect const & other)
{
    assert(other.valid());
    assert(valid());

    int x_off = other.x - x;
    int y_off = other.y - y;

    // Clip right edge

    if (width > x_off + other.width)
        width =  x_off + other.width;

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
}

void exclude(Rect const & other)
{
    if (!overlaps(other))
        return;
    x = std::min(std::max(x, other.right()), right());
    y = std::min(std::max(y, other.bottom()), bottom())
    x = std::min(std::max(x, other.right()), right());
    y = std::min(std::max(y, other.bottom()), bottom())
}
