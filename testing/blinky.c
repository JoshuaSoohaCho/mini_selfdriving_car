#include "stm32f446xx.h"

#define GPIOAEN                     (1 << 0)
#define GPIOCEN                     (1 << 2)

#define PIN5                        (1 << 5)
#define PIN13                       (1 << 13)

#define LED_PIN                     PIN5
#define PUSH_BUTTON                 PIN13


int main(){
    RCC->AHB1ENR |= GPIOAEN;
    RCC->AHB1ENR |= GPIOCEN;

    // SET PIN A5 to Output

    GPIOA->MODER |= (1 << 10);
    GPIOA->MODER &= ~(1 << 11);

    // SET PIN C13 to Input
    
    GPIOC->MODER &= ~(1 << 26);
    GPIOC->MODER &= ~(1 << 27);


    // push to turn on led
    while(1){

        if (GPIOC->IDR & 1) {
            GPIOA->ODR |= (LED_PIN);
        }
        else {
            GPIOA->ODR &= ~(LED_PIN);
        }
    }


}