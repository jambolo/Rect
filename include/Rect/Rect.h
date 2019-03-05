#if !defined(RECT_RECT_H)
#define RECT_RECT_H

#pragma once

class Rect
{
public:

    // Constructor
    Rect() = default;
    Rect(int x_, int y_, int w_, int h_);

    // Returns the X value of the right edge of the rect
    int right() const { return x + width; }

    // Returns the Y value of the bottom edge of the rect
    int bottom() const { return y + height; }

    // Returns the number of pixels in the rect
    int area() const { return width * height; }

    // Returns true if this rect overlaps the given rect
    bool overlaps(Rect const & rect) const;

    // Returns true if this rect completely contains the given rect
    bool contains(Rect const & rect) const;

    // Returns true if the point is within the rect
    bool contains(int xx, int yy) const;

    // Returns true if the width or height == 0
    bool isEmpty() const;

    // Returns true if the width and height >= 0
    bool isNormal() const;

    // Forces the width and height to be >= 0
    void normalize();

    // Change size to include the given rect
    void include(Rect const & rect);

    // Change size to include a point
    void include(int xx, int yy);

    // Change size and position to clip to given rect
    void clip(Rect const & rect);

    int x;
    int y;
    int width;
    int height;
};

inline Rect::Rect(int x_, int y_, int w_, int h_)
    : x(x_)
    , y(y_)
    , width(w_)
    , height(h_)
{
}

inline bool Rect::isEmpty() const
{
    return width <= 0 || height <= 0;
}

inline bool Rect::isNormal() const
{
    return width >= 0 && height >= 0;
}

#endif // !defined(RECT_RECT_H)
