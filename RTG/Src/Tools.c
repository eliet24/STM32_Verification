#include "RTG.h"

typedef struct ip4_addr ip4_addr_t;

// printf
int __io_putchar(int ch)
{
	HAL_UART_Transmit(UART_DEBUG, (uint8_t *)&ch, 1, 0xFFFF);
	return ch;
}

int _write(int file, char *ptr, int len)
{
	HAL_UART_Transmit(UART_DEBUG, (uint8_t *)ptr, len, 0xFFFF);
	return len;
}

// scanf
int __io_getchar(void) {
	uint8_t ch = 0;
	HAL_UART_Receive(UART_DEBUG, &ch, 1, HAL_MAX_DELAY);
	HAL_UART_Transmit(UART_DEBUG, &ch, 1, HAL_MAX_DELAY);
	return ch;
}

int _read(int file, char *ptr, int len) {
	int DataIdx = 0;
	char ch;
	//int counter = 0;
	for (; DataIdx < len; DataIdx++) {
		ch = __io_getchar();
		*ptr = ch;
		ptr++;
		if (ch == CARRIAGE_RETURN || ch == LINE_FEED) {
			*(ptr) = 0;
			break;
		}
	}
	return DataIdx + 1;
}

void udp_receive_callback(
		void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip4_addr_t *addr, uint16_t port)
{
	/* Get the IP of the Client */
	const char* remoteIP = ipaddr_ntoa(addr);
	char buf[100];

	const int len = sprintf (buf,"test id: %d check result is: %c\n",output.test_id,output.result);
	memcpy(&data, p->payload,p->len);


	/* allocate pbuf from RAM*/
	struct pbuf *txBuf = pbuf_alloc(PBUF_TRANSPORT,len, PBUF_RAM);

	/* copy the data into the buffer  */
	pbuf_take(txBuf, buf, len);

	/* Connect to the remote client */
	udp_connect(upcb, addr, port);

	/* Send a Reply to the Client */
	udp_send(upcb, txBuf);
	/* free the UDP connection, so we can accept new clients */
	udp_disconnect(upcb);

	/* Free the p_tx buffer */
	pbuf_free(txBuf);

	/* Free the p buffer */
	pbuf_free(p);

}

void udpServer_init(void) {
	// UDP Control Block structure
   struct udp_pcb* upcb = udp_new();
   err_t err = udp_bind(upcb, IP_ADDR_ANY, SERVER_PORT);  // 7 is the server UDP port

  // 3. Set a receive callback for the upcb
   if (err == ERR_OK) {
	   udp_recv(upcb, udp_receive_callback, NULL);
   }
   else {
	   udp_remove(upcb);
   }
}


uint8_t u2_flag = 0;
uint8_t u6_flag = 0;
//uint8_t result = 1;

//uart check function: sends data from uart2 to uart6 and back iter_num times return 1 if the values remain the same 0 otherwise.
int uart_check(uint8_t iter_num, uint8_t pattern[],uint8_t len)
{
	uint8_t msg[len];
	memcpy(msg,pattern,len);
	uint8_t u6_recieved[len];
	uint8_t u2_recieved[len];
	HAL_UART_Receive_DMA(UART_6, u6_recieved, len);
	HAL_UART_Transmit_DMA(UART_2, msg, len);
 	for(int i=0; i<(int)iter_num;i++)
	{
		if(u2_flag)
		{
			HAL_UART_Receive_IT(UART_2, u2_recieved, len);
			HAL_UART_Transmit_IT(UART_6, u6_recieved, len); //send back to uart2
		}
		if(u6_flag)
		{
			//memcpy(u6_recieved,"other",8);    //checking if different strings will return 0 as needed
			if(strcmp(u6_recieved,u2_recieved) != 0)
				return 0;
			HAL_UART_Receive_DMA(UART_6, u6_recieved, len);
			HAL_UART_Transmit_DMA(UART_2, msg, len);
		}
	}
	return 1;
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == UART_6)
		u2_flag = 1;
	if(huart == UART_2)
		u6_flag = 1;
}

uint8_t flag1 = 0;
uint8_t flag2 = 0;


//i2c check function: send the data from i2c1 to i2c2 and back iter_num times.
//* I2C1 (Master):
//* I2C1_SCL PB6 (CN10)
//* I2C1_SDA PB9 (CN7)

int i2c_check(uint8_t iter_num, uint8_t pattern[],uint8_t len)
{
	uint8_t data_from_i2c1[len];
	uint8_t data_from_i2c2[len];
	uint8_t data_to_i2c2[len];
	uint8_t data_to_i2c1[len];
	memcpy(data_to_i2c2,pattern,len);
	HAL_I2C_Slave_Receive_IT(I2C_2, data_from_i2c1, len);
	HAL_I2C_Master_Transmit_IT(I2C_1, 44, data_to_i2c2, len);
	for(int i=0; i<iter_num;i++)
	{
		if (flag1) {
			memcpy(data_to_i2c1,data_from_i2c1,len);
			HAL_I2C_Master_Receive_IT(I2C_1,44, data_from_i2c2, len);
			HAL_I2C_Slave_Transmit_IT(I2C_2, data_to_i2c1, len);
			flag1 = 0;
		}
		if (flag2) {
			HAL_I2C_Slave_Receive_IT(I2C_2, data_from_i2c1, len);
			HAL_I2C_Master_Transmit_IT(I2C_1, 44, data_to_i2c2, len);
			//memcpy(data_to_i2c2,"other string",13);       //check if function return 0 if the strings of data differ.
			if(strcmp(data_to_i2c2,data_from_i2c2) != 0)
				return 0;
			flag2 = 0;
		}
	}
	return 1;
 }

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c) {
	if (hi2c == I2C_2)
		flag1 = 1;
	if (hi2c == I2C_1) {
		flag2 = 1;
	}
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c){
	if (hi2c == I2C_1)
		flag2 = 1;
}

int adc_flag = 0; //flag for ADC Interrupt
int tim_flag=0;

int adc_check(uint8_t iter_num)
{
	int num = 1000; ///5 volts for conversion.
	HAL_TIM_Base_Start_IT(TIM_2); //Initiate timer 2
	HAL_DAC_SetValue(DAC_1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, num); //Sets the DAC output according to num (0-4k)
	HAL_DAC_Start(DAC_1, DAC_CHANNEL_1); //Enables DAC conversion
	for (int i =0; i<iter_num;i++)
	{
		if(tim_flag)
			HAL_ADC_Start_IT(&hadc1);
		if (adc_flag) //When TIM2 triggers
		{
			adc_flag = 0;
		}
		if(abs(num - HAL_ADC_GetValue(ADC_1)) > num*0.05)
			return 0;
	}
	return 1;
}

// ADC interrupt Callback (convert data finish)
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	adc_flag = 1;
}

// Timer interrupt Callback is triggered every 1 second
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	tim_flag=1;
}

