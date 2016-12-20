#ifndef __I2C_H__
#define __I2C_H__

#include "stm32f4xx.h"
#include "stm32util.h"

/*----I2C1----SCL----PB8---*/
/*----I2C1----SDA----PB9---*/

#define IIC_DEBUG 1

#define IIC_SCL_PIN      PB8
#define IIC_SDA_PIN      PB9

#define IIC_SCL_H()          GPIO_SET(IIC_SCL_PIN)
#define IIC_SCL_L()          GPIO_RST(IIC_SCL_PIN)
#define IIC_SDA_H()          GPIO_SET(IIC_SDA_PIN)
#define IIC_SDA_L()          GPIO_RST(IIC_SDA_PIN)
#define IIC_SDA_READ()       GPIO_READ_IN(IIC_SDA_PIN)

#define IIC_ERR(c) (((uint32_t)1)<<c)

void IIC_Init(void);
int IIC_Write(u8 dev_addr,u8 reg_addr,u8 data);
int IIC_Read(u8 dev_addr,u8 reg_addr,u8 *pdata,u8 count);

#endif
