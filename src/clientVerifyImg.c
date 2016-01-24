/* 

$URL: http://10.0.0.249/repos/certifaceWebC/trunk/src/clientTEST.c $
$Rev: 12 $
$Author: cabelo $
$Date: 2012-08-21 19:12:58 -0300 (Ter, 21 Ago 2012) $
$Id: clientTEST.c 12 2012-08-21 22:12:58Z cabelo $

*/

#include "soapH.h"
#include "opencertiface.nsmap"
#define DEBUG printf("> Linha %d\n",__LINE__)

//const char server[] = "http://localhost:8080";
int save(unsigned char *image, const char *fileName,int len)
{
   FILE *pFileZ;
   pFileZ = fopen(fileName, "wb");
   if (pFileZ != NULL)
   {
      fwrite(image,len, 1, pFileZ);
      fclose(pFileZ);
      return 1;
   }
   else
   return 0;
}

int main(int argc, char **argv)
{
	char fileName[256];
	char server[256];
	FILE *pFILE;
	struct soap soap;
	int loopC;
	int i;
	char *result;
	struct xsd__base64Binary template1;
	struct xsd__base64Binary template2;

 	if(argc<5)
	{
 		printf("Usage: clientTEST http://server:port template1 template2 Num.loop\n");
		exit(0);
	}
	printf("numero de args %d\n",argc);
   loopC        = atoi(argv[4]);

		soap_init1(&soap, SOAP_XML_INDENT);

      FILE *pFile;
		pFile = fopen ( argv[2] , "r" );
		if (pFile!=NULL)
		{
			printf("Carregando:  %s\n",argv[2]);
			fseek (pFile , 0 , SEEK_END);
			template1.__size = ftell(pFile);
			rewind (pFile);
			template1.__ptr = (unsigned char *)malloc(template1.__size);
			fread(template1.__ptr,1,template1.__size,pFile);
			fclose (pFile);
			save(template1.__ptr,"/tmp/fir1",template1.__size);
		}


		pFile = fopen ( argv[3] , "r" );
		if (pFile!=NULL)
		{
			printf("Carregando:  %s\n",argv[3]);
			fseek (pFile , 0 , SEEK_END);
			template2.__size = ftell(pFile);
			rewind (pFile);
			template2.__ptr = (unsigned char *)malloc(template2.__size);
			fread(template2.__ptr,1,template2.__size,pFile);
			fclose (pFile);
			save(template2.__ptr,"/tmp/fir2",template2.__size);
		}

	
  for(i=0 ; i<loopC;++i)
  {
      soap_call_ns__verifyImg(&soap, argv[1], "", &template1, &template2, "0", &result);
      if (soap.error)
      {
         soap_print_fault(&soap, stderr);
         exit(1);
      }
      else
      {
         printf("Resultado: %s\n", result);

      }
      printf("N.loop=%d\n",i);
      
   } 

   soap_destroy(&soap);
   soap_end(&soap);
   soap_done(&soap);
   return 0;
}   

