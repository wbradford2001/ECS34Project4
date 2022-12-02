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
TEST(CDijkstraPathRouterTest, AddEdgeRegular){
    CDijkstraPathRouter DijkstraPathRouter;
    int tag1Index = DijkstraPathRouter.AddVertex("tag1");
    int tag2Index = DijkstraPathRouter.AddVertex("tag2");
    int tag3Index = DijkstraPathRouter.AddVertex("tag3");
    int tag4Index = DijkstraPathRouter.AddVertex("tag4"); 

    DijkstraPathRouter.AddEdge(tag1Index, tag2Index, 4, false);
}
TEST(CDijkstraPathRouterTest, AddEdgeRegular2){
    CDijkstraPathRouter DijkstraPathRouter;
    int tag1Index = DijkstraPathRouter.AddVertex("tag1");
    int tag2Index = DijkstraPathRouter.AddVertex("tag2");
    int tag3Index = DijkstraPathRouter.AddVertex("tag3");
    int tag4Index = DijkstraPathRouter.AddVertex("tag4"); 

    DijkstraPathRouter.AddEdge(tag1Index, tag2Index, 4, false);
    DijkstraPathRouter.AddEdge(tag1Index, tag3Index, 3, false);    
    DijkstraPathRouter.AddEdge(tag2Index, tag3Index, 2, false);    
    DijkstraPathRouter.AddEdge(tag4Index, tag1Index, 1, false);    
}
TEST(CDijkstraPathRouterTest, AddEdgeBidir){
    CDijkstraPathRouter DijkstraPathRouter;
    int tag1Index = DijkstraPathRouter.AddVertex("tag1");
    int tag2Index = DijkstraPathRouter.AddVertex("tag2");
    int tag3Index = DijkstraPathRouter.AddVertex("tag3");
    int tag4Index = DijkstraPathRouter.AddVertex("tag4"); 

    DijkstraPathRouter.AddEdge(tag1Index, tag2Index, 4, true);
    DijkstraPathRouter.AddEdge(tag1Index, tag3Index, 3, true);    
    DijkstraPathRouter.AddEdge(tag2Index, tag3Index, 2, true);    
    DijkstraPathRouter.AddEdge(tag4Index, tag1Index, 1, true);    
}
TEST(CDijkstraPathRouterTest, Precompute1){
    CDijkstraPathRouter DijkstraPathRouter;
    int tag1Index = DijkstraPathRouter.AddVertex("tag1");
    int tag2Index = DijkstraPathRouter.AddVertex("tag2");
    int tag3Index = DijkstraPathRouter.AddVertex("tag3");
    int tag4Index = DijkstraPathRouter.AddVertex("tag4"); 

    DijkstraPathRouter.AddEdge(tag1Index, tag2Index, 4, true);
    DijkstraPathRouter.AddEdge(tag1Index, tag3Index, 3, true);    
    DijkstraPathRouter.AddEdge(tag2Index, tag3Index, 2, true);    
    DijkstraPathRouter.AddEdge(tag4Index, tag1Index, 1, true);  
    std::chrono::steady_clock::time_point t;
    DijkstraPathRouter.Precompute(t);


}

TEST(CDijkstraPathRouterTest, Precompute2){
    CDijkstraPathRouter DijkstraPathRouter;
    int tag1Index = DijkstraPathRouter.AddVertex("tag1");
    int tag2Index = DijkstraPathRouter.AddVertex("tag2");
    int tag3Index = DijkstraPathRouter.AddVertex("tag3");
    int tag4Index = DijkstraPathRouter.AddVertex("tag4"); 
    int tag5Index = DijkstraPathRouter.AddVertex("tag5");
    int tag6Index = DijkstraPathRouter.AddVertex("tag6");
    int tag7Index = DijkstraPathRouter.AddVertex("tag7");
    int tag8Index = DijkstraPathRouter.AddVertex("tag8");
    int tag9Index = DijkstraPathRouter.AddVertex("tag9");     

    DijkstraPathRouter.AddEdge(tag1Index, tag2Index, 4, true);
    DijkstraPathRouter.AddEdge(tag1Index, tag8Index, 8, true); 
    DijkstraPathRouter.AddEdge(tag2Index, tag8Index, 11, true); 
    DijkstraPathRouter.AddEdge(tag2Index, tag3Index, 8, true); 
    DijkstraPathRouter.AddEdge(tag8Index, tag7Index, 1, true); 
    DijkstraPathRouter.AddEdge(tag8Index, tag9Index, 7, true); 
    DijkstraPathRouter.AddEdge(tag3Index, tag9Index, 2, true); 
    DijkstraPathRouter.AddEdge(tag9Index, tag7Index, 6, true); 
    DijkstraPathRouter.AddEdge(tag3Index, tag6Index, 4, true); 
    DijkstraPathRouter.AddEdge(tag3Index, tag4Index, 7, true);
    DijkstraPathRouter.AddEdge(tag7Index, tag6Index, 2, true);
    DijkstraPathRouter.AddEdge(tag4Index, tag6Index, 14, true);
    DijkstraPathRouter.AddEdge(tag4Index, tag5Index, 9, true);
    DijkstraPathRouter.AddEdge(tag5Index, tag6Index, 10, true);

    std::chrono::steady_clock::time_point t;
    DijkstraPathRouter.Precompute(t);    
}

