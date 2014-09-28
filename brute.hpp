#ifndef _BRUTE_H_INCLUDED_
#define _BRUTE_H_INCLUDED_

#define THREAD_ID first
#define THREAD_NUMBER second

#include <iostream>
#include <cmath>	// pow(x,y) -> Potenzen
#include <cstdlib>	// exit()
#include <mutex>	// mutex.lock()
#include <cstring>
#include "param.hpp"	// Parameter
#include <algorithm> // invert array (PW-Array)
#include <atomic>
#include <thread>
#include <map>
// #include "http_req.h"	// HTTP-Request not needed until now

const std::string password_test = "~~~H";

void get_http();

class Brute {
public:
    Brute();
    ~Brute();
   void start_brute();
private:
    int min_astriction, max_astriction;
    void brute_impl(std::string, int, int, std::string); 
    std::string getLastPW();
    const un_int get_this_thread_no() const;
    void get_astriction();
    Param Parameter;
    static std::atomic<bool> pw_found;
    static std::map<std::thread::id, un_int> thread_ids;
};
#endif
