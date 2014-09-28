#include <iostream>	// cout
#include "param.hpp"	// getParams
#include "m_threads.hpp" // Start threads & brute


typedef unsigned int un_int;

int main(int argc, char **argv){
  Param myParams(argc, argv);
  myParams.setParams();
  m_threads myThreads;
  myThreads.start_threads();
  return 1;
}
