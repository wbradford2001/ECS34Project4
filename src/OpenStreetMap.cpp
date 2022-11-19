#include "OpenStreetMap.h"

#include <unordered_map>
#include <iostream>

struct COpenStreetMap::SImplementation{

    struct SNode : public CStreetMap::SNode{
        TNodeID ThisNodeID;
        TLocation ThisNodeLocation;

        std::unordered_map<std::string,std::string> Attributes;
        std::vector< std::string > Keys;        

        ~SNode(){};
        TNodeID ID() const noexcept{
            // Returns the id of the SNode
            return ThisNodeID;
        }

        TLocation Location() const noexcept{
            // Returns the lat/lon location of the SNode
            return ThisNodeLocation;

        }

        std::size_t AttributeCount() const noexcept{
            // Returns the number of attributes attached to the SNode
            return Keys.size();
        }

        std::string GetAttributeKey(std::size_t index) const noexcept{
            // Returns the key of the attribute at index, returns empty string if index
            // is greater than or equal to AttributeCount()
            return index < Keys.size() ? Keys[index] : std::string();
        }

        bool HasAttribute(const std::string &key) const noexcept{
            // Returns if the attribute is attached to the SNode
            return Attributes.find(key) != Attributes.end();
        }

        std::string GetAttribute(const std::string &key) const noexcept{
            // Returns the value of the attribute specified by key, returns empty string
            // if key hasn't been attached to SNode
            auto Search = Attributes.find(key);
            if(Search == Attributes.end()){
                return std::string();
            }
            return Search->second;
        }

    };

    struct SWay : public CStreetMap::SWay{
        std::vector< TNodeID > NodesOfTheWay;
        TWayID ThisWayID;
        std::unordered_map<std::string,std::string> Attributes;
        std::vector< std::string > Keys;

        ~SWay(){};
        TWayID ID() const noexcept{
            // Returns the id of the SWay
            return ThisWayID;
        }

        std::size_t NodeCount() const noexcept{
            // Returns the number of nodes in the way
            return NodesOfTheWay.size();
        }

        TNodeID GetNodeID(std::size_t index) const noexcept{
            // Returns the node id of the node at index, returns InvalidNodeID if index
        // is greater than or equal to NodeCount()
            return index < NodesOfTheWay.size() ? NodesOfTheWay[index] : InvalidNodeID;
        }

        std::size_t AttributeCount() const noexcept{
            // Returns the number of attributes attached to the SWay
            return Attributes.size();
        }

        std::string GetAttributeKey(std::size_t index) const noexcept{
            // Returns the key of the attribute at index, returns empty string if index
            // is greater than or equal to AttributeCount()
            return index < Keys.size() ? Keys[index] : std::string();
        }

        bool HasAttribute(const std::string &key) const noexcept{
            // Returns if the attribute is attached to the SWay
            return Attributes.find(key) != Attributes.end();
        }

        std::string GetAttribute(const std::string &key) const noexcept{
            // Returns if the attribute is attached to the SWay
            auto Search = Attributes.find(key);
            if(Search == Attributes.end()){
                return std::string();
            }
            return Search->second;
        }

    };

    std::vector< std::shared_ptr< SWay > > AllWays;
    std::unordered_map< TWayID, std::shared_ptr< SWay > > MappedWays;
    
    std::vector< std::shared_ptr< SNode > > AllNodes;
    std::unordered_map< TNodeID, std::shared_ptr< SNode > > MappedNodes;    

