//gsoap ns service name:        opencertiface service
//gsoap ns service style:       rpc
//gsoap ns service encoding:    encoded
//gsoap ns service namespace:   http://opencertiface.com.br/bioCubeVF103.wsdl
//gsoap ns service location:    http://opencertiface.com.br/bioCubeVF103.cgi
//
//gsoap ns schema namespace:    urn:opencertiface 
//

//! Int type for gsoap library.
typedef int xsd__int;

//! Int type for gsoap library.
typedef int xsd__float;

//! String type for gsoap library.
typedef char *xsd__string;

//! Base64 struct for gsoap library.
struct xsd__base64Binary
{
   unsigned char *__ptr;
   int __size;
};

xsd__int ns__ping(xsd__string stringHello, xsd__string *result);

xsd__int ns__verifyImg(struct xsd__base64Binary *template1, struct xsd__base64Binary *template2, xsd__string id, xsd__string *result);


