#include <mchck.h>

PORT_MemMapPtr
pin_physport_from_pin(enum pin_id pin)
{
        switch (pin_port_from_pin(pin)) {
        case PIN_PORTA:
                return (PORTA_BASE_PTR);
        case PIN_PORTB:
                return (PORTB_BASE_PTR);
        case PIN_PORTC:
                return (PORTC_BASE_PTR);
        case PIN_PORTD:
                return (PORTD_BASE_PTR);
        case PIN_PORTE:
                return (PORTE_BASE_PTR);
        default:
                return (NULL);
        }
}

void
pin_mode(enum pin_id pin, enum pin_mode mode)
{
        int pinnum = pin_physpin_from_pin(pin);

        /* enable port clock */
        SIM_SCGC5 |= 1 << (pin_portnum_from_pin(pin) + 8);

        uint32_t pcr = PORT_PCR_REG(pin_physport_from_pin(pin), pinnum);

        if (mode & PIN_MODE_RESET) {
                pcr &= ~0xff;
                bf_set(pcr, PORT_PCR_MUX, 0);
        }

        if (mode & PIN_MODE__SLEW) {
                switch (mode & PIN_MODE__SLEW_MASK) {
                case PIN_MODE_SLEW_FAST:
                        bf_set(pcr, PORT_PCR_SRE, 0);
                        break;
                case PIN_MODE_SLEW_SLOW:
                        bf_set(pcr, PORT_PCR_SRE, 1);
                        break;
                }
        }

        if (mode & PIN_MODE__PULL) {
                switch (mode & PIN_MODE__PULL_MASK) {
                case PIN_MODE_PULL_OFF:
                        bf_set(pcr, PORT_PCR_PE, 0);
                        break;
                case PIN_MODE_PULLDOWN:
                        bf_set(pcr, PORT_PCR_PE, 1);
                        bf_set(pcr, PORT_PCR_PS, 0);
                        break;
                case PIN_MODE_PULLUP:
                        bf_set(pcr, PORT_PCR_PE, 1);
                        bf_set(pcr, PORT_PCR_PS, 1);
                        break;
                }
        }

        if (mode & PIN_MODE__DRIVE) {
                switch (mode & PIN_MODE__DRIVE_MASK) {
                case PIN_MODE_DRIVE_LOW:
                        bf_set(pcr, PORT_PCR_DSE, 0);
                        break;
                case PIN_MODE_DRIVE_HIGH:
                        bf_set(pcr, PORT_PCR_DSE, 1);
                        break;
                }
        }

        if (mode & PIN_MODE__FILTER) {
                switch (mode & PIN_MODE__FILTER_MASK) {
                case PIN_MODE_FILTER_OFF:
                        bf_set(pcr, PORT_PCR_PFE, 0);
                        break;
                case PIN_MODE_FILTER_ON:
                        bf_set(pcr, PORT_PCR_PFE, 1);
                        break;
                }
        }

        if (mode & PIN_MODE__OPEN_DRAIN) {
#ifdef PORT_PCR_ODE_MASK
                switch (mode & PIN_MODE__OPEN_DRAIN_MASK) {
                case PIN_MODE_OPEN_DRAIN_OFF:
                        bf_set(pcr, PORT_PCR_ODE, 0);
                        break;
                case PIN_MODE_OPEN_DRAIN_ON:
                        bf_set(pcr, PORT_PCR_ODE, 1);
                        break;
                }
#else
                panic("no open drain mode available");
#endif
        }

        if (mode & PIN_MODE__MUX) {
                bf_set(pcr, PORT_PCR_MUX, (mode & PIN_MODE__MUX_MASK) >> 13);
        }

        PORT_PCR_REG(pin_physport_from_pin(pin), pinnum) = pcr;
}
