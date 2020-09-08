/*
 * libwebsockets-test-client - libwebsockets test implementation
 *
 * Copyright (C) 2011 Andy Green <andy@warmcat.com>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation:
 *  version 2.1 of the License.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <signal.h>

#ifdef _WIN32
#define random rand
#else
#include <unistd.h>
#endif

#include "lws_config.h"

#include "../lib/libwebsockets.h"
#include <sys/types.h>
#include <regex.h>

#define LOG printf

static unsigned int opts;
static int was_closed;
static int deny_deflate;
static int deny_mux;
static struct libwebsocket *wsi_mirror;
static int mirror_lifetime = 0;
static volatile int force_exit = 0;
static int longlived = 0;

typedef void (callback_init)( struct libwebsocket *wsi, const char *type);


typedef struct BmobUserContext{
    callback_init *onInit;
} BmobUserContext;
/*
 * This demo shows how to connect multiple websockets simultaneously to a
 * websocket server (there is no restriction on their having to be the same
 * server just it simplifies the demo).
 *
 *  dumb-increment-protocol:  we connect to the server and print the number
 *				we are given
 *
 *  lws-mirror-protocol: draws random circles, which are mirrored on to every
 *				client (see them being drawn in every browser
 *				session also using the test server)
 */

enum demo_protocols {

	PROTOCOL_DUMB_INCREMENT,
	PROTOCOL_LWS_MIRROR,

	/* always last */
	DEMO_PROTOCOL_COUNT
};


/* dumb_increment protocol */

/*
static const char *cookie_bmob = "ebf__user_login=qsrFw5OOY2NoaGFUcY7Xq8ejmNLFosiSj46m2M6cVKFixs7Gn8-PjWBrYWVmk5luk2R51dVjxqOfhHOQ1ZGf2pO"
                             "-jMuq05hRcGCglKnU2afUlZfBj2uZZmTJmZbHlpacZpTHlZeUaZSWlZZpaMPHbZiWmsWc; PHPSESSID=thdeuq8lbt19ljha4nucgmbv42;"
                             " Hm_lvt_8bd127f03a0fb66b134fe06b676f0dfc=1435910401,1435928129,1436153517,1436318227; Hm_lpvt_8bd127f03a0fb66b134fe06b676f0dfc=1437530708;"
                             " Hm_lvt_b38f059c148ad1aa4d0fc03a170a5561=1435910401,1435928129,1436153518,1436318227; Hm_lpvt_b38f059c148ad1aa4d0fc03a170a5561=1437530708";
*/

static const char *cookie_bmob = "PHPSESSID=kho0v1a27epb8d49v7ud1oj986; Hm_lvt_8bd127f03a0fb66b134fe06b676f0dfc=1437537165; Hm_lpvt_8bd127f03a0fb66b134fe06b676f0dfc=1437537168; Hm_lvt_b38f059c148ad1aa4d0fc03a170a5561=1437537165; Hm_lpvt_b38f059c148ad1aa4d0fc03a170a5561=1437537168";

static const char *agent = "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/43.0.2357.125 Safari/537.36";

static const char *encoding = "gzip, deflate, sdch";

static const char *language = "en-US,en;q=0.8";

//static const char *ext = "permessage-deflate; client_max_window_bits";

//static const char *key = "HkmWhr8yx3fgTXuQmoAofQ==";


void getSubstr(char *stbuf,const char*str, unsigned start, unsigned end) 
{ 
    unsigned n = end - start; 
    memcpy(stbuf, str + start, n);
    *(stbuf + n) = 0;
} 

/*
   var packets = parser.packets = [
   'disconnect'
   , 'connect'
   , 'heartbeat'
   , 'message'
   , 'json'
   , 'event'
   , 'ack'
   , 'error'
   , 'noop'
   ];

 */

//format the input by  regexp = /([^:]+):([0-9]+)?(\+)?:([^:]+)?:?([\s\S]*)?/
//input like 
//1:::
//5:::{"name":"server_
//2::

#define type_disconnect 0
#define type_connect 1
#define type_heartbeat 2
#define type_message 3
#define type_json 4
#define type_event 5
#define type_ack 6
#define type_error 7
#define type_noop 8



