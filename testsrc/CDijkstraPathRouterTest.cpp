#include <gtest/gtest.h>
#include "XMLReader.h"
#include "StringUtils.h"
#include "StringDataSource.h"
#include "DSVReader.h"
#include "CSVBusSystem.h"
#include "BusSystemIndexer.h"
#include "DijkstraPathRouter.h"

TEST(CDijkstraPathRouterTest, SimpleTest){
    CDijkstraPathRouter DijkstraPathRouter;

}

TEST(CDijkstraPathRouterTest, VertexCount1){
    CDijkstraPathRouter DijkstraPathRouter;
    DijkstraPathRouter.AddVertex("tag1");
    DijkstraPathRouter.AddVertex("tag2");

    EXPECT_EQ(2,DijkstraPathRouter.VertexCount());
}

TEST(CDijkstraPathRouterTest, VertexCount2){
    CDijkstraPathRouter DijkstraPathRouter;
    DijkstraPathRouter.AddVertex("tag1");
    DijkstraPathRouter.AddVertex("tag2");
    DijkstraPathRouter.AddVertex(2);
    DijkstraPathRouter.AddVertex(134);

    EXPECT_EQ(4,DijkstraPathRouter.VertexCount());
}
TEST(CDijkstraPathRouterTest, AddVertex1){
    CDijkstraPathRouter DijkstraPathRouter;
    DijkstraPathRouter.AddVertex("tag1");
    DijkstraPathRouter.AddVertex("tag2");
    DijkstraPathRouter.AddVertex(2);
    DijkstraPathRouter.AddVertex(134);
    EXPECT_EQ(0,0);
}
TEST(CDijkstraPathRouterTest, AddVertex2){
    CDijkstraPathRouter DijkstraPathRouter;

    DijkstraPathRouter.AddVertex(2);
    DijkstraPathRouter.AddVertex(134);
    EXPECT_EQ(0,0);
}
TEST(CDijkstraPathRouterTest, GetVertexTag1){

    EXPECT_EQ(0,0);
}
TEST(CDijkstraPathRouterTest, GetVertexTag2){

    EXPECT_EQ(0,0);
}
TEST(CDijkstraPathRouterTest, AddEdge1){

    EXPECT_EQ(0,0);
}
TEST(CDijkstraPathRouterTest, AddEdge2){

    EXPECT_EQ(0,0);
}
TEST(CDijkstraPathRouterTest, Precompute){

    EXPECT_EQ(0,0);
}
TEST(CDijkstraPathRouterTest, FindShortestPath1){

    EXPECT_EQ(0,0);
}
TEST(CDijkstraPathRouterTest, FindShortestPath2){

    EXPECT_EQ(0,0);
}
TEST(CDijkstraPathRouterTest, FindShortestPath3){

    EXPECT_EQ(0,0);
}


