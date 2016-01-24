#include "functionSRV.h"


void log_message(char *filename ,char *message)
{
   FILE   *logfile;
//   struct timeval tv;
   struct tm *date_time;
   time_t dtimer;

   char filelog[255];

   char timeNow0[20];
   char timeNow1[20];
//   char timeNow2[20];

//   long ms;

   time(&dtimer);
   //gettimeofday(&tv,NULL);
   date_time=localtime(&dtimer);
   //date_time=localtime(&tv.tv_sec);
   //ms = tv.tv_sec/1000;

   strftime(timeNow0,20,"-%Y%m%d.log",date_time);

   strftime(timeNow1,20,"%H:%M:%S\0" ,date_time);
   //sprintf(timeNow2,"%s-.%031d",timeNow1,ms);

   sprintf(filelog,"/var/log/%s%s",filename,timeNow0);

   logfile=fopen(filelog,"a");
   if(logfile)
   {
      fprintf(logfile,"%s %s\n",timeNow1,message);
      fclose(logfile);
   }
   return;

}

void savePIDrun(char *serviceName)
{
   char stringFULLNAME[255];
   sprintf(stringFULLNAME,"/var/run/%s.pid",serviceName);
   FILE *logfile;
   logfile=fopen(stringFULLNAME,"wb");
   if(!logfile) return;
   fprintf(logfile,"%d\n",getpid());
   //ntPIDGuardian = getpid();
   fclose(logfile);
}

void signal_handler(int sig)
{

   switch(sig)
   {
      case SIGHUP:
      // "hangup signal catched";
      break;
      case SIGTERM:
      // "terminate signal catched";
      exit(0);
      break;
   }
}


void daemonize()
{
   int i;
   i=fork();
   if (i<0) exit(1); /* fork error */
   if (i>0) exit(0); /* parent exits */
   signal(SIGCHLD,SIG_IGN);             /* ignore child */
   signal(SIGTSTP,SIG_IGN);             /* ignore tty signals */
   signal(SIGTTOU,SIG_IGN);
   signal(SIGTTIN,SIG_IGN);
   signal(SIGHUP,signal_handler);       /* catch hangup signal */
   signal(SIGTERM,signal_handler);      /* catch kill signal */
}


void daemonizeGuardian(char *serviceName)
{
   int runGuardian;
   int i;
//   int status1;
   int createPIDFile = 1;
//   pid_t ret1;
   pid_t pidP;
   i=fork();
   if (i<0) exit(1); /* fork error */
   if (i>0) exit(0); /* parent exits */
   runGuardian = 1;
   while(runGuardian)
   {
      pidP=fork();
      if(pidP==0)
      {
         signal(SIGCHLD,SIG_IGN);             /* ignore child */
         signal(SIGTSTP,SIG_IGN);             /* ignore tty signals */
         signal(SIGTTOU,SIG_IGN);
         signal(SIGTTIN,SIG_IGN);
         signal(SIGHUP,signal_handler);       /* catch hangup signal */
         signal(SIGTERM,signal_handler);      /* catch kill signal */
         runGuardian = 0;
      }
      if(pidP>0)
      { 
         if(createPIDFile)
         {
            createPIDFile = 0;
            savePIDrun(serviceName);
         }
         waitpid(pidP,NULL,0);
         sleep(2);
      }
   }
}

