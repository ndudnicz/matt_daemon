/* STATIC VARIABLES ==========================================================*/


/* CONSTRUCTORS ==============================================================*/
Tintin_reporter::Tintin_reporter( void ) {
}
Tintin_reporter::Tintin_reporter( Tintin_reporter const & src ) {
}

/* MEMBER OPERATORS OVERLOAD =================================================*/
Tintin_reporter    &Tintin_reporter::operator=( Tintin_reporter const & rhs ) {
}


/* DESTRUCTOR ================================================================*/
Tintin_reporter::~Tintin_reporter( void ) {
}

/* MEMBER FUNCTIONS ==========================================================*/
void  Tintin_reporter::info(std::string str){

  write("INFO", str);
}

void  Tintin_reporter::warning(std::string str){

  write("WARNING", str);
}

void  Tintin_reporter::error(std::string str){
  write("ERROR", str);
}

void  Tintin_reporter::write(std::string errortype, std::string str){

}

Tintin_reporter   &Tintin_reporter::setFilename(std::string str){

  this->fileName  str;
  return this;
}
/* NON MEMBER FUNCTIONS ======================================================*/


/* OPERATOR ==================================================================*/
