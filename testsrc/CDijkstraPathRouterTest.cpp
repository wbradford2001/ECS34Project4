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
    CDijkstraPathRouter DijkstraPathRouter;
    int tag1Index = DijkstraPathRouter.AddVertex("tag1");
    int tag2Index = DijkstraPathRouter.AddVertex("tag2");

    std::any tag1 = DijkstraPathRouter.GetVertexTag(tag1Index);
    std::any tag2 = DijkstraPathRouter.GetVertexTag(tag2Index);


    EXPECT_EQ("tag1",std::any_cast<const char*>(tag1));
    EXPECT_EQ("tag2",std::any_cast<const char*>(tag2));
 
    

}
TEST(CDijkstraPathRouterTest, GetVertexTag2){
    CDijkstraPathRouter DijkstraPathRouter;
    int tag1Index = DijkstraPathRouter.AddVertex("tag1");
    int tag2Index = DijkstraPathRouter.AddVertex("tag2");
    int tag3Index = DijkstraPathRouter.AddVertex(2);
    int tag4Index = DijkstraPathRouter.AddVertex(3);    

    std::any tag1 = DijkstraPathRouter.GetVertexTag(tag1Index);
    std::any tag2 = DijkstraPathRouter.GetVertexTag(tag2Index);
    std::any tag3 = DijkstraPathRouter.GetVertexTag(tag3Index);
    std::any tag4 = DijkstraPathRouter.GetVertexTag(tag4Index);    


    EXPECT_EQ("tag1",std::any_cast<const char*>(tag1));
    EXPECT_EQ("tag2",std::any_cast<const char*>(tag2));  
    EXPECT_EQ(2,std::any_cast<int>(tag3));
    EXPECT_EQ(3,std::any_cast<int>(tag4));     
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