    SImplementation(std::shared_ptr<CXMLReader> src){

        SXMLEntity Entity;

        
        // check osm start first
        while(src->ReadEntity(Entity)){
            if(Entity.DNameData == "osm" && Entity.DType == SXMLEntity::EType::EndElement || src->End()){
                break;
            }
            if(Entity.DNameData == "node"){

                // Parse node
                auto NewNode = std::make_shared<SNode>();

                NewNode->ThisNodeID = stoi(Entity.AttributeValue("id"));
   
                NewNode->ThisNodeLocation.first = stod(Entity.AttributeValue("lat"));
                NewNode->ThisNodeLocation.second = stod(Entity.AttributeValue("lon"));

                if (Entity.DType == SXMLEntity::EType::StartElement){
                    
                    src->ReadEntity(Entity);
                    while (Entity.DNameData != "node"){
                        
                        if (Entity.DNameData=="tag"){
                            NewNode->Keys.push_back(Entity.AttributeValue("k"));
                            NewNode->Attributes[Entity.AttributeValue("k")] = Entity.AttributeValue("v"); 
                        }
                        src->ReadEntity(Entity);
                    }
                }
                
                // Parse rest
                AllNodes.push_back(NewNode);
                MappedNodes[NewNode->ID()] = NewNode;

            }
            
            if(Entity.DNameData == "way" && Entity.DType == SXMLEntity::EType::StartElement){
                // Parse way
                auto NewWay = std::make_shared<SWay>();
                    //id
                NewWay->ThisWayID = stoi(Entity.AttributeValue("id"));
                
                
                // Parse nodes
                src->ReadEntity(Entity);

                while (Entity.DNameData != "way" && !(src->End())){
                    if (Entity.DNameData=="nd"){
                        TNodeID id = stoi(Entity.AttributeValue("ref"));
                        NewWay->NodesOfTheWay.push_back(id);                        
                        
                    } else if (Entity.DNameData == "tag"){
                        NewWay->Keys.push_back(Entity.AttributeValue("k"));
                        NewWay->Attributes[Entity.AttributeValue("k")] = Entity.AttributeValue("v");                        
                    }
                    src->ReadEntity(Entity);

                    }

                AllWays.push_back(NewWay);
                MappedWays[NewWay->ID()] = NewWay;
            }

        }
        

    }

    std::size_t NodeCount() const noexcept{
        // Returns the number of nodes in the map
        return AllNodes.size();
    }

    std::size_t WayCount() const noexcept{
        // Returns the number of ways in the map
        return AllWays.size();
    }

    std::shared_ptr<CStreetMap::SNode> NodeByIndex(std::size_t index) const noexcept{
        // Returns the SNode associated with index, returns nullptr if index is
        // larger than or equal to NodeCount()
        return index < AllNodes.size()? AllNodes[index] : nullptr ;
    }

    std::shared_ptr<CStreetMap::SNode> NodeByID(TNodeID id) const noexcept{
        // Returns the SNode with the id of id, returns nullptr if doesn't exist
        auto Search = MappedNodes.find(id);
        if(Search == MappedNodes.end()){
            return nullptr;
        }
        return Search->second;         

    }

    std::shared_ptr<CStreetMap::SWay> WayByIndex(std::size_t index) const noexcept{
        // Returns the SWay associated with index, returns nullptr if index is
        // larger than or equal to WayCount()
        return index < AllWays.size()? AllWays[index] : nullptr;
    }

    std::shared_ptr<CStreetMap::SWay> WayByID(TWayID id) const noexcept{
        // Returns the SWay with the id of id, returns nullptr if doesn't exist
        auto Search = MappedWays.find(id);
        if(Search == MappedWays.end()){
            return nullptr;
        }
        return Search->second;        

    }
};

COpenStreetMap::COpenStreetMap(std::shared_ptr<CXMLReader> src){
    DImplementation = std::make_unique<SImplementation>(src);
}

COpenStreetMap::~COpenStreetMap(){

}

std::size_t COpenStreetMap::NodeCount() const noexcept{
    return DImplementation->NodeCount();
}

std::size_t COpenStreetMap::WayCount() const noexcept{
    return DImplementation->WayCount();
}

std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByIndex(std::size_t index) const noexcept{
    return DImplementation->NodeByIndex(index);
}

std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByID(TNodeID id) const noexcept{
    return DImplementation->NodeByID(id);
}

std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByIndex(std::size_t index) const noexcept{
    return DImplementation->WayByIndex(index);
}

std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByID(TWayID id) const noexcept{
    return DImplementation->WayByID(id);
}