void sendPacket(struct libwebsocket *wsi,int type,char *data)
{
    char typeStr[16] = "";
    char id[16] = "";
    char argStr[16] = "";
    char endpoint[32] = "";
    //actually we don't known the message length
    void *messageData = NULL;
    sprintf(typeStr,"%d",type);
    switch(type){
        case type_heartbeat:
        {            
            break;
        }
        case type_event:
        {
            if(strlen(data) != 0){
                messageData = malloc(strlen(data) + 1);
                memcpy(messageData,data,strlen(data));
                *((char*)messageData + strlen(data)) = 0;
            }
            break;
        }
        default:
        {
            LOG("send unknown packet \n");
            return;
        }
    }
    void *sendBuffer = NULL;
    int len = LWS_SEND_BUFFER_PRE_PADDING + 
        strlen(typeStr) + strlen(id) + strlen(argStr) + strlen(endpoint) + (messageData == NULL ? 0 : strlen((char*)messageData)) + 10
        + LWS_SEND_BUFFER_POST_PADDING;
    sendBuffer = malloc(len);  
    memset(sendBuffer,'\1',LWS_SEND_BUFFER_PRE_PADDING);
    //the format is type:id+:endpoint:data
    sprintf((char*)(sendBuffer + LWS_SEND_BUFFER_PRE_PADDING),"%s:%s%s:%s:%s",typeStr,id,argStr,endpoint,(messageData == NULL) ? "" : (char *)messageData);
    LOG("send buffer %s\n",(char*)sendBuffer + LWS_SEND_BUFFER_PRE_PADDING);
    int n = libwebsocket_write(wsi, sendBuffer + LWS_SEND_BUFFER_PRE_PADDING, strlen((char*)sendBuffer) - LWS_SEND_BUFFER_PRE_PADDING, LWS_WRITE_TEXT);
    LOG("send return %d\n",n);
    if (n < 0) {
        LOG("send packet fail---\n");
    }
    if(messageData != NULL) free(messageData);

    if(sendBuffer) free(sendBuffer);
}

static const char *BMOB_SUB_FORMAT = "{\"name\":\"client_sub\",\"args\":[\"{\\\"appKey\\\":\\\"%s\\\",\\\"tableName\\\":\\\"%s\\\",\\\"objectId\\\":\\\"%s\\\",\\\"action\\\":\\\"%s\\\"}\"]}";

void subTable(struct libwebsocket *wsi,const char *appKey,const char *tableName)
{
    char data[512] = "";
    sprintf(data,BMOB_SUB_FORMAT,appKey,tableName,"","updateTable");      
    sendPacket(wsi,type_event,data);
}


void unsubTable(struct libwebsocket *wsi,const char *appKey,const char *tableName)
{       
    char data[512] = "";
    sprintf(data,BMOB_SUB_FORMAT,appKey,tableName,"","unsub_updateTable");      
    sendPacket(wsi,type_event,data);
}


void subRow(struct libwebsocket *wsi,const char *appKey,const char *tableName,const char *objectId)
{       
    char data[512] = "";
    sprintf(data,BMOB_SUB_FORMAT,appKey,tableName,objectId,"updateRow");      
    sendPacket(wsi,type_event,data);
}


void unsubRow(struct libwebsocket *wsi,const char *appKey,const char *tableName,const char *objectId)
{       
    char data[512] = "";
    sprintf(data,BMOB_SUB_FORMAT,appKey,tableName,objectId,"unsub_updateRow");      
    sendPacket(wsi,type_event,data);
}

void onBombServerInit(struct libwebsocket *wsi, const char *type)
{
    LOG("onBombServerInit\n");
    subTable(wsi,"e9978deb922a3ee549d8ca3266edff09","Chat");
}

void handlePacket(struct libwebsocket *wsi,char *type,char *id,char *args,char *endpoint,void *data)
{
    int type_value = -1;
    if(strlen(type) != 0){
        type_value = atoi(type);
    }

    switch(type_value)
    {
        case -1:
        {
            LOG("translate type %s faile\n",type);
            break;
        }
        case type_error:
        {
            break;
        }
        case type_message:
        {
            break;
        }
        case type_event:
        {
            break;
        }
        case type_json:
        {
            break;
        }
        case type_connect:
        {
            //subscribe data
             //access it use libwebsockets_get_protocol(wsi)->user
            
            void *userData = libwebsockets_get_protocol(wsi)->user;
            if(userData != NULL){
                BmobUserContext *userContext = (BmobUserContext*)userData;
               ((callback_init*)(userContext->onInit))(wsi,type); 
            }            
            break;
        }
        case type_ack:
        {
            break;
        }
        case type_disconnect:
        {
            break;
        }
        case type_heartbeat:
        {
            sendPacket(wsi,type_heartbeat,NULL);
            break;
        }
        default:
        {
            LOG("unknown type %d\n",type_value);
            break;
        }
    }

}

