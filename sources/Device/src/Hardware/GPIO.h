#pragma once


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GPIO
{
public:
    void SetInputPullDown(GPIO_TypeDef *gpio, uint numPin);
    void SetOutputPP_PullDown(GPIO_TypeDef *gpio, uint numPin);
private:
};

extern GPIO gpio;

// Недостаток макросов в том, что их испоьзование расходует больше места.
// Их следует использовать тогда, когда необходима скорость. Например, при переключении режимов шины

#define SET_INPUT_PULL_DOWN(gpio, pin)                  \
    gpio->MODER &= ~(3U << (pin * 2U));                 \
    gpio->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (pin * 2U));  \
    gpio->PUPDR |= GPIO_PULLDOWN << (pin * 2U);

#define SET_OUTPUT_PP_PULL_DOWN(gpio, pin)              \
    gpio->MODER &= ~(GPIO_MODER_MODER0 << (pin * 2U));  \
    gpio->MODER |= (GPIO_MODE_OUTPUT_PP << (pin * 2U)); \
    gpio->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (pin * 2U));  \
    gpio->PUPDR |= GPIO_PULLDOWN << (pin * 2U);

