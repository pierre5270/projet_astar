#include "vertex.h"
#include <fstream>
#include <iterator>
#include <map>
#include <queue>
#include <set>
#include <string.h>

class Graph {

private:
  map<uint32_t, Vertex *>
      vertices; // key is vertex_id and the value is de vertex
  map<pair<uint32_t, uint32_t>, Edge> road;

public:
  Graph() {}

  // Ce constructeur permet de lire les fichiers dans le fichier
  // grap_dc_area.csv
  Graph(string file_name) {

    // Ouverture du fichier passé en paramètre
    ifstream file(file_name);
    if (!file.is_open()) {
      cerr << "Fichier non trouvé" << endl;
      exit(EXIT_FAILURE);
    }
    // Lecture du fichier
    string line;
    while (getline(file, line)) {

      // on traite chaque ligne une à une
      if (line[0] != '#') {
        // si la ligne n'est pas un commentaire

        stringstream ss(line);
        string data;

        vector<string> datas; // on stocke chaque données de la ligne dans le
                              // vecteur de string nommé datas
        while (getline(ss, data, ',')) // on sépare les données avec la virgule
        {
          datas.push_back(data); // on ajoute la donnée au vecteur datas
        }

        if (line[0] == 'V') {
          // si la ligne correspond à un vertex
          initVertex(datas);
        }

        else if (line[0] == 'E') {
          // si la ligne correspond à un edge
          initEdge(datas);
        }
      }
    }
  }

  void initVertex(const vector<string> datas) {
    uint32_t id = static_cast<uint32_t>(
        stol(datas[1])); // convertit le string en long int puis en uint32_t
    double longitude = stod(datas[2]); // convertit en double
    double latitude = stod(datas[3]);  // convertit en double
    Vertex *v = new Vertex(id, longitude, latitude);
    ; // création du vertex
    addVertex(
        v); // on rajoute le vertex dans la map vertices de la classe Graph
  }

  double heuristic_distance_estimator(uint32_t id1, uint32_t id2) {
    Vertex *v1 = getVertex(id1);
    Vertex *v2 = getVertex(id2);
    const double R = 6371000;       // Earth's radius in meters
    const double lambda_c = 2.3518; // Paris longitude in radians
    const double phi_c = 48.8566;   // Paris latitude in radians
    const double R0 =
        R * cos(phi_c); // Local radius of curvature at Paris latitude
    double delta_x = R0 * (v1->get_longitude() - v2->get_longitude());
    double delta_y = R0 * (log(tan(M_PI / 4 + v1->get_lattitude() / 2)) -
                           log(tan(M_PI / 4 + v2->get_lattitude() / 2)));
    // cout << to_string(sqrt(pow(delta_x,2)+pow(delta_y,2)))<<endl ;
    return sqrt(pow(delta_x, 2) + pow(delta_y, 2));
  }
  double to_cartesianX(int id) {

    Vertex * v= getVertex(id);

      const double R = 6371000;       // Earth's radius in meters
    const double lambda_c = M_PI*64.56/180; // Paris longitude in radians
    const double phi_c = M_PI*18.26/180;   // Paris latitude in radians
    const double R0 =
        R * cos(phi_c); // Local radius of curvature at Paris latitude
 return  R0 * M_PI*(v->get_longitude() - lambda_c)/180;


  }
  double  to_cartesianY(int id) {
    vector<double> tab;
    Vertex * v= getVertex(id);
    double x,y;
      const double R = 6371000;       // Earth's radius in meters
    const double lambda_c = M_PI*64.56/180; // Paris longitude in radians
    const double phi_c = M_PI*18.26/180;   // Paris latitude in radians
    const double R0 =
        R * cos(phi_c); // Local radius of curvature at Paris latitude

  return  R * log(tan(M_PI / 4 + M_PI*(v->get_lattitude() - phi_c) / 360));

  }


  // Rôle : initialiser un Edge en fonction d'un string
  void initEdge(const vector<string> datas) {
    uint32_t start = static_cast<uint32_t>(
        stol(datas[1])); // convertit le string en long int puis en uint32_t
    uint32_t end = static_cast<uint32_t>(
        stol(datas[2])); // convertit le string en long int puis en uint32_t
    double length = stod(datas[3]);
    Edge e(start, end, length); // création du edge
    addEdge(e);                 // on rajoute le edge dans la map edges
    AddAdjacentlist(e);         // on ajoute le Edge dans une liste d'adjacence
  }

  bool checkIfGraphEmpty() {
    return this->vertices.empty() && this->road.empty();
  }

  bool checkIfVertexExist(uint32_t vid) {

    if (!this->checkIfGraphEmpty()) {
      for (map<uint32_t, Vertex *>::iterator it = vertices.begin();
           it != vertices.end(); it++) {
        if (it->first == vid) {
          return true;
        }
      }
    }
    return false;
  }

  void addVertex(Vertex *newVertex) {
    vertices.emplace(newVertex->getStateID(), newVertex);
  }

  Vertex *getVertex(uint32_t vid) { return vertices[vid]; }

