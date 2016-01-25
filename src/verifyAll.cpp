#include "soapH.h"
#include <exception>
#include <list>
#include <cstdlib>
#include <vector>
#include <openbr/openbr_plugin.h>
#include <cv.h>
#include <highgui.h>
#include <syslog.h>

using namespace std;
class _bio
{  
   char *msgOk;
   public:
   _bio(void)
   {
      char **_value = NULL;
      int _count = 0;
      br::Context::initialize(_count,_value);
      msgOk = (char *)malloc(10);
   }
   ~_bio(void)
   {
      free(msgOk);
      br::Context::finalize();
   }

   void _printTemplate(const br::Template &t)
   {  
      char *msgLog;
      openlog("opencertiface", LOG_PID|LOG_CONS, LOG_USER);
      const QPoint firstEye = t.file.get<QPoint>("Affine_0");
      const QPoint secondEye = t.file.get<QPoint>("Affine_1");
      asprintf(&msgLog,"Eyes: (%d, %d) (%d, %d)", firstEye.x(), firstEye.y(), secondEye.x(), secondEye.y());
      syslog(LOG_INFO,msgLog);
      free(msgLog);
      closelog();
   }


   char *verify(cv::Mat img1, cv::Mat img2)
   {
      QSharedPointer<br::Transform> transform = br::Transform::fromAlgorithm("FaceRecognition");
      QSharedPointer<br::Distance> distance = br::Distance::fromAlgorithm("FaceRecognition");

      br::Template queryA(img1);
      br::Template target(img2);
      queryA >> *transform;
      target >> *transform;
      _printTemplate(queryA);
      _printTemplate(target);

      float comparisonA = distance->compare(target, queryA);
      sprintf(msgOk,"%.5f", comparisonA);
      return msgOk;


   }
};

_bio opencertiface;

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

   std::vector<char> data1(template1->__ptr, template1->__ptr+template1->__size);
   std::vector<char> data2(template2->__ptr, template2->__ptr+template2->__size);

   cv::Mat img1 = cv::imdecode(data1, CV_LOAD_IMAGE_COLOR);
   cv::Mat img2 = cv::imdecode(data2, CV_LOAD_IMAGE_COLOR);

   *result = strdup(opencertiface.verify(img1,img2));
   return SOAP_OK;

}

