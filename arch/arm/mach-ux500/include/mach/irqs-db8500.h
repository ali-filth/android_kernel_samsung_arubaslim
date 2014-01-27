/*
 * Copyright (C) ST-Ericsson SA 2010
 *
 * Author: Rabin Vincent <rabin.vincent@stericsson.com>
 * License terms: GNU General Public License (GPL) version 2
 */

#ifndef __MACH_IRQS_DB8500_H
#define __MACH_IRQS_DB8500_H

#define IRQ_DB8500_MTU0			(IRQ_SHPI_START + 4)
#define IRQ_DB8500_SPI2			(IRQ_SHPI_START + 6)
#define IRQ_DB8500_PMU			(IRQ_SHPI_START + 7)
#define IRQ_DB8500_SPI0			(IRQ_SHPI_START + 8)
#define IRQ_DB8500_RTT			(IRQ_SHPI_START + 9)
#define IRQ_DB8500_PKA			(IRQ_SHPI_START + 10)
#define IRQ_DB8500_UART0		(IRQ_SHPI_START + 11)
#define IRQ_DB8500_I2C3			(IRQ_SHPI_START + 12)
#define IRQ_DB8500_L2CC			(IRQ_SHPI_START + 13)
#define IRQ_DB8500_SSP0			(IRQ_SHPI_START + 14)
#define IRQ_DB8500_CRYP1		(IRQ_SHPI_START + 15)
#define IRQ_DB8500_MSP1_RX		(IRQ_SHPI_START + 16)
#define IRQ_DB8500_MTU1			(IRQ_SHPI_START + 17)
#define IRQ_DB8500_RTC			(IRQ_SHPI_START + 18)
#define IRQ_DB8500_UART1		(IRQ_SHPI_START + 19)
#define IRQ_DB8500_USB_WAKEUP		(IRQ_SHPI_START + 20)
#define IRQ_DB8500_I2C0			(IRQ_SHPI_START + 21)
#define IRQ_DB8500_I2C1			(IRQ_SHPI_START + 22)
#define IRQ_DB8500_USBOTG		(IRQ_SHPI_START + 23)
#define IRQ_DB8500_DMA_SECURE		(IRQ_SHPI_START + 24)
#define IRQ_DB8500_DMA			(IRQ_SHPI_START + 25)
#define IRQ_DB8500_UART2		(IRQ_SHPI_START + 26)
#define IRQ_DB8500_ICN_PMU1		(IRQ_SHPI_START + 27)
#define IRQ_DB8500_ICN_PMU2		(IRQ_SHPI_START + 28)
#define IRQ_DB8500_HSIR_EXCEP		(IRQ_SHPI_START + 29)
#define IRQ_DB8500_MSP0			(IRQ_SHPI_START + 31)
#define IRQ_DB8500_HSIR_CH0_OVRRUN	(IRQ_SHPI_START + 32)
#define IRQ_DB8500_HSIR_CH1_OVRRUN	(IRQ_SHPI_START + 33)
#define IRQ_DB8500_HSIR_CH2_OVRRUN	(IRQ_SHPI_START + 34)
#define IRQ_DB8500_HSIR_CH3_OVRRUN	(IRQ_SHPI_START + 35)
#define IRQ_DB8500_HSIR_CH4_OVRRUN	(IRQ_SHPI_START + 36)
#define IRQ_DB8500_HSIR_CH5_OVRRUN	(IRQ_SHPI_START + 37)
#define IRQ_DB8500_HSIR_CH6_OVRRUN	(IRQ_SHPI_START + 38)
#define IRQ_DB8500_HSIR_CH7_OVRRUN	(IRQ_SHPI_START + 39)
#define IRQ_DB8500_AB8500		(IRQ_SHPI_START + 40)
#define IRQ_DB8500_SDMMC2		(IRQ_SHPI_START + 41)
#define IRQ_DB8500_SIA			(IRQ_SHPI_START + 42)
#define IRQ_DB8500_SIA2			(IRQ_SHPI_START + 43)
#define IRQ_DB8500_SVA			(IRQ_SHPI_START + 44)
#define IRQ_DB8500_SVA2			(IRQ_SHPI_START + 45)
#define IRQ_DB8500_PRCMU0		(IRQ_SHPI_START + 46)
#define IRQ_DB8500_PRCMU1		(IRQ_SHPI_START + 47)
#define IRQ_DB8500_DISP			(IRQ_SHPI_START + 48)
#define IRQ_DB8500_SPI3			(IRQ_SHPI_START + 49)
#define IRQ_DB8500_SDMMC1		(IRQ_SHPI_START + 50)
#define IRQ_DB8500_I2C4			(IRQ_SHPI_START + 51)
#define IRQ_DB8500_SSP1			(IRQ_SHPI_START + 52)
#define IRQ_DB8500_SKE			(IRQ_SHPI_START + 53)
#define IRQ_DB8500_KB			(IRQ_SHPI_START + 54)
#define IRQ_DB8500_I2C2			(IRQ_SHPI_START + 55)
#define IRQ_DB8500_B2R2			(IRQ_SHPI_START + 56)
#define IRQ_DB8500_CRYP0		(IRQ_SHPI_START + 57)
#define IRQ_DB8500_SDMMC3		(IRQ_SHPI_START + 59)
#define IRQ_DB8500_SDMMC0		(IRQ_SHPI_START + 60)
#define IRQ_DB8500_HSEM			(IRQ_SHPI_START + 61)
#define IRQ_DB8500_MSP1			(IRQ_SHPI_START + 62)
#define IRQ_DB8500_SBAG			(IRQ_SHPI_START + 63)
#define IRQ_DB8500_SPI1			(IRQ_SHPI_START + 96)
#define IRQ_DB8500_SRPTIMER		(IRQ_SHPI_START + 97)
#define IRQ_DB8500_MSP2			(IRQ_SHPI_START + 98)
#define IRQ_DB8500_SDMMC4		(IRQ_SHPI_START + 99)
#define IRQ_DB8500_SDMMC5		(IRQ_SHPI_START + 100)
#define IRQ_DB8500_HSIRD0		(IRQ_SHPI_START + 104)
#define IRQ_DB8500_HSIRD1		(IRQ_SHPI_START + 105)
#define IRQ_DB8500_HSITD0		(IRQ_SHPI_START + 106)
#define IRQ_DB8500_HSITD1		(IRQ_SHPI_START + 107)
#define IRQ_DB8500_CTI0			(IRQ_SHPI_START + 108)
#define IRQ_DB8500_CTI1			(IRQ_SHPI_START + 109)
#define IRQ_DB8500_ICN_ERR		(IRQ_SHPI_START + 110)
#define IRQ_DB8500_MALI_PPMMU		(IRQ_SHPI_START + 112)
#define IRQ_DB8500_MALI_PP		(IRQ_SHPI_START + 113)
#define IRQ_DB8500_MALI_GPMMU		(IRQ_SHPI_START + 114)
#define IRQ_DB8500_MALI_GP		(IRQ_SHPI_START + 115)
#define IRQ_DB8500_MALI			(IRQ_SHPI_START + 116)
#define IRQ_DB8500_PRCMU_SEM		(IRQ_SHPI_START + 118)
#define IRQ_DB8500_GPIO0		(IRQ_SHPI_START + 119)
#define IRQ_DB8500_GPIO1		(IRQ_SHPI_START + 120)
#define IRQ_DB8500_GPIO2		(IRQ_SHPI_START + 121)
#define IRQ_DB8500_GPIO3		(IRQ_SHPI_START + 122)
#define IRQ_DB8500_GPIO4		(IRQ_SHPI_START + 123)
#define IRQ_DB8500_GPIO5		(IRQ_SHPI_START + 124)
#define IRQ_DB8500_GPIO6		(IRQ_SHPI_START + 125)
#define IRQ_DB8500_GPIO7		(IRQ_SHPI_START + 126)
#define IRQ_DB8500_GPIO8		(IRQ_SHPI_START + 127)

