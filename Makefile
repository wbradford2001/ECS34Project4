CC=gcc
CXX=g++

INC_DIR=./include
SRC_DIR=./src
TESTSRC_DIR=./testsrc
BIN_DIR=./bin
OBJ_DIR=./obj

CXXFLAG=-std=c++17 -I $(INC_DIR) -I /opt/local/include
LDFLAGS=-L /opt/local/lib -lgtest -lgtest_main -lpthread -lexpat

all: directories  runtest

runtest:  $(BIN_DIR)/teststrutils $(BIN_DIR)/teststrdatasource $(BIN_DIR)/teststrdatasink $(BIN_DIR)/testdsv $(BIN_DIR)/testxml $(BIN_DIR)/testosm $(BIN_DIR)/testcsvbs $(BIN_DIR)/testcsvbsi $(BIN_DIR)/testdpr $(BIN_DIR)/testtp $(BIN_DIR)/testtpcl $(BIN_DIR)/testfiledatass $(BIN_DIR)/testkml $(BIN_DIR)/transplanner $(BIN_DIR)/speedtest
	$(BIN_DIR)/teststrutils && $(BIN_DIR)/teststrdatasource && $(BIN_DIR)/teststrdatasink &&  $(BIN_DIR)/testdsv && $(BIN_DIR)/testxml && $(BIN_DIR)/testosm && $(BIN_DIR)/testcsvbs && $(BIN_DIR)/testcsvbsi && $(BIN_DIR)/testdpr && $(BIN_DIR)/testtp && $(BIN_DIR)/testtpcl && $(BIN_DIR)/testfiledatass && $(BIN_DIR)/testkml


#DataSink
$(OBJ_DIR)/StringDataSink.o: $(SRC_DIR)/StringDataSink.cpp $(INC_DIR)/StringDataSink.h
	$(CXX) -o $(OBJ_DIR)/StringDataSink.o $(CXXFLAG) -c $(SRC_DIR)/StringDataSink.cpp

$(OBJ_DIR)/StringDataSinkTest.o: $(TESTSRC_DIR)/StringDataSinkTest.cpp $(INC_DIR)/StringDataSink.h
	$(CXX) -o $(OBJ_DIR)/StringDataSinkTest.o $(CXXFLAG) -c $(TESTSRC_DIR)/StringDataSinkTest.cpp 	
	
		
$(BIN_DIR)/teststrdatasink: $(OBJ_DIR)/StringDataSinkTest.o $(OBJ_DIR)/StringDataSink.o
	$(CXX) -o $(BIN_DIR)/teststrdatasink $(OBJ_DIR)/StringDataSinkTest.o $(OBJ_DIR)/StringDataSink.o $(LDFLAGS)







#DataSource
$(BIN_DIR)/teststrdatasource: $(OBJ_DIR)/StringDataSourceTest.o $(OBJ_DIR)/StringDataSource.o
	$(CXX) -o $(BIN_DIR)/teststrdatasource $(OBJ_DIR)/StringDataSourceTest.o $(OBJ_DIR)/StringDataSource.o $(LDFLAGS)

$(OBJ_DIR)/StringDataSourceTest.o: $(TESTSRC_DIR)/StringDataSourceTest.cpp $(INC_DIR)/StringDataSource.h
	$(CXX) -o $(OBJ_DIR)/StringDataSourceTest.o $(CXXFLAG) -c $(TESTSRC_DIR)/StringDataSourceTest.cpp 

$(OBJ_DIR)/StringDataSource.o: $(SRC_DIR)/StringDataSource.cpp $(INC_DIR)/StringDataSource.h
	$(CXX) -o $(OBJ_DIR)/StringDataSource.o $(CXXFLAG) -c $(SRC_DIR)/StringDataSource.cpp




#strUtils
$(BIN_DIR)/teststrutils: $(OBJ_DIR)/StringUtilsTest.o $(OBJ_DIR)/StringUtils.o
	$(CXX) -o $(BIN_DIR)/teststrutils $(OBJ_DIR)/StringUtilsTest.o $(OBJ_DIR)/StringUtils.o $(LDFLAGS)

