typedef enum n_adc_ref {
    N_ADC_AREF = 0x0,
    N_ADC_AVCC = 0x1,
    N_ADC_RESERVED = 0x2,
    N_ADC_INTERNAL = 0x3
} n_adc_ref_t;

typedef enum n_adc_pin {
    N_ADC0,
    N_ADC1,
    N_ADC2,
    N_ADC3,
    N_ADC4,
    N_ADC5,
    N_ADC6,
    N_ADC7
} n_adc_pin_t;

typedef enum n_usart_bits {
    N_USART_5BIT = 0x0,
    N_USART_6BIT = 0x1,
    N_USART_7BIT = 0x2,
    N_USART_8BIT = 0x3,
    N_USART_RESERVED = 0x4,
    N_USART_9BIT = 0x7
} n_usart_bits_t;

typedef enum n_usart_parity {
    N_USART_PARITY_NONE = 0x0,
    N_USART_PARITY_EVEN = 0x2,
    N_USART_PARITY_ODD  = 0x3
} n_usart_parity_t;

typedef enum n_usart_stopbit {
    N_USART_STOPBIT1 = 0x0,
    N_USART_STOPBIT2 = 0x1,
} n_usart_stopbit_t;

typedef enum n_delay_sleep_mode {
    N_DELAY_IDLE = SLEEP_MODE_IDLE,
    N_DELAY_ADC_NR = SLEEP_MODE_ADC,
    N_DELAY_POWER_DOWN = SLEEP_MODE_PWR_DOWN,
    N_DELAY_POWER_SAVE = SLEEP_MODE_PWR_SAVE,
    N_DELAY_STANDBY = SLEEP_MODE_STANDBY,
    N_DELAY_EXTENDED_STANDBY = SLEEP_MODE_EXT_STANDBY
} n_delay_sleep_mode_t;
