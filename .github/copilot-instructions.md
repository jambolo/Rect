# Copilot Instructions for Rect Project

## Project Overview

This is a C++ library providing operations on rectangles with integer coordinates. The library is designed to be a reusable CMake package with a focus on correctness, testability, and documentation.

## Core Concepts

### Coordinate System
- **Origin**: Upper-left corner (0,0)
- **X-axis**: Increases to the right
- **Y-axis**: Increases downward
- **Constraints**: `left <= right` and `top <= bottom`
- **Dimensions**: Width and height are always non-negative

### Rectangle Representation
Rectangles are defined by anchor point `(x, y)` and dimensions `(width, height)`:
- `x`, `y`: Anchor position (can be anywhere)
- `width`, `height`: Must be non-negative
- Computed properties: `right() = x + width`, `bottom() = y + height`

### Range Semantics (CRITICAL!)
**Rectangles use half-inclusive ranges:**
- **Horizontal range**: `[x, x+width)` — left edge **inclusive**, right edge **exclusive**
- **Vertical range**: `[y, y+height)` — top edge **inclusive**, bottom edge **exclusive**

This means:
- A point at `(x, y)` is **inside** the rectangle
- A point at `(x+width, y)` or `(x, y+height)` is **outside** the rectangle
- Two rectangles touching at edges (e.g., `rect1.right() == rect2.x`) do **not overlap**
- `contains(px, py)` returns true when `x <= px < x+width` AND `y <= py < y+height`

**Example:**
```cpp
Rect r = {5, 6, 4, 3};  // x=5, y=6, width=4, height=3
// Contains points in [5,9) × [6,9)
r.contains(5, 6);  // true  - top-left corner (inclusive)
r.contains(8, 8);  // true  - interior point
r.contains(9, 8);  // false - right edge x=9 (exclusive)
r.contains(8, 9);  // false - bottom edge y=9 (exclusive)

// Edge-touching rectangles do NOT overlap
Rect a = {0, 0, 10, 10};  // [0,10) × [0,10)
Rect b = {10, 0, 5, 5};   // [10,15) × [0,5)
a.overlaps(b);            // false - only touching at x=10
```

## Code Standards

### Language & Build
- **C++ Standard**: C++17
- **Build System**: CMake 3.23+
- **Toolchain**: MSVC with Ninja generator
- **File Organization**:
  - Header-only interface in `include/Rect/Rect.h`
  - Implementation in `Rect.cpp` (root level)
  - Tests in `test/RectTests.cpp`

### Coding Style

#### Naming Conventions
- **Classes**: PascalCase (e.g., `Rect`)
- **Member variables**: lowercase snake_case (e.g., `x`, `y`, `width`, `height`)
- **Methods**: camelCase (e.g., `overlaps()`, `contains()`)
- **Parameters**: lowercase snake_case (e.g., `other`, `px`, `py`)

#### Documentation
- Use Doxygen-style comments (`//!` for inline, `/*!` for block)
- Document:
  - All public methods with brief description
  - Method parameters with `@param`
  - Preconditions with `@pre`
  - Return values when non-obvious
  - **Range semantics when relevant** (inclusive/exclusive edge behavior)
- Example:
  ```cpp
  /*!
   * Checks if this rectangle overlaps with another rectangle.
   * Two rectangles overlap if they share any common area.
   * Rectangles that only touch at edges do not overlap (half-inclusive ranges).
   * 
   * @param other The rectangle to check for overlap
   * @pre other.valid() - The other rectangle must have non-negative dimensions
   * @pre valid() - This rectangle must have non-negative dimensions
   */
  bool overlaps(Rect const & other) const;
  ```

#### Code Organization
- Keep class declaration minimal in header
- Inline only trivial getters (e.g., `right()`, `bottom()`, `area()`)
- Implement complex logic in `.cpp` file
- Use `const` references for parameters when appropriate
- Mark methods `const` when they don't modify state

### MSVC Specifics
When adding code that compiles under MSVC, use these target compile definitions (already configured):
- `NOMINMAX` - Avoid min/max macro conflicts
- `WIN32_LEAN_AND_MEAN` - Reduce Windows header overhead
- `VC_EXTRALEAN` - Further reduce header overhead
- `_CRT_SECURE_NO_WARNINGS` - Suppress secure CRT warnings
- `_SCL_SECURE_NO_WARNINGS` - Suppress secure SCL warnings