$(OBJ_DIR)/StringUtilsTest.o: $(TESTSRC_DIR)/StringUtilsTest.cpp $(INC_DIR)/StringUtils.h
	$(CXX) -o $(OBJ_DIR)/StringUtilsTest.o $(CXXFLAG) -c $(TESTSRC_DIR)/StringUtilsTest.cpp 

$(OBJ_DIR)/StringUtils.o: $(SRC_DIR)/StringUtils.cpp $(INC_DIR)/StringUtils.h
	$(CXX) -o $(OBJ_DIR)/StringUtils.o $(CXXFLAG) -c $(SRC_DIR)/StringUtils.cpp


#DSV
$(BIN_DIR)/testdsv: $(OBJ_DIR)/DSVTest.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/DSVWriter.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSink.o  $(OBJ_DIR)/StringUtils.o 
	$(CXX) -o $(BIN_DIR)/testdsv $(OBJ_DIR)/DSVTest.o $(OBJ_DIR)/StringUtils.o  $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/DSVWriter.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSink.o $(LDFLAGS)

$(OBJ_DIR)/DSVTest.o: $(OBJ_DIR)/StringUtils.o $(TESTSRC_DIR)/DSVTest.cpp $(INC_DIR)/DSVReader.h $(INC_DIR)/DSVWriter.h
	$(CXX) -o $(OBJ_DIR)/DSVTest.o $(OBJ_DIR)/StringUtils.o $(CXXFLAG) -c $(TESTSRC_DIR)/DSVTest.cpp 

$(OBJ_DIR)/DSVWriter.o: $(SRC_DIR)/DSVWriter.cpp $(INC_DIR)/DSVWriter.h $(OBJ_DIR)/StringDataSink.o
	$(CXX) -o $(OBJ_DIR)/DSVWriter.o $(OBJ_DIR)/StringDataSink.o $(CXXFLAG) -c $(SRC_DIR)/DSVWriter.cpp

$(OBJ_DIR)/DSVReader.o: $(SRC_DIR)/DSVReader.cpp $(INC_DIR)/DSVReader.h $(OBJ_DIR)/StringDataSource.o
	$(CXX) -o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/StringDataSource.o $(CXXFLAG) -c $(SRC_DIR)/DSVReader.cpp

#XML
$(BIN_DIR)/testxml: $(OBJ_DIR)/XMLTest.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSink.o
	$(CXX) -o $(BIN_DIR)/testxml $(OBJ_DIR)/XMLTest.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSink.o $(LDFLAGS)

$(OBJ_DIR)/XMLTest.o: $(TESTSRC_DIR)/XMLTest.cpp $(INC_DIR)/XMLReader.h $(INC_DIR)/XMLWriter.h
	$(CXX) -o $(OBJ_DIR)/XMLTest.o $(CXXFLAG) -c $(TESTSRC_DIR)/XMLTest.cpp 

$(OBJ_DIR)/XMLWriter.o: $(SRC_DIR)/XMLWriter.cpp $(INC_DIR)/XMLWriter.h $(OBJ_DIR)/StringDataSink.o
	$(CXX) -o $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/StringDataSink.o $(CXXFLAG) -c $(SRC_DIR)/XMLWriter.cpp

$(OBJ_DIR)/XMLReader.o: $(SRC_DIR)/XMLReader.cpp $(INC_DIR)/XMLReader.h $(OBJ_DIR)/StringDataSource.o
	$(CXX) -o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/StringDataSource.o $(CXXFLAG) -c $(SRC_DIR)/XMLReader.cpp



#OpenStreetMap
$(BIN_DIR)/testosm: $(OBJ_DIR)/OSMTest.o $(OBJ_DIR)/OpenStreetMap.o  $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/XMLReader.o
	$(CXX) -o $(BIN_DIR)/testosm $(OBJ_DIR)/OSMTest.o $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/StringDataSource.o  $(LDFLAGS)

$(OBJ_DIR)/OSMTest.o: $(TESTSRC_DIR)/OSMTest.cpp $(INC_DIR)/OpenStreetMap.h 
	$(CXX) -o $(OBJ_DIR)/OSMTest.o $(CXXFLAG) -c $(TESTSRC_DIR)/OSMTest.cpp $(LDFLAGS) 

