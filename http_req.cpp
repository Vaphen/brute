#include "http_req.hpp"

Http_req::Http_req() : url("google.de") {}

Http_req::~Http_req(){}

std::string Http_req::buffer;

int Http_req::writer(char *data, size_t size, size_t nmemb, std::string *buffer){
  int result = 0;
  if(buffer != NULL) {
    buffer->append(data, size * nmemb);
    result = size * nmemb;
  }
  return result;
}

std::string Http_req::POST(char* brute_pw) {
    struct curl_httppost *formpost=NULL;
    struct curl_httppost *lastptr=NULL;
    struct curl_slist *headerlist=NULL;
    
    for(un_int i = 0; i < Parameter.pInfos.names.size(); i++){
      char* copycontents = const_cast<char*>(Parameter.pInfos.values.at(i).c_str());
      if(*copycontents == '?') copycontents = brute_pw;
      
      curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, Parameter.pInfos.names.at(i).c_str(),
		CURLFORM_COPYCONTENTS, Parameter.pInfos.values.at(i).c_str(),
		CURLFORM_END);
    }
    
   curl = curl_easy_init();

   headerlist = curl_slist_append(headerlist, buffer.c_str());
   
   if(curl) {
     curl_easy_setopt(curl, CURLOPT_URL, Parameter.pInfos.url.c_str());
     curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
     curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);  
     curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
     curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
     
     res = curl_easy_perform(curl);

     if(res != CURLE_OK)
       std::cout << "curl_easy_perform() faileds\n" <<  curl_easy_strerror(res) << std::endl;
  
     curl_easy_cleanup(curl);
   }
   
   curl_formfree(formpost);
   curl_slist_free_all (headerlist);
   return buffer;
}

std::string Http_req::GET(char* brute_pw) {
   curl = curl_easy_init();  
   
   std::string url_sendstr = Parameter.pInfos.url;
   for(un_int i = 0; i < Parameter.pInfos.names.size(); i++){
     i == 0 ? url_sendstr += '?' : url_sendstr += '&';
     url_sendstr.append(Parameter.pInfos.names.at(i));
     url_sendstr += '=';
     url_sendstr.append(Parameter.pInfos.values.at(i));
   }
   if(curl) {
     curl_easy_setopt(curl, CURLOPT_URL, url_sendstr.c_str());
     curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
     curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer); 
        
     res = curl_easy_perform(curl);
 
     if(res != CURLE_OK)
       std::cout << "curl_easy_perform() faileds\n" <<  curl_easy_strerror(res) << std::endl;
     
     curl_easy_cleanup(curl);
   }
   return buffer;
}