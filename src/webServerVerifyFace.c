/*

$URL: http://10.0.0.249/repos/certifaceWebC/trunk/src/webServerExtractor.c $
$Rev: 12 $
$Author: cabelo $
$Date: 2012-08-21 19:12:58 -0300 (Ter, 21 Ago 2012) $
$Id: webServerExtractor.c 12 2012-08-21 22:12:58Z cabelo $

*/

#include "webServerVerifyFace.h"

int main(int argc, char **argv)
{

   char msgLog[255];

   SOAP_SOCKET m, s; /* master and slave sockets */
   struct soap soap;
   soap_init(&soap);
   soap.bind_flags=SO_REUSEADDR;
   soap.send_timeout = 10; // 60 seconds
   soap.recv_timeout = 10; // 60 seconds
   soap.max_keep_alive = 60; // max keep-alive sequence 

//   bzero(argv[0],strlen(argv[0]));
//   strcpy(argv[0], pidNameGuardian);
//   daemonizeGuardian(processNameGuardian );

   bzero(argv[0],strlen(argv[0]));
   strcpy(argv[0], pidName);
   savePIDrun(processName);

 
   if (argc < 2)
      soap_serve(&soap);  /* serve as CGI application */
   else
   {
      //m = soap_bind(&soap, "localhost", atoi(argv[1]), 100);
      m = soap_bind(&soap, NULL, atoi(argv[1]), 100);
      if (!soap_valid_socket(m))
      {
         soap_print_fault(&soap, stderr);
         exit(-1);
      }

      sprintf(msgLog,"Socket connection successful: master socket = %d\0", m);
      log_message((char *)processName,(char *)msgLog);

      //fprintf(stderr, "Socket connection successful: master socket = %d\n", m);
      for ( ; ; )
      { 
         s = soap_accept(&soap);

         //fprintf(stderr, "Socket connection successful: slave socket = %d\n", s);
         sprintf(msgLog,"Socket connection successful: slave socket = %d\0", s);
         log_message((char *)processName,(char *)msgLog);

         if (!soap_valid_socket(s))
         {
            soap_print_fault(&soap, stderr);
            exit(-1);
         }
         soap_serve(&soap);
         soap_end(&soap);
      }
   }
   return 0;
} 

