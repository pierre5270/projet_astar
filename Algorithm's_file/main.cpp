#include "graph.hpp"
#include <string.h>
#include <chrono>

int main(int argc, char *argv[]) {


	int start=0 ;// le vertex de départ
  int end=0; // le vertex d'arrivée

	string algorithm;
  string file;
	
	for(int x=1;x<argc;x++) {
		string s(argv[x]);
		if(s=="--start")
			start=stoi(argv[x+1]);
				
		else if(s=="--end")
			end=stoi(argv[x+1]);
				
		else if(s=="--algorithm")
			algorithm=argv[x+1];

		else if(s=="--file")
			file=argv[x+1];
		
	}
  auto start_time = std::chrono::high_resolution_clock::now();

  Graph graph(file);


  if(algorithm == "bfs" ) graph.bfs(start, end);

  if(algorithm =="djikstra" ) graph.djikstra(start, end);

  if(algorithm =="astar" ) graph.astar(start, end);
  return EXIT_SUCCESS;
}