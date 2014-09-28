#include "brute.hpp"
#include <thread>

/*
 * TODO: is it possible to make mutex member variable??
 */

std::mutex th_mutex;									// gegen konkurrierende Threads (feste Reihenfolge)
std::atomic<bool> Brute::pw_found (0);
std::map<std::thread::id, un_int> Brute::thread_ids;

Brute::Brute() : min_astriction(33), max_astriction(127)  {}

Brute::~Brute() {}									// Destruktor

void Brute::brute_impl(std::string str, int index, int max_depth, std::string end_pw)
{ 
    for (un_int i = static_cast<un_int>(str.at(index)); i < 127; ++i)
    {
        str.at(index) = static_cast<char>(i);
        
        if(this->pw_found)  return;
	
        if (index == max_depth - 1)
        {
	    th_mutex.lock();
            std::cout << "Thread " << get_this_thread_no() << ": " << str << end_pw <<  std::endl;	
	    th_mutex.unlock();
             if(str == password_test){
                 th_mutex.lock();
                std::cout << "Passwort gefunden: \n>>  \033[1;32m" << str << "\033[0m" << std::endl;
                th_mutex.unlock();
                this->pw_found = 1;
            }
	    if(str == end_pw) {
                th_mutex.lock();
                 std::cout << "Thread \033[1;31m" << get_this_thread_no() << 
                "\033[0m has been finished without result. Its last PW was \"" << 
                end_pw << "\"." << std::endl;
                th_mutex.unlock();
                return;			// if pw_tmp is equal to the last pw return from this function and leave the thread 
            }  
         }else{
            brute_impl(str, index + 1, max_depth, end_pw);
        }
    }
}

void Brute::start_brute(){
    
    th_mutex.lock();
   this->thread_ids.insert(std::make_pair<std::thread::id, un_int>(std::this_thread::get_id(), thread_ids.size() + 1));         // + 1 to make ThreadNumber human readable (Thread 1 instead of 0)
   th_mutex.unlock();
   
   std::string pw_tmp = "";
   
   if(std::this_thread::get_id() == thread_ids.begin()->THREAD_ID){
     pw_tmp = std::string("").append<int>(Parameter.pInfos.len_min, 0x21); 	// first PW is minimal pw-length * char 0x21 ('!')
   }else{
     pw_tmp = this->getLastPW();				// other PWs are saved in array
   }
   
    this->brute_impl(pw_tmp, 0, this->getLastPW().length(), this->getLastPW());
    //if(POST(pw_tmp).find(Parameter.pInfos.error_msg) == std::string::npos){   
}

std::string Brute::getLastPW(){	
  /*
   * Method to calculate last PW:
   * It's the same procedure as numbersystem conversion
   * get number of trys per Thread
   * take this number * current thread number (lets call the result pw_digits)
   * 1) pw_digits % ASCII_possibilities (number of possible ASCII-Chars (94)) = last char of new Password
   * 2) pw_digits = pw_digits / ASCII_possibilities
   * 3) repeat from 1 until pw_digits == 0
   * 4) if pw_digits is 0 and we reverse the password we got it
   */
   // get_astriction();
   

      un_int this_thread_number = get_this_thread_no();
      if(this_thread_number >= Parameter.pInfos.th_num) 				 			// if its the last thread
      return std::string("").append<int>(Parameter.pInfos.len_size, 0x7e); 			// last pw is always maxSize of pw * char 0x7e ('~')
    
    unsigned long long max_trys = 0;							// maximale Versuche
    for(un_int s = Parameter.pInfos.len_min; s <= Parameter.pInfos.len_size; s++) max_trys += pow(94, s);	// maximale Versuche ausrechnen
    

    double trys_pp = static_cast<double>(max_trys) / Parameter.pInfos.th_num;		// Trys Per Process		
    double calculateLastPW_tmp = trys_pp;
    std::string last_pw = "";							// last pw from special thread
	 
    /*
    * TODO: last calculated pw is nonsense (to high or to low char-values)
    * why it is like this?
    */
    while(trys_pp != 0){								// last_pw errechnen
	char newPWCharacter = 0;
	if((calculateLastPW_tmp * this_thread_number) <= 126){					// if product is above 94 but less than 126 
	  newPWCharacter = static_cast<char>(calculateLastPW_tmp * this_thread_number + 32);	// there should be no modulo, otherwise the solution will be wrong
	}else{									// otherwise
	  newPWCharacter = static_cast<char>(fmod(calculateLastPW_tmp * this_thread_number, 94) + 33); // calculate with modulo
	}
	    	    
	last_pw.push_back(newPWCharacter);						// push the Character to last position of string
	calculateLastPW_tmp = calculateLastPW_tmp / 94;				// calculateLastPW_tmp should be exact for calculating next char,
	trys_pp = floor(trys_pp / 94);						// trys_pp is for looping (must be floored)
    }
    std::reverse(std::begin(last_pw), std::end(last_pw));				// last_pw is wrong order -> we need to reverse it
    return last_pw;
}

const un_int Brute::get_this_thread_no() const{
    return (this->thread_ids.find(std::this_thread::get_id())->THREAD_NUMBER);      // unique number of this ThreadID
}

void Brute::get_astriction(){
  /*switch(Parameter.pInfos.astr){
    case 'c': min_astriction = 65; max_astriction = 90; break;				// char astriction
    case 'n': min_astriction = 48; max_astriction = 57; break;				// number astriction
    case 'b': break;									// both astriction (ohne Sonderzeichen)
  }*/
}
