#pragma once
#include <chrono>
#include <iostream> 
using namespace std;

class Commify {
    private:
        std::chrono::microseconds value_;
        std::chrono::time_point<std::chrono::high_resolution_clock> clock ; 
    public:
        explicit Commify(std::chrono::microseconds value = std::chrono::microseconds(0)) : value_{value} {
            clock = std::chrono::high_resolution_clock::now();
        }
        
        void set_timer(){
            auto end_time = std::chrono::high_resolution_clock::now();
            value_ = std::chrono::duration_cast<std::chrono::microseconds>(end_time - clock) ;
        }
        
        string toString() const {
            ostringstream s;

            string time = to_string(value_.count()) ;// temps de simulation en microsecondes

            int insertPosition = time.length() - 3; 
            while (insertPosition > 0) {
                time.insert(insertPosition, ",");// insertion du séparateur dans le terme 
                insertPosition -= 3;
            }
            s << "Info path Calculated: " << time << " us" << endl; // affichage du temps de simulation
            return s.str() ;
        }


        friend std::ostream& operator<<(std::ostream& f, const Commify& c) {
           return f << c.toString();
        }

};
