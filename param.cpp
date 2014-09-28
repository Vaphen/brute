#include "param.hpp"

Param::p_info Param::pInfos;

Param::Param(int &argc, char** argv){
  this->argc = argc;
  this->argv = argv;
}

Param::Param(){}

Param::~Param(){}

//--------------------------------------------------------------------------------------

void Param::setParams(){
  bool valid = true;				// Wenn false, Programm wird nicht ausgeführt
  char param = 0;				// Parameter fuer getopt
  while((param = getopt(argc, argv, ":t:l:a:m:u:e:o:n:v:h")) != -1){
      switch(param){
	case 't': 
	  if(atoi(optarg) >= 1){		// Nur ausfuehren wenn optarg eine Zahl und groessergleich eins ist
	    pInfos.th_num = atoi(optarg);
	  }else{
	    showErrorMsg(param, ERROR_DEFAULT);
	    valid = false;
	  }break;
	case 'm': 
	  if(atoi(optarg) >= 1){
	    pInfos.len_min = atoi(optarg);
	  }else{
	    showErrorMsg(param, ERROR_DEFAULT);
	    valid = false;
	  }break;
	case 'l':
	  if(atoi(optarg) >= 1){
 	    pInfos.len_size = atoi(optarg);
	  }else{
	    showErrorMsg(param, ERROR_DEFAULT);
	    valid = false;
	  } break;
	case 'a':
	  if(*optarg == 'c')      pInfos.astr = 'c';
	  else if(*optarg == 'n') pInfos.astr = 'n';
	  else if(*optarg == 'b') pInfos.astr = 'b';
	  else {
	    showErrorMsg(param, ERROR_DEFAULT);
	    valid = false;
	  } break;
	case 'u': 
	    pInfos.url = optarg; break;
	case 'e':
	    pInfos.error_msg = optarg; break;
	case 'o':
	  if(*optarg == 'g' || *optarg == 'G'){
	    pInfos.method = 1; 
	  }else{
	    showErrorMsg(param, ERROR_DEFAULT);
	    valid = false;
	  } break;
	case 'n':
	  splitKomma(optarg, 0);
	  break;
	case 'v':
	  splitKomma(optarg, 1);
	  break;
	case 'h':
	  showHelp(); exit(EXIT_SUCCESS);
	case '?': 
	  showErrorMsg(param, ERROR_PARAMLESS);
	  valid = false;
	  break;
	default:  
	  showErrorMsg(param, ERROR_UNKNOWN);
	  valid = false;
	  break;
      }
  }

  if(pInfos.error_msg == ""){
    showErrorMsg('e', ERROR_MISSING);
    valid = false;
  }
  if(pInfos.values.size() == 0){
    showErrorMsg('v', ERROR_MISSING);
    valid = false;
  }
  if(pInfos.names.size() == 0){
    showErrorMsg('n', ERROR_MISSING);
    valid = false;
  }
  if((pInfos.names.size() && pInfos.values.size()) > 0 &&
    pInfos.names.size() != pInfos.values.size()){
    showErrorMsg('*', ERROR_DIFF);
    valid = false;
  }
  
  if(!valid){    
    std::cout << "Help: " << this->argv[0] << " -h" << std::endl;
    exit(EXIT_FAILURE);
  }  
}

//--------------------------------------------------------------------------------------

void Param::splitKomma(std::string optarg, bool mycase){

  std::size_t found = 0;
  std::vector<std::string> *vecRef = &pInfos.names;
  
  switch(mycase){
    case 0: 			     break;
    case 1: vecRef = &pInfos.values; break;
  }
  
  while(found != std::string::npos){				// solange bis ende vom string
    found = optarg.find_first_of(",");				// komma finden
    std::string value = optarg.substr(0, found);		// bis komma speichern
    vecRef->push_back(value);					// in Vector speichern
    optarg.erase(0, found+1);					// Zeichenkette bis dahin löschen
  }
}

//--------------------------------------------------------------------------------------

const std::string Param::getUrl() const{
  return pInfos.url;
}

//--------------------------------------------------------------------------------------

void Param::showErrorMsg(char param, errorMsg err) const{
  switch(err){
    case ERROR_DEFAULT:    std::cout << "FEHLER! Operation -" << param <<  
			   " wurde ein falscher Parameter mitgegeben." << std::endl;
			   break;
    case ERROR_MISSING:    std::cout << "FEHLER! Parameter -" << param << " muss"
			   " mitgegeben werden." << std::endl;
			   break;
    case ERROR_PARAMLESS:  std::cerr << "FEHLER! Operation -" << static_cast<char>(param) << 
			   " wurde Parameterlos mitgegeben oder ist nicht bekannt."  << std::endl;
			   break;
    case ERROR_UNKNOWN:    std::cerr << "FEHLER! Es ist ein unbekannter Fehler in den Paramtern aufgetreten." 
			   << std::endl;
			   break;
    case ERROR_DIFF:       std::cerr << "FEHLER! Parameter -n und -v muessen gleich viele Werte besitzen!"
			   << std::endl;
			   break;
  }
}

//--------------------------------------------------------------------------------------

void Param::showHelp() const{								// Hilfeanzeige
  std::cout << "              ##################################             \n";
  std::cout << "              # BruteForce Programm von V4PH3N #             \n";
  std::cout << "              ##################################             \n";
  std::cout << "|-----------------------------------------------------------|\n";
  std::cout << "| Parameter:                                 |   Default    |\n";
  std::cout << "|-----------------------------------------------------------|\n";
  std::cout << "|   -t(hreads)   : Anzahl der Threads        |   1          |\n";
  std::cout << "|   -m(inimum)   : Minimale Passwortlaenge   |   4          |\n";
  std::cout << "|   -l(imit)     : Maximale Passwortlaenge   |   6          |\n";
  std::cout << "|   -u(rl)       : Ziel-URL                  |   google.de  |\n";
  std::cout << "|   -a(striction): Begrenzung                |   ---        |\n";
  std::cout << "|      c(hars)        nur Buchstaben         |              |\n";
  std::cout << "|      n(umbers)      nur Zahlen             |              |\n";
  std::cout << "|      b(oth)         Zahlen und Buchstaben  |              |\n";
  std::cout << "|   -e(rror)     : Fehlerausgabe bei falschem|   ---        |\n";
  std::cout << "|                  Nutzernamen oder Passwort |              |\n";
  std::cout << "|   -(meth)o(d)  : POST oder GET             |   POST       |\n";
  std::cout << "|   -n(ames)     : Namen der Variablen       |   ---        |\n";
  std::cout << "|                  (Durch Komma getrennt)    |              |\n";
  std::cout << "|   -v(alues)    : zu sendende Werte durch   |   ---        |\n";
  std::cout << "|                  Komma getrennt            |              |\n";
  std::cout << "|                  Brutewert ist '?'         |              |\n";
  std::cout << "|   -h(elp)      : Diese Anzeige             |              |\n";
  std::cout << "|-----------------------------------------------------------|\n";
  std::cout << "\nBeispiel:                                                  \n";
  std::cout << "   " << argv[0] << " -t 2 -m 3 -l 5 -u vaphen.funpic.de -a c -e \"incorrect Password\"";
  std::cout << " -o GET -n \"uname,pw,senden\" -v \"Vaphen,?,submit\"" << std::endl;
}