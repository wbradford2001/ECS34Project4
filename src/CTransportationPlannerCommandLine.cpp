#include "TransportationPlannerCommandLine.h"
#include "StringUtils.h"
#include "DijkstraTransportationPlanner.h"
#include "GeographicUtils.h"
#include <sstream>
#include<iomanip>
#include <iostream>
#include <cmath>

struct CTransportationPlannerCommandLine::SImplementation {
    std::shared_ptr<CDataSource> InputSource;
    std::shared_ptr<CDataSink> OutputSink;
    std::shared_ptr<CDataSink> ErrorSink;
    std::shared_ptr<CDataFactory> Factory;
    std::shared_ptr<CTransportationPlanner> Planner;

    SImplementation(std::shared_ptr<CDataSource> cmdsrc,
                     std::shared_ptr<CDataSink> outsink,
                     std::shared_ptr<CDataSink> errsink,
                     std::shared_ptr<CDataFactory> results,
                     std::shared_ptr<CTransportationPlanner> planner)
    {
        /*
            Constructor for SImplementation.
        */
       InputSource = cmdsrc;
       OutputSink = outsink;
       ErrorSink = errsink;
       Factory = results;
       Planner = planner;
    };

    ~SImplementation(){
        /*
            Destructor for SImplementation.
        */
    };
    bool paramValidation (std::string param) {
        for (int i = 0; i < param.length(); i++){
            if (!isdigit(param[i])) {
                return false;
            }
        }
        return true;
    };
    std::string Error(std::string wrongCommand, bool command, int last) { 
        //pass in first param of vector (have to get rid of newline char)
        //command = 1 means something's wrong w command
        //command = 0 means something's wrong w param
        std::string returnVal;
        bool exists = false;
        std::vector<std::string> allCommands = {"help", "exit", "count", "node", "fastest", "shortest", "save", "print"};
        for (int i = 0; i < allCommands.size(); i++) {
            if (wrongCommand == allCommands[i]){
                exists = true;
            }
        };
        if (exists == false) {
            returnVal = "Unknown command \"" + wrongCommand + "\" type help for help.\n";
        } else if (wrongCommand == "save" || wrongCommand == "print") {
            if (last == 0) {
                returnVal = "No valid path to " + wrongCommand + ", see help.\n";
            }
        } else if (exists == true && command == true) {
            returnVal = "Invalid " + wrongCommand + " command, see help.\n";
        } else if (exists == true && command == false) {
            returnVal = "Invalid " + wrongCommand + " parameter, see help.\n";
        }
        return returnVal;
    };
    std::string timeConverter(std::string timeString) {
        int hours = 0;
        int minutes = 0;
        int seconds = 0;
        double time = std::stod(timeString); 
        if (time >= 1) {
            hours = std::floor(time);
            time = time - std::floor(time);
        };
        if (time > floor(time)) { //time is now a decimal
            minutes = floor(time*60);
            time = (time*60) - std::floor(time*60);
        };
        if (time > floor(time)) {
            seconds = time*60;
        };
        std::string returnVal = "";
        if (hours) {
            returnVal = std::to_string(hours) + " hr";
        };
        if (minutes) {
            returnVal += (hours ? " ": "") + std::to_string(minutes) + " min";
        };
        if (seconds) {
            returnVal += (hours || minutes ? " ": "") + std::to_string(seconds) + " sec";
        };
        returnVal += ".";
        return returnVal;
    };

    void OutputString(std::string str) {
        std::vector<char> out_vector(str.begin(), str.end());
        OutputSink->Write(out_vector);
    };
    void OutputErrorString(std::string str) {
        std::vector<char> out_vector(str.begin(), str.end());
        ErrorSink->Write(out_vector);
    };

