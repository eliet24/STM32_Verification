#ifndef INC_RTG_H_
#define INC_RTG_H_
#include "main.h"
#include "stm32f7xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lwip.h"


extern UART_HandleTypeDef huart3;		//Change to match your UART number
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart6;
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;
extern I2C_HandleTypeDef hi2c4;
extern DAC_HandleTypeDef hdac;
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim2;

#define UART_DEBUG &huart3				//Change to match your UART number
#define UART_2 &huart2
#define UART_6 &huart6
#define I2C_1 &hi2c1
#define I2C_2 &hi2c2
#define I2C_4 &hi2c4
#define DAC_1 &hdac
#define ADC_1 &hadc1
#define TIM_2 &htim2
#define MAX_LENGTH 255
#define SERVER_PORT 7

extern struct netif gnetif;	//Generic data structure used for all lwIP network interfaces.


#define END_OF_STRING 0
#define BACKSPACE 8
#define LINE_FEED 10
#define CARRIAGE_RETURN 13

void rtg_main();
int uart_check(uint8_t iter_num, uint8_t pattern[],uint8_t len);
int i2c_check(uint8_t iter_num, uint8_t pattern[],uint8_t len);
int adc_check(uint8_t iter_num);
void udpServer_init();

extern uint8_t uart3_flag_calback;
extern uint8_t uart3_flag_h;
extern uint8_t u2_flag;
extern uint8_t u6_flag;

#pragma pack(1)
typedef struct DATA{
uint32_t test_id;
uint8_t peripheria;
uint8_t iterations;
uint8_t patt_length;
uint8_t pattern[MAX_LENGTH];
}DATA;

#pragma pack(1)
typedef struct OUTPUT{
uint32_t test_id;
uint8_t result;
}OUTPUT;


extern OUTPUT output;
extern DATA data;


typedef struct ip4_addr ip4_addr_t;
typedef uint8_t   u8_t;
typedef int8_t    s8_t;
typedef uint16_t  u16_t;
typedef int16_t   s16_t;
typedef uint32_t  u32_t;
typedef int32_t   s32_t;
//#if LWIP_HAVE_INT64
typedef uint64_t  u64_t;
typedef int64_t   s64_t;
typedef uintptr_t mem_ptr_t;
/** Main packet buffer struct */


#endif /* INC_RTG_H_ */