$(OBJ_DIR)/OpenStreetMap.o: $(SRC_DIR)/OpenStreetMap.cpp $(INC_DIR)/OpenStreetMap.h $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/XMLReader.o
	$(CXX) -o $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/XMLReader.o $(CXXFLAG) -c $(SRC_DIR)/OpenStreetMap.cpp 

#CSVBus
$(BIN_DIR)/testcsvbs: $(OBJ_DIR)/CSVBusSystemTest.o $(OBJ_DIR)/CSVBusSystem.o  $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/DSVReader.o
	$(CXX) -o $(BIN_DIR)/testcsvbs $(OBJ_DIR)/CSVBusSystemTest.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/StringDataSource.o  $(LDFLAGS)

$(OBJ_DIR)/CSVBusSystemTest.o: $(TESTSRC_DIR)/CSVBusSystemTest.cpp $(INC_DIR)/CSVBusSystem.h 
	$(CXX) -o $(OBJ_DIR)/CSVBusSystemTest.o $(CXXFLAG) -c $(TESTSRC_DIR)/CSVBusSystemTest.cpp $(LDFLAGS) 

$(OBJ_DIR)/CSVBusSystem.o: $(SRC_DIR)/CSVBusSystem.cpp $(INC_DIR)/CSVBusSystem.h $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/DSVReader.o
	$(CXX) -o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/DSVReader.o $(CXXFLAG) -c $(SRC_DIR)/CSVBusSystem.cpp 

#CBusSystemIndexer
$(BIN_DIR)/testcsvbsi: $(OBJ_DIR)/CSVBusSystemIndexerTest.o $(OBJ_DIR)/CBusSystemIndexer.o  $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/CSVBusSystem.o
	$(CXX) -o $(BIN_DIR)/testcsvbsi $(OBJ_DIR)/CSVBusSystemIndexerTest.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/CBusSystemIndexer.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/StringDataSource.o  $(LDFLAGS)

$(OBJ_DIR)/CSVBusSystemIndexerTest.o: $(TESTSRC_DIR)/CSVBusSystemIndexerTest.cpp $(INC_DIR)/BusSystemIndexer.h 
	$(CXX) -o $(OBJ_DIR)/CSVBusSystemIndexerTest.o $(CXXFLAG) -c $(TESTSRC_DIR)/CSVBusSystemIndexerTest.cpp $(LDFLAGS) 

$(OBJ_DIR)/CBusSystemIndexer.o: $(SRC_DIR)/CBusSystemIndexer.cpp $(SRC_DIR)/CSVBusSystem.cpp $(INC_DIR)/BusSystemIndexer.h $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/CSVBusSystem.o
	$(CXX) -o $(OBJ_DIR)/CBusSystemIndexer.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/CSVBusSystem.o $(CXXFLAG) -c $(SRC_DIR)/CBusSystemIndexer.cpp 

#CDijkstraPathRouter
$(BIN_DIR)/testdpr: $(OBJ_DIR)/CDijkstraPathRouterTest.o $(OBJ_DIR)/CDijkstraPathRouter.o  $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/DSVReader.o
	$(CXX) -o $(BIN_DIR)/testdpr $(OBJ_DIR)/CDijkstraPathRouterTest.o $(OBJ_DIR)/CDijkstraPathRouter.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/StringDataSource.o  $(LDFLAGS)

$(OBJ_DIR)/CDijkstraPathRouterTest.o: $(TESTSRC_DIR)/CDijkstraPathRouterTest.cpp $(INC_DIR)/DijkstraPathRouter.h 
	$(CXX) -o $(OBJ_DIR)/CDijkstraPathRouterTest.o $(CXXFLAG) -c $(TESTSRC_DIR)/CDijkstraPathRouterTest.cpp $(LDFLAGS) 

