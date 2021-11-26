#include "main.h"
#include "lwip.h"
#include "sockets.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal_gpio.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdbool.h>

#define PORTNUM 5678UL
#define PORTNUM2 1234UL

#define CMD_BUFFER_MAX_LEN 32U
#define GPIO_STATUS_MSG_LEN 29U
#define ERR_MSG_LEN 15U

#if (USE_UDP_SERVER_PRINTF == 1)
#include <stdio.h>
#define UDP_SERVER_PRINTF(...) do { printf("[udp_server.c: %s: %d]: ",__func__, __LINE__);printf(__VA_ARGS__); } while (0)
#else
#define UDP_SERVER_PRINTF(...)
#endif

static struct sockaddr_in serv_addr, client_addr;
static int socket_fd;

static struct sockaddr_in serv_addr, client2_addr;
static int socket2_fd;

static int udpServerInit(uint16_t portnum)
{
	uint16_t port;
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd == -1) {
		UDP_SERVER_PRINTF("socket() error\n");
		return -1;
	}

	port = htons((uint16_t)portnum);

	bzero(&serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = port;

	if(bind(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))==-1) {
		UDP_SERVER_PRINTF("bind() error\n\r");
		close(fd);
		return -1;
	}

	UDP_SERVER_PRINTF("UDP Server is bound to port %d\n\r", portnum);

	return fd;
}

typedef enum {
	COMMAND_UNKNOWN_ERROR = -1,
	COMMAND_ERR_WRONG_FORMAT = -2,
	COMMAND_ERR_LED_NUMBER = -3,
	COMMAND_ERR_LED_CMD = -4,
	COMMAND_ERR_GPIO_NAME = -5,
	COMMAND_ERR_GPIO_PIN = -6,
	COMMAND_ERR_ARGUMENT = -7,
	COMMAND_OK = 0
}command_error_t;

static command_error_t led_status_handler(const uint8_t * buffer, size_t len, char* msg)
{
	int pin_num;
	char gpio_port;
	if(sscanf((const char *)buffer, "read gpio %s %d", &gpio_port, &pin_num) != 2){
		return COMMAND_ERR_WRONG_FORMAT;
	}
	if(buffer == 0 || len ==0 || len > CMD_BUFFER_MAX_LEN){
		return COMMAND_UNKNOWN_ERROR;
	}
	if(pin_num < 12 || pin_num > 15){
		return COMMAND_ERR_GPIO_PIN;
	}
	if(gpio_port != 'd'){
		return COMMAND_ERR_GPIO_NAME;
	}else {
		uint8_t pin_status = HAL_GPIO_ReadPin (GPIOD, 1 << pin_num);
		sprintf(msg, "Status of GPIOD pin %d = %d\n", pin_num, pin_status);
	}
	return COMMAND_OK;
}

static command_error_t gpio_command_handler(const uint8_t * buffer, size_t len){

	int pin_state;
	int pin_num;
	sscanf((const char *)buffer, "GPIOD.%d=%d", &pin_num, &pin_state);
	if(buffer == 0 || len ==0 || len > CMD_BUFFER_MAX_LEN){
		return COMMAND_ERR_WRONG_FORMAT;
	}
	if(pin_num < 12 || pin_num > 15){
		return COMMAND_ERR_GPIO_PIN;
	}
	if(pin_state == 0 || pin_state == 1){
		HAL_GPIO_WritePin(GPIOD, 1 << pin_num, pin_state);
	} else {
		return COMMAND_UNKNOWN_ERROR;
	}
	return COMMAND_OK;
}

