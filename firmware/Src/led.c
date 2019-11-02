#include "main.h"
#include "led.h"

void led_loop(uint16_t led_loop_count) {
	for (int i = 0; i < led_loop_count; i++) {
		
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		HAL_Delay(1000);
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		
		HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
		HAL_Delay(1000);
		HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
				
		HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
		HAL_Delay(1000);
		HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
	}
}

/*
Number 0-6 is a colour
0 red
1 green
2 blue
3 yellow
4 cyan
5 violet
6 white
*/

void led_toogle(uint8_t color) {
    switch (color) {
        case 0:
            HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
            break;
        case 1:
            HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
            break;
        case 2:
            HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
            break;
        case 3:
            HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
            HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
            break;
        case 4:
            HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
            HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
            break;
        case 5:
            HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
            HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
            break;
        case 6:
            HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
            HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
            HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
            break;
        default:
            HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);
    }
}