$(OBJ_DIR)/CDijkstraPathRouter.o: $(SRC_DIR)/CDijkstraPathRouter.cpp $(INC_DIR)/DijkstraPathRouter.h $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/CSVBusSystem.o
	$(CXX) -o $(OBJ_DIR)/CDijkstraPathRouter.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/CSVBusSystem.o $(CXXFLAG) -c $(SRC_DIR)/CDijkstraPathRouter.cpp 	


#TransportationPlanner
$(BIN_DIR)/testtp: $(OBJ_DIR)/CSVOSMTransportationPlannerTest.o $(OBJ_DIR)/CBusSystemIndexer.o $(OBJ_DIR)/CDijkstraTransportationPlanner.o $(OBJ_DIR)/CDijkstraPathRouter.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/GeographicUtils.o
	$(CXX) -o $(BIN_DIR)/testtp $(OBJ_DIR)/CSVOSMTransportationPlannerTest.o $(OBJ_DIR)/CDijkstraPathRouter.o $(OBJ_DIR)/CBusSystemIndexer.o  $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/CDijkstraTransportationPlanner.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/GeographicUtils.o $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/CSVBusSystem.o $(LDFLAGS)

$(OBJ_DIR)/CSVOSMTransportationPlannerTest.o: $(TESTSRC_DIR)/CSVOSMTransportationPlannerTest.cpp $(INC_DIR)/DijkstraTransportationPlanner.h $(OBJ_DIR)/CBusSystemIndexer.o $(OBJ_DIR)/CDijkstraPathRouter.o $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/GeographicUtils.o
	$(CXX) -o $(OBJ_DIR)/CSVOSMTransportationPlannerTest.o $(OBJ_DIR)/CDijkstraPathRouter.o  $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/CBusSystemIndexer.o $(OBJ_DIR)/GeographicUtils.o $(OBJ_DIR)/CSVBusSystem.o $(CXXFLAG) -c $(TESTSRC_DIR)/CSVOSMTransportationPlannerTest.cpp $(LDFLAGS) 

$(OBJ_DIR)/CDijkstraTransportationPlanner.o: $(SRC_DIR)/CDijkstraTransportationPlanner.cpp $(OBJ_DIR)/CDijkstraPathRouter.o $(OBJ_DIR)/CBusSystemIndexer.o $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/XMLReader.o $(INC_DIR)/DijkstraTransportationPlanner.h $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/GeographicUtils.o
	$(CXX) -o $(OBJ_DIR)/CDijkstraTransportationPlanner.o $(OBJ_DIR)/GeographicUtils.o $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/CDijkstraPathRouter.o $(OBJ_DIR)/CBusSystemIndexer.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/CSVBusSystem.o $(CXXFLAG) -c $(SRC_DIR)/CDijkstraTransportationPlanner.cpp 	

$(OBJ_DIR)/GeographicUtils.o: $(SRC_DIR)/GeographicUtils.cpp $(INC_DIR)/GeographicUtils.h
	$(CXX) -o $(OBJ_DIR)/GeographicUtils.o $(CXXFLAG) -c $(SRC_DIR)/GeographicUtils.cpp

#TransportationPlannerCommandLine
$(BIN_DIR)/testtpcl: $(OBJ_DIR)/TPCommandLineTest.o $(OBJ_DIR)/CTransportationPlannerCommandLine.o  $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/DSVReader.o
	$(CXX) -o $(BIN_DIR)/testtpcl $(OBJ_DIR)/TPCommandLineTest.o $(OBJ_DIR)/CTransportationPlannerCommandLine.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/StringDataSource.o  $(LDFLAGS)

$(OBJ_DIR)/TPCommandLineTest.o: $(TESTSRC_DIR)/TPCommandLineTest.cpp $(INC_DIR)/TransportationPlannerCommandLine.h 
	$(CXX) -o $(OBJ_DIR)/TPCommandLineTest.o $(CXXFLAG) -c $(TESTSRC_DIR)/TPCommandLineTest.cpp $(LDFLAGS) 

$(OBJ_DIR)/CTransportationPlannerCommandLine.o: $(SRC_DIR)/CTransportationPlannerCommandLine.cpp $(INC_DIR)/TransportationPlannerCommandLine.h $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/CSVBusSystem.o
	$(CXX) -o $(OBJ_DIR)/CTransportationPlannerCommandLine.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/CSVBusSystem.o $(CXXFLAG) -c $(SRC_DIR)/CTransportationPlannerCommandLine.cpp 	

