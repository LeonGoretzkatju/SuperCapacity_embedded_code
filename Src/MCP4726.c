#include "MCP4726.h"
#include "stm32f1xx_hal.h"
#include "i2c.h"
#include "stm32f1xx_hal_conf.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
static u8 tx_data[4];
//static u8 rx_data[4];
GPIO_InitTypeDef GPIO_InitStruct;
void TransmitOneByte(u16 WriteAddr,u8 DataToWrite);

////在AT24CXX指定地址读出一个数据
////ReadAddr:开始读数的地址  
////返回值  :读到的数据

extern void IIC_Send_Byte(u8 txd);
u8 IIC_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//发送高地址	    
	}else IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据 	   
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //发送低地址
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           //进入接收模式			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//产生一个停止条件	    
	return temp;
}
//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
extern void delay_ms();
void TransmitOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//发送高地址	  
	}else IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //发送低地址
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //发送字节							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//产生一个停止条件 
	delay_ms(10);	 
}
//在AT24CXX里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址  
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
void TransmitLenByte(u16 WriteAddr,u8 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		TransmitOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}
//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void Transmit(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		TransmitOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
//检查AT24CXX是否正常
//这里用了24XX的最后一个地址(255)来存储标志字.
//如果用其他24C系列,这个地址要修改
//返回1:检测失败
//返回0:检测成功
u8 TransmitReceive_Check(void)
{
	u8 temp;
	temp=IIC_ReadOneByte(255);//避免每次开机都写AT24CXX			   
	if(temp==0X55)return 0;		   
	else//排除第一次初始化的情况
	{
		TransmitOneByte(255,0X55);
	    temp=IIC_ReadOneByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}
uint8_t ee_CheckDevice(uint8_t _Address){
	uint8_t ucAck;
	IIC_Start();
	IIC_Send_Byte(_Address | EEPROM_I2C_WR);
	ucAck = IIC_Wait_Ack();
	IIC_Stop();
	IIC_NAck();
	return ucAck;
}
void dacInit(dac_t* dac) {
	dac->driver = &hi2c1;
	hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	__HAL_RCC_I2C1_CLK_ENABLE();
	tx_data[0] = 0x60;
  tx_data[1] = 0;
  tx_data[2] = 0;
	TransmitLenByte(DACADDR, *tx_data, 0x03);
}
void dacUpdate(dac_t* dac) {

  //[7:6] W/ DAC, [5:4] power on, [3:0] for DAC[11:8]
  tx_data[0] = (dac->dacVal >> 8) & 0x0f;
  //[7:0] for DAC[7:0]
  tx_data[1] = dac->dacVal & 0xff;

	dac->driver = &hi2c1;
	hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	__HAL_RCC_I2C1_CLK_ENABLE();
	TransmitLenByte(DACADDR, tx_data[0], 0x02);
}
