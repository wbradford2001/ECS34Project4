#include "DSVReader.h"
#include "StringDataSource.h"

#include <iostream>

//CDSVReader
struct CDSVReader::SImplementation{
    std::shared_ptr< CDataSource > src;
    char delimiter;
};


CDSVReader::CDSVReader(std::shared_ptr< CDataSource > src, char delimiter){
    DImplementation = std::make_unique<SImplementation>();
    DImplementation->src = src;

    DImplementation->delimiter = delimiter;
};

CDSVReader::~CDSVReader(){
    
};


bool CDSVReader::End() const{
    
    return (DImplementation->src->End());
};
bool CDSVReader::ReadRow(std::vector<std::string> &row){
    if (CDSVReader::End()){
        return false;
    }

    char cur;
    DImplementation->src->Get(cur);
    std::string curString;


    
    
    while (cur != '\n'){
        //std::cout << cur << std::endl;
        if  (DImplementation->src->End()){
            curString = curString + cur;
            
            break;
        }
        if (cur != DImplementation->delimiter){
            curString = curString + cur;
        } else {
            row.push_back(curString);
            curString = "";
        };
        DImplementation->src->Get(cur);

        
    }
    row.push_back(curString);
    

    
    return true;
};

