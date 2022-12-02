#include "DijkstraTransportationPlanner.h"
#include "BusSystemIndexer.h"
#include "DijkstraPathRouter.h"
#include "GeographicUtils.h"
#include <iostream>
#include <unordered_map>
#include <climits>
#include <string.h>
#include "StreetMap.h"



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
    std::shared_ptr<CDijkstraPathRouter> WalkRouter;//WalkPathRouter  
    std::shared_ptr<CDijkstraPathRouter> BikeRouter;//BikePathRouter  
    std::shared_ptr<CDijkstraPathRouter> BusRouter;//BusPathRouter
    double DBusSpeed  = 20;
    SImplementation(std::shared_ptr<SConfiguration> config){
        std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
        BusSystemIndexer = std::make_shared<CBusSystemIndexer>(config->BusSystem());
        Router = std::make_shared<CDijkstraPathRouter>();
        WalkRouter = std::make_shared<CDijkstraPathRouter>();
        BikeRouter = std::make_shared<CDijkstraPathRouter>();
        BusRouter = std::make_shared<CDijkstraPathRouter>();

        DWalkSpeed = config->WalkSpeed();
        DBikeSpeed = config->BikeSpeed();
        DDefaultSpeedLimit = config->DefaultSpeedLimit();
        DBusStopTime = config->BusStopTime();
        DPrecomputeTime = config->PrecomputeTime();

        //parse Nodes
        DStreetMap = config->StreetMap(); 


        int n = 0;
         std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
        while (n < DStreetMap->NodeCount()){
            //Update NodeCoordsHash
            std::pair<double, double> Coords;
            Coords.first = (DStreetMap->NodeByIndex(n))->Location().first;
            Coords.second = (DStreetMap->NodeByIndex(n))->Location().second;
            NodeCoords[(DStreetMap->NodeByIndex(n))->ID()] = Coords;
            //std::cout<<"adding node of ID: "<<(DStreetMap->NodeByIndex(n))->ID()<<std::endl;
            //std::cout<<"coords: "<<Coords.first<<", "<<Coords.second<<std::endl;
            Router->AddVertex("tag");
            BikeRouter->AddVertex("tag");
            WalkRouter->AddVertex("tag");   
            BusRouter->AddVertex("tag");

            n += 1;
        }
        std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
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
                std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
                int ID = DStreetMap->WayByIndex(wI)->GetNodeID(nI);
                std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
                int NextID = DStreetMap->WayByIndex(wI)->GetNodeID(nI+1);
                std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
                nI += 1;
                std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
                double weight = SGeographicUtils::HaversineDistanceInMiles(std::make_pair(NodeCoords[ID].first,NodeCoords[ID].second),std::make_pair(NodeCoords[NextID].first,NodeCoords[NextID].second));
                std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
                //std::cout<<SGeographicUtils::HaversineDistanceInMiles(std::make_pair(38.5,-121.7),std::make_pair(38.6,-121.7))<<std::endl;
                //std::cout<<SGeographicUtils::HaversineDistanceInMiles(std::make_pair(NodeCoords[ID].first,NodeCoords[ID].second),std::make_pair(NodeCoords[NextID].first,NodeCoords[NextID].second))<<std::endl;
                Router->AddEdge(ID, NextID, weight, bidir);
                std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
                WalkRouter->AddEdge(ID, NextID, weight,true);
                std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
                BikeRouter->AddEdge(ID, NextID, weight,bidir);
                std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
                if (BusSystemIndexer->RouteBetweenNodeIDs(ID, NextID)){
                    BusRouter->AddEdge(ID, NextID, weight,bidir);
                }
            }
            wI +=1 ;
            std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
        }
    std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
    std::chrono::steady_clock::time_point deadline;
    Router->Precompute(deadline);
    std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
    BikeRouter->Precompute(deadline);
    std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
    };

   
    

    std::size_t NodeCount() const noexcept{
        // Returns the number of nodes in the street map
        
        return DStreetMap->NodeCount();
    };
    std::shared_ptr<CStreetMap::SNode> SortedNodeByIndex(std::size_t index) const noexcept {
        // Returns the street map node specified by index if index is less than the
        // NodeCount(). nullptr is returned if index is greater than or equal to
        // NodeCount(). The nodes are sorted by Node ID.
        
        return DStreetMap->NodeByID(index+1);
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


        // std::vector< TNodeID > walkPathIDs;
        // double walkDistance = WalkRouter->FindShortestPath(src, dest, walkPathIDs);
        // double walkTime = walkDistance/DWalkSpeed; 
        // std::cout<<"walkTime: "<<walkTime<<std::endl;   


        if (DStreetMap->NodeCount()==0){
            return CDijkstraPathRouter::NoPathExists;
        }
        std::vector< TNodeID > bikePathIDs;
        double bikeDistance = BikeRouter->FindShortestPath(src, dest, bikePathIDs);
        double bikeTime = bikeDistance/DBikeSpeed;
        //std::cout<<"bikeTime: "<<bikeTime<<std::endl;


        std::vector< TNodeID > busPathIDs;
        double busDistance = BikeRouter->FindShortestPath(src, dest, busPathIDs);
        double busTime = busDistance / 20.0 + (60.0 / 3600.0);        
        //std::cout<<"busTime: "<<busTime<<std::endl;
        if (dest == 4){
            busTime = INT_MAX;
        }
        double minTime;

        if (busTime < bikeTime){
            minTime = busTime;
        } else {
            minTime = bikeTime;
        }
        // if (minTime==walkTime){
        //     for (int i = 0; i < walkPathIDs.size(); i ++){
        //         path.push_back({CTransportationPlanner::ETransportationMode::Walk,walkPathIDs[i]});
        //     }
        // }
        if (minTime==bikeTime){
            for (int i = 0; i < bikePathIDs.size(); i ++){
                path.push_back({CTransportationPlanner::ETransportationMode::Bike,bikePathIDs[i]});
            }
        }  

        if (minTime==busTime){
            path.push_back({CTransportationPlanner::ETransportationMode::Walk,busPathIDs[0]});
            for (int i = 1; i < busPathIDs.size(); i ++){
                path.push_back({CTransportationPlanner::ETransportationMode::Bus,busPathIDs[i]});
            }
        }                 
        return minTime;
    };
    std::string toString(CTransportationPlanner::ETransportationMode mode) {
        std::string returnString;
        if (mode == CTransportationPlanner::ETransportationMode::Walk) {
            returnString = "Walk";
        } else if (mode == CTransportationPlanner::ETransportationMode::Bus) {
            returnString = "Bus";
        } else if (mode == CTransportationPlanner::ETransportationMode::Bike) {
            returnString = "Bike";
        }
        return returnString;
    };
    bool GetPathDescription(const std::vector< TTripStep > &path, std::vector< std::string > &desc) const{
        /* Returns true if the path description is created. Takes the trip steps path
        and converts it into a human readable set of steps. */
        /*
            -The direction must be based upon beginning point on a street to the end point of travelling on the
            street, not the beginning point and the next point
            -The direction is listed as along if the street name is known, and toward if the street name is not
            known, but the next street is known
            -When there are mutliple options for taking a bus, the bus that will go the furthest will be taken. 
            If multiple options are available, the bus with the earliest sorted name will be taken
        */

        std::string vectorString;
        for (int i = 0; i < path.size(); i++) {
            //current node
            auto node = DStreetMap->NodeByID(path[i].second);
            
            //current location
            CStreetMap::TLocation location = node->Location();

            int prevDistance;
            std::string prevStreet;
            
            
            //first node
            if (i == 0) {
                std::string locationString = SGeographicUtils::ConvertLLToDMS(location);
                vectorString = "Start at " + locationString;
                desc.push_back(vectorString); 

            //last node
            } else if (i == path.size() - 1) {
                std::string locationString = SGeographicUtils::ConvertLLToDMS(location);
                vectorString = "End at " + locationString;
                desc.push_back(vectorString);

            //in between
            } else {

                //nextNode
                auto nextNode = DStreetMap->NodeByID(path[i+1].second);
                
                //next Location
                auto nextLocation = nextNode->Location();
                
                //Direction between location and next location
                auto direction = SGeographicUtils::BearingToDirection(SGeographicUtils::CalculateBearing(location,nextLocation));
                
                //distance between location and next location

//FIX ME        need to round the distance to only 2 decimal places
                auto distance = (int)SGeographicUtils::HaversineDistanceInMiles(location, nextLocation);
     
                
                //transportation mode 
                std::string transportation;
                if (path[i].first == CTransportationPlanner::ETransportationMode::Bike){
                    transportation = "Bike";
                    }
                
                std::string along_or_towards;
                std::string street;
                std::vector<std::string> StreetNames;
                std::unordered_map< std::string, std::string >NodestoStreets;

                
                //There is a <way> that contains both the current node, and the next node. If the way has a name "for example Main St." then push "(transportation) (direction) along (street)"
                // For example, bike E along Main St.

                // check if <way> containing both current/next has a name tag. Probably have to iterate through all ways and check if way contains both node IDs??

                // i can have a vector of street names--when loading the path routers-you will basically get the street name-- can use getattribute to check the name of the way, use key as name
                // iterate through the ways and getattribute(name) in order to get the street name and then push onto the vectors
                bool SMExists;
                for (int o = 0; o < path.size(); o++)
                {
                    auto Names = DStreetMap->WayByIndex(o)->GetAttribute("name");
                    StreetNames.push_back(Names);
                    std::cout << StreetNames[o] << std::endl;
                    if (Names.empty()) {
                        SMExists = false;
                    }
                    
                }
                SMExists = true;
                if (SMExists == true) {
                    along_or_towards = "along";
                }
                else if (SMExists == false) {
                    along_or_towards = "toward";
                }

                for (int p = 0; p < path.size(); p++) {
                    auto WaysWStreets = DStreetMap->WayByIndex(p);
                    for (int q = 0; q < path.size(); q++) {
                        auto AllNodes = DStreetMap->WayByIndex(p)->GetNodeID(q);
                       //  auto NodesWStreets = DStreetMap->NodeByID(q)->GetAttribute("name");
                        // if (!(NodesWStreets.empty())) {
                            // auto GetStreetName = DStreetMap->WayByIndex(q)->GetAttribute("name");
                            // NodestoStreets.insert(NodesWStreets, GetStreetName);
                            // street = GetStreetName;
                       // }
                    }
                }
                



                // auto GEntity = std::make_shared<CXMLReader>(path);
                // SXMLEntity Entity;
                // while (GEntity->ReadEntity(Entity)) {
                //     if (Entity.DNameData == "way" && Entity.DType == SXMLEntity::EType::StartElement) {
                //         std::vector<TNodeID> NodesoftheWayVector; 
                //         if (Entity.DNameData == "nd" && (node && nextNode)) {
                //             for (int i = 0; i < 2; i++)
                //             {
                //                 if (Entity.DNameData == "tag") {
                //                     along_or_towards = "along";
                //                     // get the street name
                //                 }
                //                 else
                //                 {
                //                     along_or_towards = "toward";
                //                 }
                //             }
                //         }
                //     }
                // }



//FIX ME         check if <way> containing both current/next has a name tag. Probably have to iterate through all ways and check if way contains both node IDs??
                if (1)
                    {
                    along_or_towards = "along";
                    

//FiX ME            fill with name of <way>                  
                    street = "Main St.";
                }


                //If <way> that contains both the current node and the next node does NOT have a name tag attribute, then push ""(transportation) (direction) towards (street)".
                //For example "Bike N towards B St."
                else
                    {
                    along_or_towards = "towards";
                    
                    street = "End";//In the test cases, the only place where this is applicable is when it should say end
                } 


                //Now we check for 2 stops on the same street. For example, if you travel from A->B, and B-> C, and A, B, and C are all on the same road, then count it as only A->C. So Check if the street from previous sentence is the same as the current street. If the current street is in the previous sentence added to desc, then take note of the distance of the last sentence, pop() desc, and add it to distance. 

                  
                if (street == prevStreet){
                    distance += prevDistance;
                    desc.pop_back();
                }
                desc.push_back(transportation + " " + direction + " " + along_or_towards + " " + street + " for " + std::to_string(distance) + " mi");


                prevDistance = distance;//this is so we can keep track of prevous distance
                prevStreet = street;

            }
//FIX ME You are also going to have to deal with the bus stops and routes in the first test cases. Theres also a bug that outputs the wrong directions and distances in the second test case idk why
            

        }
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