#define IRQ_CA_WAKE_REQ_ED			(IRQ_SHPI_START + 71)
#define IRQ_AC_READ_NOTIFICATION_0_ED		(IRQ_SHPI_START + 66)
#define IRQ_AC_READ_NOTIFICATION_1_ED		(IRQ_SHPI_START + 64)
#define IRQ_CA_MSG_PEND_NOTIFICATION_0_ED	(IRQ_SHPI_START + 67)
#define IRQ_CA_MSG_PEND_NOTIFICATION_1_ED	(IRQ_SHPI_START + 65)

#define IRQ_CA_WAKE_REQ_V1			(IRQ_SHPI_START + 83)
#define IRQ_AC_READ_NOTIFICATION_0_V1		(IRQ_SHPI_START + 78)
#define IRQ_AC_READ_NOTIFICATION_1_V1		(IRQ_SHPI_START + 76)
#define IRQ_CA_MSG_PEND_NOTIFICATION_0_V1	(IRQ_SHPI_START + 79)
#define IRQ_CA_MSG_PEND_NOTIFICATION_1_V1	(IRQ_SHPI_START + 77)

#ifdef CONFIG_UX500_SOC_DB8500

/* Virtual interrupts corresponding to the PRCMU wakeups.  */
#define IRQ_PRCMU_BASE IRQ_SOC_START
#define NUM_PRCMU_WAKEUPS (IRQ_PRCMU_END - IRQ_PRCMU_BASE)

