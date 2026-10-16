#include "controller.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>


/* ================================================================
 *  Register map (STM32H753ZI)
 * ================================================================ */
#define REG32(a)             (*(volatile uint32_t *)(a))

#define PERIPH_BASE          0x40000000UL
#define SRD_AHB4_BASE        (PERIPH_BASE + 0x18020000UL)
#define CD_APB1_BASE         (PERIPH_BASE + 0x00000000UL)

#define GPIOA_BASE           (SRD_AHB4_BASE + 0x0000UL)
#define GPIOB_BASE           (SRD_AHB4_BASE + 0x0400UL)
#define RCC_BASE             (SRD_AHB4_BASE + 0x4400UL)

#define TIM3_BASE            (CD_APB1_BASE + 0x0400UL)
#define TIM4_BASE            (CD_APB1_BASE + 0x0800UL)

#define GPIOA_MODER          REG32(GPIOA_BASE + 0x00)
#define GPIOA_OTYPER         REG32(GPIOA_BASE + 0x04)
#define GPIOA_OSPEEDR        REG32(GPIOA_BASE + 0x08)
#define GPIOA_PUPDR          REG32(GPIOA_BASE + 0x0C)
#define GPIOA_AFRL           REG32(GPIOA_BASE + 0x20)

#define GPIOB_MODER          REG32(GPIOB_BASE + 0x00)
#define GPIOB_OTYPER         REG32(GPIOB_BASE + 0x04)
#define GPIOB_OSPEEDR        REG32(GPIOB_BASE + 0x08)
#define GPIOB_PUPDR          REG32(GPIOB_BASE + 0x0C)
#define GPIOB_AFRL           REG32(GPIOB_BASE + 0x20)
#define GPIOB_BSRR           REG32(GPIOB_BASE + 0x18)

#define RCC_CR               REG32(RCC_BASE + 0x00)
#define RCC_CFGR             REG32(RCC_BASE + 0x10)
#define RCC_D1CFGR           REG32(RCC_BASE + 0x18)
#define RCC_D2CFGR           REG32(RCC_BASE + 0x1C)
#define RCC_AHB4ENR          REG32(RCC_BASE + 0xE0)
#define RCC_APB1LENR         REG32(RCC_BASE + 0xE8)

#define TIM_CR1_O            0x00
#define TIM_SMCR_O           0x08
#define TIM_EGR_O            0x14
#define TIM_CCMR1_O          0x18
#define TIM_CCER_O           0x20
#define TIM_CNT_O            0x24
#define TIM_PSC_O            0x28
#define TIM_ARR_O            0x2C
#define TIM_CCR1_O           0x34
#define TIM_CCR2_O           0x38

#define TIM3_CR1             REG32(TIM3_BASE + TIM_CR1_O)
#define TIM3_SMCR            REG32(TIM3_BASE + TIM_SMCR_O)
#define TIM3_EGR             REG32(TIM3_BASE + TIM_EGR_O)
#define TIM3_CCMR1           REG32(TIM3_BASE + TIM_CCMR1_O)
#define TIM3_CCER            REG32(TIM3_BASE + TIM_CCER_O)
#define TIM3_CNT             REG32(TIM3_BASE + TIM_CNT_O)
#define TIM3_ARR             REG32(TIM3_BASE + TIM_ARR_O)

#define TIM4_CR1             REG32(TIM4_BASE + TIM_CR1_O)
#define TIM4_EGR             REG32(TIM4_BASE + TIM_EGR_O)
#define TIM4_CCMR1           REG32(TIM4_BASE + TIM_CCMR1_O)
#define TIM4_CCER            REG32(TIM4_BASE + TIM_CCER_O)
#define TIM4_PSC             REG32(TIM4_BASE + TIM_PSC_O)
#define TIM4_ARR             REG32(TIM4_BASE + TIM_ARR_O)
#define TIM4_CCR1            REG32(TIM4_BASE + TIM_CCR1_O)
#define TIM4_CCR2            REG32(TIM4_BASE + TIM_CCR2_O)

#define SYST_CSR             REG32(0xE000E010U)
#define SYST_RVR             REG32(0xE000E014U)
#define SYST_CVR             REG32(0xE000E018U)
#define SCB_CPACR            REG32(0xE000ED88U)
#define SCB_CCR              REG32(0xE000ED14U)

#define RCC_CR_HSION         (1U << 0)
#define RCC_CR_HSIRDY        (1U << 2)

