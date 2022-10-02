 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_Cfg.h
 *
 * Description: Pre-Compile Configuration Header file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Menna Sayed
 ******************************************************************************/

#ifndef PORT_CFG_H
#define PORT_CFG_H

/*
 * Module Version 1.0.0
 */
#define PORT_CFG_SW_MAJOR_VERSION              (1U)
#define PORT_CFG_SW_MINOR_VERSION              (0U)
#define PORT_CFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_CFG_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_CFG_AR_RELEASE_PATCH_VERSION     (3U)


/* Pre-compile option for Development Error Detect */
#define PORT_DEV_ERROR_DETECT                (STD_OFF)

/* Pre-compile option for Version Info API */
#define PORT_VERSION_INFO_API                (STD_OFF)

/* Pre-compile option  to enable / disable the use of the function Port_SetPinDirection() */
#define PORT_SET_PIN_DIRECTION_API           (STD_ON)
   
/* Pre-compile option  to enable / disable the use of the function Port_SetPinMode() */   
#define PORT_SET_PIN_MODE_API                (STD_ON)
   

/* Number of the configured Port Channels */
#define PORT_CONFIGURED_PINS                 (39U)
   
/* Channel Index in the array of structures in Port_PBcfg.c */   
#define  PA0_PIN_ID_INDEX                    (uint8)0x00   
#define  PA1_PIN_ID_INDEX                    (uint8)0x01   
#define  PA2_PIN_ID_INDEX                    (uint8)0x02
#define  PA3_PIN_ID_INDEX                    (uint8)0x03
#define  PA4_PIN_ID_INDEX                    (uint8)0x04
#define  PA5_PIN_ID_INDEX                    (uint8)0x05
#define  PA6_PIN_ID_INDEX                    (uint8)0x06
#define  PA7_PIN_ID_INDEX                    (uint8)0x07

#define  PB0_PIN_ID_INDEX                    (uint8)0x08   
#define  PB1_PIN_ID_INDEX                    (uint8)0x09   
#define  PB2_PIN_ID_INDEX                    (uint8)0x0A
#define  PB3_PIN_ID_INDEX                    (uint8)0x0B
#define  PB4_PIN_ID_INDEX                    (uint8)0x0C
#define  PB5_PIN_ID_INDEX                    (uint8)0x0D
#define  PB6_PIN_ID_INDEX                    (uint8)0x0E
#define  PB7_PIN_ID_INDEX                    (uint8)0x0F

#define  PC4_PIN_ID_INDEX                    (uint8)0x10
#define  PC5_PIN_ID_INDEX                    (uint8)0x11
#define  PC6_PIN_ID_INDEX                    (uint8)0x12
#define  PC7_PIN_ID_INDEX                    (uint8)0x13

#define  PD0_PIN_ID_INDEX                    (uint8)0x14
#define  PD1_PIN_ID_INDEX                    (uint8)0x15
#define  PD2_PIN_ID_INDEX                    (uint8)0x16
#define  PD3_PIN_ID_INDEX                    (uint8)0x17
#define  PD4_PIN_ID_INDEX                    (uint8)0x18   
#define  PD5_PIN_ID_INDEX                    (uint8)0x19   
#define  PD6_PIN_ID_INDEX                    (uint8)0x1A
#define  PD7_PIN_ID_INDEX                    (uint8)0x1B

#define  PE0_PIN_ID_INDEX                    (uint8)0x1C
#define  PE1_PIN_ID_INDEX                    (uint8)0x1D
#define  PE2_PIN_ID_INDEX                    (uint8)0x1E
#define  PE3_PIN_ID_INDEX                    (uint8)0x1F
#define  PE4_PIN_ID_INDEX                    (uint8)0x20
#define  PE5_PIN_ID_INDEX                    (uint8)0x21

#define  PF0_PIN_ID_INDEX                    (uint8)0x22
#define  PF1_PIN_ID_INDEX                    (uint8)0x23
#define  PF2_PIN_ID_INDEX                    (uint8)0x24
#define  PF3_PIN_ID_INDEX                    (uint8)0x25
#define  PF4_PIN_ID_INDEX                    (uint8)0x26

/* PORT Configured Port ID's  */
#define PORTA_NUM                            (Port_PortType)0x00 /* PORTA */
#define PORTB_NUM                            (Port_PortType)0x01 /* PORTB */
#define PORTC_NUM                            (Port_PortType)0x02 /* PORTC */
#define PORTD_NUM                            (Port_PortType)0x03 /* PORTD */
#define PORTE_NUM                            (Port_PortType)0x04 /* PORTE */
#define PORTF_NUM                            (Port_PortType)0x05 /* PORTF */

