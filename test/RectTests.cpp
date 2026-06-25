#include <Rect/Rect.h>
#include <gtest/gtest.h>

// Helper to construct Rect instances inline
namespace
{
Rect make_rect(int x, int y, int width, int height)
{
	return Rect{x, y, width, height};
}
}

// ============================================================================
// Basic Geometry Helpers
// ============================================================================

TEST(RectBasics, EdgeAndAreaHelpers)
{
	const Rect r = make_rect(2, 3, 7, 5);
	EXPECT_EQ(r.right(), 9);
	EXPECT_EQ(r.bottom(), 8);
	EXPECT_EQ(r.area(), 35);
	EXPECT_FALSE(r.empty());
}

TEST(RectBasics, EmptyDetection)
{
	EXPECT_TRUE(make_rect(0, 0, 0, 1).empty());
	EXPECT_TRUE(make_rect(0, 0, 1, 0).empty());
	EXPECT_TRUE(make_rect(0, 0, 0, 0).empty());
	EXPECT_TRUE(make_rect(5, 5, -1, 10).empty());  // negative width
	EXPECT_TRUE(make_rect(5, 5, 10, -1).empty());  // negative height
	EXPECT_FALSE(make_rect(0, 0, 1, 1).empty());
}

TEST(RectBasics, EqualityOperators)
{
	const Rect a = make_rect(1, 2, 3, 4);
	const Rect b = make_rect(1, 2, 3, 4);
	const Rect c = make_rect(1, 2, 3, 5);

	EXPECT_EQ(a, b);
	EXPECT_NE(a, c);
	EXPECT_FALSE(a != b);
	EXPECT_TRUE(a != c);
}

// ============================================================================
// Overlaps
// ============================================================================

TEST(RectOverlaps, TrueForInteriorIntersection)
{
	const Rect a = make_rect(0, 0, 10, 10);
	const Rect b = make_rect(5, 5, 10, 10);

	EXPECT_TRUE(a.overlaps(b));
	EXPECT_TRUE(b.overlaps(a));
}

TEST(RectOverlaps, FalseWhenOnlyTouchingEdges)
{
	const Rect a = make_rect(0, 0, 10, 10);
	const Rect b = make_rect(10, 0, 3, 3);    // touching right edge
	const Rect c = make_rect(0, 10, 3, 3);    // touching bottom edge

	EXPECT_FALSE(a.overlaps(b));
	EXPECT_FALSE(a.overlaps(c));
}

TEST(RectOverlaps, FalseForDisjointRects)
{
	const Rect a = make_rect(0, 0, 5, 5);
	const Rect b = make_rect(10, 10, 5, 5);

	EXPECT_FALSE(a.overlaps(b));
	EXPECT_FALSE(b.overlaps(a));
}

TEST(RectOverlaps, FalseForEmptyRects)
{
	const Rect a = make_rect(0, 0, 10, 10);
	const Rect empty = make_rect(5, 5, 0, 0);

	EXPECT_FALSE(a.overlaps(empty));
	EXPECT_FALSE(empty.overlaps(a));
}

// ============================================================================
// Contains Rect
// ============================================================================

TEST(RectContainsRect, TrueForFullContainment)
{
	const Rect outer = make_rect(0, 0, 10, 10);
	const Rect inner = make_rect(2, 3, 5, 4);

	EXPECT_TRUE(outer.contains(inner));
	EXPECT_FALSE(inner.contains(outer));
}

TEST(RectContainsRect, TrueWhenIdentical)
{
	const Rect a = make_rect(5, 5, 3, 3);
	const Rect b = make_rect(5, 5, 3, 3);

	EXPECT_TRUE(a.contains(b));
	EXPECT_TRUE(b.contains(a));
}

TEST(RectContainsRect, FalseForPartialOverlap)
{
	const Rect outer = make_rect(0, 0, 10, 10);
	const Rect partial = make_rect(8, 8, 5, 5);

	EXPECT_FALSE(outer.contains(partial));
}

TEST(RectContainsRect, FalseForEmptyRect)
{
	const Rect outer = make_rect(0, 0, 10, 10);
	const Rect empty = make_rect(5, 5, 0, 0);

	EXPECT_FALSE(outer.contains(empty));
}

TEST(RectContainsRect, FalseWhenJustTouchingBoundary)
{
	const Rect outer = make_rect(0, 0, 10, 10);
	const Rect touching = make_rect(10, 5, 2, 2);

	EXPECT_FALSE(outer.contains(touching));
}

// ============================================================================
// Contains Point
// ============================================================================

TEST(RectContainsPoint, InclusiveTopLeftExclusiveBottomRight)
{
	const Rect r = make_rect(5, 6, 4, 3);

	// Top-left corner and interior
	EXPECT_TRUE(r.contains(5, 6));
	EXPECT_TRUE(r.contains(6, 7));
	EXPECT_TRUE(r.contains(8, 8));

	// Right and bottom edges are exclusive
	EXPECT_FALSE(r.contains(9, 8));   // right edge (x = 5+4 = 9)
	EXPECT_FALSE(r.contains(8, 9));   // bottom edge (y = 6+3 = 9)
	EXPECT_FALSE(r.contains(9, 9));   // bottom-right corner

	// Outside
	EXPECT_FALSE(r.contains(4, 6));   // left of rect
	EXPECT_FALSE(r.contains(5, 5));   // above rect
}

