#include <iostream>
#include <string>

using namespace std;


#include "COptions.hpp"
#include "CGptAsm.hpp"


void showSyntax();


int main (int argc, char *argv[])
{
   int i;
   COptions options;

   cout << "GptAsm - GEPETO - Release 0.1.0 (development release)" << endl;

   while ((i = getopt(argc, argv, "h")) != -1) {
      switch (i) {
         case 'h':
            showSyntax();
            return EXIT_SUCCESS;
            break;
         default:
            break;
      }
   }

   argc -= optind;
   argv += optind;

   if (argc < 1) {
      showSyntax();
      return EXIT_FAILURE;
   }

   options.sourcefile = string(argv[0]) + ".gasm";
   options.destfile   = string(argv[0]) + ".gvm";

   cout << "Compiling " << options.sourcefile << "..." << endl;

   CGptAsm gptc( &options );

   if (gptc.run()) {
      cout << "Program " << options.sourcefile << " ok !!!" << endl << endl;
   } else {
      cout << "Program " << options.sourcefile << " has errors..." << endl << endl;
      exit( EXIT_FAILURE );
   }

   return EXIT_SUCCESS;
}


void showSyntax() {
   cout << endl 
        << "Desenvolvido por XXX, YYY e ZZZ" << endl 
        << "Novembro, 2007" << endl 
        << endl 
        << "Syntax: gptc [options] file" << endl 
        << "Options:" << endl 
        << "         -h help" << endl 
        << endl;
}

