#include "soapH.h"
//#include "functionSRV.h"
#include <exception>
#include <list>
#include <cstdlib>
#include <vector>
#include <openbr/openbr_plugin.h>
#include <cv.h>
#include <highgui.h>
#include <syslog.h>

using namespace std;
   char **argv = NULL;
   int argc = 0;

static void printTemplate(const br::Template &t)
{  char *msgLog;
   openlog("opencertiface", LOG_PID|LOG_CONS, LOG_USER);
   const QPoint firstEye = t.file.get<QPoint>("Affine_0");
   const QPoint secondEye = t.file.get<QPoint>("Affine_1");
   asprintf(&msgLog,"Eyes: (%d, %d) (%d, %d)", firstEye.x(), firstEye.y(), secondEye.x(), secondEye.y());
   syslog(LOG_INFO,msgLog);
   closelog();
}
xsd__int ns__verifyImg(struct soap *soap, xsd__base64Binary *template1, struct xsd__base64Binary *template2, xsd__string id, xsd__string *result)
{
   char *msgOk;
    br::Context::initialize(argc,argv);
   QSharedPointer<br::Transform> transform = br::Transform::fromAlgorithm("FaceRecognition");
   QSharedPointer<br::Distance> distance = br::Distance::fromAlgorithm("FaceRecognition");

 //msgOk = strdup("0.9999");   
   std::vector<char> data1(template1->__ptr, template1->__ptr+template1->__size);
   std::vector<char> data2(template2->__ptr, template2->__ptr+template2->__size);

   cv::Mat img1 = cv::imdecode(data1, CV_LOAD_IMAGE_COLOR);
   cv::Mat img2 = cv::imdecode(data2, CV_LOAD_IMAGE_COLOR);

   //cv::imwrite("/tmp/teste.jpg", img);

   br::Template queryA(img1);
   br::Template target(img2);
   queryA >> *transform;
   target >> *transform;
   printTemplate(queryA);
   printTemplate(target);
   float comparisonA = distance->compare(target, queryA);
   asprintf(&msgOk,"%.5f", comparisonA);
//   br::Context::finalize();

   *result = msgOk; 
   return SOAP_OK;

}




