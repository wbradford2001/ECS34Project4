#include "DijkstraPathRouter.h"
#include "PathRouter.h"
#include <unordered_map>
#include <unordered_set>
#include <limits.h>
#include <bits/stdc++.h>
struct CDijkstraPathRouter::SImplementation{
    struct Vertex{
            std::any tag;
            int ID;
            std::vector< std::pair< std::shared_ptr<Vertex>, double > > Nexts;
        };    

    std::unordered_map< int, std::shared_ptr<Vertex> > VertexHash; //Hash that maps vertex ID's to Vertex Objects
    int ID = 0;//Start with ID of 1

    std::vector<int> VList;//Vertex List

    //Map from Source to a Map which contains the distance to all nodes
    std::unordered_map<int, std::unordered_map<int, double> > VertexToPathLengths;


    //Map from Source to a Map which contains the path to all nodes
    std::unordered_map<int,  std::unordered_map<int, std::vector<int> > > VertexToPaths;
    SImplementation(){
    };

    std::size_t VertexCount() const noexcept{
        // Returns the number of vertices in the path router
        return VertexHash.size();
    };
    TVertexID AddVertex(std::any tag) noexcept{
        // Adds a vertex with the tag provided. The tag can be of any type
        std::shared_ptr<Vertex> newVertex = std::make_unique<Vertex>();
        newVertex->tag = tag;
        ID += 1;
        newVertex->ID = ID;
        VertexHash[ID] = newVertex;

        VList.push_back(ID);
        
        return ID;

    };
    std::any GetVertexTag(TVertexID id) const noexcept{
        // Gets the tag of the vertex specified by id if id is in the path router.
        // A std::any() is returned if id is not a valid vertex ID.
        if (VertexHash.at(id)){
            return VertexHash.at(id)->tag;
        }
        return std::any();
    };
    bool AddEdge(TVertexID src, TVertexID dest, double weight, bool bidir) noexcept{
        // Adds an edge between src and dest vertices with a weight of weight. If
        // bidir is set to true an additional edge between dest and src is added. If
        // src or dest nodes do not exist, or if the weight is negative the AddEdge
        // will return false, otherwise it returns true.
        if (!(VertexHash.at(src)) || !(VertexHash.at(dest)) || weight < 0){
            return false;
        }
        std::pair<std::shared_ptr<Vertex>, double> newPair;
        newPair.first = VertexHash.at(dest); 
        newPair.second = weight;
        //std::cout<<newPair.second<<std::endl;
        VertexHash.at(src)->Nexts.push_back(newPair);

        if (bidir==true){
            std::pair<std::shared_ptr<Vertex>, int> newPair;
            newPair.first = VertexHash.at(src); 
            newPair.second = weight;
            VertexHash.at(dest)->Nexts.push_back(newPair);
        }
        return true;

    };

    bool Precompute(std::chrono::steady_clock::time_point deadline) noexcept{
        // Allows the path router to do any desired precomputation up to the deadline
    

        //compute Dijkstra's for every vertex
        for (int i = 0; i < VList.size(); i ++){
            
            DijkStraAuxill(VList[i]);
        }
       
        //print all paths
        // for (int i = 0; i < VList.size(); i ++){
        //     std::cout<<"Starting at Vertex: "<<VList[i]<<std::endl;
        //     for (int j = 0; j < VList.size(); j++){
        //         std::cout<<"From "<<VList[i]<<" to "<< VList[j]<<" = "<<VertexToPathLengths[VList[i]][VList[j]] <<std::endl;
        //     }
        // }
        return true;
    };