  Edge getEdge(uint32_t sid, uint32_t did) { return road[pair(sid, did)]; }

  // I have to do this

  bool checkIfEdgeExist(Edge e) {

    if (this->checkIfGraphEmpty())
      return false;
    for (map<pair<uint32_t, uint32_t>, Edge>::iterator it = road.begin();
         it != road.end(); it++) {
      if (it->first ==
          pair(e.getSourceVertexID(), e.getDestinationVertexID())) {
        return true;
      }
    }
    return false;
  }

  void addEdge(Edge e) {
    road.emplace(pair(e.getSourceVertexID(), e.getDestinationVertexID()), e);
  }

  void updateEdge(Edge e, int newWeight) {
    for (map<pair<uint32_t, uint32_t>, Edge>::iterator it = road.begin();
         it != road.end(); it++) {
      if (it->first == pair(e.getSourceVertexID(), e.getDestinationVertexID()))
        it->second.setLength(newWeight);
      break;
    }
  }

  void deleteEdge(Edge e) {
    if (checkIfEdgeExist(e)) {
      road.erase(pair(e.getSourceVertexID(), e.getDestinationVertexID()));
    } else {
      cout << " Edge is not exist" << endl;
    }
  }

  void deleteVertexByID(uint32_t vid) {
    if (checkIfVertexExist(vid)) {
      cout << "Vertex Deleted Successfully" << endl;
      vertices.erase(vid);
    } else {
      cout << "Vertex is not exist " << endl;
    }
  }

  void AddAdjacentlist(Edge e) {
    Vertex *v = getVertex(e.getSourceVertexID());
    v->addEdgeToEdgelist(e);
  }

  void getAllNeigborsByID(int vid) {
    vector<Edge> v = getVertex(vid)->getEdgeList();
    // cout << v.size() << endl ;
    for (vector<Edge>::iterator it = v.begin(); it != v.end(); it++) {
      for (map<uint32_t, Vertex *>::iterator it2 = vertices.begin();
           it2 != vertices.end(); it2++) {
        if ((*it).getDestinationVertexID() == it2->second->getStateID())
          cout << it2->second << endl;
      }
    }
  }

  void printGraph() {
    cout << "Liste des Vertex :" << endl;
    for (map<uint32_t, Vertex *>::iterator it = vertices.begin();
         it != vertices.end(); it++)
      cout << *it->second << endl;

    cout << "Liste des chemins (Edge) " << endl;
    for (map<pair<uint32_t, uint32_t>, Edge>::iterator it = road.begin();
         it != road.end(); it++)
      cout << it->second << endl;
  }

  void bfs(uint32_t vstart, uint32_t vend,vector<uint32_t>*tab_queue,vector<uint32_t> *tab_set) {
    deque<uint32_t> active_queue; // conteneur des vertex à expecter
    set<uint32_t> closed_set;     //
    uint32_t path_counter = 0;
    ostringstream end_result;

;
    vector<uint32_t> parent(100 * vertices.size());
    active_queue.push_back(vstart);
    path_counter++;
    // end_result << "Vertex ["<<to_string(path_counter)<<"] ="<<
    // to_string(vstart)<< " ,length = 0.0 "<< endl ;

    do {

      auto vcurrent = active_queue.front();
      active_queue.pop_front();
      closed_set.insert(vcurrent);
        tab_queue->push_back(vcurrent);

      if (vend == vcurrent) {
        // cout << "It's the same point : " << to_string(path_counter) << endl;
        break;
      }

      for (Edge e : getVertex(vcurrent)->getEdgeList()) {
        uint32_t vnext = e.getDestinationVertexID();

        if (closed_set.find(vnext) != closed_set.end()) {

          continue;
        }

        if (find(active_queue.begin(), active_queue.end(), vnext) ==
            active_queue.end()) {

          active_queue.push_back(vnext);
          tab_set->push_back(vnext);
          parent[vnext] = vcurrent;
        }
      }
    } while (!active_queue.empty());

    uint32_t current = vend;
    while (current != vstart) {
      current = parent[current];
      path_counter++;
      // end_result << "Vertex ["<<to_string(path_counter)<<"] ="<<
      // to_string(current)<< " ,length = "<< endl ;
    }
/*            auto *item2=new QGraphicsLineItem(x1,y1,x2,y2);
            QPen pen1;
            pen1.setWidth(20);
            pen1.setColor(QColor(128, 109, 52));
            item2->setLine(x1+20,y1+20,x2+20,y2+20);
            item2->setPen(pen1 );

            scene_->addItem(item2);*/
    cout << "\nBFS Algorithm " << endl;
    cout << "Total visted vertex :" << to_string(closed_set.size()) << endl;
    cout << "Length of the path :" << to_string(path_counter) << endl;
    // cout <<end_result.str() ;
  }

  // avec l'utilisation de mercator
  // Convert longitude and latitude to Cartesian coordinates using Mercator's
  // formula

