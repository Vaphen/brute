#ifndef _PARAM_H_INCLUDED_
#define _PARAM_H_INCLUDED_

#include <iostream>
#include <vector>
#include <unistd.h>	// getopt()

typedef unsigned int un_int;

class Param{
private:
  enum errorMsg { ERROR_DEFAULT, ERROR_PARAMLESS, ERROR_UNKNOWN, ERROR_MISSING, ERROR_DIFF };
  int argc;
  char **argv;
  void showHelp() const;
  void showErrorMsg(char, enum errorMsg) const;
  void splitKomma(std::string, bool);
  typedef struct process_infos{
    un_int		     th_num;
    std::string 	     url;
    std::string 	     error_msg;
    un_int		     len_min;
    un_int		     len_size;
    char		     astr;
    bool 		     method;
    std::vector<std::string> names;
    std::vector<std::string> values;
    process_infos() : th_num(1), url("google.de"), error_msg(""), len_min(4), len_size(6), astr('a'), method(0), names(0), values(0){} 
  } p_info;
public:
    Param(int &argc, char **argv);
    Param();
    ~Param();
    void setParams();
    const std::string getUrl() const;
    static p_info pInfos;
};

#endif