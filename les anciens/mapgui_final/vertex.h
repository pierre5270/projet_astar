#include "edge.h"
#include <algorithm>
#include <list>
#include <map>
#include <vector>
#include <cmath>

class Vertex {
  public:
    uint32_t state_id; // identifiant du lieu
    double longitude ;
    double lattitude ;
    double weight ;
    double estimate ;
    vector<Edge> edgeList;
    //vector <uint32_t> *parent_map  ;


  public:
    Vertex(uint32_t id = 0, double lon =0.0, double latt=0.0) {
      state_id = id;
      longitude = lon;
      lattitude = latt ;
      estimate = 0.0 ;
    }

    uint32_t getStateID() const {
      return state_id;
    }

    vector <Edge> getEdgeList() const {
      return edgeList;
    }

    double get_longitude() const {
      return longitude ;
    }

    double get_lattitude() const {
      return lattitude ;
    }

    double get_weight() const {
      return weight ;
    }

    double get_estimate() const {
      return estimate ;
    }

    void setID(uint32_t id) {
      state_id = id;
    }

    void SetLong(double L)  {
      longitude = L ;
    }

    void SetLatt(double l){
      lattitude = l ;
    }

    void Set_weight(double w){
      weight = w ;
    }

    void Set_estimate(double f){
      estimate = f ;
    }

    void addEdgeToEdgelist(Edge e)  {
      //Edge e(sourcID,destVID,l,ex0,ex1);
      if(e.getSourceVertexID()==state_id){
        edgeList.push_back(e) ;
        //cout<<"Edge between "<<state_id<<" and "<<e.getDestinationVertexID()<<" added Successfully"<<endl;
      }
      else{
        cout << " Edge is not correct" << endl ;
      }
    }

    string toString() const {

    ostringstream ss ;
    ss << "Identifiant :"<< to_string(state_id) << endl ;
    ss << "Longitude :"<< to_string(longitude) << endl ;
    ss << "Lattitude :" << to_string(lattitude) << endl ;
    ss <<" Djikstra Weight : "<< to_string(weight)<< endl ;

    return ss.str() ;

    }


    friend ostream & operator<<(ostream &f, const Vertex v){
      return f << v.toString() ;
    }


    void printEdgeList() {

      cout << "[";
      for (auto it = edgeList.begin(); it != edgeList.end(); it++) {
        cout << *it <<endl ;
        cout << it -> getDestinationVertexID() << "(" << it -> getLength() << ") --> ";
      }
      cout << "]";
      cout << endl;
    }

};
