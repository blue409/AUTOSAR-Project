 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Menna Sayed
 ******************************************************************************/

#ifndef PORT_H
#define PORT_H


/* Id for the company in the AUTOSAR */
#define PORT_VENDOR_ID    (1000U)

/* PORT Module Id */
#define PORT_MODULE_ID    (50U)

/* PORT Instance Id */
#define PORT_INSTANCE_ID  (0U)

/*
 * Module Version 1.0.0
 */
#define PORT_SW_MAJOR_VERSION           (1U)
#define PORT_SW_MINOR_VERSION           (0U)
#define PORT_SW_PATCH_VERSION           (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_AR_RELEASE_MAJOR_VERSION   (4U)
#define PORT_AR_RELEASE_MINOR_VERSION   (0U)
#define PORT_AR_RELEASE_PATCH_VERSION   (3U)

/*
 * Macros for PORT Status
 */
#define PORT_INITIALIZED                (1U)
#define PORT_NOT_INITIALIZED            (0U)

/* Standard AUTOSAR types */
#include "Std_Types.h"

/* AUTOSAR checking between Std Types and PORT Modules */
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Std_Types.h does not match the expected version"
#endif

 /* Port Pre-Compile Configuration Header file */
#include "Port_Cfg.h"

/* AUTOSAR Version checking between Port_Cfg.h and Port.h files */
#if ((PORT_CFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Port_Cfg.h does not match the expected version"
#endif

/* Software Version checking between Port_Cfg.h and Port.h files */
#if ((PORT_CFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
 ||  (PORT_CFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
 ||  (PORT_CFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
  #error "The SW version of Port_Cfg.h does not match the expected version"
#endif

/* Non AUTOSAR files */
#include "Common_Macros.h"  
    
 /******************************************************************************
 *                      API Service Id Macros                                 *
 ******************************************************************************/
/* Service ID for PORT Init Function */
#define PORT_INIT_SID                                     (uint8)0x00

/* Service ID for function Port_SetPinDirection() */
#define PORT_SET_PIN_DIRECTION_SID                        (uint8)0x01

/* Service ID for  function Port_RefreshPortDirection() */
#define PORT_REFRESH_PORT_DIRECTION_SID                   (uint8)0x02

/* Service ID for PORT GetVersionInfo */
#define PORT_GET_VERSION_INFO_SID                         (uint8)0x03

/* Service ID for function Port_SetPinMode() */                                                  
#define PORT_SET_PIN_Mode_SID                            (uint8)0x04
   
/*******************************************************************************
 *                      DET Error Codes                                        *
 *******************************************************************************/
/* DET code to report Invalid Invalid Port Pin ID 
requested  */
#define PORT_E_PARAM_PIN               (uint8)0x0A

/* DET code to report Port Pin not configured as changeable */
#define PORT_E_DIRECTION_UNCHANGEABLE  (uint8)0x0B
   
/* Port_Init API service called with wrong parameter */
#define PORT_E_PARAM_CONFIG            (uint8)0x0C   

/* API Port_SetPinMode service called when Port Pin Mode passed is not valid */
#define PORT_E_PARAM_INVALID_MODE      (uint8)0x0D  

/* API Port_SetPinMode service called when mode is unchangeable. */   
#define PORT_E_MODE_UNCHANGEABLE       (uint8)0x0E

/* The API service called without module initialization*/
#define PORT_E_UNINIT                  (uint8)0x0F

/* API service called with a Null Pointer */
#define PORT_E_PARAM_POINTER           (uint8)0x10   
   
/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/

/* Type definition for Data type for the symbolic name of a port pin used by the PORT APIs(pin index) */
typedef uint8 Port_PinType;

/* Type definition for Different port pin modes used by the PORT APIs  */
typedef uint8 Port_PinModeType;

/* Type definition for different ports used by the Port APIs (port index)*/
typedef uint8 Port_PortType;

/* Type definition for Port_LevelType used by the Port APIs (value to be assigned to pin....1 or 0)*/
typedef uint8 Port_LevelType;

/* Type definition for Port_ModeChangeble used by the Port APIs (STD_ON/STD_OFF)*/
typedef uint8 Port_ModeChangeble;

/* Type definition for Port_PinDirectionChangeble used by the Port APIs (STD_ON/STD_OFF)*/
typedef uint8 Port_PinDirectionChangeble;

/*optional Parameter to indicate if the input pin is attached to internal resistance or not*/
typedef enum
{
  
 PullUp,PullDown,PullNone

} Port_PinResistanceMode;

/*Parameter to indicate direction of a pin used by the PORT APIs */
typedef enum
{
  
  PORT_PIN_IN,PORT_PIN_OUT
 
} Port_PinDirectionType; 

/*Parameter Port pin mode from mode list for use with Port_Init() function.*/
typedef enum
 {
  PORT_PIN_MODE_GPIO_DIO,
  PORT_PIN_MODE_GPIO_AIO,  
  PORT_PIN_MODE_ADC,
  PORT_PIN_MODE_CAN,
  PORT_PIN_MODE_DIO_GPT,
  PORT_PIN_MODE_PWM,
  PORT_PIN_MODE_I2C,
  PORT_PIN_MODE_UART,
  PORT_PIN_MODE_QEI,  
  PORT_PIN_MODE_SPI    
 } Port_PinInitialMode;

/* Type for the external data structure containing the initialization data for the PORT Driver.*/
typedef struct
{
   /*1. Member contains the ID of the Port that this channel belongs to */
   Port_PortType Port_Index;
   /*2. Member contains the ID of the Pin that this channel belongs to */
   Port_PinType  Pin_Index;
   /*3. Member contains the direction of the Pin that this channel belongs to */
   Port_PinDirectionType Pin_Direction;
   /*4. Member contains the mode of the Pin that this channel belongs to */
   Port_PinModeType Pin_Mode;
   /*5. Member contains the value of the Pin that this channel belongs to */
   Port_LevelType Pin_Value;
   /*6. Member contains the resistance mode of the Pin that this channel belongs to */
   Port_PinResistanceMode Pin_Resistance;
   /*7. Member indicates if the direction of the Pin is changable or not during runtime*/
   Port_PinDirectionChangeble Pin_Changeable_Direction;
   /*8. Member indicates if the mode of the Pin is changable or not during runtime*/
   Port_ModeChangeble Pin_Changeable_Mode;
   
} Port_ConfigPin ;

/*structure to be passed to init functions contain array of pins we want to use*/
typedef struct
{
  Port_ConfigPin  Pins[PORT_CONFIGURED_PINS];
} Port_ConfigType;

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

/* Function for Port Initialization API */
void Port_Init( const Port_ConfigType* ConfigPtr ); 

#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
/*Function Sets the port pin direction*/
void Port_SetPinDirection( Port_PinType Pin, Port_PinDirectionType Direction ); 
#endif

/*Function Refreshes port pin direction. */
void Port_RefreshPortDirection( void ) ;

#if (PORT_VERSION_INFO_API == STD_ON )
/*Function Returns the version information of this module. */
void Port_GetVersionInfo(Std_VersionInfoType* versioninfo) ;
#endif

#if (PORT_SET_PIN_MODE_API == STD_ON )
/*Function Sets the port pin mode. */
void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode) ;
#endif

/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/

/* Extern PB structures to be used by Port and other modules */
extern const Port_ConfigType Port_Configuration;

#endif /* PORT_H */
   