
#pragma once

#include <string.h>


/* vendor chip id*/
#define QMC6310U_IIC_ADDR               0x1c
#define QMC6310N_IIC_ADDR               0x3c
#define QMC6310_CHIP_ID_REG             0x00

/*data output register*/
#define QMC6310_DATA_OUT_X_LSB_REG      0x01
#define QMC6310_DATA_OUT_X_MSB_REG      0x02
#define QMC6310_DATA_OUT_Y_LSB_REG      0x03
#define QMC6310_DATA_OUT_Y_MSB_REG      0x04
#define QMC6310_DATA_OUT_Z_LSB_REG      0x05
#define QMC6310_DATA_OUT_Z_MSB_REG      0x06
/*Status registers */
#define QMC6310_STATUS_REG              0x09
/* configuration registers */
#define QMC6310_CTL_REG_ONE             0x0A  /* Contrl register one */
#define QMC6310_CTL_REG_TWO             0x0B  /* Contrl register two */

/* Magnetic Sensor Operating Mode MODE[1:0]*/
#define QMC6310_SUSPEND_MODE            0x00
#define QMC6310_NORMAL_MODE             0x01
#define QMC6310_SINGLE_MODE             0x02
#define QMC6310_H_PFM_MODE              0x03


#define QMC6310_DEFAULT_DELAY           200


void getQMC6310ID(uint8_t* id);
void setQMC6310Enable(void);
void getQMC6310XYZGaussRAW(int16_t* x, int16_t* y, int16_t* z, uint32_t* gaussFactor, uint8_t* ovfl, uint8_t* dataValid);
void getQMC6310Heading(double* heading);