    std::string toString(CTransportationPlanner::TTripStep step) {
        std::string returnString = "";
        if (step.first == CTransportationPlanner::ETransportationMode::Walk) {
            returnString = returnString + "Walk," + std::to_string(step.second);
        } else if (step.first == CTransportationPlanner::ETransportationMode::Bus) {
            returnString = returnString + "Bus," + std::to_string(step.second);
        } else if (step.first == CTransportationPlanner::ETransportationMode::Bike) {
            returnString = returnString + "Bike," + std::to_string(step.second);
        }
        return returnString;
    }
    std::vector<std::string> inputCommand() {
        char nextChar; //stores char when going through input datasource.
        std::string command; //stores the command.
        std::vector<std::string> splitCommand; //stores each part of the command
        std::string angleBracket = "> ";
        OutputString(angleBracket);
        while(!InputSource->End()) {
            InputSource->Get(nextChar);
            if (nextChar != '\n') {
                command.push_back(nextChar);
            } else { //end of command
                command = StringUtils::Strip(command); //I'm not so sure this is neccessary...
                splitCommand = StringUtils::Split(command);
                return splitCommand;
            }
        }
        //return splitCommand;
    }
    bool ProcessCommands() {
        std::vector<std::string> splitCommand; //stores each part of the command
        int last = 0;
        /* 
            ~~ Stores what whether the last command is shortest/fastest/neither ~~
            last = 0 means neither shortest/fastest have been called
            last = 1 means that shortest was the last to be called
            last = 2 means that fastest was the last to be called 
        */

        double fastestPath; //unit in hours
        double shortestPath; //unit in miles

        std::vector< CTransportationPlanner::TNodeID > previousPath; //shortest
        std::vector< CTransportationPlanner::TTripStep > previousPath2; //shortest but in steps
        std::vector< CTransportationPlanner::TTripStep > previousSteps; //fastest

        std::string name; 
        std::vector< std::string > pathDescription;
        std::shared_ptr< CDataSink > sink;

        std::string ErrorString;

        while(!InputSource->End()) {
            splitCommand = inputCommand();
            if (splitCommand[0] == "exit") {
                break;

            } else if (splitCommand[0] == "help"){
                std::string helpString = 
                                "------------------------------------------------------------------------\n"
                                "help     Display this help menu\n"
                                "exit     Exit the program\n"
                                "count    Output the number of nodes in the map\n"
                                "node     Syntax \"node [0, count)\" \n"
                                "         Will output node ID and Lat/Lon for node\n"
                                "fastest  Syntax \"fastest start end\" \n"
                                "         Calculates the time for fastest path from start to end\n"
                                "shortest Syntax \"shortest start end\" \n"
                                "         Calculates the distance for the shortest path from start to end\n"
                                "save     Saves the last calculated path to file\n"
                                "print    Prints the steps for the last calculated path\n";
                OutputString(helpString);                

            } else if (splitCommand[0] == "count"){
                std::string outString = std::to_string(Planner->NodeCount()) + " nodes\n";
                OutputString(outString);

            } else if (splitCommand[0] == "node"){
                int index;
                if (splitCommand.size() != 2) {
                    ErrorString = Error(splitCommand[0], true, last);
                    OutputErrorString(ErrorString);
                    std::cout<<"NODE"<<std::endl;
                    continue;
                } else if (splitCommand.size() == 2) {
                    if (paramValidation(splitCommand[1])) {
                        index = std::stoi(splitCommand[1]);
                    } else {
                        ErrorString = Error(splitCommand[0], false, last);
                        OutputErrorString(ErrorString);
                        std::cout<<"NODE OOPS"<<std::endl;
                        continue;
                    } 
                }
                auto node = Planner->SortedNodeByIndex(index);

                CStreetMap::TLocation location = node->Location();
                std::string outString = "Node " + splitCommand[1] + ": id = " + std::to_string(node->ID()) + " is at " + SGeographicUtils::ConvertLLToDMS(location) + "\n";

                OutputString(outString);

            } else if (splitCommand[0] == "shortest"){ 
                int index1;
                int index2;
                std::cout<<"splitCommand"<<splitCommand.size()<<std::endl;
                if (splitCommand.size() != 3) {
                    ErrorString = Error(splitCommand[0], true, last);
                    OutputErrorString(ErrorString);

                    continue;
                } else if (splitCommand.size() == 3) {
                    if (paramValidation(splitCommand[1]) && paramValidation(splitCommand[2])) {
                        index1 = std::stoi(splitCommand[1]);
                        index2 = std::stoi(splitCommand[2]);
                    } else {
                        ErrorString = Error(splitCommand[0], false, last);
                        OutputErrorString(ErrorString);
                        continue;
                    }
                }
                shortestPath = Planner->FindShortestPath(index1, index2, previousPath);
                //shortestPath = Planner->FindShortestPath(index1, index2, previousPath);
                std::stringstream shortestPathString;
                shortestPathString<<std::setprecision(4)<<shortestPath;
                std::string outString = "Shortest path is " + shortestPathString.str() + " mi.\n";
                OutputString(outString);

                last = 1;
                name = splitCommand[1] + "_" + splitCommand[2] + "_" + shortestPathString.str()+ "mi.csv";

                /*
                loop that goes through all node ids in path, for each node id, 
                add to previouSteps.
                (std::make_pair(CTransportationPlanner::ETransportationMode::Walk,NodeID)
                */

                for (int i = 0; i < previousPath.size(); i++) {
                    auto step = std::make_pair(CTransportationPlanner::ETransportationMode::Walk, previousPath[i]);
                    previousPath2.push_back(step);
                }

            } else if (splitCommand[0] == "fastest"){
                int index1;
                int index2;
                if (splitCommand.size() != 3) {
                    ErrorString = Error(splitCommand[0], true, last);
                    OutputErrorString(ErrorString);
                    std::cout<<"FASTEST"<<std::endl;
                    continue;
                } else if (splitCommand.size() == 3) {
                    if (paramValidation(splitCommand[1]) && paramValidation(splitCommand[2])) {
                        index1 = std::stoi(splitCommand[1]);
                        index2 = std::stoi(splitCommand[2]);
                    } else {
                        ErrorString = Error(splitCommand[0], false, last);
                        OutputErrorString(ErrorString);
                        std::cout<<"FASTEST 123 NOPE"<<std::endl;
                        continue;
                    }
                }
                fastestPath = Planner->FindFastestPath(index1, index2, previousSteps);
                std::stringstream fastestPathString;
                fastestPathString<<std::setprecision(4)<<fastestPath;
                std::cout<< "TIMEEE" << timeConverter(fastestPathString.str()) <<std::endl;
                std::string outString = "Fastest path takes " + timeConverter(fastestPathString.str()) + "\n";
                OutputString(outString);

                last = 2;
                name = splitCommand[1] + "_" + splitCommand[2] + "_" + std::to_string(fastestPath) + "hr.csv";

            } else if (splitCommand[0] == "print"){
                if (last == 0) {
                    ErrorString = Error(splitCommand[0], true, last);
                    OutputErrorString(ErrorString);
                    std::cout<<"SAVE"<<std::endl;
                    continue;
                }
                else if (last == 1) { //shortest
                    
                    Planner->GetPathDescription(previousPath2, pathDescription);

                } else if (last == 2) { //fastest

                    Planner->GetPathDescription(previousSteps, pathDescription);
                }

                for (int i = 0; i < pathDescription.size(); i++) {
                        OutputString(pathDescription[i] + "\n");
                    }


            } else if (splitCommand[0] == "save"){
                if (last == 0) {
                    ErrorString = Error(splitCommand[0], true, last);
                    OutputErrorString(ErrorString);
                    std::cout<<"SAVE"<<std::endl;
                    continue;
                }
                sink = Factory->CreateSink(name);
                std::string outString2 = "Path saved to <results>/" + name + "\n";
                    OutputString(outString2);

                std::string saveString = "mode,node_id\n";
                if (last == 1) { //shortest //previousPath2

                    for (int i = 0; i < previousPath2.size(); i++) {
                        saveString = saveString + toString(previousPath2[i]);
                    }

                } else if (last == 2) { //fastest

                    for (int i = 0; i < previousSteps.size(); i++) {
                        if (i != previousSteps.size() - 1) {
                            saveString = saveString + toString(previousSteps[i]) + "\n";
                        } else {
                            saveString = saveString + toString(previousSteps[i]);
                        }
                        
                    }
                }
                std::vector<char> saveVector(saveString.begin(), saveString.end());
                sink->Write(saveVector);
            } else {
                std::cout<<"FOO"<<std::endl;
                ErrorString = Error(splitCommand[0], true, last);
                OutputErrorString(ErrorString);
            }
        }    
        return true;
    };
};
CTransportationPlannerCommandLine::CTransportationPlannerCommandLine(std::shared_ptr<CDataSource> cmdsrc, 
                                        std::shared_ptr<CDataSink> outsink, 
                                        std::shared_ptr<CDataSink> errsink, 
                                        std::shared_ptr<CDataFactory> results, 
                                        std::shared_ptr<CTransportationPlanner> planner) 
{
    DImplementation = std::make_unique<SImplementation>(cmdsrc, outsink, errsink, results, planner);
};
CTransportationPlannerCommandLine::~CTransportationPlannerCommandLine() {

};
bool CTransportationPlannerCommandLine::ProcessCommands() {
    return DImplementation->ProcessCommands();
};
