SRCS-kinetis=	adc.c crc.c flash.c flashconfig.c ftfl.c ftm.c gpio.c i2c.c onboard-led.c pin.c pin_change.c pit.c rtc.c spi.c stdio.c timeout.c uart.c usb.c
SRCS-kinetis.dir=	soc/kinetis
SRCS.libs+=	kinetis

include ${_libdir}/src/soc/cortex-m/soc.mk

include ${_libdir}/src/soc/${SOC}/${TARGET}.mk

TARGETLD?=	${_libdir}/src/soc/${SOC}/${TARGET}.ld
LDSCRIPTS+=	${TARGETLD}
