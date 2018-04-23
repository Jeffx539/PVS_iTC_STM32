
#ifndef _PVS_H_

#define _PVS_H_
#include "stm32f0xx_hal.h"


struct {
	SPI_HandleTypeDef *spi_bus;
	uint16_t width; // in bytes
	uint16_t height;



	uint8_t dc_on;

	// Pins
	uint16_t pin_chip_select;
	uint16_t pin_panel_on;
	uint16_t pin_panel_reset;
	uint16_t pin_dc;
	uint16_t pin_busy;


	//Ports
	GPIO_TypeDef* port_busy;
	GPIO_TypeDef* port_chip_select;
	GPIO_TypeDef* port_panel_on;
	GPIO_TypeDef* port_panel_reset;
	GPIO_TypeDef* port_dc;
} typedef PVS_DISP;




enum {
	PVS_POWER_OK,
	PVS_BREAKAGE,
	PVS_ERROR_DC_DC,
} typedef PVS_POWERUP_STATUS;



enum {
	PVS_DRAW_NORMAL,
	PVS_DRAW_INVERSE,
	PVS_DRAW_NOTHING
} typedef PVS_TYPE;







void PVS_Init(PVS_DISP *disp);
void PVS_InputTemperature(PVS_DISP *disp,uint8_t temp);
uint8_t PVS_PowerUp(PVS_DISP *disp);
uint8_t PVS_CommandPair(PVS_DISP *disp,uint8_t b1,uint8_t b2);
uint8_t PVS_CommandSingle(PVS_DISP *disp,uint8_t b1);
uint8_t PVS_CommandSingleData(PVS_DISP *disp,uint8_t b1);
void PVS_DrawBitmap(PVS_DISP *disp, uint8_t *data);
void PVS_RawSPI(PVS_DISP *disp,uint8_t reg ,uint8_t* bytes,uint16_t sz);
void PVS_RawSPIS(PVS_DISP *disp,uint8_t reg ,uint8_t byte,uint16_t sz);
void PVS_PowerDown(PVS_DISP *disp);

#endif
