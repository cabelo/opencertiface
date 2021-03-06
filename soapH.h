/* soapH.h
   Generated by gSOAP 2.8.29 for functions.h

gSOAP XML Web services tools
Copyright (C) 2000-2016, Robert van Engelen, Genivia Inc. All Rights Reserved.
The soapcpp2 tool and its generated software are released under the GPL.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#ifndef soapH_H
#define soapH_H
#include "soapStub.h"
#ifdef __cplusplus
extern "C" {
#endif
#ifndef WITH_NOIDREF
SOAP_FMAC3 void SOAP_FMAC4 soap_markelement(struct soap*, const void*, int);
SOAP_FMAC3 int SOAP_FMAC4 soap_putindependent(struct soap*);
SOAP_FMAC3 int SOAP_FMAC4 soap_getindependent(struct soap*);
#endif
SOAP_FMAC3 void *SOAP_FMAC4 soap_getelement(struct soap*, int*);
SOAP_FMAC3 int SOAP_FMAC4 soap_putelement(struct soap*, const void*, const char*, int, int);
SOAP_FMAC3 int SOAP_FMAC4 soap_ignore_element(struct soap*);

SOAP_FMAC3 const char ** SOAP_FMAC4 soap_faultcode(struct soap *soap);

#ifndef SOAP_TYPE_byte_DEFINED
#define SOAP_TYPE_byte_DEFINED
SOAP_FMAC3 void SOAP_FMAC4 soap_default_byte(struct soap*, char *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_byte(struct soap*, const char*, int, const char *, const char*);
SOAP_FMAC3 char * SOAP_FMAC4 soap_in_byte(struct soap*, const char*, char *, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_byte(struct soap*, const char *, const char*, const char*);

#ifndef soap_write_byte
#define soap_write_byte(soap, data) ( soap_free_temp(soap), soap_begin_send(soap) || soap_put_byte(soap, data, "byte", NULL) || soap_end_send(soap), (soap)->error )
#endif

SOAP_FMAC3 char * SOAP_FMAC4 soap_get_byte(struct soap*, char *, const char*, const char*);

#ifndef soap_read_byte
#define soap_read_byte(soap, data) ( soap_begin_recv(soap) || !soap_get_byte(soap, (data), NULL, NULL) || soap_end_recv(soap), (soap)->error )
#endif

#endif

#ifndef SOAP_TYPE_xsd__float_DEFINED
#define SOAP_TYPE_xsd__float_DEFINED
SOAP_FMAC3 void SOAP_FMAC4 soap_default_xsd__float(struct soap*, int *);

#define soap_xsd__float2s soap_int2s

SOAP_FMAC3 int SOAP_FMAC4 soap_out_xsd__float(struct soap*, const char*, int, const int *, const char*);

#define soap_s2xsd__float soap_s2int

SOAP_FMAC3 int * SOAP_FMAC4 soap_in_xsd__float(struct soap*, const char*, int *, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_xsd__float(struct soap*, const int *, const char*, const char*);

#ifndef soap_write_xsd__float
#define soap_write_xsd__float(soap, data) ( soap_free_temp(soap), soap_begin_send(soap) || soap_put_xsd__float(soap, data, "xsd:float", NULL) || soap_end_send(soap), (soap)->error )
#endif

SOAP_FMAC3 int * SOAP_FMAC4 soap_get_xsd__float(struct soap*, int *, const char*, const char*);

#ifndef soap_read_xsd__float
#define soap_read_xsd__float(soap, data) ( soap_begin_recv(soap) || !soap_get_xsd__float(soap, (data), NULL, NULL) || soap_end_recv(soap), (soap)->error )
#endif

#endif

#ifndef SOAP_TYPE_xsd__int_DEFINED
#define SOAP_TYPE_xsd__int_DEFINED
SOAP_FMAC3 void SOAP_FMAC4 soap_default_xsd__int(struct soap*, int *);

#define soap_xsd__int2s soap_int2s

SOAP_FMAC3 int SOAP_FMAC4 soap_out_xsd__int(struct soap*, const char*, int, const int *, const char*);

#define soap_s2xsd__int soap_s2int

SOAP_FMAC3 int * SOAP_FMAC4 soap_in_xsd__int(struct soap*, const char*, int *, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_xsd__int(struct soap*, const int *, const char*, const char*);

#ifndef soap_write_xsd__int
#define soap_write_xsd__int(soap, data) ( soap_free_temp(soap), soap_begin_send(soap) || soap_put_xsd__int(soap, data, "xsd:int", NULL) || soap_end_send(soap), (soap)->error )
#endif

SOAP_FMAC3 int * SOAP_FMAC4 soap_get_xsd__int(struct soap*, int *, const char*, const char*);

#ifndef soap_read_xsd__int
#define soap_read_xsd__int(soap, data) ( soap_begin_recv(soap) || !soap_get_xsd__int(soap, (data), NULL, NULL) || soap_end_recv(soap), (soap)->error )
#endif

#endif

#ifndef SOAP_TYPE_int_DEFINED
#define SOAP_TYPE_int_DEFINED
SOAP_FMAC3 void SOAP_FMAC4 soap_default_int(struct soap*, int *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_int(struct soap*, const char*, int, const int *, const char*);
SOAP_FMAC3 int * SOAP_FMAC4 soap_in_int(struct soap*, const char*, int *, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_int(struct soap*, const int *, const char*, const char*);

#ifndef soap_write_int
#define soap_write_int(soap, data) ( soap_free_temp(soap), soap_begin_send(soap) || soap_put_int(soap, data, "int", NULL) || soap_end_send(soap), (soap)->error )
#endif

SOAP_FMAC3 int * SOAP_FMAC4 soap_get_int(struct soap*, int *, const char*, const char*);

#ifndef soap_read_int
#define soap_read_int(soap, data) ( soap_begin_recv(soap) || !soap_get_int(soap, (data), NULL, NULL) || soap_end_recv(soap), (soap)->error )
#endif

#endif

#ifndef SOAP_TYPE_unsignedByte_DEFINED
#define SOAP_TYPE_unsignedByte_DEFINED
SOAP_FMAC3 void SOAP_FMAC4 soap_default_unsignedByte(struct soap*, unsigned char *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_unsignedByte(struct soap*, const char*, int, const unsigned char *, const char*);
SOAP_FMAC3 unsigned char * SOAP_FMAC4 soap_in_unsignedByte(struct soap*, const char*, unsigned char *, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_unsignedByte(struct soap*, const unsigned char *, const char*, const char*);

#ifndef soap_write_unsignedByte
#define soap_write_unsignedByte(soap, data) ( soap_free_temp(soap), soap_begin_send(soap) || soap_put_unsignedByte(soap, data, "unsignedByte", NULL) || soap_end_send(soap), (soap)->error )
#endif

SOAP_FMAC3 unsigned char * SOAP_FMAC4 soap_get_unsignedByte(struct soap*, unsigned char *, const char*, const char*);

#ifndef soap_read_unsignedByte
#define soap_read_unsignedByte(soap, data) ( soap_begin_recv(soap) || !soap_get_unsignedByte(soap, (data), NULL, NULL) || soap_end_recv(soap), (soap)->error )
#endif

#endif

#ifndef SOAP_TYPE_unsignedInt_DEFINED
#define SOAP_TYPE_unsignedInt_DEFINED
SOAP_FMAC3 void SOAP_FMAC4 soap_default_unsignedInt(struct soap*, unsigned int *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_unsignedInt(struct soap*, const char*, int, const unsigned int *, const char*);
SOAP_FMAC3 unsigned int * SOAP_FMAC4 soap_in_unsignedInt(struct soap*, const char*, unsigned int *, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_unsignedInt(struct soap*, const unsigned int *, const char*, const char*);

#ifndef soap_write_unsignedInt
#define soap_write_unsignedInt(soap, data) ( soap_free_temp(soap), soap_begin_send(soap) || soap_put_unsignedInt(soap, data, "unsignedInt", NULL) || soap_end_send(soap), (soap)->error )
#endif

SOAP_FMAC3 unsigned int * SOAP_FMAC4 soap_get_unsignedInt(struct soap*, unsigned int *, const char*, const char*);

#ifndef soap_read_unsignedInt
#define soap_read_unsignedInt(soap, data) ( soap_begin_recv(soap) || !soap_get_unsignedInt(soap, (data), NULL, NULL) || soap_end_recv(soap), (soap)->error )
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Fault_DEFINED
#define SOAP_TYPE_SOAP_ENV__Fault_DEFINED
SOAP_FMAC3 void SOAP_FMAC4 soap_default_SOAP_ENV__Fault(struct soap*, struct SOAP_ENV__Fault *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_SOAP_ENV__Fault(struct soap*, const struct SOAP_ENV__Fault *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_SOAP_ENV__Fault(struct soap*, const char*, int, const struct SOAP_ENV__Fault *, const char*);
SOAP_FMAC3 struct SOAP_ENV__Fault * SOAP_FMAC4 soap_in_SOAP_ENV__Fault(struct soap*, const char*, struct SOAP_ENV__Fault *, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_SOAP_ENV__Fault(struct soap*, const struct SOAP_ENV__Fault *, const char*, const char*);

#ifndef soap_write_SOAP_ENV__Fault
#define soap_write_SOAP_ENV__Fault(soap, data) ( soap_free_temp(soap), soap_begin_send(soap) || (soap_serialize_SOAP_ENV__Fault(soap, data), 0) || soap_put_SOAP_ENV__Fault(soap, data, "SOAP-ENV:Fault", NULL) || soap_end_send(soap), (soap)->error )
#endif

SOAP_FMAC3 struct SOAP_ENV__Fault * SOAP_FMAC4 soap_get_SOAP_ENV__Fault(struct soap*, struct SOAP_ENV__Fault *, const char*, const char*);

#ifndef soap_read_SOAP_ENV__Fault
#define soap_read_SOAP_ENV__Fault(soap, data) ( ((data) ? (soap_default_SOAP_ENV__Fault(soap, (data)), 0) : 0) || soap_begin_recv(soap) || !soap_get_SOAP_ENV__Fault(soap, (data), NULL, NULL) || soap_end_recv(soap), (soap)->error )
#endif

#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Reason_DEFINED
#define SOAP_TYPE_SOAP_ENV__Reason_DEFINED
SOAP_FMAC3 void SOAP_FMAC4 soap_default_SOAP_ENV__Reason(struct soap*, struct SOAP_ENV__Reason *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_SOAP_ENV__Reason(struct soap*, const struct SOAP_ENV__Reason *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_SOAP_ENV__Reason(struct soap*, const char*, int, const struct SOAP_ENV__Reason *, const char*);
SOAP_FMAC3 struct SOAP_ENV__Reason * SOAP_FMAC4 soap_in_SOAP_ENV__Reason(struct soap*, const char*, struct SOAP_ENV__Reason *, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_SOAP_ENV__Reason(struct soap*, const struct SOAP_ENV__Reason *, const char*, const char*);

#ifndef soap_write_SOAP_ENV__Reason
#define soap_write_SOAP_ENV__Reason(soap, data) ( soap_free_temp(soap), soap_begin_send(soap) || (soap_serialize_SOAP_ENV__Reason(soap, data), 0) || soap_put_SOAP_ENV__Reason(soap, data, "SOAP-ENV:Reason", NULL) || soap_end_send(soap), (soap)->error )
#endif

SOAP_FMAC3 struct SOAP_ENV__Reason * SOAP_FMAC4 soap_get_SOAP_ENV__Reason(struct soap*, struct SOAP_ENV__Reason *, const char*, const char*);

#ifndef soap_read_SOAP_ENV__Reason
#define soap_read_SOAP_ENV__Reason(soap, data) ( ((data) ? (soap_default_SOAP_ENV__Reason(soap, (data)), 0) : 0) || soap_begin_recv(soap) || !soap_get_SOAP_ENV__Reason(soap, (data), NULL, NULL) || soap_end_recv(soap), (soap)->error )
#endif

#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Detail_DEFINED
#define SOAP_TYPE_SOAP_ENV__Detail_DEFINED
SOAP_FMAC3 void SOAP_FMAC4 soap_default_SOAP_ENV__Detail(struct soap*, struct SOAP_ENV__Detail *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_SOAP_ENV__Detail(struct soap*, const struct SOAP_ENV__Detail *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_SOAP_ENV__Detail(struct soap*, const char*, int, const struct SOAP_ENV__Detail *, const char*);
SOAP_FMAC3 struct SOAP_ENV__Detail * SOAP_FMAC4 soap_in_SOAP_ENV__Detail(struct soap*, const char*, struct SOAP_ENV__Detail *, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_SOAP_ENV__Detail(struct soap*, const struct SOAP_ENV__Detail *, const char*, const char*);

#ifndef soap_write_SOAP_ENV__Detail
#define soap_write_SOAP_ENV__Detail(soap, data) ( soap_free_temp(soap), soap_begin_send(soap) || (soap_serialize_SOAP_ENV__Detail(soap, data), 0) || soap_put_SOAP_ENV__Detail(soap, data, "SOAP-ENV:Detail", NULL) || soap_end_send(soap), (soap)->error )
#endif

SOAP_FMAC3 struct SOAP_ENV__Detail * SOAP_FMAC4 soap_get_SOAP_ENV__Detail(struct soap*, struct SOAP_ENV__Detail *, const char*, const char*);

#ifndef soap_read_SOAP_ENV__Detail
#define soap_read_SOAP_ENV__Detail(soap, data) ( ((data) ? (soap_default_SOAP_ENV__Detail(soap, (data)), 0) : 0) || soap_begin_recv(soap) || !soap_get_SOAP_ENV__Detail(soap, (data), NULL, NULL) || soap_end_recv(soap), (soap)->error )
#endif

#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Code_DEFINED
#define SOAP_TYPE_SOAP_ENV__Code_DEFINED
SOAP_FMAC3 void SOAP_FMAC4 soap_default_SOAP_ENV__Code(struct soap*, struct SOAP_ENV__Code *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_SOAP_ENV__Code(struct soap*, const struct SOAP_ENV__Code *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_SOAP_ENV__Code(struct soap*, const char*, int, const struct SOAP_ENV__Code *, const char*);
SOAP_FMAC3 struct SOAP_ENV__Code * SOAP_FMAC4 soap_in_SOAP_ENV__Code(struct soap*, const char*, struct SOAP_ENV__Code *, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_SOAP_ENV__Code(struct soap*, const struct SOAP_ENV__Code *, const char*, const char*);

#ifndef soap_write_SOAP_ENV__Code
#define soap_write_SOAP_ENV__Code(soap, data) ( soap_free_temp(soap), soap_begin_send(soap) || (soap_serialize_SOAP_ENV__Code(soap, data), 0) || soap_put_SOAP_ENV__Code(soap, data, "SOAP-ENV:Code", NULL) || soap_end_send(soap), (soap)->error )
#endif

SOAP_FMAC3 struct SOAP_ENV__Code * SOAP_FMAC4 soap_get_SOAP_ENV__Code(struct soap*, struct SOAP_ENV__Code *, const char*, const char*);

#ifndef soap_read_SOAP_ENV__Code
#define soap_read_SOAP_ENV__Code(soap, data) ( ((data) ? (soap_default_SOAP_ENV__Code(soap, (data)), 0) : 0) || soap_begin_recv(soap) || !soap_get_SOAP_ENV__Code(soap, (data), NULL, NULL) || soap_end_recv(soap), (soap)->error )
#endif

#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Header_DEFINED
#define SOAP_TYPE_SOAP_ENV__Header_DEFINED
SOAP_FMAC3 void SOAP_FMAC4 soap_default_SOAP_ENV__Header(struct soap*, struct SOAP_ENV__Header *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_SOAP_ENV__Header(struct soap*, const struct SOAP_ENV__Header *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_SOAP_ENV__Header(struct soap*, const char*, int, const struct SOAP_ENV__Header *, const char*);
SOAP_FMAC3 struct SOAP_ENV__Header * SOAP_FMAC4 soap_in_SOAP_ENV__Header(struct soap*, const char*, struct SOAP_ENV__Header *, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_SOAP_ENV__Header(struct soap*, const struct SOAP_ENV__Header *, const char*, const char*);

#ifndef soap_write_SOAP_ENV__Header
#define soap_write_SOAP_ENV__Header(soap, data) ( soap_free_temp(soap), soap_begin_send(soap) || (soap_serialize_SOAP_ENV__Header(soap, data), 0) || soap_put_SOAP_ENV__Header(soap, data, "SOAP-ENV:Header", NULL) || soap_end_send(soap), (soap)->error )
#endif

SOAP_FMAC3 struct SOAP_ENV__Header * SOAP_FMAC4 soap_get_SOAP_ENV__Header(struct soap*, struct SOAP_ENV__Header *, const char*, const char*);

#ifndef soap_read_SOAP_ENV__Header
#define soap_read_SOAP_ENV__Header(soap, data) ( ((data) ? (soap_default_SOAP_ENV__Header(soap, (data)), 0) : 0) || soap_begin_recv(soap) || !soap_get_SOAP_ENV__Header(soap, (data), NULL, NULL) || soap_end_recv(soap), (soap)->error )
#endif

#endif

#endif

#ifndef SOAP_TYPE_ns__verifyImg_DEFINED
#define SOAP_TYPE_ns__verifyImg_DEFINED
SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__verifyImg(struct soap*, struct ns__verifyImg *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__verifyImg(struct soap*, const struct ns__verifyImg *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__verifyImg(struct soap*, const char*, int, const struct ns__verifyImg *, const char*);
SOAP_FMAC3 struct ns__verifyImg * SOAP_FMAC4 soap_in_ns__verifyImg(struct soap*, const char*, struct ns__verifyImg *, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__verifyImg(struct soap*, const struct ns__verifyImg *, const char*, const char*);

#ifndef soap_write_ns__verifyImg
#define soap_write_ns__verifyImg(soap, data) ( soap_free_temp(soap), soap_begin_send(soap) || (soap_serialize_ns__verifyImg(soap, data), 0) || soap_put_ns__verifyImg(soap, data, "ns:verifyImg", NULL) || soap_end_send(soap), (soap)->error )
#endif

SOAP_FMAC3 struct ns__verifyImg * SOAP_FMAC4 soap_get_ns__verifyImg(struct soap*, struct ns__verifyImg *, const char*, const char*);

#ifndef soap_read_ns__verifyImg
#define soap_read_ns__verifyImg(soap, data) ( ((data) ? (soap_default_ns__verifyImg(soap, (data)), 0) : 0) || soap_begin_recv(soap) || !soap_get_ns__verifyImg(soap, (data), NULL, NULL) || soap_end_recv(soap), (soap)->error )
#endif

#endif

#ifndef SOAP_TYPE_ns__verifyImgResponse_DEFINED
#define SOAP_TYPE_ns__verifyImgResponse_DEFINED
SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__verifyImgResponse(struct soap*, struct ns__verifyImgResponse *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__verifyImgResponse(struct soap*, const struct ns__verifyImgResponse *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__verifyImgResponse(struct soap*, const char*, int, const struct ns__verifyImgResponse *, const char*);
SOAP_FMAC3 struct ns__verifyImgResponse * SOAP_FMAC4 soap_in_ns__verifyImgResponse(struct soap*, const char*, struct ns__verifyImgResponse *, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__verifyImgResponse(struct soap*, const struct ns__verifyImgResponse *, const char*, const char*);

#ifndef soap_write_ns__verifyImgResponse
#define soap_write_ns__verifyImgResponse(soap, data) ( soap_free_temp(soap), soap_begin_send(soap) || (soap_serialize_ns__verifyImgResponse(soap, data), 0) || soap_put_ns__verifyImgResponse(soap, data, "ns:verifyImgResponse", NULL) || soap_end_send(soap), (soap)->error )
#endif

SOAP_FMAC3 struct ns__verifyImgResponse * SOAP_FMAC4 soap_get_ns__verifyImgResponse(struct soap*, struct ns__verifyImgResponse *, const char*, const char*);

#ifndef soap_read_ns__verifyImgResponse
#define soap_read_ns__verifyImgResponse(soap, data) ( ((data) ? (soap_default_ns__verifyImgResponse(soap, (data)), 0) : 0) || soap_begin_recv(soap) || !soap_get_ns__verifyImgResponse(soap, (data), NULL, NULL) || soap_end_recv(soap), (soap)->error )
#endif

#endif

#ifndef SOAP_TYPE_ns__ping_DEFINED
#define SOAP_TYPE_ns__ping_DEFINED
SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__ping(struct soap*, struct ns__ping *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__ping(struct soap*, const struct ns__ping *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__ping(struct soap*, const char*, int, const struct ns__ping *, const char*);
SOAP_FMAC3 struct ns__ping * SOAP_FMAC4 soap_in_ns__ping(struct soap*, const char*, struct ns__ping *, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__ping(struct soap*, const struct ns__ping *, const char*, const char*);

#ifndef soap_write_ns__ping
#define soap_write_ns__ping(soap, data) ( soap_free_temp(soap), soap_begin_send(soap) || (soap_serialize_ns__ping(soap, data), 0) || soap_put_ns__ping(soap, data, "ns:ping", NULL) || soap_end_send(soap), (soap)->error )
#endif

SOAP_FMAC3 struct ns__ping * SOAP_FMAC4 soap_get_ns__ping(struct soap*, struct ns__ping *, const char*, const char*);

#ifndef soap_read_ns__ping
#define soap_read_ns__ping(soap, data) ( ((data) ? (soap_default_ns__ping(soap, (data)), 0) : 0) || soap_begin_recv(soap) || !soap_get_ns__ping(soap, (data), NULL, NULL) || soap_end_recv(soap), (soap)->error )
#endif

#endif

#ifndef SOAP_TYPE_ns__pingResponse_DEFINED
#define SOAP_TYPE_ns__pingResponse_DEFINED
SOAP_FMAC3 void SOAP_FMAC4 soap_default_ns__pingResponse(struct soap*, struct ns__pingResponse *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_ns__pingResponse(struct soap*, const struct ns__pingResponse *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_ns__pingResponse(struct soap*, const char*, int, const struct ns__pingResponse *, const char*);
SOAP_FMAC3 struct ns__pingResponse * SOAP_FMAC4 soap_in_ns__pingResponse(struct soap*, const char*, struct ns__pingResponse *, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_ns__pingResponse(struct soap*, const struct ns__pingResponse *, const char*, const char*);

#ifndef soap_write_ns__pingResponse
#define soap_write_ns__pingResponse(soap, data) ( soap_free_temp(soap), soap_begin_send(soap) || (soap_serialize_ns__pingResponse(soap, data), 0) || soap_put_ns__pingResponse(soap, data, "ns:pingResponse", NULL) || soap_end_send(soap), (soap)->error )
#endif

SOAP_FMAC3 struct ns__pingResponse * SOAP_FMAC4 soap_get_ns__pingResponse(struct soap*, struct ns__pingResponse *, const char*, const char*);

#ifndef soap_read_ns__pingResponse
#define soap_read_ns__pingResponse(soap, data) ( ((data) ? (soap_default_ns__pingResponse(soap, (data)), 0) : 0) || soap_begin_recv(soap) || !soap_get_ns__pingResponse(soap, (data), NULL, NULL) || soap_end_recv(soap), (soap)->error )
#endif

#endif

#ifndef SOAP_TYPE_xsd__base64Binary_DEFINED
#define SOAP_TYPE_xsd__base64Binary_DEFINED
SOAP_FMAC3 void SOAP_FMAC4 soap_default_xsd__base64Binary(struct soap*, struct xsd__base64Binary *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_xsd__base64Binary(struct soap*, const struct xsd__base64Binary *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_xsd__base64Binary(struct soap*, const char*, int, const struct xsd__base64Binary *, const char*);
SOAP_FMAC3 struct xsd__base64Binary * SOAP_FMAC4 soap_in_xsd__base64Binary(struct soap*, const char*, struct xsd__base64Binary *, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_xsd__base64Binary(struct soap*, const struct xsd__base64Binary *, const char*, const char*);

#ifndef soap_write_xsd__base64Binary
#define soap_write_xsd__base64Binary(soap, data) ( soap_free_temp(soap), soap_begin_send(soap) || (soap_serialize_xsd__base64Binary(soap, data), 0) || soap_put_xsd__base64Binary(soap, data, "xsd:base64Binary", NULL) || soap_end_send(soap), (soap)->error )
#endif

SOAP_FMAC3 struct xsd__base64Binary * SOAP_FMAC4 soap_get_xsd__base64Binary(struct soap*, struct xsd__base64Binary *, const char*, const char*);

#ifndef soap_read_xsd__base64Binary
#define soap_read_xsd__base64Binary(soap, data) ( ((data) ? (soap_default_xsd__base64Binary(soap, (data)), 0) : 0) || soap_begin_recv(soap) || !soap_get_xsd__base64Binary(soap, (data), NULL, NULL) || soap_end_recv(soap), (soap)->error )
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_PointerToSOAP_ENV__Reason_DEFINED
#define SOAP_TYPE_PointerToSOAP_ENV__Reason_DEFINED
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerToSOAP_ENV__Reason(struct soap*, struct SOAP_ENV__Reason *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerToSOAP_ENV__Reason(struct soap*, const char *, int, struct SOAP_ENV__Reason *const*, const char *);
SOAP_FMAC3 struct SOAP_ENV__Reason ** SOAP_FMAC4 soap_in_PointerToSOAP_ENV__Reason(struct soap*, const char*, struct SOAP_ENV__Reason **, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerToSOAP_ENV__Reason(struct soap*, struct SOAP_ENV__Reason *const*, const char*, const char*);
SOAP_FMAC3 struct SOAP_ENV__Reason ** SOAP_FMAC4 soap_get_PointerToSOAP_ENV__Reason(struct soap*, struct SOAP_ENV__Reason **, const char*, const char*);
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_PointerToSOAP_ENV__Detail_DEFINED
#define SOAP_TYPE_PointerToSOAP_ENV__Detail_DEFINED
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerToSOAP_ENV__Detail(struct soap*, struct SOAP_ENV__Detail *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerToSOAP_ENV__Detail(struct soap*, const char *, int, struct SOAP_ENV__Detail *const*, const char *);
SOAP_FMAC3 struct SOAP_ENV__Detail ** SOAP_FMAC4 soap_in_PointerToSOAP_ENV__Detail(struct soap*, const char*, struct SOAP_ENV__Detail **, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerToSOAP_ENV__Detail(struct soap*, struct SOAP_ENV__Detail *const*, const char*, const char*);
SOAP_FMAC3 struct SOAP_ENV__Detail ** SOAP_FMAC4 soap_get_PointerToSOAP_ENV__Detail(struct soap*, struct SOAP_ENV__Detail **, const char*, const char*);
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_PointerToSOAP_ENV__Code_DEFINED
#define SOAP_TYPE_PointerToSOAP_ENV__Code_DEFINED
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerToSOAP_ENV__Code(struct soap*, struct SOAP_ENV__Code *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerToSOAP_ENV__Code(struct soap*, const char *, int, struct SOAP_ENV__Code *const*, const char *);
SOAP_FMAC3 struct SOAP_ENV__Code ** SOAP_FMAC4 soap_in_PointerToSOAP_ENV__Code(struct soap*, const char*, struct SOAP_ENV__Code **, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerToSOAP_ENV__Code(struct soap*, struct SOAP_ENV__Code *const*, const char*, const char*);
SOAP_FMAC3 struct SOAP_ENV__Code ** SOAP_FMAC4 soap_get_PointerToSOAP_ENV__Code(struct soap*, struct SOAP_ENV__Code **, const char*, const char*);
#endif

#endif

#ifndef SOAP_TYPE_PointerToxsd__base64Binary_DEFINED
#define SOAP_TYPE_PointerToxsd__base64Binary_DEFINED
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerToxsd__base64Binary(struct soap*, struct xsd__base64Binary *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerToxsd__base64Binary(struct soap*, const char *, int, struct xsd__base64Binary *const*, const char *);
SOAP_FMAC3 struct xsd__base64Binary ** SOAP_FMAC4 soap_in_PointerToxsd__base64Binary(struct soap*, const char*, struct xsd__base64Binary **, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerToxsd__base64Binary(struct soap*, struct xsd__base64Binary *const*, const char*, const char*);
SOAP_FMAC3 struct xsd__base64Binary ** SOAP_FMAC4 soap_get_PointerToxsd__base64Binary(struct soap*, struct xsd__base64Binary **, const char*, const char*);
#endif

#ifndef SOAP_TYPE_PointerToxsd__string_DEFINED
#define SOAP_TYPE_PointerToxsd__string_DEFINED
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerToxsd__string(struct soap*, char **const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerToxsd__string(struct soap*, const char *, int, char **const*, const char *);
SOAP_FMAC3 char *** SOAP_FMAC4 soap_in_PointerToxsd__string(struct soap*, const char*, char ***, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerToxsd__string(struct soap*, char **const*, const char*, const char*);
SOAP_FMAC3 char *** SOAP_FMAC4 soap_get_PointerToxsd__string(struct soap*, char ***, const char*, const char*);
#endif

#ifndef SOAP_TYPE_PointerTounsignedByte_DEFINED
#define SOAP_TYPE_PointerTounsignedByte_DEFINED
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTounsignedByte(struct soap*, unsigned char *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTounsignedByte(struct soap*, const char *, int, unsigned char *const*, const char *);
SOAP_FMAC3 unsigned char ** SOAP_FMAC4 soap_in_PointerTounsignedByte(struct soap*, const char*, unsigned char **, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTounsignedByte(struct soap*, unsigned char *const*, const char*, const char*);
SOAP_FMAC3 unsigned char ** SOAP_FMAC4 soap_get_PointerTounsignedByte(struct soap*, unsigned char **, const char*, const char*);
#endif

#ifndef SOAP_TYPE_xsd__string_DEFINED
#define SOAP_TYPE_xsd__string_DEFINED
SOAP_FMAC3 void SOAP_FMAC4 soap_default_xsd__string(struct soap*, char **);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_xsd__string(struct soap*, char *const*);

#define soap_xsd__string2s soap_string2s

SOAP_FMAC3 int SOAP_FMAC4 soap_out_xsd__string(struct soap*, const char*, int, char*const*, const char*);

#define soap_s2xsd__string soap_s2string

SOAP_FMAC3 char * * SOAP_FMAC4 soap_in_xsd__string(struct soap*, const char*, char **, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_xsd__string(struct soap*, char *const*, const char*, const char*);

#ifndef soap_write_xsd__string
#define soap_write_xsd__string(soap, data) ( soap_free_temp(soap), soap_begin_send(soap) || soap_put_xsd__string(soap, data, "xsd:string", NULL) || soap_end_send(soap), (soap)->error )
#endif

SOAP_FMAC3 char ** SOAP_FMAC4 soap_get_xsd__string(struct soap*, char **, const char*, const char*);

#ifndef soap_read_xsd__string
#define soap_read_xsd__string(soap, data) ( soap_begin_recv(soap) || !soap_get_xsd__string(soap, (data), NULL, NULL) || soap_end_recv(soap), (soap)->error )
#endif

#endif

#ifndef SOAP_TYPE__XML_DEFINED
#define SOAP_TYPE__XML_DEFINED
#endif

#ifndef SOAP_TYPE__QName_DEFINED
#define SOAP_TYPE__QName_DEFINED
SOAP_FMAC3 void SOAP_FMAC4 soap_default__QName(struct soap*, char **);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__QName(struct soap*, char *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__QName(struct soap*, const char*, int, char*const*, const char*);
SOAP_FMAC3 char * * SOAP_FMAC4 soap_in__QName(struct soap*, const char*, char **, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put__QName(struct soap*, char *const*, const char*, const char*);

#ifndef soap_write__QName
#define soap_write__QName(soap, data) ( soap_free_temp(soap), soap_begin_send(soap) || soap_put__QName(soap, data, "QName", NULL) || soap_end_send(soap), (soap)->error )
#endif

SOAP_FMAC3 char ** SOAP_FMAC4 soap_get__QName(struct soap*, char **, const char*, const char*);

#ifndef soap_read__QName
#define soap_read__QName(soap, data) ( soap_begin_recv(soap) || !soap_get__QName(soap, (data), NULL, NULL) || soap_end_recv(soap), (soap)->error )
#endif

#endif

#ifndef SOAP_TYPE_string_DEFINED
#define SOAP_TYPE_string_DEFINED
SOAP_FMAC3 void SOAP_FMAC4 soap_default_string(struct soap*, char **);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_string(struct soap*, char *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_string(struct soap*, const char*, int, char*const*, const char*);
SOAP_FMAC3 char * * SOAP_FMAC4 soap_in_string(struct soap*, const char*, char **, const char*);
SOAP_FMAC3 int SOAP_FMAC4 soap_put_string(struct soap*, char *const*, const char*, const char*);

#ifndef soap_write_string
#define soap_write_string(soap, data) ( soap_free_temp(soap), soap_begin_send(soap) || soap_put_string(soap, data, "string", NULL) || soap_end_send(soap), (soap)->error )
#endif

SOAP_FMAC3 char ** SOAP_FMAC4 soap_get_string(struct soap*, char **, const char*, const char*);

#ifndef soap_read_string
#define soap_read_string(soap, data) ( soap_begin_recv(soap) || !soap_get_string(soap, (data), NULL, NULL) || soap_end_recv(soap), (soap)->error )
#endif

#endif

#ifdef __cplusplus
}
#endif

#endif

/* End of soapH.h */
