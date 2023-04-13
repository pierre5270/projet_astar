
#include<iostream>
#include <ostream>
#include <sstream>
#include <cassert>
using namespace std;


class Edge {
  private:
    uint32_t SourceVertexID;
    uint32_t DestinationVertexID ;
    double length;
    string name ;
    string extra0, extra1 ;
  public :
    Edge() {}
    Edge(uint32_t sourcID,uint32_t destVID, double l,string n ="",string ex0="",string ex1="") {

      SourceVertexID = sourcID ;
      DestinationVertexID = destVID ;
      name = n ;
      length = l ;
      extra0 = ex0 ;
      extra1 = ex1 ;
    }


    void SetSourceID(uint32_t sid){
      SourceVertexID = sid ;
    }


    void SetDestinationID(uint32_t did){
      DestinationVertexID = did ;
    }

    void setLength(double l) {
      length = l;
    }

    void SetName(string s){
      name = s ;
    }

    
    uint32_t getSourceVertexID() const {
      return SourceVertexID;
    }

    uint32_t getDestinationVertexID() const {
      return DestinationVertexID;
    }

    double getLength() const {
      return length;
    }

    string getName() const{
      return name ;
    }

    string toString() const {
      ostringstream ss ;
      ss<< "Source :"<< to_string(SourceVertexID) << endl ;
      ss <<"Destination :"<< to_string(DestinationVertexID) << endl ;
      ss << "length :"<< to_string(length) << endl ;
      ss<< "Name :"<< name << endl ;
      ss << "Reference_1 :"<< extra0 << endl ;
      ss << "Reference_2 :"<< extra1 << endl ;
      return ss.str() ;
    }

    friend ostream & operator<<(ostream &f, const Edge e){
      return f <<e.toString() ;
    }
  
};