void onPayload(struct libwebsocket *wsi,void *in, size_t len)
{
    //format the input by  regexp
    static const char *STR_BMOB_PATTERN = "([^:]+):([0-9]+)?([+])?:([^:]+)?:([^\f\n\r\t\v]*)?";
    regex_t regex;
    int errcode = regcomp(&regex, STR_BMOB_PATTERN , REG_EXTENDED);
    if(errcode != 0){
        LOG("regcomp pattern error %d\n",errcode);      
        return;
    }
    regmatch_t value[6];
    errcode = regexec(&regex, (char *)in, 6, value, 0);
    if(errcode != 0){        
        LOG("regexec does't match error %d\n",errcode);      
        return;
    }
    //type:id:args:endpoint:data
    char type[16] = "";
    char id[16] = "";
    char args[16] = "";
    char endpoint[32] = "";
    //actually we don't known the message length
    void *messageData = NULL;
    int i;
    for (i = 0; i < 6; ++i) {
        if( value[i].rm_so == -1 || i == 0 ) continue;
        if(i == 1){
            getSubstr(type,in,value[i].rm_so,value[i].rm_eo);            
        }else if(i == 2){
            getSubstr(id,in,value[i].rm_so,value[i].rm_eo);
        }else if(i == 3){
            getSubstr(args,in,value[i].rm_so,value[i].rm_eo);
        }else if(i == 4){
            getSubstr(endpoint,in,value[i].rm_so,value[i].rm_eo);
        }else if(i == 5){
            if(value[i].rm_eo - value[i].rm_so > 0){
                messageData = malloc(value[i].rm_eo - value[i].rm_so + 1);
                getSubstr(messageData,in,value[i].rm_so,value[i].rm_eo);
            }
        }        
    }
    LOG("----type %s id %s args %s endpoint %s \n",type,id,args,endpoint);
    if(messageData != NULL) LOG("----message %s \n",(char*)messageData);

    handlePacket(wsi,type,id,args,endpoint,messageData);

    //free
    regfree(&regex);
    if(messageData != NULL) free(messageData);
}

static int
callback_dumb_increment(struct libwebsocket_context *this,
			struct libwebsocket *wsi,
			enum libwebsocket_callback_reasons reason,
					       void *user, void *in, size_t len)
{
    if(reason != LWS_CALLBACK_GET_THREAD_ID) LOG("callback reason %d \n",reason);
	switch (reason) {
    case LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER:
        printf("add---header-len %d-\n",(int)len);
        char **p = (char **)in;
        if (len < 100)
            return 1;
        *p += sprintf(*p, "User-Agent: %s\x0d\x0a",agent);
        *p += sprintf(*p, "Cookie: %s\x0d\x0a",cookie_bmob);
        *p += sprintf(*p, "Accept-Encoding: %s\x0d\x0a",encoding);
        *p += sprintf(*p, "Accept-Language: %s\x0d\x0a",language);
  //      *p += sprintf(*p, "Sec-WebSocket-Key: %s\x0d\x0a",key);
//        *p += sprintf(*p, "Sec-WebSocket-Extensions: %s\x0d\x0a",ext);
        break;

	case LWS_CALLBACK_CLIENT_ESTABLISHED:
		fprintf(stderr, "callback_dumb_increment: LWS_CALLBACK_CLIENT_ESTABLISHED\n");
		break;

	case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
		fprintf(stderr, "LWS_CALLBACK_CLIENT_CONNECTION_ERROR\n");
		was_closed = 1;
		break;

	case LWS_CALLBACK_CLOSED:
		fprintf(stderr, "LWS_CALLBACK_CLOSED\n");
		was_closed = 1;
		break;

	case LWS_CALLBACK_CLIENT_RECEIVE:
		((char *)in)[len] = '\0';
		fprintf(stderr, "rx %d '%s'\n", (int)len, (char *)in);
        onPayload(wsi,in,len);
		break;

	/* because we are protocols[0] ... */

	case LWS_CALLBACK_CLIENT_CONFIRM_EXTENSION_SUPPORTED:
		if ((strcmp(in, "deflate-stream") == 0) && deny_deflate) {
			fprintf(stderr, "denied deflate-stream extension\n");
			return 1;
		}
		if ((strcmp(in, "deflate-frame") == 0) && deny_deflate) {
			fprintf(stderr, "denied deflate-frame extension\n");
			return 1;
		}
		if ((strcmp(in, "x-google-mux") == 0) && deny_mux) {
			fprintf(stderr, "denied x-google-mux extension\n");
			return 1;
		}

		break;

	default:
		break;
	}

	return 0;
}


