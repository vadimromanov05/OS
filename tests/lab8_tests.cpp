#include <gtest/gtest.h>
#include "../CP/include/DAG.h"
#include "../CP/include/Job.h"
#include <sstream>


TEST(DAGTest, ValidateDAG) {
    DAG dag("dag1.ini");
    if (!dag.is_valid()) {
        std::cout << "DAG is invalid!" << std::endl;
    }
    EXPECT_TRUE(dag.is_valid());
}

TEST(DAGTest, CycleforDAG) {
    DAG dag("dag2.ini");
    EXPECT_FALSE(dag.is_valid());
}

TEST(DAGTest, NoStartJobsDAG) {
    DAG dag("dag3.ini");
    EXPECT_FALSE(dag.is_valid());
}

TEST(DAGTest, MultipleComponentsDAG) {
    DAG dag("dag4.ini");
    EXPECT_TRUE(dag.is_valid());
}

TEST(DAGTest, SingleJobDAG) {
    DAG dag("dag5.ini");
    EXPECT_TRUE(dag.is_valid());
}