#include "soapH.h"
#include "opencertiface.nsmap"


int main(int argc, char **argv)
{
  struct soap soap;
  char *stringHello, *result, *server;
  int loopC,i,j;

  if(argc<2)
  {
     printf("Usage: clientTEST http://server:port [string Test] Num.loop\n");
     exit(0);
  }


  server       = strdup(argv[1]);
  stringHello  = strdup(argv[2]);
  loopC        = atoi(argv[3]);
 
  soap_init1(&soap, SOAP_XML_INDENT);
 
  for(i=0 ; i<loopC;++i)
  {
     soap_call_ns__ping(&soap, argv[1], "", stringHello, &result);
      if (soap.error)
      {
         soap_print_fault(&soap, stderr);
         exit(1);
      }
      else
      printf("result = %s\n", result);

      printf("N.loop=%d\n",i);
      
   } 

   soap_destroy(&soap);
   soap_end(&soap);
   soap_done(&soap);
   return 0;
}   