#Testfiledatass
$(BIN_DIR)/testfiledatass: $(OBJ_DIR)/FileDataSSTest.o $(OBJ_DIR)/FileDataSource.o  $(OBJ_DIR)/FileDataSink.o $(OBJ_DIR)/FileDataFactory.o
	$(CXX) -o $(BIN_DIR)/testfiledatass $(OBJ_DIR)/FileDataSSTest.o $(OBJ_DIR)/FileDataSource.o $(OBJ_DIR)/FileDataSink.o $(OBJ_DIR)/FileDataFactory.o  $(LDFLAGS)

$(OBJ_DIR)/FileDataSSTest.o: $(TESTSRC_DIR)/FileDataSSTest.cpp $(INC_DIR)/FileDataSource.h $(INC_DIR)/FileDataSink.h $(INC_DIR)/FileDataFactory.h 
	$(CXX) -o $(OBJ_DIR)/FileDataSSTest.o $(CXXFLAG) -c $(TESTSRC_DIR)/FileDataSSTest.cpp $(LDFLAGS) 

$(OBJ_DIR)/FileDataSource.o: $(SRC_DIR)/FileDataSource.cpp $(INC_DIR)/FileDataSource.h 
	$(CXX) -o $(OBJ_DIR)/FileDataSource.o $(CXXFLAG) -c $(SRC_DIR)/FileDataSource.cpp 	

$(OBJ_DIR)/FileDataSink.o: $(SRC_DIR)/FileDataSink.cpp $(INC_DIR)/FileDataSink.h 
	$(CXX) -o $(OBJ_DIR)/FileDataSink.o $(CXXFLAG) -c $(SRC_DIR)/FileDataSink.cpp

$(OBJ_DIR)/FileDataFactory.o: $(SRC_DIR)/FileDataFactory.cpp $(INC_DIR)/FileDataFactory.h 
	$(CXX) -o $(OBJ_DIR)/FileDataFactory.o $(CXXFLAG) -c $(SRC_DIR)/FileDataFactory.cpp

#KML
$(BIN_DIR)/testkml: $(OBJ_DIR)/KMLWriter.o  $(OBJ_DIR)/KMLTest.o $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/StringDataSink.o
	$(CXX) -o $(BIN_DIR)/testkml $(OBJ_DIR)/KMLWriter.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/KMLTest.o $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/XMLWriter.o $(LDFLAGS)

$(OBJ_DIR)/KMLTest.o: $(TESTSRC_DIR)/KMLTest.cpp $(INC_DIR)/KMLWriter.h $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/XMLWriter.o
	$(CXX) -o $(OBJ_DIR)/KMLTest.o  $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringUtils.o $(CXXFLAG) -c $(TESTSRC_DIR)/KMLTest.cpp 

$(OBJ_DIR)/KMLWriter.o: $(SRC_DIR)/KMLWriter.cpp $(INC_DIR)/KMLWriter.h  $(OBJ_DIR)/StringUtils.o  $(INC_DIR)/XMLWriter.h $(OBJ_DIR)/XMLWriter.o
	$(CXX) -o $(OBJ_DIR)/KMLWriter.o $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/StringUtils.o  $(CXXFLAG) -c $(SRC_DIR)/KMLWriter.cpp 


#transplanner

$(BIN_DIR)/transplanner: $(TESTSRC_DIR)/transplanner.cpp 
	$(CXX) -o $(BIN_DIR)/transplanner $(CXXFLAG) -c $(TESTSRC_DIR)/transplanner.cpp 

#speedtest
$(BIN_DIR)/speedtest: $(SRC_DIR)/speedtest.cpp 
	$(CXX) -o $(BIN_DIR)/speedtest $(CXXFLAG) -c $(SRC_DIR)/speedtest.cpp 


directories:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)

clean:
	rm -r $(BIN_DIR)
	rm -r $(OBJ_DIR)