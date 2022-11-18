#include "XMLWriter.h"
#include "XMLEntity.h"
#include "DataSink.h"
#include <iostream>


//CDSVReader
struct CXMLWriter::SImplementation{
    std::shared_ptr< CDataSink > sink;
    std::vector<std::string> Q;

};


CXMLWriter::CXMLWriter(std::shared_ptr< CDataSink > sink){
    DImplementation = std::make_unique<SImplementation>();
    DImplementation->sink = sink;
    
};

CXMLWriter::~CXMLWriter(){};

bool CXMLWriter::Flush(){

    for (int j = DImplementation->Q.size()-1; j>=0; j--){
        std::string str = DImplementation->Q[j];
        DImplementation->sink->Put('<');
        DImplementation->sink->Put('/');
        for (int i=0; i<str.length(); i++){
            DImplementation->sink->Put(str[i]);
        }
        DImplementation->sink->Put('>');
    }
    return true;
};

bool CXMLWriter::WriteEntity(const SXMLEntity &entity){
    if (entity.DType == SXMLEntity::EType::StartElement){
        DImplementation->Q.push_back(entity.DNameData);

    }
    if (entity.DType == SXMLEntity::EType::EndElement){
        if (DImplementation->Q.back() == entity.DNameData){
            DImplementation->Q.pop_back();    
        }
    }

    if (entity.DType != SXMLEntity::EType::CharData){

        DImplementation->sink->Put('<');
    }

    if (entity.DType==SXMLEntity::EType::EndElement){
        DImplementation->sink->Put('/');
    }    
    for (int i = 0; i < entity.DNameData.length(); i ++){
        DImplementation->sink->Put(entity.DNameData[i]);
    }

    for(SXMLEntity::TAttribute attribute : entity.DAttributes){
        DImplementation->sink->Put(' ');
        for (int i = 0; i < attribute.first.length(); i ++){
            DImplementation->sink->Put(attribute.first[i]);
        }        
        DImplementation->sink->Put('=');
        DImplementation->sink->Put('"');
        for (int i = 0; i < attribute.second.length(); i ++){
            DImplementation->sink->Put(attribute.second[i]);
        }           
        DImplementation->sink->Put('"');

    } 

    if (entity.DType==SXMLEntity::EType::CompleteElement){
        DImplementation->sink->Put('/');
    }

    if (entity.DType != SXMLEntity::EType::CharData){

        DImplementation->sink->Put('>');
    }

    return true;

};


                                      