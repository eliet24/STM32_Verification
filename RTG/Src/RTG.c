#include "RTG.h"

/*
 * This example will show how to use UART interrupt handler
 * by receiving and displaying input keys from the debugger console
 * and displays when IRQ handler fires up.
 */

DATA data;
OUTPUT output;

void rtg_main()              //make swich cases to each function
{
	uint8_t result;
	int select;
	uint8_t check_select;
	printf("Start of program\n\r");
	udpServer_init();					//UDP server initialization
	while (1)
	{
		ethernetif_input(&gnetif);		//Handles the actual reception of bytes from the network interface
		sys_check_timeouts();
		check_select = data.peripheria;
		select = check_select-'0';    //convert char to int for switch case
		//check_select = 2;
		switch (select) {

		case 2:          //uart check
			result = uart_check(data.iterations, data.pattern,data.patt_length);
			//printf("uart check result is: %d\r\n",result);
			break;

		case 8:			//i2c check
			result = i2c_check(data.iterations, data.pattern,data.patt_length);
			//printf("i2c check result is: %d\r\n",result);
			break;

		case 16:		//ADC check
			result = adc_check(data.iterations);
			//printf("ADC check result is:%d\r\n", result);
			break;
		}
		output.result = result;          //creating the return output
		output.test_id = data.test_id;
	}
}


/**
  * @brief This function is called when *any* UART completes receiving X bytes.
  */


/**
  * @brief This function handles USART3 global interrupt (any type of occurrences).
  */
//void USART3_IRQHandler(void)
//{
//	HAL_UART_IRQHandler(&huart3);
//	uart3_flag_h = 1;
//}

//Available types of callback for UARTs only

/*
(+) TxHalfCpltCallback        : Tx Half Complete Callback.
(+) TxCpltCallback            : Tx Complete Callback.
(+) RxHalfCpltCallback        : Rx Half Complete Callback.
(+) RxCpltCallback            : Rx Complete Callback.
(+) ErrorCallback             : Error Callback.
(+) AbortCpltCallback         : Abort Complete Callback.
(+) AbortTransmitCpltCallback : Abort Transmit Complete Callback.
(+) AbortReceiveCpltCallback  : Abort Receive Complete Callback.
(+) WakeupCallback            : Wakeup Callback.
(+) RxFifoFullCallback        : Rx Fifo Full Callback.
(+) TxFifoEmptyCallback       : Tx Fifo Empty Callback.
(+) MspInitCallback           : UART MspInit.
(+) MspDeInitCallback         : UART MspDeInit.
*/
