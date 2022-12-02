#include "XMLReader.h"
#include "StringUtils.h"
#include <expat.h>
#include <deque> 
#include <algorithm>
#include <iostream>

struct CXMLReader::SImplementation{
    std::shared_ptr< CDataSource > Source;
    /*Pointer to Data Source*/
    XML_Parser Parser;
    std::deque<SXMLEntity> DEntities;
    /*A deque of XMLEntities*/

    void StartElement(const XML_Char *name, const XML_Char **atts) {
        SXMLEntity NewEntity;
        size_t Index = 0;
        NewEntity.DNameData = name;
        NewEntity.DType = SXMLEntity::EType::StartElement;
        while(atts[Index]) {
            NewEntity.SetAttribute(atts[Index], atts[Index + 1]);
            Index += 2;
        }
        DEntities.push_back(NewEntity);
    };

    void EndElement(const XML_Char *name) {
        SXMLEntity NewEntity;
        NewEntity.DNameData = name;
        NewEntity.DType = SXMLEntity::EType::EndElement;
        DEntities.push_back(NewEntity);
    };

    void CharacterData(const XML_Char *s, int len) {
        SXMLEntity NewEntity;
        NewEntity.DType = SXMLEntity::EType::CharData;
        std::string name(s,len);

        name = StringUtils::Replace(name, "#", "&");

        name = StringUtils::Replace(name, "&amp;", "&");
        name = StringUtils::Replace(name, "&quot;", "\"");
        name = StringUtils::Replace(name, "&apos;", "'");
        name = StringUtils::Replace(name, "&lt;", "<");
        name = StringUtils::Replace(name, "&gt;", ">");

        NewEntity.DNameData = name;
        // std::cout<<"name: "<<name<<std::endl;
        // std::cout<<"DNameData: "<<NewEntity.DNameData<<std::endl;
        DEntities.push_back(NewEntity);
    };

    static void StartElementCallback(void *userData, const XML_Char* name, const XML_Char **atts) {
        /*
            userData is a pointer to the instance of SImplementation ("this is passed into here")
            passes in name and atts (attributes) automaticallly from the parser.
        */
        SImplementation* ReaderImplementation = (SImplementation*) userData;
        ReaderImplementation->StartElement(name, atts);
    };
    static void EndElementCallback(void *userData, const XML_Char* name) {
        SImplementation* ReaderImplementation = (SImplementation*) userData;
        ReaderImplementation->EndElement(name);
    };
    static void CharacterDataCallback(void *userData, const XML_Char* s, int len) {
        SImplementation* ReaderImplementation = (SImplementation*) userData;
        ReaderImplementation->CharacterData(s, len);
    };
    
    SImplementation(std::shared_ptr< CDataSource > src) {
        Source = src;
        Parser = XML_ParserCreate(NULL);
        /*Creates a Parser*/
        XML_SetUserData(Parser, this);
        /*Does this refer to the instance of CXMLReader/SImplementation created?*/
        XML_SetElementHandler(Parser,StartElementCallback,EndElementCallback);
        /*Sets handlers for start and end tags*/
        XML_SetCharacterDataHandler(Parser, CharacterDataCallback);
        /*Sets handler for text*/
    };
    ~SImplementation() {
        XML_ParserFree(Parser);
        /*I am guessing this frees up memory taken up by the Parser?*/
    };

    bool End() const {
        return DEntities.size() <= 0;
    };

    bool ReadEntity(SXMLEntity &entity, bool skipcdata = false) {
        bool Done = false;
        while(!Done) {
            std::vector<char> Buffer;
            if(Source->Read(Buffer, 1800)) {
                /*we're only reading 128 characters of the string at a time*/

                //std::replace(Buffer.begin(), Buffer.end(), '&', '#');

                bool opened = false; //not opened (a char data) <a>hii</a>
                for (int i = 0; i < Buffer.size(); i++) {
                    if (Buffer[i] == '<') {
                        opened = true;
                    } else if (Buffer[i] == '>') {
                        opened = false;
                    }
                    if (opened == false && Buffer[i] == '&') {
                        Buffer[i] = '#';
                    }
                }

                XML_Parse(Parser, Buffer.data(), Buffer.size(), Source->End());
            } 
            if (!DEntities.empty()) {
                while (skipcdata == true && DEntities.front().DType == SXMLEntity::EType::CharData){
                    DEntities.pop_front(); //<a> | hiii | </a>
                }
                entity = DEntities.front();
                DEntities.pop_front();
                Done = true;
                return true;
            }
        }
    };
};

CXMLReader::CXMLReader(std::shared_ptr< CDataSource > src) {
    DImplementation = std::make_unique<SImplementation>(src);
};

CXMLReader::~CXMLReader() {

};

bool CXMLReader::End() const {
    return DImplementation->End();
};

bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata) {
    return DImplementation->ReadEntity(entity, skipcdata);
};