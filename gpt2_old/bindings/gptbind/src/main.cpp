#include <iostream>
#include <string>

using namespace std;


#include "COptions.hpp"
#include "CGptBind.hpp"


void showSyntax();


int main (int argc, char *argv[])
{
   int i;
   COptions options;

   cout << "GptBind - Release 0.1.0 (GEPETO)" << endl;

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

   options.sourcefile = argv[0];
   options.destfile   = argv[0];

   cout << "Binding " << options.sourcefile << "..." << endl;

   CGptBind gptbind( &options );

   if (gptbind.run()) {
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
        << "Dezembro, 2007" << endl 
        << endl 
        << "Syntax: gptbind [options] file" << endl 
        << "Options:" << endl 
        << "         -h help" << endl 
        << endl;
}

