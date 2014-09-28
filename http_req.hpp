#ifndef __http_req_h_included__
#define __http_req_h_included__

#include <iostream>
#include <string>
#include <curl/curl.h>
#include "param.hpp"

class Http_req{
public:
  Http_req();
  ~Http_req();
  std::string POST(char*);
  std::string GET(char*);
private:
  Param Parameter;
  std::string url;
  CURL *curl;
  CURLcode res;
protected:  
  static int writer(char *data, size_t size, size_t nmemb, std::string *buffer);
  static std::string buffer;
};

#endif