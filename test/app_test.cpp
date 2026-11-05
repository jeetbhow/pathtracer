#include "app.hpp"

#include <gtest/gtest.h>

TEST(App, GreetingIncludesProjectName) {
    EXPECT_STREQ(app::greeting(), "Hello from pathtracer!");
}
