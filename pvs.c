#include "pvs.h"

#define U8(...) ((uint8_t[]){__VA_ARGS__})
void PVS_Init(PVS_DISP *disp){

	 HAL_GPIO_WritePin(disp->port_panel_on,disp->pin_panel_on,GPIO_PIN_SET);
	 HAL_GPIO_WritePin(disp->port_chip_select,disp->pin_chip_select,GPIO_PIN_SET);
	 HAL_GPIO_WritePin(disp->port_dc, disp->pin_dc, GPIO_PIN_SET);
	 HAL_GPIO_WritePin(disp->port_panel_reset, disp->pin_panel_reset,GPIO_PIN_RESET);

}




void PVS_RawSPI(PVS_DISP *disp,uint8_t reg ,uint8_t* bytes,uint16_t sz){
	HAL_GPIO_WritePin(disp->port_dc,disp->pin_dc,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(disp->port_chip_select,disp->pin_chip_select,GPIO_PIN_RESET);
	HAL_SPI_Transmit(disp->spi_bus,&reg,1,0xFF);
	HAL_GPIO_WritePin(disp->port_chip_select,disp->pin_chip_select,GPIO_PIN_SET);
	HAL_GPIO_WritePin(disp->port_dc,disp->pin_dc,GPIO_PIN_SET);


	for(size_t i = 0 ;i < sz;i++){
		HAL_GPIO_WritePin(disp->port_chip_select,disp->pin_chip_select,GPIO_PIN_RESET);
		HAL_SPI_Transmit(disp->spi_bus,&bytes[i],1,0xFF);
		HAL_GPIO_WritePin(disp->port_chip_select,disp->pin_chip_select,GPIO_PIN_SET);
	}


}




void PVS_RawSPIS(PVS_DISP *disp,uint8_t reg ,uint8_t byte,uint16_t sz){
	HAL_GPIO_WritePin(disp->port_dc,disp->pin_dc,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(disp->port_chip_select,disp->pin_chip_select,GPIO_PIN_RESET);
	HAL_SPI_Transmit(disp->spi_bus,&reg,1,0xFF);
	HAL_GPIO_WritePin(disp->port_chip_select,disp->pin_chip_select,GPIO_PIN_SET);
	HAL_GPIO_WritePin(disp->port_dc,disp->pin_dc,GPIO_PIN_SET);


	for(size_t i = 0 ;i < sz;i++){
		HAL_GPIO_WritePin(disp->port_chip_select,disp->pin_chip_select,GPIO_PIN_RESET);
		HAL_SPI_Transmit(disp->spi_bus,&byte,1,0xFF);
		HAL_GPIO_WritePin(disp->port_chip_select,disp->pin_chip_select,GPIO_PIN_SET);
	}


}



void PVS_InputTemperature(PVS_DISP *disp,uint8_t temp){
	PVS_RawSPI(disp,0xe5,U8(temp),1);
	PVS_RawSPI(disp,0xe0,U8(0x02),1);
}

uint8_t PVS_PowerUp(PVS_DISP *disp){
	 HAL_GPIO_WritePin(disp->port_panel_reset, disp->pin_panel_reset,GPIO_PIN_SET);
   	 HAL_Delay(10);
	 HAL_GPIO_WritePin(disp->port_panel_reset, disp->pin_panel_reset,GPIO_PIN_RESET);
	 HAL_Delay(10);
	 HAL_GPIO_WritePin(disp->port_panel_reset, disp->pin_panel_reset,GPIO_PIN_SET);
	 HAL_Delay(10);
	 PVS_RawSPI(disp,0x00,U8(0x0E),1);
	 HAL_Delay(10);

	 return PVS_POWER_OK;

}



void PVS_DrawBitmap(PVS_DISP *disp, uint8_t *data){

	PVS_RawSPI(disp,0x10,data,disp->width/8 * disp->height);
	PVS_RawSPIS(disp,0x13,0x00,disp->width/8 * disp->height);

	while(HAL_GPIO_ReadPin(disp->port_busy,disp->pin_busy) == GPIO_PIN_RESET){
		HAL_Delay(2);
	}


	if(!disp->dc_on){
		PVS_RawSPI(disp,0x04,NULL,0);
		while(HAL_GPIO_ReadPin(disp->port_busy,disp->pin_busy) == GPIO_PIN_RESET){
			HAL_Delay(2);
		}
		disp->dc_on = 1;

	}
	PVS_RawSPI(disp,0x12,NULL,0);
}



void PVS_PowerDown(PVS_DISP *disp){
	if(!disp->dc_on)
		return;

	PVS_RawSPI(disp,0x02,NULL,0);
	disp->dc_on = 0;
}

