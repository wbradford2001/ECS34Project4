#include "DijkstraTransportationPlanner.h"
#include "BusSystemIndexer.h"
#include "DijkstraPathRouter.h"
#include "GeographicUtils.h"
#include <iostream>
#include <unordered_map>



struct CDijkstraTransportationPlanner::SImplementation{
    std::shared_ptr<CStreetMap> DStreetMap;
    std::shared_ptr<CBusSystemIndexer> BusSystemIndexer;
    
    //CBusSystemIndexer BusSystem;
    double DWalkSpeed;
    double DBikeSpeed;
    double DDefaultSpeedLimit;
    double DBusStopTime;
    int DPrecomputeTime; 


    std::unordered_map<int , std::pair<double,double> > NodeCoords;//Map that stores coordinates of nodes 


    std::shared_ptr<CDijkstraPathRouter> Router;//PathRouter  
    SImplementation(std::shared_ptr<SConfiguration> config){
        
        BusSystemIndexer = std::make_shared<CBusSystemIndexer>(config->BusSystem());
        Router = std::make_shared<CDijkstraPathRouter>();

        DWalkSpeed = config->WalkSpeed();
        DBikeSpeed = config->BikeSpeed();
        DDefaultSpeedLimit = config->DefaultSpeedLimit();
        DBusStopTime = config->BusStopTime();
        DPrecomputeTime = config->PrecomputeTime();

        //parse Nodes
        DStreetMap = config->StreetMap(); 


        int n = 0;
        while (n < DStreetMap->NodeCount()){
            //Update NodeCoordsHash
            std::pair<double, double> Coords;
            Coords.first = (DStreetMap->NodeByIndex(n))->Location().first;
            Coords.second = (DStreetMap->NodeByIndex(n))->Location().second;
            NodeCoords[(DStreetMap->NodeByIndex(n))->ID()] = Coords;
            //std::cout<<"adding node of ID: "<<(DStreetMap->NodeByIndex(n))->ID()<<std::endl;
            //std::cout<<"coords: "<<Coords.first<<", "<<Coords.second<<std::endl;
            
            Router->AddVertex("tag");

            n += 1;
        }

        //parse Ways
        int wI = 0;
        bool bidir = true;
        //std::cout<<"\n\nWayCount: "<<DStreetMap->WayCount()<<std::endl;
        while (wI < DStreetMap->WayCount()){
            int nI = 0;
            //std::cout<<"For Way: "<<(DStreetMap->WayByIndex(wI))->ID()<<std::endl;
            if ((DStreetMap->WayByIndex(wI))->HasAttribute("oneway") && (DStreetMap->WayByIndex(wI))->GetAttribute("oneway")=="yes"){
                //std::cout<<"One Way!!"<<std::endl;
                bidir = false;
            }
            while (nI < DStreetMap->WayByIndex(wI)->NodeCount()-1){
                int ID = DStreetMap->WayByIndex(wI)->GetNodeID(nI);
                int NextID = DStreetMap->WayByIndex(wI)->GetNodeID(nI+1);
                nI += 1;
                
                double weight = SGeographicUtils::HaversineDistanceInMiles(std::make_pair(NodeCoords[ID].first,NodeCoords[ID].second),std::make_pair(NodeCoords[NextID].first,NodeCoords[NextID].second));
                
                //std::cout<<SGeographicUtils::HaversineDistanceInMiles(std::make_pair(38.5,-121.7),std::make_pair(38.6,-121.7))<<std::endl;
                //std::cout<<SGeographicUtils::HaversineDistanceInMiles(std::make_pair(NodeCoords[ID].first,NodeCoords[ID].second),std::make_pair(NodeCoords[NextID].first,NodeCoords[NextID].second))<<std::endl;
                Router->AddEdge(ID, NextID, weight,bidir);
                //std::cout<<"edge from "<<ID<<" with coords "<<NodeCoords[ID].first<<", "<<NodeCoords[ID].second<<" to "<<NextID<<" with coords "<<NodeCoords[NextID].first<<", "<<NodeCoords[NextID].second<<" with weight "<<weight<<std::endl;
            }
            wI +=1 ;
        }
    std::chrono::steady_clock::time_point deadline;
    Router->Precompute(deadline);
    };

   
    

    std::size_t NodeCount() const noexcept{
        // Returns the number of nodes in the street map
        
        return DStreetMap->NodeCount();
    };
    std::shared_ptr<CStreetMap::SNode> SortedNodeByIndex(std::size_t index) const noexcept {
        // Returns the street map node specified by index if index is less than the
        // NodeCount(). nullptr is returned if index is greater than or equal to
        // NodeCount(). The nodes are sorted by Node ID.
        
        return DStreetMap->NodeByIndex(index);
    };

    double FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path) {
        // Returns the distance in miles between the src and dest nodes of the
        // shortest path if one exists. NoPathExists is returned if no path exists.
        // The nodes of the shortest path are filled in the path parameter.
        double p = Router->FindShortestPath(src, dest, path);
        return p;
    };

    double FindFastestPath(TNodeID src, TNodeID dest, std::vector< TTripStep > &path) {
        // Returns the time in hours for the fastest path between the src and dest
        // nodes of the if one exists. NoPathExists is returned if no path exists.
        // The transportation mode and nodes of the fastest path are filled in the
        // path parameter.        
        double p;
        return p;
    };
    bool GetPathDescription(const std::vector< TTripStep > &path, std::vector< std::string > &desc) const{
        // Returns true if the path description is created. Takes the trip steps path
        // and converts it into a human readable set of steps.
        return true;
    };
};



CDijkstraTransportationPlanner::CDijkstraTransportationPlanner(std::shared_ptr<SConfiguration> config){
    DImplementation = std::make_unique<SImplementation>(config);
};
CDijkstraTransportationPlanner::~CDijkstraTransportationPlanner(){};

std::size_t CDijkstraTransportationPlanner::NodeCount() const noexcept{
    return DImplementation->NodeCount();
};
std::shared_ptr<CStreetMap::SNode> CDijkstraTransportationPlanner::SortedNodeByIndex(std::size_t index) const noexcept {
    return DImplementation->SortedNodeByIndex(index);
};

double CDijkstraTransportationPlanner::FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path) {
    return DImplementation->FindShortestPath(src, dest, path);
};
double CDijkstraTransportationPlanner::FindFastestPath(TNodeID src, TNodeID dest, std::vector< TTripStep > &path) {
    return DImplementation->FindFastestPath(src, dest, path);
};
bool CDijkstraTransportationPlanner::GetPathDescription(const std::vector< TTripStep > &path, std::vector< std::string > &desc) const {
    return DImplementation->GetPathDescription(path, desc);
};
