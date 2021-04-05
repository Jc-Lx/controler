#include "iic_ee.h"

/*
*********************************************************************************************************
*	函 数 名: uint8_t ee_CheckOk(void)
*	功能说明: 判断串行EERPOM是否正常
*	形    参: 无
*	返 回 值: 1 表示正常， 0 表示不正常
*********************************************************************************************************
*/
uint8_t ee_CheckOk(void)
{
	uint8_t temp;
	IIC_Init(EEPROM_DEV_ADDR);
	temp = ee_ReadOneByte(255);
	if(temp == 0x55) return 1;
	else
	{
		ee_WriteOneByte(255,0x55);
		temp = ee_ReadOneByte(255);
		if(temp == 0x55)  return 1;
	}
	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: uint8_t ee_ReadOneByte(uint16_t ReadAddr)
*	功能说明: 从串行EEPROM指定地址处开始读取若干数据
*	形    参: ReadAddr : 起始地址
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t ee_ReadOneByte(uint16_t ReadAddr)
{				  
	uint8_t temp=0;		  	    																 
    IIC_Start();  
	IIC_SendByte(0XA0+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据 	   
	IIC_WaitAck(); 
    IIC_SendByte(ReadAddr%256);   //发送低地址
	IIC_WaitAck();	    
	IIC_Start();  	 	   
	IIC_SendByte(0XA1);           //进入接收模式			   
	IIC_WaitAck();	 
    temp=IIC_ReadByte(0);		   
    IIC_Stop();//产生一个停止条件	    
	return temp;
}

/*
*********************************************************************************************************
*	函 数 名: uint8_t ee_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite)
*	功能说明: 从串行EEPROM指定地址处开始读取若干数据
*	形    参: WriteAddr : 起始地址
*			  DataToWrite:要写入的数据
*	返 回 值: 0 表示失败，1表示成功
*********************************************************************************************************
*/
uint8_t ee_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite)
{				   	  	    																 
    IIC_Start();  
	IIC_SendByte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 	 
	IIC_WaitAck();	   
    IIC_SendByte(WriteAddr%256);   //发送低地址
	IIC_WaitAck(); 	 										  		   
	IIC_SendByte(DataToWrite);     //发送字节							   
	IIC_WaitAck();  		    	   
    IIC_Stop();//产生一个停止条件 
	delay_ms(10);

	return 1;	 
}

/*
*********************************************************************************************************
*	函 数 名: uint8_t ee_ReadBytes(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead)
*	功能说明: 从串行EEPROM指定地址处开始读取若干数据
*	形    参: ReadAddr : 起始地址
*			  pBuffer : 存放读到的数据的缓冲区指针
*			  NumToRead : 数据长度，单位为字节
*	返 回 值: 其他表示失败，1表示成功
*********************************************************************************************************
*/
uint8_t ee_ReadBytes(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=ee_ReadOneByte(ReadAddr++);
		NumToRead--;
	}
	return 1;
}

/*
*********************************************************************************************************
*	函 数 名: uint8_t ee_WriteBytes(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite)
*	功能说明: 向串行EEPROM指定地址写入若干数据，采用页写操作提高写入效率
*	形    参: ReadAddr : 起始地址
*			  pBuffer : 存放读到的数据的缓冲区指针
*			  NumToWrite : 数据长度，单位为字节
*	返 回 值: 其他表示失败，1表示成功
*********************************************************************************************************
*/
uint8_t ee_WriteBytes(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite)
{
	while(NumToWrite--)
	{
		ee_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
	return 1;
}

/*
*********************************************************************************************************
*	函 数 名: void ee_Erase(void)
*	功能说明: 擦除ee
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void ee_Erase(void)
{
	uint16_t i;
	uint8_t buf[EEPROM_SIZE];
	
	/* 填充缓冲区 */
	for (i = 0; i < EEPROM_SIZE; i++)
	{
		buf[i] = 0xFF;
	}
	
	/* 写EEPROM, 起始地址 = 0，数据长度为 256 */
	if (ee_WriteBytes(0,buf,256) == 0)
	{
		printf("擦除eeprom出错!\r\n");
		return;
	}
	else
	{
		printf("擦除eeprom成功!\r\n");
	}
}


/*********************************************END OF FILE**********************/
