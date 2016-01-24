/*

$URL: http://10.0.0.249/repos/certifaceWebC/trunk/src/webServerExtractor.c $
$Rev: 9 $
$Author: cabelo $
$Date: 2012-03-15 13:02:06 -0300 (Qui, 15 Mar 2012) $
$Id: webServerExtractor.c 9 2012-03-15 16:02:06Z cabelo $

*/

#include "webServerVerifyFace.h"
//#include "functionSRV.c"
//void *process_request(void*);
#define BACKLOG (400) // Max. request backlog
#define MAX_THR (40) // Size of thread pool
#define MAX_QUEUE (4000) // Max. size of request queue
SOAP_SOCKET queue[MAX_QUEUE]; // The global request queue of sockets


int head = 0, tail = 0; 
void *process_queue(void*);
int enqueue(SOAP_SOCKET);
SOAP_SOCKET dequeue();
pthread_mutex_t queue_cs;
pthread_cond_t queue_cv;


int main(int argc, char **argv)
{
   bzero(argv[0],strlen(argv[0]));
   strcpy(argv[0], pidNameGuardian);
   daemonizeGuardian(processNameGuardian );


   char msgLog[255];
   int i=0;
   SOAP_SOCKET m, s; /* master and slave sockets */
   struct soap soap;

   soap_init(&soap);
   soap.bind_flags=SO_REUSEADDR;
   soap.send_timeout = 10; // 60 seconds
   soap.recv_timeout = 10; // 60 seconds
   soap.max_keep_alive = 60; // max keep-alive sequence 

//   soap_init2(&soap, SOAP_IO_KEEPALIVE, SOAP_IO_KEEPALIVE) ;
//   soap.bind_flags=SO_REUSEADDR;
//   soap.send_timeout = 10; // 5 seconds
//   soap.recv_timeout = 10; // 5 seconds
//   soap.accept_timeout = 10; // server stops after 10 seconds of inactivity
//   soap.max_keep_alive = 100; // max keep-alive sequence

   bzero(argv[0],strlen(argv[0]));
   strcpy(argv[0], pidName);
   savePIDrun(processName);
 
   if (argc < 2)
      soap_serve(&soap);  /* serve as CGI application */
   else
   {
      struct soap *soap_thr[MAX_THR];
      pthread_t tid[MAX_THR];
      int port = atoi(argv[1]);
     
      m = soap_bind(&soap, NULL, port, 100);
      if (!soap_valid_socket(m))
      {
         soap_print_fault(&soap, stderr);
         exit(-1);
      }

      sprintf(msgLog,"Socket connection successful: master socket = %d\0", m);
      log_message((char *)processName,(char *)msgLog);
      pthread_mutex_init(&queue_cs, NULL);
      pthread_cond_init(&queue_cv, NULL);

      for (i = 0; i < MAX_THR; i++)
      { 
         soap_thr[i] = soap_copy(&soap);
         //fprintf(stderr, "Socket connection successful: slave socket = %d\n", s);
         sprintf(msgLog,"Starting thread %d\0", i);
         log_message((char *)processName,(char *)msgLog);
         //fprintf(stderr, "Thread %d accepts socket %d connection from IP %d.%d.%d.%d\n", i, s, (soap.ip>>24)&0xFF, (soap.ip>>16)&0xFF, (soap.ip>>8)&0xFF, soap.ip&0xFF);
         pthread_create(&tid[i], NULL, (void*(*)(void*))process_queue, (void*)soap_thr[i]);
      }

      for (;;)
      {
         s = soap_accept(&soap);
         if (!soap_valid_socket(s))
         {
            if (soap.errnum)
            {
               soap_print_fault(&soap, stderr);
               continue; // retry
            }
            else
            {
               fprintf(stderr, "Server timed out\n");
               break;
            }
         }
         fprintf(stderr, "Thread %d accepts socket %d connection from IP %d.%d.%d.%d\n", i, s, (soap.ip>>24)&0xFF, (soap.ip>>16)&0xFF, (soap.ip>>8)&0xFF, soap.ip&0xFF);
         while (enqueue(s) == SOAP_EOM)
            sleep(1);
      }
      
      for (i = 0; i < MAX_THR; i++)
      {
         while (enqueue(SOAP_INVALID_SOCKET) == SOAP_EOM)
            sleep(1);
      }

      for (i = 0; i < MAX_THR; i++)
      {
         fprintf(stderr, "Waiting for thread %d to terminate... ", i);
         pthread_join(tid[i], NULL);
         fprintf(stderr, "terminated\n");
         soap_done(soap_thr[i]);
         free(soap_thr[i]);
      }

      pthread_mutex_destroy(&queue_cs);
      pthread_cond_destroy(&queue_cv);

   }
   soap_done(&soap); 
   return 0;
} 


void *process_queue(void *soap)
{
   struct soap *tsoap = (struct soap*)soap;
   for (;;)
   {
      tsoap->socket = dequeue();
      if (!soap_valid_socket(tsoap->socket))
         break;
     soap_serve(tsoap);
     soap_destroy(tsoap);
     soap_end(tsoap);
     fprintf(stderr, "served\n");
   }
   return NULL;
}

int enqueue(SOAP_SOCKET sock)
{
   int status = SOAP_OK;
   int next;
   pthread_mutex_lock(&queue_cs);
   next = tail + 1;
   if (next >= MAX_QUEUE)
      next = 0;
   if (next == head)
      status = SOAP_EOM;
   else
   {
      queue[tail] = sock;
      tail = next;
   }
   pthread_cond_signal(&queue_cv);
   pthread_mutex_unlock(&queue_cs);
   return status;
}

SOAP_SOCKET dequeue()
{
   SOAP_SOCKET sock;
   pthread_mutex_lock(&queue_cs);
   while (head == tail)
      pthread_cond_wait(&queue_cv, &queue_cs);
   sock = queue[head++];
   if (head >= MAX_QUEUE)
      head = 0;
   pthread_mutex_unlock(&queue_cs);
   return sock;
}