/* Port Configured PIN ID's */
#define  PA0_PIN_ID_NUM                      (Port_PinType)0 /* Pin 0 in PORTA */
#define  PA1_PIN_ID_NUM                      (Port_PinType)1 /* Pin 1 in PORTA */
#define  PA2_PIN_ID_NUM                      (Port_PinType)2 /* Pin 2 in PORTA */
#define  PA3_PIN_ID_NUM                      (Port_PinType)3 /* Pin 3 in PORTA */
#define  PA4_PIN_ID_NUM                      (Port_PinType)4 /* Pin 4 in PORTA */
#define  PA5_PIN_ID_NUM                      (Port_PinType)5 /* Pin 5 in PORTA */
#define  PA6_PIN_ID_NUM                      (Port_PinType)6 /* Pin 6 in PORTA */
#define  PA7_PIN_ID_NUM                      (Port_PinType)7 /* Pin 7 in PORTA */

#define  PB0_PIN_ID_NUM                      (Port_PinType)0 /* Pin 0 in PORTB */
#define  PB1_PIN_ID_NUM                      (Port_PinType)1 /* Pin 1 in PORTB */
#define  PB2_PIN_ID_NUM                      (Port_PinType)2 /* Pin 2 in PORTB */
#define  PB3_PIN_ID_NUM                      (Port_PinType)3 /* Pin 3 in PORTB */
#define  PB4_PIN_ID_NUM                      (Port_PinType)4 /* Pin 4 in PORTB */
#define  PB5_PIN_ID_NUM                      (Port_PinType)5 /* Pin 5 in PORTB */
#define  PB6_PIN_ID_NUM                      (Port_PinType)6 /* Pin 6 in PORTB */
#define  PB7_PIN_ID_NUM                      (Port_PinType)7 /* Pin 7 in PORTB */

#define  PC4_PIN_ID_NUM                      (Port_PinType)4 /* Pin 4 in PORTC */
#define  PC5_PIN_ID_NUM                      (Port_PinType)5 /* Pin 5 in PORTC */
#define  PC6_PIN_ID_NUM                      (Port_PinType)6 /* Pin 6 in PORTC */
#define  PC7_PIN_ID_NUM                      (Port_PinType)7 /* Pin 7 in PORTC */

#define  PD0_PIN_ID_NUM                      (Port_PinType)0 /* Pin 0 in PORTD */
#define  PD1_PIN_ID_NUM                      (Port_PinType)1 /* Pin 1 in PORTD */
#define  PD2_PIN_ID_NUM                      (Port_PinType)2 /* Pin 2 in PORTD */
#define  PD3_PIN_ID_NUM                      (Port_PinType)3 /* Pin 3 in PORTD */
#define  PD4_PIN_ID_NUM                      (Port_PinType)4 /* Pin 4 in PORTD */
#define  PD5_PIN_ID_NUM                      (Port_PinType)5 /* Pin 5 in PORTD */
#define  PD6_PIN_ID_NUM                      (Port_PinType)6 /* Pin 6 in PORTD */
#define  PD7_PIN_ID_NUM                      (Port_PinType)7 /* Pin 7 in PORTD */

#define  PE0_PIN_ID_NUM                      (Port_PinType)0 /* Pin 0 in PORTE */
#define  PE1_PIN_ID_NUM                      (Port_PinType)1 /* Pin 1 in PORTE */
#define  PE2_PIN_ID_NUM                      (Port_PinType)2 /* Pin 2 in PORTE */
#define  PE3_PIN_ID_NUM                      (Port_PinType)3 /* Pin 3 in PORTE */
#define  PE4_PIN_ID_NUM                      (Port_PinType)4 /* Pin 4 in PORTE */
#define  PE5_PIN_ID_NUM                      (Port_PinType)5 /* Pin 5 in PORTE */

#define  PF0_PIN_ID_NUM                      (Port_PinType)0 /* Pin 0 in PORTF */
#define  PF1_PIN_ID_NUM                      (Port_PinType)1 /* Pin 1 in PORTF */
#define  PF2_PIN_ID_NUM                      (Port_PinType)2 /* Pin 2 in PORTF */
#define  PF3_PIN_ID_NUM                      (Port_PinType)3 /* Pin 3 in PORTF */
#define  PF4_PIN_ID_NUM                      (Port_PinType)4 /* Pin 4 in PORTF */


#endif /* PORT_CFG_H */