/* lws-mirror_protocol */


static int
callback_lws_mirror(struct libwebsocket_context *context,
			struct libwebsocket *wsi,
			enum libwebsocket_callback_reasons reason,
					       void *user, void *in, size_t len)
{
	unsigned char buf[LWS_SEND_BUFFER_PRE_PADDING + 4096 +
						  LWS_SEND_BUFFER_POST_PADDING];
	int l = 0;
	int n;
	unsigned int rands[4];

	switch (reason) {

	case LWS_CALLBACK_CLIENT_ESTABLISHED:

		fprintf(stderr, "callback_lws_mirror: LWS_CALLBACK_CLIENT_ESTABLISHED\n");

		libwebsockets_get_random(context, rands, sizeof(rands[0]));
		mirror_lifetime = 10 + (rands[0] & 1023);
		/* useful to test single connection stability */
		if (longlived)
			mirror_lifetime += 50000;

		fprintf(stderr, "opened mirror connection with "
				     "%d lifetime\n", mirror_lifetime);

		/*
		 * mirror_lifetime is decremented each send, when it reaches
		 * zero the connection is closed in the send callback.
		 * When the close callback comes, wsi_mirror is set to NULL
		 * so a new connection will be opened
		 */

		/*
		 * start the ball rolling,
		 * LWS_CALLBACK_CLIENT_WRITEABLE will come next service
		 */

		libwebsocket_callback_on_writable(context, wsi);
		break;

	case LWS_CALLBACK_CLOSED:
		fprintf(stderr, "mirror: LWS_CALLBACK_CLOSED mirror_lifetime=%d\n", mirror_lifetime);
		wsi_mirror = NULL;
		break;

	case LWS_CALLBACK_CLIENT_RECEIVE:
  /*		fprintf(stderr, "rx %d '%s'\n", (int)len, (char *)in); */
		break;

	case LWS_CALLBACK_CLIENT_WRITEABLE:

		for (n = 0; n < 1; n++) {
			libwebsockets_get_random(context, rands, sizeof(rands));
			l += sprintf((char *)&buf[LWS_SEND_BUFFER_PRE_PADDING + l],
					"c #%06X %d %d %d;",
					(int)rands[0] & 0xffffff,
					(int)rands[1] % 500,
					(int)rands[2] % 250,
					(int)rands[3] % 24);
		}

		n = libwebsocket_write(wsi,
		   &buf[LWS_SEND_BUFFER_PRE_PADDING], l, opts | LWS_WRITE_TEXT);

		if (n < 0)
			return -1;
		if (n < l) {
			lwsl_err("Partial write LWS_CALLBACK_CLIENT_WRITEABLE\n");
			return -1;
		}

		mirror_lifetime--;
		if (!mirror_lifetime) {
			fprintf(stderr, "closing mirror session\n");
			return -1;
		} else
			/* get notified as soon as we can write again */
			libwebsocket_callback_on_writable(context, wsi);
		break;

	default:
		break;
	}

	return 0;
}


/* list of supported protocols and callbacks */

static struct libwebsocket_protocols protocols[] = {
	{
//		"dumb-increment-protocol,fake-nonexistant-protocol",
        "",
		callback_dumb_increment,
		0,
		8192,
        2,
	},
	{
		"fake-nonexistant-protocol,lws-mirror-protocol",
		callback_lws_mirror,
		0,
		128,
	},
	{ NULL, NULL, 0, 0 } /* end */
};

void sighandler(int sig)
{
	force_exit = 1;
}

static struct option options[] = {
	{ "help",	no_argument,		NULL, 'h' },
	{ "debug",      required_argument,      NULL, 'd' },
	{ "port",	required_argument,	NULL, 'p' },
	{ "ssl",	no_argument,		NULL, 's' },
	{ "version",	required_argument,	NULL, 'v' },
	{ "undeflated",	no_argument,		NULL, 'u' },
	{ "nomux",	no_argument,		NULL, 'n' },
	{ "longlived",	no_argument,		NULL, 'l' },
	{ NULL, 0, 0, 0 }
};


