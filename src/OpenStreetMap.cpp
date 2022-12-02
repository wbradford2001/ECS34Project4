#include "OpenStreetMap.h"
#include <unordered_map>
#include <iostream>

struct COpenStreetMap::SImplementation{

    struct SNode : public CStreetMap::SNode{
        std::vector< TNodeID > Nodess;
        TNodeID ThisNodeID;
        TLocation Locations;
        std::vector<std::pair<double,double>> locations;
        std::unordered_map<std::string,std::string> NAttributes;
        std::vector< std::string > NKeys;
        ~SNode(){};
        TNodeID ID() const noexcept{
            return ThisNodeID;
        }

        TLocation Location() const noexcept{
            return Locations;
        }

        std::size_t AttributeCount() const noexcept{
            return NAttributes.size();
        }

        std::string GetAttributeKey(std::size_t index) const noexcept{
            return index < NKeys.size() ? NKeys[index] : std::string();

        }

        bool HasAttribute(const std::string &key) const noexcept{
            return NAttributes.find(key) != NAttributes.end();

        }

        std::string GetAttribute(const std::string &key) const noexcept{
            auto Search = NAttributes.find(key);
            if(Search == NAttributes.end()){
                return std::string();
            }
            return Search->second;
        }

    };

    struct SWay : public CStreetMap::SWay{
        std::vector< TNodeID > NodesOfTheWay;
        TWayID ThisWayID;
        TNodeID ThisNodeID;
        std::unordered_map<std::string,std::string> Attributes;
        std::vector< std::string > Keys;

        ~SWay(){};
        TWayID ID() const noexcept{
            return ThisWayID;
        }

        std::size_t NodeCount() const noexcept{
            return NodesOfTheWay.size();
        }

        TNodeID GetNodeID(std::size_t index) const noexcept{
            return index < NodesOfTheWay.size() ? NodesOfTheWay[index] : InvalidNodeID;
        }

        std::size_t AttributeCount() const noexcept{
            return Attributes.size();
        }

        std::string GetAttributeKey(std::size_t index) const noexcept{
            return index < Keys.size() ? Keys[index] : std::string();
        }

        bool HasAttribute(const std::string &key) const noexcept{
            return Attributes.find(key) != Attributes.end();
        }

        std::string GetAttribute(const std::string &key) const noexcept{
            auto Search = Attributes.find(key);
            if(Search == Attributes.end()){
                return std::string();
            }
            return Search->second;
        }
    };

    std::vector< std::shared_ptr< SWay > > AllWays;
    std::unordered_map< TWayID, std::shared_ptr< SWay > > MappedWays;
    std::vector <std::shared_ptr< SNode > > AllNodes;
    std::unordered_map< TNodeID, std::shared_ptr< SNode > > MappedNodes;

