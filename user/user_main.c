#include "mem.h"
#include "user_interface.h"
#include "espconn.h"
#include "mystuff.h"
#include <string.h>

#include "gpio.h"
#include "os_type.h"

#include "user_config.h"

#define PORT 7777
#define SERVER_TIMEOUT 1000
#define MAX_CONNS 5
#define MAX_FRAME 2000

#define procTaskPrio        0
#define procTaskQueueLen    1


static volatile os_timer_t some_timer;
static struct espconn *pUdpServer;

//Tasks that happen all the time.
os_event_t procTaskQueue[procTaskQueueLen];
static void ICACHE_FLASH_ATTR procTask(os_event_t *events)
{
	system_os_post(procTaskPrio, 0, 0 );
	if( events->sig == 0 && events->par == 0 )
	{
		//Idle Event.
	}
}

//Called when new packet comes in.
static void ICACHE_FLASH_ATTR udpserver_recv(void *arg, char *pusrdata, unsigned short len)
{
	struct espconn *pespconn = (struct espconn *)arg;
	WS2812OutBuffer( pusrdata, len );
}

void user_init(void)
{
<<<<<<< HEAD
	uart_init(BIT_RATE_115200, BIT_RATE_115200);
        
        uart0_sendStr("\r\nCustom Server\r\n");
        
        char ssid[32] = WIFI_USER;
        char password[64] = WIFI_PASS;
        
        struct station_config stconf;
        
        wifi_set_opmode(0x1);
        
        // this is necessary, see: http://41j.com/blog/2015/01/esp8266-wifi-doesnt-connect/
        stconf.bssid_set = 0;
        
        
        os_memcpy((char *)stconf.ssid,ssid, 32);
        os_memcpy((char *)stconf.password,password, 64);
        
        wifi_station_set_config(&stconf);
        wifi_station_set_auto_connect(1);
        

        pUdpServer = (struct espconn *)os_zalloc(sizeof(struct espconn));
	ets_memset( pUdpServer, 0, sizeof( struct espconn ) );
	espconn_create( pUdpServer );
	pUdpServer->type = ESPCONN_UDP;
	pUdpServer->proto.udp = (esp_udp *)os_zalloc(sizeof(esp_udp));
	pUdpServer->proto.udp->local_port = 7777;
	espconn_regist_recvcb(pUdpServer, udpserver_recv);


	if( espconn_create( pUdpServer ) )
	{
		while(1) ;
	}

	ets_wdt_disable();
	char outbuffer[] = { 0x00, 0x00, 0x00 };
	WS2812OutBuffer( outbuffer, 1 ); //Initialize the output.

	//Add a process
	system_os_task(procTask, procTaskPrio, procTaskQueue, procTaskQueueLen);
	uart0_sendStr("\r\nCustom Server\r\n");
	WS2812OutBuffer( outbuffer, sizeof(outbuffer) );
	system_os_post(procTaskPrio, 0, 0 );
}


