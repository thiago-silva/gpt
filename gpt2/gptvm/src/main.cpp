#include <iostream>
#include <string>

using namespace std;

#include "COptions.hpp"
#include "CGptVm.hpp"


void showSyntax();


int main (int argc, char *argv[])
{
   int i;
   COptions options;

   cout << "GptVM, release 0.1.0 (development release)" << endl;

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

   options.sourcefile = string(argv[0]) + ".gvm";

//   cout << "Running " << options.sourcefile << "..." << endl;

   CGptVm gptvm( &options );

   return gptvm.run();
}


void showSyntax() {
   cout << endl 
        << "Desenvolvido por XXX, YYY e ZZZ" << endl 
        << "Novembro, 2007" << endl 
        << endl 
        << "Syntax: gptvm [options] file" << endl 
        << "Options:" << endl 
        << "         -h help" << endl 
        << endl;
}