    SImplementation(std::shared_ptr<CXMLReader> src){
        SXMLEntity Entity;
        std::cout<<"beforeread"<<std::endl;

        // check osm start first
        while(src->ReadEntity(Entity)){
            std::cout<<"afterread"<<std::endl;
            if(Entity.DNameData == "osm" && Entity.DType == SXMLEntity::EType::EndElement){
                break;
            }
            std::cout<<"afterosm"<<std::endl;
            if(Entity.DNameData == "node" && Entity.DType == SXMLEntity::EType::StartElement){
                // Parse node
                // std::vector< TNodeID> NodesOfTheWayTVector;

                std::vector<std::string>NKeyVector;
                std::unordered_map<std::string,std::string>NAttributeMap;
                std::vector<std::pair<double,double>> locations;

                auto NewNode = std::make_shared<SNode>(); // pair up the nodes
                // auto locations = std::make_pair(double (val1), double (val2)); // pair up the lon and lat
                NewNode->ThisNodeID = stod(Entity.AttributeValue("id"));
                 //if (Entity.DNameData == "lat" && Entity.DNameData == "lon")
                //{
                    
                NewNode->Locations = std::make_pair(stod(Entity.AttributeValue("lat")),(stod(Entity.AttributeValue("lon"))));
                     // NewNode->Locations = std::make_pair(1,2);
                //}
                 std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
                while(src->ReadEntity(Entity))
                {
                     std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
                    if(Entity.DNameData == "tag" && Entity.DType == SXMLEntity::EType::StartElement)
                    {
                        NKeyVector.push_back(Entity.AttributeValue("k"));
                        NewNode->NKeys = NKeyVector;
                        NAttributeMap.insert({Entity.AttributeValue("k"), Entity.AttributeValue("v")});
                        for (int t = 0; t < NAttributeMap.size(); t++)
                        {
                            std::cout<<"NAttributeMap:"<<NAttributeMap[NKeyVector[t]]<<std::endl;
                        }
                        NewNode->NAttributes = NAttributeMap;
                    }
                    if(Entity.DNameData == "tag" && Entity.DType == SXMLEntity::EType::EndElement)
                    {
                        continue;
                    }
                    else if (Entity.DNameData == "node" && Entity.DType == SXMLEntity::EType::EndElement)
                    {
                        break;
                    }
                }
                 std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
                AllNodes.push_back(NewNode); // push all of them back into a vector afterwards
                MappedNodes[NewNode->ID()] = NewNode;
            }
            if(Entity.DNameData == "way" && Entity.DType == SXMLEntity::EType::StartElement){
                // Parse way
                std::vector< TNodeID> NodesOfTheWayTVector;
                std::vector<std::string>WKeyVector;
                std::unordered_map<std::string,std::string>WAttributeMap;

                auto NewWay = std::make_shared<SWay>();
                NewWay->ThisWayID = stod(Entity.AttributeValue("id")); // makes the string into an integer id
                 std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
                while(src->ReadEntity(Entity))
                {
                    if (Entity.DNameData == "nd" && Entity.DType == SXMLEntity::EType::StartElement)
                    {
                        NodesOfTheWayTVector.push_back(stod(Entity.AttributeValue("ref")));
                        NewWay->NodesOfTheWay = NodesOfTheWayTVector;
                        for (int s = 0; s < NodesOfTheWayTVector.size(); s++)
                        {
                            std::cout<<"Nodes:"<<NodesOfTheWayTVector[s]<<std::endl;
                        }
                    }
                    if (Entity.DNameData == "tag" && Entity.DType == SXMLEntity::EType::StartElement)
                    {
                        WKeyVector.push_back(Entity.AttributeValue("k"));
                        NewWay->Keys = WKeyVector;
                        WAttributeMap.insert({Entity.AttributeValue("k"), (Entity.AttributeValue("v"))});
                        NewWay->Attributes = WAttributeMap;
                    }
                    if (Entity.DNameData == "nd" && Entity.DType == SXMLEntity::EType::EndElement)
                    {
                        continue;
                    }
                    if (Entity.DNameData == "tag" && Entity.DType == SXMLEntity::EType::EndElement)
                    {
                        continue;
                    }
                    else if(Entity.DNameData == "way" && Entity.DType == SXMLEntity::EType::EndElement)
                    {
                        break;
                    }
                     std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
                }
                std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
                // Parse rest
                AllWays.push_back(NewWay);
                MappedWays[NewWay->ID()] = NewWay;
            }
        }
        std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl; 
    };

    std::size_t NodeCount() const noexcept{
        return AllNodes.size();
    };

    std::size_t WayCount() const noexcept{
        return AllWays.size();
    };

    std::shared_ptr<CStreetMap::SNode> NodeByIndex(std::size_t index) const noexcept{
        return AllNodes[index];
    };

    std::shared_ptr<CStreetMap::SNode> NodeByID(TNodeID id) const noexcept{
            auto Search = MappedNodes.find(id);
            if(Search == MappedNodes.end()){
                return nullptr;
            }
            return Search->second;
    };

    std::shared_ptr<CStreetMap::SWay> WayByIndex(std::size_t index) const noexcept{
        return AllWays[index];
    };

    std::shared_ptr<CStreetMap::SWay> WayByID(TWayID id) const noexcept{
            auto Search = MappedWays.find(id);
            if(Search == MappedWays.end()){
                return nullptr;
            }
            return Search->second;
    };
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
