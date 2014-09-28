#include "brute.hpp"

/*
 * TODO: is it possible to make mutex member variable??
 */

std::mutex th_mutex;									// gegen konkurrierende Threads (feste Reihenfolge)

Brute::Brute() : min_astriction(33), max_astriction(127) {}

Brute::~Brute() {}									// Destruktor

//void Brute::brute_impl(std::string& pw_tmp, int index, int max_depth, un_int& thread_no){
 /* std::string buf = "";
  for(int i = 33; i < 127; i++){							// alle ASCII-Values durchgehen
    if(thread_no == 0){
      pw_tmp = std::string("").append<int>(Parameter.pInfos.len_min, 0x21); 
    }else{
      pw_tmp = lastPWsForThreads.at(thread_no - 1);
    }
    if(index == max_depth - 1){	
      if(pw_tmp == lastPWsForThreads.at(thread_no)) return;				// if pw_tmp is equal to the last pw return from this function and leave the thread 
      //if(POST(pw_tmp).find(Parameter.pInfos.error_msg) == std::string::npos){
      if(pw_tmp == password_test){
	  std::cout << "Passwort gefunden: " << pw_tmp << std::endl;
	  exit(EXIT_SUCCESS);
      }
    }else{								// naechstes passwort
      brute_impl(pw_tmp, index + 1, max_depth, thread_no);		// function recursiv aufrufen
    }
  }*/
//}

void Brute::brute_impl(std::string str, int index, int max_depth, std::string end_pw, un_int thread_no)
{
    for (un_int i = static_cast<un_int>(str.at(index)); i < 127; ++i)
    {
        str.at(index) = static_cast<char>(i);
	
        if (index == max_depth - 1)
        {
	  th_mutex.lock();
            std::cout << "Thread " << thread_no << ": " << str << end_pw <<  std::endl;	
	    th_mutex.unlock();
	    if(str == end_pw) return;			// if pw_tmp is equal to the last pw return from this function and leave the thread 
        }
        else
        {
            brute_impl(str, index + 1, max_depth, end_pw, thread_no);
        }
    }
}

void Brute::start_brute(un_int thread_no){
   std::string pw_tmp = "";
   if(thread_no == 0){
     pw_tmp = std::string("").append<int>(Parameter.pInfos.len_min, 0x21); 	// first PW is minimal pw-length * char 0x21 ('!')
   }else{
     pw_tmp = this->getLastPW(thread_no - 1);				// other PWs are saved in array
   }
   
      this->brute_impl(pw_tmp, 0, this->getLastPW(thread_no).length(), this->getLastPW(thread_no), thread_no);
      th_mutex.lock();
      std::cout << "Thread " << thread_no << ": " << pw_tmp << std::endl;
      th_mutex.unlock();
      //if(POST(pw_tmp).find(Parameter.pInfos.error_msg) == std::string::npos){
      if(pw_tmp == password_test){
	  std::cout << "Passwort gefunden: " << pw_tmp << std::endl;
	//  exit(EXIT_SUCCESS);
      }
  
   th_mutex.lock();
   
   std::cout << "Thread \033[1;31m" << thread_no + 1 << 
   "\033[0m has been finished without result. Its last PW was \"" << 
   this->getLastPW(thread_no) << "\"." << std::endl;
   
   th_mutex.unlock();
}

std::string Brute::getLastPW(un_int pw_no){	
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
    if(pw_no + 1 >= Parameter.pInfos.th_num) 				 			// if its the last thread
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
    pw_no++;									// pw_no is given as index starting from 0. for calculation we need it starting with 1
    while(trys_pp != 0){								// last_pw errechnen
	char newPWCharacter = 0;
	if((calculateLastPW_tmp * pw_no) <= 126){					// if product is above 94 but less than 126 
	  newPWCharacter = static_cast<char>(calculateLastPW_tmp * pw_no + 32);	// there should be no modulo, otherwise the solution will be wrong
	}else{									// otherwise
	  newPWCharacter = static_cast<char>(fmod(calculateLastPW_tmp * pw_no, 94) + 32); // calculate with modulo
	}
	    	    
	last_pw.push_back(newPWCharacter);						// push the Character to last position of string
	calculateLastPW_tmp = calculateLastPW_tmp / 94;				// calculateLastPW_tmp should be exact for calculating next char,
	trys_pp = floor(trys_pp / 94);						// trys_pp is for looping (must be floored)
    }
    std::reverse(std::begin(last_pw), std::end(last_pw));				// last_pw is wrong order -> we need to reverse it
    return last_pw;
}

void Brute::get_astriction(){
  /*switch(Parameter.pInfos.astr){
    case 'c': min_astriction = 65; max_astriction = 90; break;				// char astriction
    case 'n': min_astriction = 48; max_astriction = 57; break;				// number astriction
    case 'b': break;									// both astriction (ohne Sonderzeichen)
  }*/
}
