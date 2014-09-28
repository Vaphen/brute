#include <vector>	// std::vector
#include <thread>	// std::thread
#include <cmath>
#include "m_threads.hpp" // Header
#include "brute.hpp"     	// bruteforcing

m_threads::m_threads() {}

m_threads::~m_threads(){}

void m_threads::start_threads(){
  
  unsigned long long max_trys = 0;
  for(un_int s = Parameter.pInfos.len_min; s <= Parameter.pInfos.len_size; s++) max_trys += pow(94, s);
  std::cout << "Maximale Versuche: " << max_trys << std::endl;
  std::vector<std::thread> brute_threads;		// Thread-Vektor
  for(un_int i = 0; i < Parameter.pInfos.th_num; i++){		// Parameter.pInfos.max_threads einfuegen
    brute_threads.push_back(std::thread(&Brute::start_brute, Brute()));
  } 
  for(unsigned int i = 0; i < brute_threads.size(); i++){
      brute_threads.at(i).join();				// tell mainprogram to wait for end of threads
  }
}