int main(int argc, char **argv)
{
	int n = 0;
	int ret = 0;
	int port = 7681;
	int use_ssl = 0;
	struct libwebsocket_context *context;
	struct libwebsocket *wsi_dumb;
	int ietf_version = -1; /* latest */
	struct lws_context_creation_info info;

    const char *address = NULL;

	memset(&info, 0, sizeof info);

	fprintf(stderr, "libwebsockets test client\n"
			"(C) Copyright 2010-2015 Andy Green <andy@warmcat.com> "
						    "licensed under LGPL2.1\n");

	if (argc < 2)
		goto usage;

    char bmob_websocket_addr[64] = "";
    const char *bmob_format_wbs = "/socket.io/1/websocket/%s";

    int getBmobAddr = 0;
	while (n >= 0) {
		n = getopt_long(argc, argv, "nuv:hsp:d:lw:", options, NULL);
		if (n < 0)
			continue;
		switch (n) {
		case 'd':
			lws_set_log_level(atoi(optarg), NULL);
			break;
		case 's':
			use_ssl = 2; /* 2 = allow selfsigned */
			break;
		case 'p':
			port = atoi(optarg);
			break;
		case 'l':
			longlived = 1;
			break;
		case 'v':
			ietf_version = atoi(optarg);
			break;
		case 'u':
			deny_deflate = 1;
			break;
		case 'n':
			deny_mux = 1;
			break;
        case 'w':
            sprintf(bmob_websocket_addr,bmob_format_wbs,optarg);
            getBmobAddr = 1;
            printf("BMOB WEBSOCKET ADDR: %s\n",bmob_websocket_addr);
            break;
		case 'h':
			goto usage;
		}
	}

    if(!getBmobAddr) sprintf(bmob_websocket_addr,bmob_format_wbs,"Jl0rM6LErOEtsHKf");

	if (optind >= argc)
		goto usage;

	signal(SIGINT, sighandler);

	address = argv[optind];

	/*
	 * create the websockets context.  This tracks open connections and
	 * knows how to route any traffic and which protocol version to use,
	 * and if each connection is client or server side.
	 *
	 * For this client-only demo, we tell it to not listen on any port.
	 */


    //create context
    //access it use libwebsockets_get_protocol(wsi)->user
    struct BmobUserContext bmobContext;
    bmobContext.onInit = onBombServerInit;

	info.port = CONTEXT_PORT_NO_LISTEN;
	info.protocols = protocols;
    info.protocols[0].user = (void *)(&bmobContext);
#ifndef LWS_NO_EXTENSIONS
	info.extensions = libwebsocket_get_internal_extensions();
#endif
	info.gid = -1;
	info.uid = -1;

	context = libwebsocket_create_context(&info);
	if (context == NULL) {
		fprintf(stderr, "Creating libwebsocket context failed\n");
		return 1;
	}


	fprintf(stderr, "address %s..ssl %d port %d.\n",address,use_ssl,port);

	/* create a client websocket using dumb increment protocol */


	wsi_dumb = libwebsocket_client_connect(context, "io.bmob.cn", 3010, 0,
			bmob_websocket_addr, "io.bmob.cn:3010", "chatroom.bmob.cn",
			 protocols[PROTOCOL_DUMB_INCREMENT].name, ietf_version);

	if (wsi_dumb == NULL) {
		fprintf(stderr, "libwebsocket connect failed\n");
		ret = 1;
		goto bail;
	}

	fprintf(stderr, "Waiting for connect...\n");

	/*
	 * sit there servicing the websocket context to handle incoming
	 * packets, and drawing random circles on the mirror protocol websocket
	 * nothing happens until the client websocket connection is
	 * asynchronously established
	 */

	n = 0;
	while (n >= 0 && !was_closed && !force_exit) {
		n = libwebsocket_service(context, 10);

		if (n < 0)
			continue;
/*
		if (wsi_mirror)
			continue;

	    wsi_mirror = libwebsocket_client_connect(context, "io.bmob.cn", 3010, 0,
			"/socket.io/1/?t=1437532263544", "io.bmob.cn", "http://chatroom.bmob.cn",
			 protocols[PROTOCOL_LWS_MIRROR].name, ietf_version);

		if (wsi_mirror == NULL) {
			fprintf(stderr, "libwebsocket "
					      "mirror connect failed\n");
			ret = 1;
			goto bail;
		}
*/        
        
	}

bail:
	fprintf(stderr, "Exiting\n");

	libwebsocket_context_destroy(context);

	return ret;

usage:
	fprintf(stderr, "Usage: libwebsockets-test-client "
				"<server address> [--port=<p>] "
				"[--ssl] [-k] [-v <ver>] "
				"[-d <log bitfield>] [-l]\n");
	return 1;
}
