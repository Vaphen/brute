#ifndef __M_THREADS_H_INCLUDED_
#define __M_THREADS_H_INCLUDED_

#include "param.hpp"

class m_threads{
public:
  m_threads();
  ~m_threads();
  void start_threads();
private:
  Param Parameter;
protected:
  
};

#endif