TEST(CDijkstraPathRouterTest, Precompute3){
    CDijkstraPathRouter DijkstraPathRouter;
    int tag1Index = DijkstraPathRouter.AddVertex("tag1");
    int tag2Index = DijkstraPathRouter.AddVertex("tag2");
    int tag3Index = DijkstraPathRouter.AddVertex("tag3");
    int tag4Index = DijkstraPathRouter.AddVertex("tag4"); 
 

    DijkstraPathRouter.AddEdge(tag1Index, tag2Index, 1, false);
    DijkstraPathRouter.AddEdge(tag2Index, tag3Index, 2, false); 
    DijkstraPathRouter.AddEdge(tag3Index, tag4Index, 20, false); 
    DijkstraPathRouter.AddEdge(tag1Index, tag4Index, 10, false); 


    std::chrono::steady_clock::time_point t;
    DijkstraPathRouter.Precompute(t);    
}
TEST(CDijkstraPathRouterTest, ShortestPath1){
    CDijkstraPathRouter DijkstraPathRouter;
    int tag1Index = DijkstraPathRouter.AddVertex("tag1");
    int tag2Index = DijkstraPathRouter.AddVertex("tag2");
    int tag3Index = DijkstraPathRouter.AddVertex("tag3");
    int tag4Index = DijkstraPathRouter.AddVertex("tag4"); 

    DijkstraPathRouter.AddEdge(tag1Index, tag2Index, 4, true);
    DijkstraPathRouter.AddEdge(tag1Index, tag3Index, 3, true);    
    DijkstraPathRouter.AddEdge(tag2Index, tag3Index, 2, true);    
    DijkstraPathRouter.AddEdge(tag4Index, tag1Index, 1, true);  
    std::chrono::steady_clock::time_point t;
    DijkstraPathRouter.Precompute(t);

    CPathRouter::TVertexID src;
    CPathRouter::TVertexID dest;
    std::vector<CPathRouter::TVertexID> path;



    src = 1;
    dest = 4;
    path.clear();
    EXPECT_EQ(1,DijkstraPathRouter.FindShortestPath(src, dest, path));
    EXPECT_EQ(path.size(),2);
    EXPECT_EQ(path[0],1);
    EXPECT_EQ(path[1],4);



    src = 4;
    dest = 1;
    path.clear();
    EXPECT_EQ(1,DijkstraPathRouter.FindShortestPath(src, dest, path));
    EXPECT_EQ(path.size(),2);
    EXPECT_EQ(path[0],4);
    EXPECT_EQ(path[1],1); 

    src = 1;
    dest = 2;
    path.clear();
    EXPECT_EQ(4,DijkstraPathRouter.FindShortestPath(src, dest, path));
    EXPECT_EQ(path.size(),2);
    EXPECT_EQ(path[0],1);
    EXPECT_EQ(path[1],2);     


    src = 2;
    dest = 1;
    path.clear();
    EXPECT_EQ(4,DijkstraPathRouter.FindShortestPath(src, dest, path));
    EXPECT_EQ(path.size(),2);
    EXPECT_EQ(path[0],2);
    EXPECT_EQ(path[1],1);  

    src = 3;
    dest = 2;
    path.clear();
    EXPECT_EQ(2,DijkstraPathRouter.FindShortestPath(src, dest, path));
    EXPECT_EQ(path.size(),2);
    EXPECT_EQ(path[0],3);
    EXPECT_EQ(path[1],2);         




}

