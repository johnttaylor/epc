#ifndef COLONY_CONFIG_H_
#define COLONY_CONFIG_H_

// Enable Trace
#define USE_CPL_SYSTEM_TRACE

// Define IO for the Pico-Display
#define OPTION_DRIVER_PICO_DISPLAY_STM32_BUTTON_A_PIN           BUTTON_A_Pin
#define OPTION_DRIVER_PICO_DISPLAY_STM32_BUTTON_A_PORT          BUTTON_A_GPIO_Port
#define OPTION_DRIVER_PICO_DISPLAY_STM32_BUTTON_B_PIN           BUTTON_B_Pin
#define OPTION_DRIVER_PICO_DISPLAY_STM32_BUTTON_B_PORT          BUTTON_B_GPIO_Port
#define OPTION_DRIVER_PICO_DISPLAY_STM32_BUTTON_X_PIN           BUTTON_X_Pin 
#define OPTION_DRIVER_PICO_DISPLAY_STM32_BUTTON_X_PORT          BUTTON_X_GPIO_Port
#define OPTION_DRIVER_PICO_DISPLAY_STM32_BUTTON_Y_PIN           BUTTON_Y_Pin  
#define OPTION_DRIVER_PICO_DISPLAY_STM32_BUTTON_Y_PORT          BUTTON_Y_GPIO_Port      
#define OPTION_DRIVER_PICO_DISPLAY_STM32_RGB_RED_TIMER          PWM_LED_RED_BLOCK_PTR
#define OPTION_DRIVER_PICO_DISPLAY_STM32_RGB_RED_CHANNEL        PWM_LED_RED_CHANNEL
#define OPTION_DRIVER_PICO_DISPLAY_STM32_RGB_GREEN_TIMER        PWM_LED_GREEN_BLOCK_PTR
#define OPTION_DRIVER_PICO_DISPLAY_STM32_RGB_GREEN_CHANNEL      PWM_LED_GREEN_CHANNEL
#define OPTION_DRIVER_PICO_DISPLAY_STM32_RGB_BLUE_TIMER         PWM_LED_BLUE_BLOCK_PTR
#define OPTION_DRIVER_PICO_DISPLAY_STM32_RGB_BLUE_CHANNEL       PWM_LED_BLUE_CHANNEL
#define OPTION_DRIVER_PICO_DISPLAY_STM32_SPI_BUS                LCD_SPI_BLOCK_PTR
#define OPTION_DRIVER_PICO_DISPLAY_STM32_SPI_CS_PIN             LCD_CS_Pin
#define OPTION_DRIVER_PICO_DISPLAY_STM32_SPI_CS_PORT            LCD_CS_GPIO_Port      
#define OPTION_DRIVER_PICO_DISPLAY_STM32_SPI_DC_PIN             LCD_DC_Pin       
#define OPTION_DRIVER_PICO_DISPLAY_STM32_SPI_DC_PORT            LCD_DC_GPIO_Port
#define OPTION_DRIVER_PICO_DISPLAY_STM32_BACKLIGHT_PWM_TIMER    PWM_BACKLIGHT_BLOCK_PTR 
#define OPTION_DRIVER_PICO_DISPLAY_STM32_BACKLIGHT_PWM_CHANNEL  PWM_BACKLIGHT_CHANNEL


//#define LCD_RESET_Pin 
//#define LCD_RESET_GPIO_Port 


#endif