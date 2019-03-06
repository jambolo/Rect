#include "Rect.h"

#include <cassert>

bool Rect::overlaps(Rect const & rect) const
{
    assert(rect.isNormal());
    assert(isNormal());

    int x_off = rect.x - x;
    int y_off = rect.y - y;

    return !rect.isEmpty() &&
           !isEmpty() &&
           -x_off < rect.width &&
           -y_off < rect.height &&
           x_off < width &&
           y_off < height;
}

bool Rect::contains(Rect const & rect) const
{
    assert(rect.isNormal());
    assert(isNormal());

    int x_off = rect.x - x;
    int y_off = rect.y - y;

    return !rect.isEmpty() &&
           x_off >= 0 && width - x_off >= rect.width &&
           y_off >= 0 && height - y_off >= rect.height;
}

bool Rect::contains(int xx, int yy) const
{
    assert(isNormal());

    int x_off = xx - x;
    int y_off = yy - y;

    return (x_off >= 0) && (x_off < width) &&
           (y_off >= 0) && (y_off < height);
}

void Rect::include(Rect const & rect)
{
    assert(rect.isNormal());
    assert(isNormal());

    if (rect.isEmpty())
        return;

    int x_off = rect.x - x;
    int y_off = rect.y - y;

    // Include right edge

    if (width < rect.width + x_off)
        width = rect.width + x_off;

    // Include bottom edge

    if (height < rect.height + y_off)
        height = rect.height + y_off;

    // Include left edge

    if (x_off < 0)
    {
        x      = rect.x;
        width -= x_off;
    }

    // Include top edge

    if (y_off < 0)
    {
        y       = rect.y;
        height -= y_off;
    }
}

void Rect::include(int xx, int yy)
{
    assert(isNormal());

    int x_off = xx - x;
    int y_off = yy - y;

    // Include on the right side

    if (width <= x_off)
        width = x_off + 1;

    // Include on the bottom side

    if (height <= y_off)
        height = y_off + 1;

    // Include on the left side

    if (x_off < 0)
    {
        x      = xx;
        width -= x_off;
    }

    // Include on the top side

    if (y_off < 0)
    {
        y       = yy;
        height -= y_off;
    }
}

void Rect::clip(Rect const & rect)
{
    assert(rect.isNormal());
    assert(isNormal());

    int x_off = rect.x - x;
    int y_off = rect.y - y;

    // Clip right edge

    if (width > x_off + rect.width)
        width =  x_off + rect.width;

    // Clip bottom edge

    if (height > y_off + rect.height)
        height = y_off + rect.height;

    // Clip left edge

    if (x_off > 0)
    {
        x      = rect.x;
        width -= x_off;
    }

    // Clip top edge

    if (y_off > 0)
    {
        y       = rect.y;
        height -= y_off;
    }
}

void Rect::normalize()
{
    if (width < 0)
    {
        x    -= width;
        width = -width;
    }

    if (height < 0)
    {
        y     -= height;
        height = -height;
    }
}
