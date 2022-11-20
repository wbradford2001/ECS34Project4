#include "XMLEntity.h"
#include "XMLReader.h"
#include "DataSource.h"
#include <expat.h>
#include <cstring>
#include <iostream>



//CDSVReader
struct CXMLReader::SImplementation{
    std::shared_ptr< CDataSource > src;
    XML_Parser p;

};


//number of spaces to print so each child element is indented
int Depth;


SXMLEntity curEntity;
bool startCompleted;


//Function to print start elements
static void XMLCALL
start(void *data, const XML_Char *el, const XML_Char **attr) {

    SXMLEntity newEntity;
    newEntity.DType = SXMLEntity::EType::StartElement;


    
    newEntity.DNameData = el;
    for (int i = 0; attr[i]; i += 2) {
        newEntity.SetAttribute(attr[i], attr[i+1]);
    }
    


    curEntity = newEntity;
    startCompleted=true;
    
//   int i;
//   (void)data;
//   //print indent
//     for (i = 0; i < Depth; i++){
//         std::cout<<"   ";
//     }
//     //print element
//     std::cout<<'<'<<el;
//     //print attributes
//     for (i = 0; attr[i]; i += 2) {
//         std::cout<<' '<<attr[i]<<"=\""<<attr[i+1]<<"\"";
//     }
//     std::cout<<'>';
//     std::cout<<std::endl;

//     Depth++;
}

//Function to print end Elements
static void XMLCALL
end(void *data, const XML_Char *el) {
   
//   (void)data;
//   (void)el;
//   //decrease indent
//     Depth--;

//     for (int j = 0; j < Depth; j++){
//         std::cout<<"   ";
//     }    

//     //print element
//     std::cout<<"</"<<el<<'>'<<std::endl;



    SXMLEntity newEntity;
    newEntity.DType = SXMLEntity::EType::EndElement;


    
    newEntity.DNameData = el;

    if (startCompleted==true){
        for (int i = 0; i<curEntity.DAttributes.size(); i += 1) {
            newEntity.SetAttribute(curEntity.DAttributes[i].first, curEntity.DAttributes[i].second);
        }        
    }

 
    


    curEntity = newEntity;

            



    


}

CXMLReader::CXMLReader(std::shared_ptr< CDataSource > src){
    DImplementation = std::make_unique<SImplementation>();
    DImplementation->src = src;
    DImplementation->p = XML_ParserCreate(NULL);
    XML_SetElementHandler(DImplementation->p, start, end);


};

CXMLReader::~CXMLReader(){};

bool CXMLReader::End() const{
    char temp;
    if (DImplementation->src->Peek(temp)){
        return false;
    }
    return true;
};

bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata){
    //vector to store all the chars in DString
    std::vector< char > TempVector;
    char temp;
    //length of current element
    int l=0;

    //handle CharData
    // DImplementation->src->Peek(temp);
    // std::cout<<temp<<std::endl;
    // if (temp!= '<'){
        
    //     while (DImplementation->src->Peek(temp)){
    //         if (temp=='<'){
    //             break;
    //         }
    //         DImplementation->src->Get(temp);
    //         l += 1;
            
    //         TempVector.push_back(temp);
   

    // }
    // char* buffer = new char[l];
    // for (int i=0; i < l;i++){
    //     buffer[i]=TempVector[i];   

    //     }

    // if (skipcdata==false){
        
    //     entity.DType = SXMLEntity::EType::CharData;
    //     entity.DNameData = buffer;
    //     return true;
    // }

    // }



    std::vector< char > TempVector2;


    //Get length of current element i.e. count number of chars until you reach ">" which means element is closed
    l=0;
    while (DImplementation->src->Peek(temp)){
        //std::cout<<temp<<std::endl;
        TempVector2.push_back(temp);
        DImplementation->src->Get(temp);
        l += 1;
        
        if (TempVector2.size()>2 && TempVector2[0]=='<' && TempVector2[1]=='?' && TempVector2[2]=='x'){
            while (temp != '>'){
                DImplementation->src->Get(temp);
            }
            DImplementation->src->Get(temp);
            
            l = 0;
            TempVector2.clear();
            TempVector2.push_back('<');
            
        }
        
        if (temp=='>'){

            break;
        }

    }
    

    //I had issues passing in the TempVector2 directly, so I created a buffer and made it the same as the TempVector2

    //Parse the buffer/element
    std::string str{ TempVector2.begin(), TempVector2.end() };
    char char_array[TempVector2.size() + 1];

    strcpy(char_array, str.c_str());
    //std::cout<<char_array<<std::endl;
    startCompleted=false;
    XML_Parse(DImplementation->p, char_array, TempVector2.size(), false);

    entity = curEntity;

    return true;
};

