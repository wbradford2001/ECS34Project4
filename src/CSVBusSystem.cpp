#include "CSVBusSystem.h"

#include <unordered_map>
#include <iostream>
#include <algorithm>

struct CCSVBusSystem::SImplementation{
    struct SStop : public CBusSystem::SStop{
            TStopID ThisStopID;
            CStreetMap::TNodeID ThisNodeID;
            ~SStop(){};
            TStopID ID() const noexcept{
                // Returns the stop id of the stop
                return ThisStopID;
            };
            CStreetMap::TNodeID NodeID() const noexcept{
                // Returns the node id of the bus stop
                return ThisNodeID;

            };
        };
    struct SRoute : public CBusSystem::SRoute{
            std::string name;
            std::vector <TStopID> Stops;
            ~SRoute(){};
            std::string Name() const noexcept{
                // Returns the name of the route
                return name;
            };

            std::size_t StopCount() const noexcept{
                // Returns the number of stops on the route
                return Stops.size();
            };
            TStopID GetStopID(std::size_t index) const noexcept{
                // Returns the stop id specified by the index, returns InvalidStopID if index
                // is greater than or equal to StopCount()
                return index < Stops.size() ? Stops[index] : InvalidStopID;
            };
        };

    std::vector<std::shared_ptr< SStop > > Stops;
    std::unordered_map< TStopID, std::shared_ptr< SStop > > MappedStops;

    std::vector<std::shared_ptr< SRoute> >Routes;
    std::unordered_map< std::string, std::shared_ptr< SRoute > > MappedRoutes;    

    SImplementation(std::shared_ptr<CDSVReader> stopsrc, std::shared_ptr<CDSVReader> routesrc){
        std::vector<std::string> stopsrow;
        std::vector<std::string> routesrow;

        while (stopsrc->ReadRow(stopsrow)){
            
            if (stopsrow[0]=="stop_id"){
                stopsrow.clear();
                continue;
            }
            auto newStop = std::make_shared<SStop>();
            
            
            newStop->ThisStopID = stoi(stopsrow[0]);
            
            newStop->ThisNodeID = stoi(stopsrow[1]);
            
            Stops.push_back(newStop);
            
            MappedStops[newStop->ID()] = newStop;
            
            stopsrow.clear();
            

        };

        std::unordered_map< std::string, std::vector< TStopID > > routeHash;  
        std::vector<std::string> routeNames;
        while (routesrc->ReadRow(routesrow)){
            if (routesrow[0]=="route"){
                routesrow.clear();
                continue;
            };    
            if (!(std::count(routeNames.begin(), routeNames.end(), routesrow[0]))){
                routeNames.push_back(routesrow[0]);
                routeHash[routesrow[0]];
            };


            routeHash[routesrow[0]].push_back(stoi(routesrow[1]));  
            
            routesrow.clear();  
        };

        for (int i=0; i < routeNames.size(); i++){
            
            auto newRoute = std::make_shared<SRoute>();
            newRoute->name = routeNames[i];
            for (int j = 0; j < routeHash[routeNames[i]].size(); j ++){
                newRoute->Stops.push_back(routeHash[routeNames[i]][j]);
            }


            Routes.push_back(newRoute);
            MappedRoutes[routeNames[i]] = newRoute;
        }


    }

    std::size_t StopCount() const noexcept{
        // Returns the number of stops in the system
        return Stops.size();

    };
    std::size_t RouteCount() const noexcept{
        // Returns the number of routes in the system
        return Routes.size();

    };


    std::shared_ptr<CBusSystem::SStop> StopByIndex(std::size_t index) const noexcept{
        // Returns the SStop specified by the index, nullptr is returned if index is
        // greater than equal to StopCount()
        return index < Stops.size() ? Stops[index] : nullptr ;

    };
    std::shared_ptr<CBusSystem::SStop> StopByID(TStopID id) const noexcept{
        // Returns the SStop specified by the stop id, nullptr is returned if id is
        // not in the stops
        auto Search = MappedStops.find(id);
        if(Search == MappedStops.end()){
            return nullptr;
        }
        return Search->second;  
    };
    std::shared_ptr<CBusSystem::SRoute> RouteByIndex(std::size_t index) const noexcept{
        // Returns the SRoute specified by the index, nullptr is returned if index is
        // greater than equal to RouteCount()
            return index < Routes.size() ? Routes[index] : nullptr;

    };
    std::shared_ptr<CBusSystem::SRoute> RouteByName(const std::string &name) const noexcept{
        // Returns the SRoute specified by the name, nullptr is returned if name is
        // not in the routes
        auto Search = MappedRoutes.find(name);
        if(Search == MappedRoutes.end()){
            return nullptr;
        }
        return Search->second; 
    };

        
};
  
CCSVBusSystem::CCSVBusSystem(std::shared_ptr< CDSVReader > stopsrc, std::shared_ptr< CDSVReader > routesrc){
    DImplementation = std::make_unique<SImplementation>(stopsrc, routesrc);
};
CCSVBusSystem::~CCSVBusSystem(){

};

std::size_t CCSVBusSystem::StopCount() const noexcept{
    return DImplementation->StopCount();
};
std::size_t CCSVBusSystem::RouteCount() const noexcept{
    return DImplementation->RouteCount();

};
std::shared_ptr<CBusSystem::SStop> CCSVBusSystem::StopByIndex(std::size_t index) const noexcept{
    return DImplementation->StopByIndex(index);

};
std::shared_ptr<CBusSystem::SStop> CCSVBusSystem::StopByID(TStopID id) const noexcept{
    return DImplementation->StopByID(id);
};
std::shared_ptr<CBusSystem::SRoute> CCSVBusSystem::RouteByIndex(std::size_t index) const noexcept{
    return DImplementation->RouteByIndex(index);
};
std::shared_ptr<CBusSystem::SRoute> CCSVBusSystem::RouteByName(const std::string &name) const noexcept{
    return DImplementation->RouteByName(name);

};

