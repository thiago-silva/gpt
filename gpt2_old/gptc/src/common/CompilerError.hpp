
/*
  Cada gramatica deve ter sua ErrorList
*/

class ErrorList {

};

class CompilerError {
  void addError(unit, line, errorCode, message);
  //tips
  //verbose, etc
  std::string toString();
};
