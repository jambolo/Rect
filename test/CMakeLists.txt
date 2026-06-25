include(FetchContent)

# Disable installation of GoogleTest
set(INSTALL_GTEST OFF)

FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
	DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(RectTests
	RectTests.cpp
)

target_link_libraries(RectTests
	PRIVATE
		Rect::Rect
		GTest::gtest_main
)

target_compile_features(RectTests PRIVATE cxx_std_17)

include(GoogleTest)
gtest_discover_tests(RectTests)
