#include <gtest/gtest.h>
#include <cstdlib>
#include <cmath>

extern "C" {
    #include "/home/vadim/OS/LW4/include/lib1.h"
    #include <dlfcn.h>
}

TEST(INTEGRAL_TRIANGLE, test1) {
    EXPECT_NEAR(SinIntegral_rectangle(0.0, M_PI, 0.001), 2.0, 0.01);
    EXPECT_NEAR(SinIntegral_rectangle(0.0, M_PI / 2, 0.001), 1.0, 0.01);
}


TEST(INTEGRAL_TRAPEZOID, test2) {
    EXPECT_NEAR(SinIntegral_trapezoid(0.0, M_PI, 0.001), 2.0, 0.01);
    EXPECT_NEAR(SinIntegral_trapezoid(0.0, M_PI / 2, 0.001), 1.0, 0.01);
}


TEST(CONVERT_UBER_NULL, test3){
    const char *answer1 = translation_3(12);
    const char *answer2 = translation_2(12);
    ASSERT_STREQ(answer1, "110");
    ASSERT_STREQ(answer2, "1100");
}



TEST(CONVERT_UNTER_NULL, test4){
    const char *answer1 = translation_3(-12);
    const char *answer2 = translation_2(-12);
    ASSERT_STREQ(answer1, "-110");
    ASSERT_STREQ(answer2, "-1100");
}


TEST(NULL_TEST, test5){
    const char *answer1 = translation_3(0);
    const char *answer2 = translation_2(0);
    ASSERT_STREQ(answer1, "0");
    ASSERT_STREQ(answer2, "0");
}



/*
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}*/