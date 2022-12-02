#include "TransportationPlannerConfig.h"
#include "DijkstraTransportationPlanner.h"
#include "OpenStreetMap.h"
#include "CSVBusSystem.h"
#include "FileDataFactory.h"
#include "StandardDataSource.h"
#include "StandardDataSink.h"
#include "StandardErrorDataSink.h"
#include "StringUtils.h"
#include "TransportationPlannerCommandLine.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <vector>
#include <cmath>
class CArgumentParser{
    private:
        std::string DDataDirectory;
        std::string DResultsDirectory;
        //uint64_t DNumPoints;
        //uint64_t DSeed;
        bool DArgumentsValid;
        
        void PrintSyntax() const;
    public:
        CArgumentParser(const std::vector<std::string> &args);

        bool ArgumentsValid() const;

        std::string DataDirectory() const;
        std::string ResultsDirectory() const;
};
int main(int argc, char *argv[]){

    std::vector<std::string> Arguments; 
    //vector of strings(commands)

    const std::string OSMFilename = "city.osm";
    const std::string StopFilename = "stops.csv";
    const std::string RouteFilename = "routes.csv";

    for(int Index = 1; Index < argc; Index++){
        Arguments.push_back(argv[Index]);
    }

    CArgumentParser Parser(Arguments);
    if(!Parser.ArgumentsValid()){
        return EXIT_FAILURE;
    }
    auto DataFactory = std::make_shared<CFileDataFactory>(Parser.DataDirectory());
    auto results = std::make_shared<CFileDataFactory>(Parser.ResultsDirectory());
    auto cmdsrc = std::make_shared<CStandardDataSource>();
    auto outsink = std::make_shared<CStandardDataSink>();
    auto errsink = std::make_shared<CStandardErrorDataSink>();
    auto StopReader = std::make_shared<CDSVReader>(DataFactory->CreateSource(StopFilename),',');
    auto RouteReader = std::make_shared<CDSVReader>(DataFactory->CreateSource(RouteFilename),',');
    auto BusSystem = std::make_shared<CCSVBusSystem>(StopReader, RouteReader);
    auto XMLReader = std::make_shared<CXMLReader>(DataFactory->CreateSource(OSMFilename));
    auto StreetMap = std::make_shared<COpenStreetMap>(XMLReader);
    auto PlannerConfig = std::make_shared<STransportationPlannerConfig>(StreetMap, BusSystem);
    auto planner = std::make_shared<CDijkstraTransportationPlanner>(PlannerConfig);
    
    CTransportationPlannerCommandLine CommandLine(cmdsrc, outsink, errsink, results, planner);
    CommandLine.ProcessCommands();
};
CArgumentParser::CArgumentParser(const std::vector<std::string> &args){
    DDataDirectory = "./data";
    DResultsDirectory = "./results";
    DArgumentsValid = true;
    // DNumPoints = 0;
    // DSeed = 0;
    // DVerbose = false;
    for(auto &Argument : args){
        if(Argument.find("--data") == 0){
            auto SplitArg = StringUtils::Split(Argument,"=");
            if(SplitArg.size() != 2 || SplitArg[0] != "--data"){
                DArgumentsValid = false;
                break;
            }
            DDataDirectory = SplitArg[1];
        }
        else if(Argument.find("--results") == 0){
            auto SplitArg = StringUtils::Split(Argument,"=");
            if(SplitArg.size() != 2 || SplitArg[0] != "--results"){
                DArgumentsValid = false;
                break;
            }
            DResultsDirectory = SplitArg[1];
        }
    }
    if(!DArgumentsValid){
        PrintSyntax();
    }
};
void CArgumentParser::PrintSyntax() const{
    std::cerr<<"Syntax Error: transplanner [--data=path | --results=path]"<<std::endl;
};

bool CArgumentParser::ArgumentsValid() const{
    return DArgumentsValid;
};

std::string CArgumentParser::DataDirectory() const{
    return DDataDirectory;
};

std::string CArgumentParser::ResultsDirectory() const{
    return DResultsDirectory;
};
