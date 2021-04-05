#include "nrf24l01p.h"


const uint8_t TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址
const uint8_t RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址


/*
*********************************************************************************************************
*	函 数 名: void NRF24L01P_SPI_Init(void)
*	功能说明: 初始化NRF24L01P用到的SPI
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
void NRF24L01P_SPI_Init(void)
{
	
 	SPI_InitTypeDef  SPI_InitStructure;
	
	SPI_Cmd(SPI1, DISABLE); //失能SPI外设
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//串行同步时钟的空闲状态为低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//串行同步时钟的第1个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
	
}
/*
*********************************************************************************************************
*	函 数 名: void NRF24L01P_Init(void)
*	功能说明: 初始化NRF24L01P
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/
void NRF24L01P_Init(void)
{  
	GPIO_InitTypeDef  GPIO_InitStructure;


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOB,A时钟
	
	//GPIOB0,1推挽输出
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化PB0,1
	
	//GPIOA.4上拉输入
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化PG8
  
 	SPI1_Init();    		//初始化SPI1  
	
	NRF24L01P_SPI_Init();//针对NRF的特点修改SPI的设置

	NRF24L01P_CE_0(); 			//使能24L01P
	NRF24L01P_CSN_1();			//SPI片选取消	 		 	 
}
/*
*********************************************************************************************************
*	函 数 名: uint8_t NRF24L01P_Check(void)
*	功能说明: 检测24L01P是否存在
*	形    参：
*	返 回 值: 0，成功;1，失败
*********************************************************************************************************
*/	
uint8_t NRF24L01P_Check(void)
{
	uint8_t buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	uint8_t i;
	SPI1_SetSpeed(SPI_BaudRatePrescaler_8); //spi速度为10.5Mhz（24L01的最大SPI时钟为10Mhz）   	 
	NRF24L01P_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//写入5个字节的地址.	
	NRF24L01P_Read_Buf(TX_ADDR,buf,5); //读出写入的地址  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;//检测24L01P错误	
	return 0;		 //检测到24L01
}
/*
*********************************************************************************************************
*	函 数 名: uint8_t NRF24L01P_Write_Reg(uint8_t reg,uint8_t value)
*	功能说明: 写入SPI寄存器值
*	形    参：reg:要写入寄存器
*			  value:写入的值
*	返 回 值: 状态值
*********************************************************************************************************
*/	 	 
uint8_t NRF24L01P_Write_Reg(uint8_t reg,uint8_t value)
{
	uint8_t status;	
  NRF24L01P_CSN_0();                 //使能SPI传输
  status =SPI1_ReadWriteByte(reg);//发送寄存器号 
  SPI1_ReadWriteByte(value);      //写入寄存器的值
  NRF24L01P_CSN_1();                 //禁止SPI传输	   
  return status;       			//返回状态值
}
/*
*********************************************************************************************************
*	函 数 名: uint8_t NRF24L01P_Read_Reg(uint8_t reg)
*	功能说明: 读取SPI寄存器值
*	形    参：reg:要读的寄存器
*	返 回 值: 状态值
*********************************************************************************************************
*/
uint8_t NRF24L01P_Read_Reg(uint8_t reg)
{
	uint8_t reg_val;	    
  NRF24L01P_CSN_0();          //使能SPI传输		
  SPI1_ReadWriteByte(reg);   //发送寄存器号
  reg_val=SPI1_ReadWriteByte(0XFF);//读取寄存器内容
  NRF24L01P_CSN_1();          //禁止SPI传输		    
  return reg_val;           //返回状态值
}
/*
*********************************************************************************************************
*	函 数 名: uint8_t NRF24L01P_Read_Buf(uint8_t reg,uint8_t *pBuf,uint8_t len)
*	功能说明: 在指定位置读出指定长度的数据
*	形    参：reg:寄存器(位置)
*			  pBuf:数据指针
*  			  len:数据长度
*	返 回 值: 此次读到的状态寄存器值 
*********************************************************************************************************
*/	
uint8_t NRF24L01P_Read_Buf(uint8_t reg,uint8_t *pBuf,uint8_t len)
{
  uint8_t status,uint8_t_ctr;	       
  NRF24L01P_CSN_0();           //使能SPI传输
  status=SPI1_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值   	   
  for(uint8_t_ctr=0;uint8_t_ctr<len;uint8_t_ctr++)pBuf[uint8_t_ctr]=SPI1_ReadWriteByte(0XFF);//读出数据
  NRF24L01P_CSN_1();       //关闭SPI传输
  return status;        //返回读到的状态值
}
/*
*********************************************************************************************************
*	函 数 名: uint8_t NRF24L01P_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
*	功能说明: 在指定位置写入指定长度的数据
*	形    参：reg:寄存器(位置)
*			  pBuf:数据指针
*  			  len:数据长度
*	返 回 值: 此次读到的状态寄存器值 
*********************************************************************************************************
*/
uint8_t NRF24L01P_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
  uint8_t status,uint8_t_ctr;	    
  NRF24L01P_CSN_0();          //使能SPI传输
  status = SPI1_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值
  for(uint8_t_ctr=0; uint8_t_ctr<len; uint8_t_ctr++)SPI1_ReadWriteByte(*pBuf++); //写入数据	 
  NRF24L01P_CSN_1();       //关闭SPI传输
  return status;          //返回读到的状态值
}
/*
*********************************************************************************************************
*	函 数 名: uint8_t NRF24L01P_TxPacket(uint8_t *txbuf)
*	功能说明: 启动NRF24L01P发送一次数据
*	形    参：txbuf:待发送数据首地址
*	返 回 值: 发送完成状况
*********************************************************************************************************
*/				   
uint8_t NRF24L01P_TxPacket(uint8_t *txbuf)
{
	uint8_t sta;
 	SPI1_SetSpeed(SPI_BaudRatePrescaler_8);//spi速度为10.5Mhz（24L01的最大SPI时钟为10Mhz）   
	NRF24L01P_CE_0();
  	NRF24L01P_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  32个字节
 	NRF24L01P_CE_1();//启动发送	   
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)!=0);//等待发送完成
	sta=NRF24L01P_Read_Reg(STATUS);  //读取状态寄存器的值	   
	NRF24L01P_Write_Reg(NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&MAX_TX)//达到最大重发次数
	{
		NRF24L01P_Write_Reg(FLUSH_TX,0xff);//清除TX FIFO寄存器 
		return MAX_TX; 
	}
	if(sta&TX_OK)//发送完成
	{
		return TX_OK;
	}
	return 0xff;//其他原因发送失败
}
/*
*********************************************************************************************************
*	函 数 名: uint8_t NRF24L01P_RxPacket(uint8_t *rxbuf)
*	功能说明: 启动NRF24L01P接收一次数据
*	形    参：txbuf:接收数据待存放首地址
*	返 回 值: 0，接收完成；其他，错误代码
*********************************************************************************************************
*/
uint8_t NRF24L01P_RxPacket(uint8_t *rxbuf)
{
	uint8_t sta;		    							   
	SPI1_SetSpeed(SPI_BaudRatePrescaler_8); //spi速度为10.5Mhz（24L01的最大SPI时钟为10Mhz）   
	sta=NRF24L01P_Read_Reg(STATUS);  //读取状态寄存器的值    	 
	NRF24L01P_Write_Reg(NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&RX_OK)//接收到数据
	{
		NRF24L01P_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
		NRF24L01P_Write_Reg(FLUSH_RX,0xff);//清除RX FIFO寄存器 
		return 0; 
	}	   
	return 1;//没收到任何数据
}
/*
*********************************************************************************************************
*	函 数 名: void NRF24L01P_RX_Mode(void)
*	功能说明: 初始化NRF24L01P到RX模式,设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
*			  当CE变高后,即进入RX模式,并可以接收数据了
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/					    		   
void NRF24L01P_RX_Mode(void)
{
  NRF24L01P_CE_0();	  
  NRF24L01P_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH);//写RX节点地址
	  
  NRF24L01P_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);    //使能通道0的自动应答    
  NRF24L01P_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);//使能通道0的接收地址  	 
  NRF24L01P_Write_Reg(NRF_WRITE_REG+RF_CH,40);	     //设置RF通信频率		  
  NRF24L01P_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度 	    
  NRF24L01P_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);//设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  NRF24L01P_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
  NRF24L01P_CE_1(); //CE为高,进入接收模式
  delay_us(130);        //开始进入接收模式，等待130us 
} 
/*
*********************************************************************************************************
*	函 数 名: void NRF24L01P_TX_Mode(void)
*	功能说明: 初始化NRF24L01P到TX模式,设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,
*			  选择RF频道,波特率和LNA HCURR
*			  当CE变高后,即进入RX模式,并可以接收数据了
*			  CE为高大于10us,则启动发送.
*	形    参：
*	返 回 值: 
*********************************************************************************************************
*/						 	 
void NRF24L01P_TX_Mode(void)
{														 
	NRF24L01P_CE_0();	    
  NRF24L01P_Write_Buf(NRF_WRITE_REG+TX_ADDR,(uint8_t*)TX_ADDRESS,TX_ADR_WIDTH);//写TX节点地址 
  NRF24L01P_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK	  

  NRF24L01P_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答    
  NRF24L01P_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址  
  NRF24L01P_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x15);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:5次
  NRF24L01P_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //设置RF通道为40
  NRF24L01P_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  NRF24L01P_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	NRF24L01P_CE_1();//CE为高,10us后启动发送
	delay_us(10);         //如果是连续发射，在初始化阶段就要拉高CE10us以上，一个一个包发送等到发送在拉高
}



