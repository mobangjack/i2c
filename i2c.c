/**
 * Copyright (c) 2011-2016, Jack Mo (mobangjack@foxmail.com).
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "main.h"

void IIC_Delay(unsigned int t)
{
	int i = 0;
	for (; i < t; i++)
	{
		int a = 6;
		while (a--);
	}
}

void IIC_Init(void)
{
	GPIO_OUT(IIC_SCL_PIN, GPIO_Fast_Speed, GPIO_OType_OD, GPIO_PuPd_NOPULL);
	GPIO_OUT(IIC_SDA_PIN, GPIO_Fast_Speed, GPIO_OType_OD, GPIO_PuPd_NOPULL);
}

void IIC_SDA_Out(void)
{
	GPIO_OUT(IIC_SDA_PIN, GPIO_High_Speed, GPIO_OType_OD, GPIO_PuPd_NOPULL);
}

void IIC_SDA_In(void)
{
	GPIO_IN(IIC_SDA_PIN, GPIO_Fast_Speed, GPIO_OType_PP, GPIO_PuPd_NOPULL);
}

void IIC_Start(void)								  
{
	IIC_SDA_Out();
	IIC_SDA_H();
	IIC_SCL_H();
	IIC_Delay(1);
	IIC_SDA_L();
	IIC_Delay(1);
	IIC_SCL_L();
}

void IIC_Stop(void)
{
	IIC_SDA_Out();
	IIC_SCL_L();
	IIC_SDA_L();
	IIC_Delay(1);
	IIC_SCL_H();
	IIC_SDA_H();
	IIC_Delay(1);
}

void IIC_Ack(u8 re)					     
{
	IIC_SDA_Out();
	if(re)
	   IIC_SDA_H();
	else
	   IIC_SDA_L();
	IIC_SCL_H();
	IIC_Delay(1);
	IIC_SCL_L();
	IIC_Delay(1);
}

int IIC_WaitAck(void)
{
	u16 time = 1000;
	IIC_SDA_H();
	IIC_SDA_In();
	IIC_Delay(1);
	IIC_SCL_H();
	IIC_Delay(1);
	while(IIC_SDA_READ())
	{
		if(--time)
		{
			IIC_Stop();
			return 0xff;
		}
	}
	IIC_SCL_L();
	return 0;
}

void IIC_WriteByte(u8 byte)
{
	u8 i;
	IIC_SDA_Out();
	IIC_SCL_L();
	for(i = 0; i < 8; i++)
	{
		if(byte & 0x80)
			IIC_SDA_H();
		else
			IIC_SDA_L();
		byte <<= 1;
		IIC_Delay(1);
		IIC_SCL_H();
		IIC_Delay(1);
		IIC_SCL_L();
	}
}

u8 IIC_ReadByte(void)
{
	u8 i, byte = 0;
	IIC_SDA_In();
	for(i = 0; i < 8; i++)
	{
		IIC_SCL_L();
		IIC_Delay(1);
		IIC_SCL_H();
		byte <<= 1;
		if(IIC_SDA_Read())
		   byte++;
		IIC_Delay(1);
	}
	IIC_SCL_L();
	return byte;
}

int IIC_Write(u8 dev_addr,u8 reg_addr,u8 data)
{
	IIC_Start();
    
	IIC_WriteByte(dev_addr);
	if(IIC_WaitAck() == 0xff)
    {
#ifdef IIC_DEBUG
        printf("IIC error 1.\r\n");
#endif
        return IIC_ERR(1);
    }
    
	IIC_WriteByte(reg_addr);
	if(IIC_WaitAck() == 0xff)
    {
#ifdef IIC_DEBUG
		printf("IIC error 2.\r\n");
#endif
        return IIC_ERR(2);
    }

    IIC_WriteByte(data);
    if(IIC_WaitAck() == 0xff)
    {
#ifdef IIC_DEBUG
    	printf("IIC error 3.\r\n");
#endif
        return IIC_ERR(3);
    }

	 IIC_Stop();
    return 0;
}

int IIC_Read(u8 dev_addr,u8 reg_addr,u8 *pdata,u8 count)
{
	u8 i;

    IIC_Start();
	
    IIC_WriteByte(dev_addr);
	if(IIC_WaitAck() == 0xff)
    {
#ifdef IIC_DEBUG
        printf("IIC error 4.\r\n");
#endif
        return IIC_ERR(4);
    }
    
    IIC_WriteByte(reg_addr);
	if(IIC_WaitAck() == 0xff)
    {
#ifdef IIC_DEBUG
        printf("IIC error 5.\r\n");
#endif
        return IIC_ERR(5);
    }
	
    IIC_Start();
    
    IIC_WriteByte(dev_addr+1);
	if(IIC_WaitAck() == 0xff)
    {
#ifdef IIC_DEBUG
		printf("IIC error 6.\r\n");
#endif
        return IIC_ERR(6);
    }
    
    for(i = 0; i < count - 1; i++)
    {
        *pdata = IIC_ReadByte();
        IIC_Ack(0);
        pdata++;
    }

    *pdata = IIC_ReadByte();

    IIC_Ack(1);
    IIC_Stop(); 
    
    return 0;    
}
