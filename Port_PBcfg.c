/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_PBcfg.c
 *
 * Description: Post Build Configuration Source file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Menna Sayed
 ******************************************************************************/

/*
 * Module Version 1.0.0
 */
#define PORT_PBCFG_SW_MAJOR_VERSION              (1U)
#define PORT_PBCFG_SW_MINOR_VERSION              (0U)
#define PORT_PBCFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_PBCFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_PBCFG_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_PBCFG_AR_RELEASE_PATCH_VERSION     (3U)

#include "Port.h"

/* AUTOSAR Version checking between Port_PBcfg.c and Port.h files */
#if ((PORT_PBCFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_PBCFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_PBCFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of PBcfg.c does not match the expected version"
#endif

/* Software Version checking between Port_PBcfg.c and Port.h files */
#if ((PORT_PBCFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
 ||  (PORT_PBCFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
 ||  (PORT_PBCFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
  #error "The SW version of PBcfg.c does not match the expected version"
#endif

/* PB structure used with Port_Init API ...led & button configurations also included*/
const Port_ConfigType Port_Configuration = {  
  /*1st channel structure*/
     PORTA_NUM,PA0_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*2st channel structure*/
     PORTA_NUM,PA1_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*3st channel structure*/
     PORTA_NUM,PA2_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*4st channel structure*/
    PORTA_NUM,PA3_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*5st channel structure*/
    PORTA_NUM,PA4_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*6st channel structure*/
    PORTA_NUM,PA5_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*7st channel structure*/
    PORTA_NUM,PA6_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*8st channel structure*/
    PORTA_NUM,PA7_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*9st channel structure*/
    PORTB_NUM,PB0_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*10st channel structure*/
    PORTB_NUM,PB1_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*11st channel structure*/
    PORTB_NUM,PB2_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*12st channel structure*/
    PORTB_NUM,PB3_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*13st channel structure*/
    PORTB_NUM,PB4_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*14st channel structure*/
    PORTB_NUM,PB5_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*15st channel structure*/
    PORTB_NUM,PB6_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*16st channel structure*/
    PORTB_NUM,PB7_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*17st channel structure*/
    PORTC_NUM,PC4_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*18st channel structure*/
    PORTC_NUM,PC5_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*19st channel structure*/
    PORTC_NUM,PC6_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*20st channel structure*/
    PORTC_NUM,PC7_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*21st channel structure*/
    PORTD_NUM,PD0_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*22st channel structure*/
    PORTD_NUM,PD1_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*23st channel structure*/
    PORTD_NUM,PD2_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*24st channel structure*/
    PORTD_NUM,PD3_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*25st channel structure*/
    PORTD_NUM,PD4_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*26st channel structure*/
    PORTD_NUM,PD5_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*27st channel structure*/
    PORTD_NUM,PD6_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*28st channel structure*/
    PORTD_NUM,PD7_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*29st channel structure*/
    PORTE_NUM,PE0_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*30st channel structure*/
    PORTE_NUM,PE1_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*31st channel structure*/
    PORTE_NUM,PE2_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*32st channel structure*/
    PORTE_NUM,PE3_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*33st channel structure*/
    PORTE_NUM,PE4_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*34st channel structure*/
    PORTE_NUM,PE5_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*35st channel structure*/
    PORTF_NUM,PF0_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*36st channel structure*/
    PORTF_NUM,PF1_PIN_ID_NUM,PORT_PIN_OUT,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*37st channel structure*/
    PORTF_NUM,PF2_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*38st channel structure*/
    PORTF_NUM,PF3_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_LOW,PullNone,STD_TRUE,STD_FALSE,
  /*39st channel structure*/
    PORTF_NUM,PF4_PIN_ID_NUM,PORT_PIN_IN,PORT_PIN_MODE_GPIO_DIO,STD_HIGH,PullUp,STD_TRUE,STD_FALSE 
  };