    //Auxillary Dijkstra's Algorithm
    void DijkStraAuxill(TVertexID src){
        int n = VList.size();
        
        //distances[NodeID] = distance
        std::unordered_map<int, double> distances;
        std::unordered_map<int, std::vector<int> > paths;
        
        for (int i = 0; i<n; i++){
            distances[VList[i]] = INT_MAX;
            paths[VList[i]] = {};
        };


        //distance from source to itself is 0
        distances[src] = 0;
        paths[src] = {(int)src};

        

        std::unordered_set<int> already_seen_map; //If Vertex ID is in already_seen, we have finished processing it.
        
        //iterate until already_seen_map has all vertices
        int count = 0;
        while (already_seen_map.size()<n && count < n){
            count += 1;

            //find minimum distance vertex
            int minDist = INT_MAX;
            int minID = src;

            for (int vI = 0; vI < n; vI += 1){
                //check if vertex not in seen and has minimal distance
                if (already_seen_map.count(VList[vI])==0 && distances[VList[vI]]<minDist){
                    minDist = distances[VList[vI]];
                    minID = VList[vI];
                }
            }

            //Add minID to already seen map
            already_seen_map.insert(minID);
            //std::cout<<"Now processing Node: "<<minID<<std::endl;

            //updated distance value of all vertices adjacent to minID
            
            //iterate through all adjacent vertices
            std::vector< std::pair< std::shared_ptr<Vertex>, double > > Nexts = VertexHash[minID]->Nexts;
            for (int adjVI = 0; adjVI < Nexts.size(); adjVI++){
                
                int adjacentID = Nexts[adjVI].first->ID;
                
                //if current distance from adjacent vertices is more than distance to srouce from minID plus weight of edge, udpate distance
                if (distances[adjacentID] > (distances[minID] + Nexts[adjVI].second)){
                    distances[adjacentID] = distances[minID] + Nexts[adjVI].second;
                    paths[adjacentID].clear();
                    //add paths leading up to minID
                    for (int k = 0; k < paths[minID].size(); k ++){
                        paths[adjacentID].push_back(paths[minID][k]);
                    }
                    paths[adjacentID].push_back(adjacentID);
                }
            }

        }
   // print distances
    // for (int vI = 0; vI < n; vI ++){
        
    //     std::cout<<"Distance to vertex: "<<VList[vI]<<"= "<<distances[VList[vI]]<<std::endl;
    //     for (int pI = 0; pI < paths[VList[vI]].size(); pI ++){
    //         std::cout<<"   "<<paths[VList[vI]][pI]<<std::endl;
    //     }
    // }
    VertexToPathLengths[src] = distances;
    VertexToPaths[src] = paths;


    }
    double FindShortestPath(TVertexID src, TVertexID dest, std::vector<TVertexID> &path) noexcept{
        // Returns the path distance of the path from src to dest, and fills out path
        // with vertices. If no path exists NoPathExists is returned.
        std::cout<<"Finding path between "<<src<<" and "<<dest<<" which is "<<VertexToPathLengths[src][dest]<<std::endl;
        if (VertexHash.count(src)==0||VertexToPathLengths[src][dest]==INT_MAX){
            
            return NoPathExists;
        };
        
        double pathLength = VertexToPathLengths[src][dest];
        std::vector<int> pathIDs = VertexToPaths[src][dest];
        for (int i=0; i<pathIDs.size(); i++){
            path.push_back(pathIDs[i]);
        }
        return pathLength;

    };

};


CDijkstraPathRouter::CDijkstraPathRouter(){
    DImplementation = std::make_unique<SImplementation>();
};
CDijkstraPathRouter::~CDijkstraPathRouter(){

};

std::size_t CDijkstraPathRouter::VertexCount() const noexcept{
    return DImplementation->VertexCount();
};
CDijkstraPathRouter::TVertexID CDijkstraPathRouter::AddVertex(std::any tag) noexcept{
    return DImplementation->AddVertex(tag);
};
std::any CDijkstraPathRouter::GetVertexTag(TVertexID id) const noexcept{
    return DImplementation->GetVertexTag(id);
};
bool CDijkstraPathRouter::AddEdge(TVertexID src, TVertexID dest, double weight, bool bidir) noexcept{
    return DImplementation->AddEdge(src, dest, weight, bidir);

};

bool CDijkstraPathRouter::Precompute(std::chrono::steady_clock::time_point deadline) noexcept{
    return DImplementation->Precompute(deadline);
};

double CDijkstraPathRouter::FindShortestPath(TVertexID src, TVertexID dest, std::vector<TVertexID> &path) noexcept{
    return DImplementation->FindShortestPath(src, dest, path);

};