TEST(RectContainsPoint, FalseForEmptyRect)
{
	const Rect empty = make_rect(5, 5, 0, 0);
	EXPECT_FALSE(empty.contains(5, 5));
}

// ============================================================================
// Include Rect
// ============================================================================

TEST(RectIncludeRect, ExpandsToContainOtherRectangle)
{
	Rect r = make_rect(10, 10, 2, 2);
	const Rect other = make_rect(8, 9, 10, 1);

	r.include(other);

	EXPECT_EQ(r, make_rect(8, 9, 10, 3));
}

TEST(RectIncludeRect, NoOpForAlreadyContainedRect)
{
	Rect r = make_rect(0, 0, 10, 10);
	const Rect inner = make_rect(2, 3, 4, 4);

	r.include(inner);

	EXPECT_EQ(r, make_rect(0, 0, 10, 10));
}

TEST(RectIncludeRect, ExpandsInAllDirections)
{
	Rect r = make_rect(5, 5, 5, 5);
	const Rect bigger = make_rect(3, 4, 10, 8);

	r.include(bigger);

	EXPECT_EQ(r, make_rect(3, 4, 10, 8));
}

TEST(RectIncludeRect, NoOpForEmptyRect)
{
	Rect r = make_rect(5, 5, 3, 3);
	const Rect empty = make_rect(10, 10, 0, 0);

	r.include(empty);

	EXPECT_EQ(r, make_rect(5, 5, 3, 3));
}

// ============================================================================
// Include Point
// ============================================================================

TEST(RectIncludePoint, ExpandsOnAllSidesAsNeeded)
{
	Rect r = make_rect(10, 10, 2, 2);

	// Expand right and down
	r.include(15, 20);
	EXPECT_EQ(r, make_rect(10, 10, 6, 11));

	// Expand left and up
	r.include(8, 7);
	EXPECT_EQ(r, make_rect(8, 7, 8, 14));
}

TEST(RectIncludePoint, NoOpForAlreadyContainedPoint)
{
	Rect r = make_rect(0, 0, 10, 10);

	r.include(5, 5);

	EXPECT_EQ(r, make_rect(0, 0, 10, 10));
}

TEST(RectIncludePoint, ExpandsMinimallyOnBoundary)
{
	Rect r = make_rect(0, 0, 5, 5);

	// Point at right edge (exclusive) should expand by 1
	r.include(5, 3);
	EXPECT_EQ(r, make_rect(0, 0, 6, 5));

	// Point at bottom edge (exclusive) should expand by 1
	r.include(3, 5);
	EXPECT_EQ(r, make_rect(0, 0, 6, 6));
}

// ============================================================================
// Clip
// ============================================================================

TEST(RectClip, ProducesIntersectionForOverlappingRects)
{
	Rect r = make_rect(0, 0, 10, 10);
	const Rect clipper = make_rect(3, 4, 8, 3);

	r.clip(clipper);

	EXPECT_EQ(r, make_rect(3, 4, 7, 3));
}

TEST(RectClip, EmptyResultForNoOverlap)
{
	Rect r = make_rect(0, 0, 10, 10);
	const Rect clipper = make_rect(20, 20, 5, 5);

	r.clip(clipper);

	EXPECT_TRUE(r.empty());
	EXPECT_GE(r.width, 0);
	EXPECT_GE(r.height, 0);
}

TEST(RectClip, NoOpWhenClipperContainsRect)
{
	Rect r = make_rect(5, 5, 3, 3);
	const Rect clipper = make_rect(0, 0, 20, 20);

	r.clip(clipper);

	EXPECT_EQ(r, make_rect(5, 5, 3, 3));
}

TEST(RectClip, PartialClipOnOneEdge)
{
	Rect r = make_rect(0, 0, 10, 10);
	const Rect clipper = make_rect(5, 0, 10, 10);

	r.clip(clipper);

	EXPECT_EQ(r, make_rect(5, 0, 5, 10));
}

TEST(RectClip, PreservesDimensionInvariant)
{
	Rect r = make_rect(0, 0, 10, 10);
	const Rect clipper = make_rect(15, 15, 5, 5);

	r.clip(clipper);

	EXPECT_GE(r.width, 0);
	EXPECT_GE(r.height, 0);
}

// ============================================================================
// Invariant and Edge Case Tests
// ============================================================================

TEST(RectInvariants, DimensionsNeverNegativeAfterOperations)
{
	Rect r = make_rect(5, 5, 10, 10);

	r.clip(make_rect(20, 20, 5, 5));
	EXPECT_GE(r.width, 0);
	EXPECT_GE(r.height, 0);
}

TEST(RectOperations, SelfOperationsAreWellDefined)
{
	Rect r = make_rect(5, 5, 10, 10);

	r.include(r);
	EXPECT_EQ(r, make_rect(5, 5, 10, 10));

	r.clip(r);
	EXPECT_EQ(r, make_rect(5, 5, 10, 10));

	EXPECT_TRUE(r.overlaps(r));
	EXPECT_TRUE(r.contains(r));
}
