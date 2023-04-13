#include "vertex.hpp"
#include "Commify.hpp"
#include <map>
#include <iterator>
#include <fstream>
#include <queue>
#include <set>
#include <cmath>

class Graph {

  private:
  map < uint32_t,Vertex* > vertices; // key is vertex_id and the value is de vertex
  map <pair<uint32_t,uint32_t>, Edge> road ;

  public:

    Graph(){}

// Ce constructeur permet de lire les fichiers dans le fichier grap_dc_area.csv
Graph(string file_name){
			
			// Ouverture du fichier passé en paramètre
			ifstream file(file_name); 
			if(!file.is_open()) {
				cerr << "Fichier non trouvé" << endl;
				exit(EXIT_FAILURE);
			}
			// Lecture du fichier
			string line;
			while (getline(file,line)){

				// on traite chaque ligne une à une
				if (line[0]!='#') {
					// si la ligne n'est pas un commentaire
					
					stringstream ss(line);
					string data;
					
					vector<string> datas; // on stocke chaque données de la ligne dans le vecteur de string nommé datas
					while(getline(ss,data,','))  // on sépare les données avec la virgule
          {
						datas.push_back(data);  // on ajoute la donnée au vecteur datas
					}
					
					if (line[0]=='V'){
						// si la ligne correspond à un vertex
						initVertex(datas);
					}
					
					else if (line[0]=='E'){
						// si la ligne correspond à un edge				
						initEdge(datas);
					}
					
				}
			}
					
		}


void initVertex(const vector<string>  datas){
  Vertex *v = new Vertex(stol(datas[1]),stod(datas[2]),stod(datas[3])) ;// création du vertex
	addVertex(v);  // on rajoute le vertex dans la map vertices de la classe Graph
}

double heuristic_distance_estimator(uint32_t id1, uint32_t id2){
    Vertex *v1 = getVertex(id1) ;
    Vertex *v2 = getVertex(id2) ;
    const double R = 6371000; // Earth's radius in meters
    const double lambda_c = 2.3518; // Paris longitude in radians
    const double phi_c = 48.8566; // Paris latitude in radians
    const double R0 = R * cos(phi_c); // Local radius of curvature at Paris latitude
    double delta_x = R0*M_PI*(v1->get_longitude()-v2->get_longitude())/180 ;
    //cout << v1->get_longitude() << endl  ;
    //cout << v1->get_lattitude() << endl ;
    double delta_y = R*(log(tan (M_PI / 4 + M_PI*(v1->get_lattitude()-phi_c) / 360))- log(tan((M_PI / 4 + M_PI*(v1->get_lattitude()-phi_c) / 360) )) );
    return sqrt(pow(delta_x,2)+pow(delta_y,2)) ;
}


// Rôle : initialiser un Edge en fonction d'un string
  void initEdge(const vector<string>  datas){
    Edge e(stol(datas[1]),stol(datas[2]),stod(datas[3])); // création du edge
	  addEdge(e) ; // on rajoute le edge dans la map edges
    AddAdjacentlist(e) ; // on ajoute le Edge dans une liste d'adjacence
  }



  void addVertex(Vertex *newVertex) {
    vertices.emplace(newVertex->getStateID(),newVertex) ;
  }

  Vertex *getVertex(uint32_t vid) {
    return vertices[vid];
  }

  
  Edge getEdge(uint32_t sid, uint32_t did){
    return road[pair(sid,did)] ;
  }



  void addEdge(Edge e) {
    road.emplace(pair(e.getSourceVertexID(),e.getDestinationVertexID()),e) ;
  }



  void AddAdjacentlist( Edge e){
    Vertex *v = getVertex(e.getSourceVertexID()) ;
    v->addEdgeToEdgelist(e) ;
  }



