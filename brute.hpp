#ifndef _BRUTE_H_INCLUDED_
#define _BRUTE_H_INCLUDED_

#include <iostream>
#include <cmath>	// pow(x,y) -> Potenzen
#include <cstdlib>	// exit()
#include <mutex>	// mutex.lock()
#include <cstring>
#include "param.hpp"	// Parameter
#include <algorithm> // invert array (PW-Array)
// #include "http_req.h"	// HTTP-Request not needed until now

const std::string password_test = "0}3l";

void get_http();

class Brute {
public:
    Brute();
    ~Brute();
   void start_brute(un_int);
private:
    int min_astriction, max_astriction;
    void brute_impl(std::string, int, int, std::string, un_int); 
    std::string getLastPW(un_int);
    void get_astriction();
    Param Parameter;
};
#endif
