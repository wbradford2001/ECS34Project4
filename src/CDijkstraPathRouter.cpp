#include "DijkstraPathRouter.h"
#include "PathRouter.h"
#include <unordered_map>
struct CDijkstraPathRouter::SImplementation{
    struct Vertex{
            std::any tag;
            std::vector< std::pair< std::shared_ptr<Vertex>, int > > Nexts;
        };    

    std::unordered_map< int, std::shared_ptr<Vertex> > VertexHash; //Hash that maps vertex ID's to Vertex Objects
    int ID = 0;//Start with ID of 1
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
        VertexHash[ID] = newVertex;
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
        std::pair<std::shared_ptr<Vertex>, int> newPair;
        newPair.first = VertexHash.at(dest); 
        newPair.second = weight;
        VertexHash.at(src)->Nexts.push_back(newPair);
        return true;

    };

    bool Precompute(std::chrono::steady_clock::time_point deadline) noexcept{
        // Allows the path router to do any desired precomputation up to the deadline


        //iterate through vertices, update adjacency matrix each time.
        return true;
    };

    double FindShortestPath(TVertexID src, TVertexID dest, std::vector<TVertexID> &path) noexcept{
        // Returns the path distance of the path from src to dest, and fills out path
    // with vertices. If no path exists NoPathExists is returned.
        double myDub;
        return myDub;

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