  void bfs(uint32_t vstart, uint32_t vend) {
   
    Commify t = Commify() ; // Début du décompte temporel ;
    deque<uint32_t> active_queue; // conteneur des vertex à expecter
    set<uint32_t> closed_set;     //
    deque <uint32_t> path ;
    ostringstream end_result;
    int path_counter = 0 ;
    map<uint32_t,uint32_t> parent ;


    active_queue.push_back(vstart);
    
    //end_result << "Vertex ["<<to_string(path_counter)<<"] ="<<
    //to_string(vstart)<< " ,length = 0.0 "<< endl ;

    do {

      auto vcurrent = active_queue.front();
      active_queue.pop_front();
      closed_set.insert(vcurrent);

      if (vend == vcurrent) {
        //cout << "It's the same point : " << to_string(path_counter) << endl;
        break;
      }

      for (Edge e : getVertex(vcurrent)->getEdgeList()) {
        uint32_t vnext = e.getDestinationVertexID();

        if (closed_set.find(vnext) != closed_set.end()) {
          continue;
        }

        if (find(active_queue.begin(), active_queue.end(), vnext) == active_queue.end()) {

          active_queue.push_back(vnext);
          parent[vnext] = vcurrent;
        }
      }
    } while (!active_queue.empty());


    uint32_t current = vend;
    while (current != vstart) {
      path.push_front(current);
      current = parent[current];
    }
    path.push_front(vstart) ;
    path_counter = path.size() ;

    int compt = 1 ;
    double length = 0.0 ;
  while(!path.empty()){
    end_result << "Vertex ["<<to_string(compt)<<"] = "<<
    to_string(path.front())<< " ,length = "<< to_string(length) <<endl ;
    compt++ ;
    length +=getEdge(path.front(),path[1]).getLength() ;
    path.pop_front() ;
  }
    t.set_timer() ; // temps de simulation
    cout << "\nBFS Algorithm "<< endl ;
    cout << "Total visted vertex :" << to_string(closed_set.size()) << endl;
    cout << "Length of the path :" << to_string(path_counter) << endl;
    cout <<end_result.str() ;
    cout << t ;
  }

// avec l'utilisation de mercator
// Convert longitude and latitude to Cartesian coordinates using Mercator's formula


// Cette fonction initialise la variable weight de chacun des vertex à value
void set_all_vertex_to_max_value(double value){
  for(map < uint32_t,Vertex* >::iterator it = vertices.begin(); it!=vertices.end(); it++){
    it->second->Set_weight(0.0) ;
  }
}


// Dijkstra algorithm 
void djikstra (uint32_t vstart, uint32_t vend){

  Commify t = Commify() ; // Début du décompte temporel ;
  deque<uint32_t> active_queue ;
  set<uint32_t> closed_set ;
  set_all_vertex_to_max_value(numeric_limits<double>::max()) ;
  map<uint32_t,uint32_t> parent ;
  deque <uint32_t> path ;
  ostringstream end_result;
  int path_counter ;

  //ID of the start Vertex
  active_queue.push_back(vstart) ;

  do{

    // from the current vertex in the front of the queue
    // compute all vertices reachable in 1 step
    auto vcurrent = active_queue.front();
    active_queue.pop_front() ;

    if(vcurrent==vend) break ;

    closed_set.insert(vcurrent) ;

    for(Edge e : getVertex(vcurrent)->getEdgeList()){
      uint32_t vnext = e.getDestinationVertexID() ;

      if(closed_set.find(vnext) != closed_set.end()) continue ; 

      auto w = getVertex(vcurrent)->get_weight() + getEdge(vcurrent,vnext).getLength() ;
      
      if(find(active_queue.begin(), active_queue.end(), vnext) == active_queue.end()){
        getVertex(vnext)->Set_weight(w) ;
        active_queue.push_back(vnext) ;
        parent[vnext] = vcurrent;
      } 
      else if (w < getVertex(vnext)->get_weight()){
        getVertex(vnext)->Set_weight(w) ;
        parent[vnext] = vcurrent;
      }
    }

    auto sort_function = [this](uint32_t a, uint32_t b) { 
      return this->getVertex(a)->get_weight() < this->getVertex(b)->get_weight(); 
    } ;
    
    sort(active_queue.begin(), active_queue.end(), sort_function);
  } while(!active_queue.empty()) ;


  uint32_t current = vend ;
  while(current != vstart) {
    path.push_front(current) ;
    current = parent[current];
  }
  path.push_front(vstart) ;
  path_counter = path.size() ;

  int compt = 1 ;
  double length = 0.0 ;
  while(!path.empty()){
    end_result << "Vertex ["<<to_string(compt)<<"] = "<<
    to_string(path.front())<< " ,length = "<< to_string(length) <<endl ;
    compt++ ;
    length +=getEdge(path.front(),path[1]).getLength() ;
    path.pop_front() ;
  }

  t.set_timer() ;
  cout << "\nDjikstra's Algorithm "<< endl ;
  cout << "Total visited vertex :" << to_string(closed_set.size()) << endl;
  cout << "Length of the path :" << to_string(path_counter) << endl;
  cout <<end_result.str() ;
  cout << t ;
}



//Astar Algorithm

void astar (uint32_t vstart, uint32_t vend){

  Commify t = Commify() ; // Début du décompte temporel ;
  deque <uint32_t> active_queue ;
  set<uint32_t> closed_set ;
  map<uint32_t,uint32_t> parent ;
  deque <uint32_t> path ;
  ostringstream end_result;
  int path_counter ;
  set_all_vertex_to_max_value(numeric_limits<double>::max()) ;

  //ID of the start vertex
  active_queue.push_back(vstart) ;

  // Set start vertex weight to 0
  getVertex(vstart)->Set_weight(0.00);
  
  do{

    // from the current vertex in the front of the queue
    // compute all vertices reachable in 1 step
    auto vcurrent = active_queue.front();
    active_queue.pop_front() ;

    if(vcurrent==vend) break ;

    closed_set.insert(vcurrent) ;

    for(Edge e : getVertex(vcurrent)->getEdgeList()){

      uint32_t vnext = e.getDestinationVertexID() ;

      if(closed_set.find(vnext) != closed_set.end()) continue ; 

      auto w = getVertex(vcurrent)->get_weight() + getEdge(vcurrent,vnext).getLength() ;
      auto f = w + heuristic_distance_estimator(vnext,vend) ;
      
      if(find(active_queue.begin(), active_queue.end(), vnext) == active_queue.end()){
        getVertex(vnext)->Set_weight(w) ;
        getVertex(vnext)->Set_estimate(f) ;
        active_queue.push_back(vnext) ;
        parent[vnext] = vcurrent;
      } 
      else if (f < getVertex(vnext)->get_estimate()){
        getVertex(vnext)->Set_weight(w) ;
        getVertex(vnext)->Set_estimate(f) ;
        parent[vnext] = vcurrent;
      }
    }

    auto sort_function_on_estimate = [&](uint32_t a, uint32_t b) { 
      return getVertex(a)->get_estimate() < getVertex(b)->get_estimate() ; 
    } ;
    
    sort(active_queue.begin(), active_queue.end(), sort_function_on_estimate);

  } while(!active_queue.empty()) ;


  uint32_t current = vend ;
  while(current != vstart) {
    path.push_front(current) ;
    current = parent[current];
  }
  path.push_front(vstart) ;
  path_counter = path.size() ;

  int compt = 1 ;
  double length = 0.0 ;
  while(!path.empty()){
    end_result << "Vertex ["<<to_string(compt)<<"] = "<<
    to_string(path.front())<< " ,length = "<< to_string(length) <<endl ;
    compt++ ;
    length +=getEdge(path.front(),path[1]).getLength() ;
    path.pop_front() ;
  }

  t.set_timer() ;
  cout << "\nAstar's Algorithm "<< endl ;
  cout << "Total visited vertex :" << to_string(closed_set.size()) << endl;
  cout << "Length of the path :" << to_string(path_counter) << endl;
  cout <<end_result.str()<< endl  ;
  cout << t ;
}

} ;