void StartUdpServerTask(void const * argument)
{
	int addr_len;
	int addr2_len;

	osDelay(5000);// wait 5 sec to init lwip stack

	if((socket_fd = udpServerInit(PORTNUM)) < 0) {
		UDP_SERVER_PRINTF("udpServerInit(PORTNUM) error\n\r");
		return;
	}

	if((socket2_fd = udpServerInit(PORTNUM2)) < 0) {
		UDP_SERVER_PRINTF("udpServerInit(PORTNUM2) error\n\r");
		return;
	}

	for(;;)
	{
		bzero(&client_addr, sizeof(client_addr));
		addr_len = sizeof(client_addr);
		bzero(&client2_addr, sizeof(client2_addr));
		addr2_len = sizeof(client2_addr);


		fd_set rfds;
		struct timeval tv;
		int retval;

		/* Watch stdin (fd 0) to see when it has input. */

		FD_ZERO(&rfds);
		FD_SET(socket_fd, &rfds);
		FD_SET(socket2_fd, &rfds);
		/* Wait up to five seconds. */

		tv.tv_sec = 5;
		tv.tv_usec = 0;

		retval = select(FD_SETSIZE, &rfds, NULL, NULL, &tv);
		/* Don't rely on the value of tv now! */

		if (retval == -1)
		{
			close(socket_fd);
			close(socket2_fd);
			break;
		}
		else if (retval)
		{
			uint8_t buffer[CMD_BUFFER_MAX_LEN];
			uint8_t buffer2[CMD_BUFFER_MAX_LEN];
			const size_t buf_size = sizeof(buffer);
			const size_t buf_size2 = sizeof(buffer2);
			command_error_t  r;
			char gpio_status_feedback[GPIO_STATUS_MSG_LEN];
			char err_msg[ERR_MSG_LEN];
			ssize_t received;
			ssize_t received2;

			if (FD_ISSET(socket_fd, &rfds)){

				received = recvfrom(socket_fd, buffer, buf_size, MSG_DONTWAIT, (struct sockaddr *)&client_addr, (socklen_t *)&addr_len);

				if (received > 0){

					if ( (r = led_status_handler(buffer, received, gpio_status_feedback)) != COMMAND_OK){

						UDP_SERVER_PRINTF("led_status_handler() returned error code = %d\n\r", (int)r);
						sprintf(err_msg, "error code %d\n", r);
						if (sendto(socket_fd, err_msg, sizeof(err_msg),  MSG_DONTWAIT, (const struct sockaddr *)&client_addr, addr_len) == -1){

							UDP_SERVER_PRINTF("sendto() returned -1 \n\r");
						}
					}
					else
					{
						UDP_SERVER_PRINTF("command was handles successfully\n\r");
						if (sendto(socket_fd, gpio_status_feedback, sizeof(gpio_status_feedback),  MSG_DONTWAIT, (const struct sockaddr *)&client_addr, addr_len) == -1){

							UDP_SERVER_PRINTF("sendto() returned -1 \n\r");
						}
					}
				}
			}
			if (FD_ISSET(socket2_fd, &rfds)){

				received2 = recvfrom(socket2_fd, buffer2, buf_size2, MSG_DONTWAIT, (struct sockaddr *)&client2_addr, (socklen_t *)&addr2_len);
				if (received2 > 0){

					if ( (r = gpio_command_handler(buffer2, received2)) != COMMAND_OK){

						UDP_SERVER_PRINTF("gpio_command_handler() returned error code = %d\n\r", (int)r);
						sprintf(err_msg, "error code %d\n", r);
						if (sendto(socket2_fd, err_msg, sizeof(err_msg),  MSG_DONTWAIT, (const struct sockaddr *)&client2_addr, addr2_len) == -1){

							UDP_SERVER_PRINTF("sendto() returned -1 \n\r");
						}
					}else{

						UDP_SERVER_PRINTF("command was handles successfully\n\r");
						if (sendto(socket2_fd, "OK\n", sizeof("OK\n"),  MSG_DONTWAIT, (const struct sockaddr *)&client2_addr, addr2_len) == -1){
							UDP_SERVER_PRINTF("sendto() returned -1 \n\r");
						}
					}
				}


			}
		}
		else
		{
			UDP_SERVER_PRINTF("No data within five seconds.\n\r");
		}
	}
}
