#include <iostream>
int main(int argc, char *argv[]){
 for(int i = 0; i < argc; i++){
 std::cout<<i<<" = "<<argv[i]<<std::endl;
 }
 return 0;
}
//link all the object files
//create osm create csv bus system (need to get dsv reader -> need data s)