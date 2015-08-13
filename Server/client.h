#ifndef CLIENT_C
#define CLIENT_C

#endif // CLIENT_C
#include "gsoap/soapH.h"
#include "gsoap/ServiceSoap.nsmap"
#include <iconv.h>
#include <string.h>
#define OUTLEN 255
const char server[] = "http://localhost:51060/webservice/Service.asmx";
int rc;
int main(int argc, char **argv)
{ struct soap soap;
/***/
if (argc < 2)
  { fprintf(stderr, "Usage: [Input account password \n");
    exit(0);
  }
char *in_utf8;
char *in_gb2312=(char *) malloc (70);
strcpy(in_gb2312,argv[4]);
char out[OUTLEN];
//unicode 2 gbk
//rc = u2g(in_utf8,strlen(in_utf8),out,OUTLEN);
//gbk 2 unicode
rc = g2u(in_gb2312,strlen(in_gb2312),out,OUTLEN);
/***/
struct _ns1__HelloWorld  ns1__HelloWorld;
struct _ns1__HelloWorldResponse ns1__HelloWorldResponse;
struct SOAP_ENV__Header header;

struct ns1__CredentialSoapHeader vheader;
vheader.Username=argv[1];
vheader.Password=argv[2];

soap_init(&soap);//初始化
soap_set_mode(&soap, SOAP_C_UTFSTRING);//设置传输模式
header.ns1__CredentialSoapHeader=&vheader;
printf("%s\n",header.ns1__CredentialSoapHeader->Username);
printf("%s\n",header.ns1__CredentialSoapHeader->Password);
soap.header=&header;//设置soap头
soap_call___ns2__HelloWorld(&soap,server,"http://tempuri.org/HelloWorld",&ns1__HelloWorld,&ns1__HelloWorldResponse);
 if (soap.error)
  { soap_print_fault(&soap, stderr);
    exit(1);
  }
  else{
 printf("result = %s\n", ns1__HelloWorldResponse.HelloWorldResult);
}
  soap_destroy(&soap);
  soap_end(&soap);
  soap_done(&soap);
  return 0;
}
//编码转换函数当前没有用到
int code_convert(char *from_charset,char *to_charset,char *inbuf,int inlen,char *outbuf,int outlen)
{
iconv_t cd;
int rc;
char **pin = &inbuf;
char **pout = &outbuf;

cd = iconv_open(to_charset,from_charset);
if (cd==0) return -1;
memset(outbuf,0,outlen);
if (iconv(cd,pin,&inlen,pout,&outlen)==-1) return -1;
iconv_close(cd);
return 0;
}
//UNICODE 2 GB2312
int u2g(char *inbuf,int inlen,char *outbuf,int outlen)
{
return code_convert("utf-8","gb2312",inbuf,inlen,outbuf,outlen);
}
//GB2312 2 UNICODE
int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,outlen);
}