TEST(CDijkstraPathRouterTest, ShortestPath2){
    CDijkstraPathRouter DijkstraPathRouter;
    int tag1Index = DijkstraPathRouter.AddVertex("tag1");
    int tag2Index = DijkstraPathRouter.AddVertex("tag2");
    int tag3Index = DijkstraPathRouter.AddVertex("tag3");
    int tag4Index = DijkstraPathRouter.AddVertex("tag4"); 
    int tag5Index = DijkstraPathRouter.AddVertex("tag5");
    int tag6Index = DijkstraPathRouter.AddVertex("tag6");
    int tag7Index = DijkstraPathRouter.AddVertex("tag7");
    int tag8Index = DijkstraPathRouter.AddVertex("tag8");
    int tag9Index = DijkstraPathRouter.AddVertex("tag9");     

    DijkstraPathRouter.AddEdge(tag1Index, tag2Index, 4, true);
    DijkstraPathRouter.AddEdge(tag1Index, tag8Index, 8, true); 
    DijkstraPathRouter.AddEdge(tag2Index, tag8Index, 11, true); 
    DijkstraPathRouter.AddEdge(tag2Index, tag3Index, 8, true); 
    DijkstraPathRouter.AddEdge(tag8Index, tag7Index, 1, true); 
    DijkstraPathRouter.AddEdge(tag8Index, tag9Index, 7, true); 
    DijkstraPathRouter.AddEdge(tag3Index, tag9Index, 2, true); 
    DijkstraPathRouter.AddEdge(tag9Index, tag7Index, 6, true); 
    DijkstraPathRouter.AddEdge(tag3Index, tag6Index, 4, true); 
    DijkstraPathRouter.AddEdge(tag3Index, tag4Index, 7, true);
    DijkstraPathRouter.AddEdge(tag7Index, tag6Index, 2, true);
    DijkstraPathRouter.AddEdge(tag4Index, tag6Index, 14, true);
    DijkstraPathRouter.AddEdge(tag4Index, tag5Index, 9, true);
    DijkstraPathRouter.AddEdge(tag5Index, tag6Index, 10, true);

    std::chrono::steady_clock::time_point t;
    DijkstraPathRouter.Precompute(t);    


    CPathRouter::TVertexID src;
    CPathRouter::TVertexID dest;
    std::vector<CPathRouter::TVertexID> path;



    src = 1;
    dest = 2;
    path.clear();
    EXPECT_EQ(4,DijkstraPathRouter.FindShortestPath(src, dest, path));
    EXPECT_EQ(path.size(),2);
    EXPECT_EQ(path[0],1);
    EXPECT_EQ(path[1],2);   


    src = 1;
    dest = 3;
    path.clear();
    EXPECT_EQ(12,DijkstraPathRouter.FindShortestPath(src, dest, path));
    EXPECT_EQ(path.size(),3);
    EXPECT_EQ(path[0],1);
    EXPECT_EQ(path[1],2);
    EXPECT_EQ(path[2],3); 

    src = 5;
    dest = 3;
    path.clear();
    EXPECT_EQ(14,DijkstraPathRouter.FindShortestPath(src, dest, path));
    EXPECT_EQ(path.size(),3);
    EXPECT_EQ(path[0],5);
    EXPECT_EQ(path[1],6);
    EXPECT_EQ(path[2],3);          
}

// TEST(CDijkstraPathRouterTest, ShortestPath3){
//     CDijkstraPathRouter DijkstraPathRouter;
//     int tag1Index = DijkstraPathRouter.AddVertex("tag1");
//     int tag2Index = DijkstraPathRouter.AddVertex("tag2");
//     int tag3Index = DijkstraPathRouter.AddVertex("tag3");
//     int tag4Index = DijkstraPathRouter.AddVertex("tag4"); 
 

//     DijkstraPathRouter.AddEdge(tag1Index, tag2Index, 1, false);
//     DijkstraPathRouter.AddEdge(tag2Index, tag3Index, 2, false); 
//     DijkstraPathRouter.AddEdge(tag3Index, tag4Index, 20, false); 
//     DijkstraPathRouter.AddEdge(tag1Index, tag4Index, 10, false); 


//     std::chrono::steady_clock::time_point t;
//     DijkstraPathRouter.Precompute(t);  


//     CPathRouter::TVertexID src;
//     CPathRouter::TVertexID dest;
//     std::vector<CPathRouter::TVertexID> path;


//     src = 1;
//     dest = 2;
//     path.clear();
//     EXPECT_EQ(1,DijkstraPathRouter.FindShortestPath(src, dest, path));
//     EXPECT_EQ(path.size(),2);
//     EXPECT_EQ(path[0],1);
//     EXPECT_EQ(path[1],2);   

//     src = 2;
//     dest = 3;
//     path.clear();
//     EXPECT_EQ(2,DijkstraPathRouter.FindShortestPath(src, dest, path));
//     EXPECT_EQ(path.size(),2);
//     EXPECT_EQ(path[0],2);
//     EXPECT_EQ(path[1],3);   

//     src = 3;
//     dest = 1;
//     path.clear();
    
//     EXPECT_EQ(CPathRouter::NoPathExists, DijkstraPathRouter.FindShortestPath(src, dest, path));
           
// }