### Testing

#### Test Framework
- Use **Google Test** (v1.15.2)
- Tests fetched via CMake FetchContent
- Test file: `test/RectTests.cpp`

#### Test Style
- **Organization**: Group tests by functionality with section comments
- **Naming**: Use descriptive test names: `TEST(Category, DescriptiveTestName)`
- **Helper functions**: Use anonymous namespace for test helpers
- **Example**:
  ```cpp
  // Helper to construct Rect instances inline
  namespace
  {
  Rect make_rect(int x, int y, int width, int height)
  {
	  return Rect{x, y, width, height};
  }
  }

  TEST(RectBasics, EdgeAndAreaHelpers)
  {
	  const Rect r = make_rect(2, 3, 7, 5);
	  EXPECT_EQ(r.right(), 9);
	  EXPECT_EQ(r.bottom(), 8);
	  EXPECT_EQ(r.area(), 35);
  }
  ```

#### Test Coverage
- Test edge cases: empty rectangles, zero dimensions, negative dimensions
- Test boundary conditions for geometric operations
- **Test half-inclusive range behavior** (edge touching, point containment at boundaries)
- Use assertions (`assert`) in implementation for preconditions
- Verify all public methods have corresponding tests

### CMake Conventions

#### Target Configuration
- Library target: `Rect` (alias: `Rect::Rect`)
- Export name: `Rect`
- Use `FILE_SET HEADERS` for public headers
- Support both static and shared builds via `BUILD_SHARED_LIBS`

#### Options
- `BUILD_SHARED_LIBS` - Build as shared library (default: OFF)
- `RECT_BUILD_DOCS` - Generate Doxygen documentation (default: OFF)
- `BUILD_TESTING` - Build tests (default: OFF, enabled when top-level)

#### Installation
- Install to standard GNU directories
- Provide `RectConfig.cmake` for downstream consumption
- Use namespace `Rect::` for imported targets

## Common Operations

### Adding New Methods
1. Declare in `include/Rect/Rect.h` with Doxygen documentation
2. Implement in `Rect.cpp` with precondition assertions
3. Add comprehensive tests in `test/RectTests.cpp`
4. Follow existing patterns for parameter passing and const-correctness
5. **Ensure half-inclusive range semantics are preserved**

### Modifying Existing Methods
1. Update implementation and documentation together
2. Ensure preconditions (`assert`) remain valid
3. Update or add tests to cover new behavior
4. Run full test suite: `ctest --test-dir build --output-on-failure`
5. **Verify range semantics remain consistent**

### Working with Geometry
- Always validate dimensions are non-negative (use `valid()`)
- Consider empty rectangles (width or height <= 0)
- Use assertions for preconditions, not error handling
- Prefer integer arithmetic to avoid floating-point issues
- **Remember half-inclusive ranges**: use `<` not `<=` for right/bottom edge checks

## Build & Test Workflow

```bash
# Configure with tests enabled
cmake -B build -DBUILD_TESTING=ON

# Build
cmake --build build

# Run tests
ctest --test-dir build --output-on-failure

# Generate documentation (if needed)
cmake -B build -DRECT_BUILD_DOCS=ON
cmake --build build --target doxygen
```

## When Reviewing Code

- [ ] Are dimensions always non-negative?
- [ ] Do methods handle empty rectangles correctly?
- [ ] Are preconditions documented and asserted?
- [ ] Is const-correctness maintained?
- [ ] Are parameters passed by const reference when appropriate?
- [ ] Do new methods have Doxygen documentation?
- [ ] Are there corresponding unit tests?
- [ ] Does code follow existing naming conventions?
- [ ] **Are half-inclusive range semantics correctly implemented?**
- [ ] **Do edge-touching rectangles correctly not overlap?**

## Performance Considerations

- Prefer inline for trivial operations (already done for getters)
- Use integer arithmetic (avoid floating-point)
- Pass by const reference for non-trivial types
- Avoid unnecessary copies

## Future Directions

If extending the library, consider:
- Additional geometric operations (intersection, union)
- Serialization/deserialization support
- Support for different coordinate systems
- Template-based version for different numeric types
