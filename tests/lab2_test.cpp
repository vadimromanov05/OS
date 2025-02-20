#include <gtest/gtest.h>
#include <cmath>
extern "C" {
    #include "../LW2/src/utils.c"
}

TEST(GaussianEliminationTest, TestSimple1) {
    double matrix[MAX_SIZE][MAX_SIZE] = {
        {2, 1, -1},
        {-3, -1, 2},
        {-2, 1, 2}
    };
    double b[MAX_SIZE] = {8, -11, -3};
    double x[MAX_SIZE];
    int n = 3;

    solve_linear_system(matrix, b, x, n);

    EXPECT_DOUBLE_EQ(x[0], 2);
    EXPECT_DOUBLE_EQ(x[1], 3);
    EXPECT_DOUBLE_EQ(x[2], -1);
}


TEST(GaussianEliminationTest, TestSimple2) {
    double matrix[MAX_SIZE][MAX_SIZE] = {
        {4, 5},
        {5, 4}
    };
    double b[MAX_SIZE] = {1, -1};
    double x[MAX_SIZE];
    int n = 2;

    solve_linear_system(matrix, b, x, n);

    EXPECT_DOUBLE_EQ(x[0], -1);
    EXPECT_DOUBLE_EQ(x[1], 1);
}


TEST(GaussianEliminationTest, TestSimple3) {
    double matrix[MAX_SIZE][MAX_SIZE] = {
        {2}
    };
    double b[MAX_SIZE] = {5};
    double x[MAX_SIZE];
    int n = 1;

    solve_linear_system(matrix, b, x, n);

    EXPECT_DOUBLE_EQ(x[0], 2.5);
}


TEST(GaussianEliminationTest, InfinitySolutions) {
    double matrix[MAX_SIZE][MAX_SIZE] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    double b[MAX_SIZE] = {0, 0, 0};
    double x[MAX_SIZE];
    int n = 3;

    solve_linear_system(matrix, b, x, n);
    double nanValue = std::nan("");
    EXPECT_TRUE(std::isnan(nanValue));
}


TEST(GaussianEliminationTest, UnrealSolutions) {
    double matrix[MAX_SIZE][MAX_SIZE] = {
        {0, 0},
        {0, 0},
    };
    double b[MAX_SIZE] = {1, 2};
    double x[MAX_SIZE];
    int n = 2;

    solve_linear_system(matrix, b, x, n);
    double nanValue = std::nan("");
    EXPECT_TRUE(std::isnan(nanValue));
}
