#ifndef REGS_RCC_H
#define REGS_RCC_H

#include "register.h"

constexpr uint32_t RCC_BASE = 0x40023800;

class RCC_AHB1ENR : Register32 {
public:
  REGS_BOOL_FIELD(GPIOAEN, 0);
  REGS_BOOL_FIELD(GPIOBEN, 1);
  REGS_BOOL_FIELD(GPIOCEN, 2);
  REGS_BOOL_FIELD(GPIODEN, 3);
  REGS_BOOL_FIELD(GPIOEEN, 4);
  REGS_BOOL_FIELD(GPIOFEN, 5);
  REGS_BOOL_FIELD(GPIOGEN, 6);
  REGS_BOOL_FIELD(GPIOHEN, 7);
  REGS_BOOL_FIELD(CRCEN, 12);
  REGS_BOOL_FIELD(DMA1EN, 21);
  REGS_BOOL_FIELD(DMA2EN, 22);
};

static inline RCC_AHB1ENR * RCC_AHB1ENR() { return (class RCC_AHB1ENR *)(RCC_BASE + 0x30); };

class RCC_APB2ENR : Register32 {
public:
  REGS_BOOL_FIELD(TIM1EN, 0);
};

static inline RCC_APB2ENR * RCC_APB2ENR() { return (class RCC_APB2ENR *)(RCC_BASE + 0x44); };

#endif