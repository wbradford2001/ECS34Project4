#include "BusSystemIndexer.h"
#include "CSVBusSystem.h"
#include <bits/stdc++.h>

struct CBusSystemIndexer::SImplementation{
    std::shared_ptr<CBusSystem> system;
    std::vector<int> StopIDs; //StopIDs to be sorted
    std::unordered_map<CStreetMap::TNodeID, CStreetMap::TNodeID>NodeToStopHash; //Hash from NodeID to StopID

    //Sorted Map of routes -> set of Stops
    std::unordered_map<std::string, std::unordered_set<int> > RouteToStopHash;   

    //vector of route names
    std::vector<std::string> RouteNamesVector; 
    SImplementation(std::shared_ptr<CBusSystem> bussystem){
       system = bussystem;

       int sI=0;
       //Iterate Through Stops
       while (bussystem->StopByIndex(sI)){
        
        StopIDs.push_back(bussystem->StopByIndex(sI)->ID());
        NodeToStopHash[bussystem->StopByIndex(sI)->NodeID()] = bussystem->StopByIndex(sI)->ID();

        sI += 1;
       }

       sort(StopIDs.begin(), StopIDs.end());


 


        //iterate through Routes
        int rI=0;

        while (bussystem->RouteByIndex(rI)){

            //route name
            std::string name = bussystem->RouteByIndex(rI)->Name();
            RouteNamesVector.push_back(name);

            //std::cout<<"Route: "<<name<<std::endl;
            std::unordered_set<int> stops;

            //populate stops for route
            int rsI = 0;
            while (bussystem->RouteByIndex(rI)->GetStopID(rsI) != CCSVBusSystem::InvalidStopID){
                int curStopID = bussystem->RouteByIndex(rI)->GetStopID(rsI);
                //std::cout<<"Stop: "<<curStopID<<std::endl;
                stops.insert(curStopID);
                rsI += 1;
            }
            RouteToStopHash[name]=stops;
            rI += 1;
       }

    //Sort RouteNames vector
    sort(RouteNamesVector.begin(), RouteNamesVector.end());




    }

    std::size_t StopCount() const noexcept{

        // Returns the number of stops in the CBusSystem being indexed
        return system->StopCount();
        
    };
    std::size_t RouteCount() const noexcept{
        std::size_t count = system->RouteCount();
         // Returns the number of routes in the CBusSystem being indexed
        return count;
    };
    std::shared_ptr<SStop> SortedStopByIndex(std::size_t index) const noexcept{
    // Returns the SStop specified by the index where the stops are sorted by
    // their ID, nullptr is returned if index is greater than equal to
    // StopCount()       
        if (index >= system->StopCount()){
            //std::cout<<"sfg: "<<system->StopCount()<<std::endl;
            return nullptr;
        } 
        
        int ID = StopIDs[index];
        return system->StopByID(ID);
    };
    std::shared_ptr<SRoute> SortedRouteByIndex(std::size_t index) const noexcept{
    // Returns the SRoute specified by the index where the routes are sorted by
    // their Name, nullptr is returned if index is greater than equal to
    // RouteCount()        
        if (index >= system->RouteCount()){
            return nullptr;
        } 
        
        std::string Name = RouteNamesVector[index];
        return system->RouteByName(Name);
    };
    std::shared_ptr<SStop> StopByNodeID(TNodeID id) const noexcept{
        // Returns the SStop associated with the specified node ID, nullptr is
        // returned if no SStop associated with the node ID exists  

        int StopID = NodeToStopHash.at(id);
        std::shared_ptr<SStop> newStop = system->StopByID(StopID);
        return newStop;
    };

    bool RoutesByNodeIDs(TNodeID src, TNodeID dest, std::unordered_set<std::shared_ptr<SRoute> > &routes) const noexcept{
        // Returns true if at least one route exists between the stops at the src and
        // dest node IDs. All routes that have a route segment between the stops at
        // the src and dest nodes will be placed in routes unordered set.   

        bool contains = false;     
        
        for (int i=0; i < RouteNamesVector.size(); i++){
            std::string curRouteName = RouteNamesVector[i];
            //std::cout<<"ROUTE NAME: "<<curRouteName<<std::endl;
            std::unordered_set<int> set= RouteToStopHash.at(curRouteName);
            if (!set.count(src) && !set.count(dest)){
                contains = true;
                routes.insert(system->RouteByName(curRouteName));
                //std::cout<<"InSERT HAPPEND"<<std::endl;
            }
        }
        return contains;
    };

    bool RouteBetweenNodeIDs(TNodeID src, TNodeID dest) const noexcept{
        // Returns true if at least one route exists between the stops at the src and
        // dest node IDs.
        for (int i=0; i < RouteNamesVector.size(); i++){
            std::string curRouteName = RouteNamesVector[i];
            std::unordered_set<int> set= RouteToStopHash.at(curRouteName);
            if (set.count(src) && set.count(dest)){
                return true;
            }
        }
        return false;
    };    
};

CBusSystemIndexer::CBusSystemIndexer(std::shared_ptr<CBusSystem> bussystem){
    /*
        Input: shared pointer to an instance of bussystem.
        Output: makes a unique pointer to an instance of SImplementation for BusSystemIndexer.
    */
    DImplementation = std::make_unique<SImplementation>(bussystem);
};

CBusSystemIndexer::~CBusSystemIndexer(){
    /*
        Destructor...
    */
};

std::size_t CBusSystemIndexer::StopCount() const noexcept{
    /*
        Returns the number of stops in the CBusSystem being indexed.
        (CBusSystem takes in 2 DSVReaders for route and stop)
    */
    return DImplementation->StopCount();
};
std::size_t CBusSystemIndexer::RouteCount() const noexcept{
    /*
        Returns the number of routes in the CBusSystem being indexed.
    */
   
    return DImplementation->RouteCount();
};

std::shared_ptr<CBusSystem::SStop> CBusSystemIndexer::SortedStopByIndex(std::size_t index) const noexcept{
    /*
        Input: an index
        ~Sorted vector of SStops (based on IDs)~
        Output: null ptr (if index >= StopCount())
                corresponding instance of SStop (otherwise)
    */

    return DImplementation->SortedStopByIndex(index);
};

std::shared_ptr<CBusSystem::SRoute> CBusSystemIndexer::SortedRouteByIndex(std::size_t index) const noexcept{
    /*
        Input: an index
        ~Sorted vector of SRoutes (based on names)~
        Output: null ptr (if index >= StopCount())
                corresponding instance of SRoute (otherwise)
    */
    return DImplementation->SortedRouteByIndex(index);
};

std::shared_ptr<CBusSystem::SStop> CBusSystemIndexer::StopByNodeID(TNodeID id) const noexcept{
    /*
        Input: an ID.
        Output: corresponding instance of SStop.
    */
    return DImplementation->StopByNodeID(id);
};

bool CBusSystemIndexer::RoutesByNodeIDs(TNodeID src, TNodeID dest, std::unordered_set<std::shared_ptr<SRoute> > &routes) const noexcept{
    /*
        Input: Node ID for source, Node ID for destination, unordered_set for storing SRoutes
        ~ All routes that have a route segment between the stops at the src and dest will be placed in the routes unordered set~
        Output: true (if at least 1 route exits)
                false (otherwise)
    */
    return DImplementation->RoutesByNodeIDs(src, dest, routes);
};

bool CBusSystemIndexer::RouteBetweenNodeIDs(TNodeID src, TNodeID dest) const noexcept{
    /*
        Returns true if at least one route exists between the stops at the src and dest node IDs.
    */
    return DImplementation->RouteBetweenNodeIDs(src,  dest);
};