#define RCC_AHB4ENR_GPIOAEN  (1U << 0)
#define RCC_AHB4ENR_GPIOBEN  (1U << 1)
#define RCC_APB1LENR_TIM3EN  (1U << 1)
#define RCC_APB1LENR_TIM4EN  (1U << 2)

#define TIM_CR1_CEN          (1U << 0)
#define TIM_CR1_ARPE         (1U << 7)
#define TIM_EGR_UG           (1U << 0)
#define TIM_SMCR_SMS_ENC3    (3U << 0)

#define TIM_CCMR1_CC1S_TI1   (1U << 0)
#define TIM_CCMR1_CC2S_TI2   (1U << 8)
#define TIM_CCMR1_IC1F_5     (5U << 4)
#define TIM_CCMR1_IC2F_5     (5U << 12)
#define TIM_CCMR1_OC1M_PWM1  (6U << 4)
#define TIM_CCMR1_OC2M_PWM1  (6U << 12)
#define TIM_CCMR1_OC1PE      (1U << 3)
#define TIM_CCMR1_OC2PE      (1U << 11)

#define TIM_CCER_CC1E        (1U << 0)
#define TIM_CCER_CC2E        (1U << 4)

#define PULL_UP              1U
#define PULL_DOWN            2U

/* ================================================================
 *  Tunables
 * ================================================================ */
#define PEND_CPR             2400.0f
#define TWO_PI_F             6.28318530718f
#define PEND_RAD_PER_CNT     (TWO_PI_F / PEND_CPR)
#define PEND_OFFSET_CNT      (1200.0f)

#define PWM_ARR              3199U
#define PWM_TOP              (PWM_ARR + 1U)
#define SYSTICK_RELOAD       (64000U - 1U)

/* ================================================================
 *  Core bring-up
 * ================================================================ */
static inline void enable_fpu(void) {
    SCB_CPACR |= (0xFU << 20);
    __asm volatile ("dsb");
    __asm volatile ("isb");
}

static inline void enable_caches(void) {
    SCB_CCR |= (1U << 17) | (1U << 16);
    __asm volatile ("dsb");
    __asm volatile ("isb");
}

static void system_clock_hsi_64mhz(void) {
    RCC_CR |= RCC_CR_HSION;
    while ((RCC_CR & RCC_CR_HSIRDY) == 0U) { }

    RCC_D1CFGR &= ~0xFU;
    RCC_D2CFGR &= ~((0x7U << 4) | (0x7U << 8));

    RCC_CFGR = (RCC_CFGR & ~0x7U);
    while ((RCC_CFGR & (0x7U << 3)) != 0U) { }
}

/* ================================================================
 *  SysTick
 * ================================================================ */
volatile uint32_t systick_ms = 0;

void SysTick_Handler(void) {
    systick_ms++;
}

/* ================================================================
 *  GPIO
 * ================================================================ */
static void gpio_init(void) {
    RCC_AHB4ENR |= RCC_AHB4ENR_GPIOAEN | RCC_AHB4ENR_GPIOBEN;
    (void)RCC_AHB4ENR;

    /* PA6/PA7 = TIM3 AF2 (pendulum encoder only) */
    uint32_t m = GPIOA_MODER;
    m &= ~((3U << 12) | (3U << 14));
    m |=  ((2U << 12) | (2U << 14));
    GPIOA_MODER = m;

    uint32_t a = GPIOA_AFRL;
    a &= ~((0xFU << 24) | (0xFU << 28));
    a |=  ((2U  << 24) | (2U  << 28));
    GPIOA_AFRL = a;

    uint32_t p = GPIOA_PUPDR;
    p &= ~((3U << 12) | (3U << 14));
    p |=  ((PULL_UP << 12) | (PULL_UP << 14));
    GPIOA_PUPDR = p;

    /* PB0 LED, PB6/PB7 TIM4 AF2, PB10/PB11 output */
    m = GPIOB_MODER;
    m &= ~((3U <<  0) | (3U << 12) | (3U << 14) | (3U << 20) | (3U << 22));
    m |=  ((1U <<  0) | (2U << 12) | (2U << 14) | (1U << 20) | (1U << 22));
    GPIOB_MODER = m;

    uint32_t b = GPIOB_AFRL;
    b &= ~((0xFU << 24) | (0xFU << 28));
    b |=  ((2U  << 24) | (2U  << 28));
    GPIOB_AFRL = b;

    GPIOB_OTYPER  &= ~((1U << 0) | (1U << 10) | (1U << 11));
    GPIOB_OSPEEDR |=  ((3U << 0) | (3U << 20) | (3U << 22));

    uint32_t bp = GPIOB_PUPDR;
    bp &= ~((3U << 20) | (3U << 22));
    bp |=  ((PULL_DOWN << 20) | (PULL_DOWN << 22));
    GPIOB_PUPDR = bp;

    GPIOB_BSRR = (1U << 10) | (1U << 11);
}

