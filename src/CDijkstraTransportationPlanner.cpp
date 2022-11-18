#include "DijkstraTransportationPlanner.h"



struct CDijkstraTransportationPlanner::SImplementation{
    SImplementation(std::shared_ptr<SConfiguration> config){

    }

    std::size_t NodeCount() const noexcept{
        // Returns the number of nodes in the street map
        std::size_t newSize;
        return newSize;
    };
    std::shared_ptr<CStreetMap::SNode> SortedNodeByIndex(std::size_t index) const noexcept {
        // Returns the street map node specified by index if index is less than the
        // NodeCount(). nullptr is returned if index is greater than or equal to
        // NodeCount(). The nodes are sorted by Node ID.
        std::shared_ptr<CStreetMap::SNode> node;
        return node;
    };

    double FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path) {
        // Returns the distance in miles between the src and dest nodes of the
        // shortest path if one exists. NoPathExists is returned if no path exists.
        // The nodes of the shortest path are filled in the path parameter.
        double hi;
        return hi;
    };

    double FindFastestPath(TNodeID src, TNodeID dest, std::vector< TTripStep > &path) {
        // Returns the time in hours for the fastest path between the src and dest
        // nodes of the if one exists. NoPathExists is returned if no path exists.
        // The transportation mode and nodes of the fastest path are filled in the
        // path parameter.        
        double hi;
        return hi;
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