#define IRQ_PRCMU_RTC (IRQ_PRCMU_BASE)
#define IRQ_PRCMU_RTT0 (IRQ_PRCMU_BASE + 1)
#define IRQ_PRCMU_RTT1 (IRQ_PRCMU_BASE + 2)
#define IRQ_PRCMU_HSI0 (IRQ_PRCMU_BASE + 3)
#define IRQ_PRCMU_HSI1 (IRQ_PRCMU_BASE + 4)
#define IRQ_PRCMU_CA_WAKE (IRQ_PRCMU_BASE + 5)
#define IRQ_PRCMU_USB (IRQ_PRCMU_BASE + 6)
#define IRQ_PRCMU_ABB (IRQ_PRCMU_BASE + 7)
#define IRQ_PRCMU_ABB_FIFO (IRQ_PRCMU_BASE + 8)
#define IRQ_PRCMU_ARM (IRQ_PRCMU_BASE + 9)
#define IRQ_PRCMU_MODEM_SW_RESET_REQ (IRQ_PRCMU_BASE + 10)
#define IRQ_PRCMU_GPIO0 (IRQ_PRCMU_BASE + 11)
#define IRQ_PRCMU_GPIO1 (IRQ_PRCMU_BASE + 12)
#define IRQ_PRCMU_GPIO2 (IRQ_PRCMU_BASE + 13)
#define IRQ_PRCMU_GPIO3 (IRQ_PRCMU_BASE + 14)
#define IRQ_PRCMU_GPIO4 (IRQ_PRCMU_BASE + 15)
#define IRQ_PRCMU_GPIO5 (IRQ_PRCMU_BASE + 16)
#define IRQ_PRCMU_GPIO6 (IRQ_PRCMU_BASE + 17)
#define IRQ_PRCMU_GPIO7 (IRQ_PRCMU_BASE + 18)
#define IRQ_PRCMU_GPIO8 (IRQ_PRCMU_BASE + 19)
#define IRQ_PRCMU_CA_SLEEP (IRQ_PRCMU_BASE + 20)
#define IRQ_PRCMU_HOTMON_LOW (IRQ_PRCMU_BASE + 21)
#define IRQ_PRCMU_HOTMON_HIGH (IRQ_PRCMU_BASE + 22)
#define IRQ_PRCMU_END (IRQ_PRCMU_BASE + 23)

/*
 * We may have several SoCs, but only one will run at a
 * time, so the one with most IRQs will bump this ahead,
 * but the IRQ_SOC_START remains the same for either SoC.
 */
#if IRQ_SOC_END < IRQ_PRCMU_END
#undef IRQ_SOC_END
#define IRQ_SOC_END IRQ_PRCMU_END
#endif

#endif /* CONFIG_UX500_SOC_DB8500 */
#endif