/* ================================================================
 *  Pendulum encoder (TIM3)
 * ================================================================ */
static void tim3_encoder_init(void) {
    RCC_APB1LENR |= RCC_APB1LENR_TIM3EN;
    (void)RCC_APB1LENR;

    TIM3_CR1   = 0;
    TIM3_CCMR1 = TIM_CCMR1_CC1S_TI1 | TIM_CCMR1_CC2S_TI2
               | TIM_CCMR1_IC1F_5   | TIM_CCMR1_IC2F_5;
    TIM3_CCER  = 0;
    TIM3_SMCR  = TIM_SMCR_SMS_ENC3;
    TIM3_ARR   = 0xFFFFU;
    TIM3_CNT   = 0;
    TIM3_EGR   = TIM_EGR_UG;
    TIM3_CR1  |= TIM_CR1_CEN;
}

/* ================================================================
 *  PWM (BTS7960)
 * ================================================================ */
static void tim4_pwm_init(void) {
    RCC_APB1LENR |= RCC_APB1LENR_TIM4EN;
    (void)RCC_APB1LENR;

    TIM4_CR1   = 0;
    TIM4_PSC   = 0;
    TIM4_ARR   = PWM_ARR;
    TIM4_CCMR1 = TIM_CCMR1_OC1M_PWM1 | TIM_CCMR1_OC2M_PWM1
               | TIM_CCMR1_OC1PE      | TIM_CCMR1_OC2PE;
    TIM4_CCER  = TIM_CCER_CC1E | TIM_CCER_CC2E;
    TIM4_CCR1  = 0;
    TIM4_CCR2  = 0;
    TIM4_CR1  |= TIM_CR1_ARPE;
    TIM4_EGR   = TIM_EGR_UG;
    TIM4_CR1  |= TIM_CR1_CEN;
}

/* ================================================================
 *  Angle
 * ================================================================ */
static inline float wrap_pi(float a) {
    return remainderf(a, TWO_PI_F);
}

static inline float read_pend_angle(void) {
    float c = (float)(int16_t)TIM3_CNT;
    return wrap_pi((c - PEND_OFFSET_CNT) * PEND_RAD_PER_CNT);
}

/* ================================================================
 *  Motor output
 * ================================================================ */
static inline void set_motor(float u) {
    if (!(u == u)) u = 0.0f;
    if (u >  2.0f) u =  2.0f;
    if (u < -2.0f) u = -2.0f;

    /* map ±2 -> 0..100% duty */
    float duty_f = (u >= 0.0f ? u : -u) * 0.5f * (float)PWM_TOP;
    uint32_t duty = (uint32_t)duty_f;

    if (u >= 0.0f) {
        TIM4_CCR1 = duty;
        TIM4_CCR2 = 0U;
    } else {
        TIM4_CCR1 = 0U;
        TIM4_CCR2 = duty;
    }
}


/* ================================================================
 *  main
 * ================================================================ */
int main(void) {
    enable_fpu();
    system_clock_hsi_64mhz();

    SYST_RVR = SYSTICK_RELOAD;
    SYST_CVR = 0U;
    SYST_CSR = 7U;

    gpio_init();
    tim3_encoder_init();
    tim4_pwm_init();

    controller_initialize();

    uint32_t next = systick_ms;


    for (;;) {

    	rtU.theta = read_pend_angle();
		controller_step();
		set_motor((float)rtY.control );


        /* Real LED toggle: on 250 ms, off 250 ms */
        {
            static uint32_t led_last  = 0;
            static uint8_t  led_state = 0;
            if ((systick_ms - led_last) >= 250U) {
                led_last = systick_ms;
                led_state = !led_state;
                if (led_state) GPIOB_BSRR = (1U << 0);    /* PB0 high */
                else           GPIOB_BSRR = (1U << 16);   /* PB0 low  */
            }
        }

        next += 1U;
        while ((int32_t)(systick_ms - next) < 0) { }
    }
}
