#include "soapH.h"
#include <linux/kernel.h>
//#include <linux/sysinfo.h>
#include <stdio.h>
#include <sys/sysinfo.h>


//! Return message of the ping funcion 
//char *msgOk;

//! Daemon name.
extern char *processName;
#define DEBUG printf("> Linha %d\n",__LINE__);

xsd__int ns__ping(struct soap *soap, xsd__string identProc, xsd__string *result)
{
   char msgLog[1024];
   char *msgOk;
	const long minute = 60;
	const long hour = minute * 60;
	const long day = hour * 24;
	const double megabyte = 1024 * 1024;
	/* Obtain system statistics.  */
	struct sysinfo si;
	sysinfo (&si);
   sprintf(msgLog,"ping:%s",(char *)identProc);
   log_message((char *)processName,(char *)msgLog);

   msgOk = (char *)soap_malloc(soap,1024);
   sprintf(msgOk,"Ok:%s uptime: %ld days, %ld:%02ld:%02ld total RAM : %5.1f MB free RAM : %5.1f MB\0",
									(char *)identProc, 
									si.uptime / day, 
									(si.uptime % day) / hour,
									(si.uptime % hour) / minute, 
									si.uptime % minute,
									si.totalram / megabyte,
									si.freeram / megabyte);

   log_message((char *)processName, msgOk);
   *result = msgOk; 
   return SOAP_OK;
}