  // Cette fonction initialise la variable weight de chacun des vertex à value
  void set_all_vertex_to_max_value(double value) {
    for (map<uint32_t, Vertex *>::iterator it = vertices.begin();
         it != vertices.end(); it++) {
      it->second->Set_weight(0.0);
    }
  }

  // Dijkstra algorithm
  void djikstra(uint32_t vstart, uint32_t vend) {

    deque<uint32_t> active_queue;
    set<uint32_t> closed_set;
    set_all_vertex_to_max_value(0.0);
    uint32_t path_counter = 0;
    vector<uint32_t> parent(100 * vertices.size());
    ostringstream end_result;
    // path_counter++;

    // ID of the start Vertex
    active_queue.push_back(vstart);
    end_result << "Vertex [" << to_string(path_counter)
               << "] =" << to_string(vstart) << " ,length = 0.0 " << endl;
    do {

      // from the current vertex in the front of the queue
      // compute all vertices reachable in 1 step
      auto vcurrent = active_queue.front();
      active_queue.pop_front();

      if (vcurrent == vend)
        break;

      closed_set.insert(vcurrent);

      for (Edge e : getVertex(vcurrent)->getEdgeList()) {
        uint32_t vnext = e.getDestinationVertexID();

        if (closed_set.find(vnext) != closed_set.end())
          continue;

        auto w = getVertex(vcurrent)->get_weight() +
                 getEdge(vcurrent, vnext).getLength();

        if (find(active_queue.begin(), active_queue.end(), vnext) ==
            active_queue.end()) {
          getVertex(vnext)->Set_weight(w);
          active_queue.push_back(vnext);
          parent[vnext] = vcurrent;
        } else if (w < getVertex(vnext)->get_weight()) {
          getVertex(vnext)->Set_weight(w);
          parent[vnext] = vcurrent;
        }
      }

      auto sort_function = [this](uint32_t a, uint32_t b) {
        return this->getVertex(a)->get_weight() <
               this->getVertex(b)->get_weight();
      };

      sort(active_queue.begin(), active_queue.end(), sort_function);
    } while (!active_queue.empty());

    uint32_t current = vend;
    while (current != vstart) {
      current = parent[current];
      path_counter++;
      end_result << "Vertex [" << to_string(path_counter)
                 << "] =" << to_string(current) << " ,length = " << endl;
    }

    cout << "\nDjikstra's Algorithm " << endl;
    cout << "Total visited vertex :" << to_string(closed_set.size()) << endl;
    cout << "Length of the path :" << to_string(path_counter) << endl;
    // cout <<end_result.str() ;
  }

  // Astar Algorithm

  void astar(uint32_t vstart, uint32_t vend) {
    deque<uint32_t> active_queue;
    set<uint32_t> closed_set;
    vector<uint32_t> parent(50 * vertices.size());
    ostringstream end_result;
    int path_counter = 0;
    set_all_vertex_to_max_value(0.0);

    // ID of the start vertex
    active_queue.push_back(vstart);
    // end_result << "Vertex ["<<to_string(path_counter)<<"] ="<<
    // to_string(vstart)<< " ,length = 0.0 "<< endl ;

    do {

      // from the current vertex in the front of the queue
      // compute all vertices reachable in 1 step
      auto vcurrent = active_queue.front();
      active_queue.pop_front();

      if (vcurrent == vend)
        break;

      closed_set.insert(vcurrent);

      for (Edge e : getVertex(vcurrent)->getEdgeList()) {
        uint32_t vnext = e.getDestinationVertexID();

        if (closed_set.find(vnext) != closed_set.end())
          continue;

        auto g = getVertex(vcurrent)->get_weight() +
                 getEdge(vcurrent, vnext).getLength();
        auto f = g + heuristic_distance_estimator(vnext, vend);

        if (find(active_queue.begin(), active_queue.end(), vnext) ==
            active_queue.end()) {
          getVertex(vnext)->Set_weight(g);
          getVertex(vnext)->Set_estimate(f);
          // cout << getVertex(vnext)->get_estimate() << endl ;
          active_queue.push_back(vnext);
          parent[vnext] = vcurrent;
        } else if (f < getVertex(vnext)->get_estimate()) {
          // cout << "hello2" << endl ;
          getVertex(vnext)->Set_weight(g);
          getVertex(vnext)->Set_estimate(f);
          parent[vnext] = vcurrent;
        }
      }

      auto sort_function_on_estimate = [this](uint32_t a, uint32_t b) {
        return this->getVertex(a)->get_estimate() <
               this->getVertex(b)->get_estimate();
      };

      sort(active_queue.begin(), active_queue.end(), sort_function_on_estimate);
    } while (!active_queue.empty());

    uint32_t current = vend;
    while (current != vstart) {
      current = parent[current];
      // end_result << "Vertex ["<<to_string(path_counter)<<"] ="<<
      // to_string(current)<< " ,length = "<< endl ;
      path_counter++;
    }

    cout << "\nAstar's Algorithm " << endl;
    cout << "Total visited vertex :" << to_string(closed_set.size()) << endl;
    cout << "Length of the path :" << to_string(path_counter) << endl;

  }
};
