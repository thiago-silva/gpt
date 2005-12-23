#ifndef ERRORHANDLER_HPP
#define ERRORHANDLER_HPP

#include <map>
#include <list>
#include <string>
#include <sstream>

using namespace std;

class ErrorHandler {
public:
  ~ErrorHandler();

  static ErrorHandler* self();

  int add(const string& msg, int line);  
//   int add(const string& msg);  
  int add(const stringstream& msg, int line);
//   int add(const stringstream& msg);

  void addTip(const string& msg, int line, int cd);

  void addFatal(const string&);
  void addFatal(const stringstream&);

  void showErrors();
  bool hasError();
private:
  ErrorHandler();

  static ErrorHandler* _self;
  bool _hasError;

  typedef map<int, list<string> > errors_map_t;
  errors_map_t _errors;
};

#endif
