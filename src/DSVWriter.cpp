#include "DSVWriter.h"
#include "StringDataSink.h"

#include <iostream>
#include <memory>
#include <string>

struct CDSVWriter::SImplementation{
    std::shared_ptr< CDataSink > sink;
    char delimiter;
    bool quoteall;
    bool started;
};



CDSVWriter::CDSVWriter(std::shared_ptr< CDataSink > sink, char delimiter, bool quoteall){
    DImplementation = std::make_unique<SImplementation>();
    DImplementation->sink = sink;
    DImplementation->delimiter = delimiter;
    DImplementation->quoteall = quoteall;
    DImplementation->started = false;
    
};

CDSVWriter::~CDSVWriter(){};

bool CDSVWriter::WriteRow(const std::vector<std::string> &row){
    if (row.size()==0){
        return false;
    }

    if ((DImplementation->started)==true){
        DImplementation->sink->Put('\n');
    }
    DImplementation->started = true;
    int i = 0;
    for (int i = 0; i < row.size(); i ++){
        if (DImplementation->quoteall == true
        ||row[i].find(DImplementation->delimiter) < row[i].length()||row[i].find('\n') < row[i].length()||row[i].find('"') < row[i].length()
        )  {
            DImplementation->sink->Put('"');
        }



        for (int j=0; j < row[i].length(); j++){
            
            DImplementation->sink->Put(row[i][j]);
            
        }



        if (DImplementation->quoteall == true
        ||row[i].find(DImplementation->delimiter) < row[i].length()||row[i].find('\n') < row[i].length()||row[i].find('"') < row[i].length()
        )  {

            DImplementation->sink->Put('"');
        }


        if (i != row.size()-1){
            DImplementation->sink->Put(DImplementation->delimiter);
        }
    }
    
    
    

    return true;
};