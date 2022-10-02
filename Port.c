 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.c
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Menna Sayed
 ******************************************************************************/
#include "Port.h"
#include "PORT_Regs.h"

#if (PORT_DEV_ERROR_DETECT == STD_ON)

#include "Port_Det.h"
/* AUTOSAR Version checking between Det and Port Modules */
#if ((DET_AR_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)
 || (DET_AR_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)
 || (DET_AR_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Det.h does not match the expected version"
#endif

#endif

STATIC const Port_ConfigPin * Port_ChannelConf = NULL_PTR;
STATIC uint8 Port_Status = PORT_NOT_INITIALIZED;
/************************************************************************************
* Service Name: Port_Init
* Service ID[hex]: 0x00
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): ConfigPtr - Pointer to post-build configuration data
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Initialize the Port module.
************************************************************************************/
void Port_Init( const Port_ConfigType* ConfigPtr )
{
   volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
   volatile uint32 delay = 0;   
    
  /*
   * Set the module state to initialized and point to the PB configuration structure using a global pointer.
   * This global pointer is global to be used by other functions to read the PB configuration structures */
   
   #if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* check if the input configuration pointer is not a NULL_PTR */
	if (NULL_PTR == ConfigPtr)
	{
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID,
		     PORT_E_PARAM_CONFIG);
                return;
	}
   #endif     
   
   Port_Status = PORT_INITIALIZED;
   Port_ChannelConf = ConfigPtr->Pins; /* address of the first Channels structure --> Pins[0] */
 
  for(uint8 i=0;i<PORT_CONFIGURED_PINS;i++)   /*To loop on the no of configured pins Only*/
  {
    switch(Port_ChannelConf[i].Port_Index)
    {
    case 0:   PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
      break;
    case 1:   PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */  
      break;
    case 2:   PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
      break;
    case 3:   PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
      break;
    case 4:   PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
      break;
    case 5:   PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
      break;      
    }
    /* Enable clock for PORT and allow time for clock to start*/
    SYSCTL_REGCGC2_REG |= (1<<Port_ChannelConf[i].Port_Index);
    delay = SYSCTL_REGCGC2_REG;
    
    if( ((Port_ChannelConf[i].Port_Index == 3) && (Port_ChannelConf[i].Pin_Index == 7)) || ((Port_ChannelConf[i].Port_Index== 5) && (Port_ChannelConf[i].Pin_Index == 0)) ) /* PD7 or PF0 */
    {
        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_LOCK_REG_OFFSET) = 0x4C4F434B;                     /* Unlock the GPIOCR register */   
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_COMMIT_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);  /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
    }
    else
    {
       /* Do Nothing ... No need to unlock the commit register for this pin */
    }
    
    switch(Port_ChannelConf[i].Pin_Mode)
    { 
    case PORT_PIN_MODE_GPIO_DIO:     /*case of using the pin as digital Gpio mode*/
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_ChannelConf[i].Pin_Index); 
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,Port_ChannelConf[i].Pin_Index);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
       *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_ChannelConf[i].Pin_Index * 4));     /* Clear the PMCx bits for this pin */
       break;
       
    case PORT_PIN_MODE_ADC:     /*case of using the pin for ADC mode*/  
     
    case PORT_PIN_MODE_GPIO_AIO:     /*case of using the pin as Analog I/O mode...IF the pin is suitable to be set as Analog*/
      
      if((Port_ChannelConf[i].Pin_Index == PB4_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index == PB5_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index == PD0_PIN_ID_NUM)
        ||(Port_ChannelConf[i].Pin_Index == PD1_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index == PD2_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index == PD3_PIN_ID_NUM)
        ||(Port_ChannelConf[i].Pin_Index == PE0_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index == PE1_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index == PE2_PIN_ID_NUM)
        ||(Port_ChannelConf[i].Pin_Index == PE3_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index == PE4_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index == PE5_PIN_ID_NUM))
      { 
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_ChannelConf[i].Pin_Index); 
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,Port_ChannelConf[i].Pin_Index);      /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);          /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register to choose GPIO */
       *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_ChannelConf[i].Pin_Index * 4));   /* Clear the PMCx bits for this pin to choose GPIO*/
      }
     else
      {
 
          return;   /*return without any action*/
      } 
      
       break;
   
    case PORT_PIN_MODE_CAN:     /*case of using the pin for CAN mode...if it is supported*/
      
     if ((Port_ChannelConf[i].Pin_Index == PA0_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index == PA1_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index == PB4_PIN_ID_NUM)
       ||(Port_ChannelConf[i].Pin_Index == PB5_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index == PE5_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index == PE4_PIN_ID_NUM)
       ||(Port_ChannelConf[i].Pin_Index == PF0_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index == PF3_PIN_ID_NUM))
      {
        if((Port_ChannelConf[i].Pin_Index == PF0_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PF3_PIN_ID_NUM))
        {
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);         
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,Port_ChannelConf[i].Pin_Index);    /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register to choose GPIO */
       *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x0000003F << (Port_ChannelConf[i].Pin_Index * 4));    /* SET the PMCx bits for this pin to choose GPIO*/

        }
        else{
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);         
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,Port_ChannelConf[i].Pin_Index);    /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register to choose GPIO */
       *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x0000008F << (Port_ChannelConf[i].Pin_Index * 4));    /* SET the PMCx bits for this pin to choose GPIO*/
            }
       }  
     else
      {
  
          return;   /*return without any action*/
      }         
       break; 
     
     case PORT_PIN_MODE_DIO_GPT:     /*case of using the pin for GPT mode...if it is supported on the pin*/ 
       
      if((Port_ChannelConf[i].Pin_Index == PB0_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PB1_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PB2_PIN_ID_NUM)
       ||(Port_ChannelConf[i].Pin_Index == PB3_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PB4_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PB5_PIN_ID_NUM)
       ||(Port_ChannelConf[i].Pin_Index == PB6_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PB7_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PC4_PIN_ID_NUM)
       ||(Port_ChannelConf[i].Pin_Index == PC5_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PC6_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PC7_PIN_ID_NUM)
       ||(Port_ChannelConf[i].Pin_Index == PD0_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PD1_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PD2_PIN_ID_NUM)
       ||(Port_ChannelConf[i].Pin_Index == PD3_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PD4_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PD5_PIN_ID_NUM)
       ||(Port_ChannelConf[i].Pin_Index == PD6_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PD7_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PF0_PIN_ID_NUM)
       ||(Port_ChannelConf[i].Pin_Index == PF1_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PF2_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PF3_PIN_ID_NUM) 
       ||(Port_ChannelConf[i].Pin_Index == PF4_PIN_ID_NUM))   
       {      
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);         
       CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,Port_ChannelConf[i].Pin_Index);    /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register to choose GPIO */
       *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x0000007F << (Port_ChannelConf[i].Pin_Index * 4));    /* SET the PMCx bits for this pin to choose GPIO*/
       }       
      else
      {
    
          return;   /*return without any action*/
      } 
       break;
    case PORT_PIN_MODE_PWM:     /*case of using the pin for PWM mode...if it is supported on the pin*/ 
      
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);         
       CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,Port_ChannelConf[i].Pin_Index);    /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register to choose GPIO */    
      if((Port_ChannelConf[i].Pin_Index == PB4_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PB5_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PB6_PIN_ID_NUM)
       ||(Port_ChannelConf[i].Pin_Index == PB7_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PC4_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PC5_PIN_ID_NUM)
       ||(Port_ChannelConf[i].Pin_Index == PD0_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PD1_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PE4_PIN_ID_NUM)
       ||(Port_ChannelConf[i].Pin_Index ==PE5_PIN_ID_NUM))
      {
       *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x0000004F << (Port_ChannelConf[i].Pin_Index * 4));    /* SET the PMCx bits for this pin to choose GPIO*/
      }
      else if((Port_ChannelConf[i].Pin_Index == PA6_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PA7_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PF0_PIN_ID_NUM)
       ||(Port_ChannelConf[i].Pin_Index == PF1_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PF2_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PF3_PIN_ID_NUM)
       ||(Port_ChannelConf[i].Pin_Index == PF4_PIN_ID_NUM))
      {
       *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x0000005F << (Port_ChannelConf[i].Pin_Index * 4));    /* SET the PMCx bits for this pin to choose GPIO*/ 
      }
      else
      {

          return;   /*return without any action*/
      } 
      break;
    case PORT_PIN_MODE_UART:     /*case of using the pin for UART mode...if it is supported on the pin*/
      
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);         
       CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,Port_ChannelConf[i].Pin_Index);    /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register to choose GPIO */    
       if ((Port_ChannelConf[i].Pin_Index == PD0_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PD1_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PD2_PIN_ID_NUM)
       ||(Port_ChannelConf[i].Pin_Index == PD3_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PF0_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PF1_PIN_ID_NUM))
       {
 
          return;   /*return without any action*/
       }
       else if ((Port_ChannelConf[i].Pin_Index == PC4_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PC5_PIN_ID_NUM))
       {
        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x0000002F << (Port_ChannelConf[i].Pin_Index * 4));    /* SET the PMCx bits for this pin to choose GPIO*/ 
       }
       else
       {
        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x0000001F << (Port_ChannelConf[i].Pin_Index * 4));    /* SET the PMCx bits for this pin to choose GPIO*/ 
       }     
       break;
     case PORT_PIN_MODE_SPI:     /*case of using the pin for SPI mode...if it is supported on the pin*/     
       if((Port_ChannelConf[i].Pin_Index == PD0_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PD1_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PD2_PIN_ID_NUM)
       ||(Port_ChannelConf[i].Pin_Index == PD3_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PC4_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PC5_PIN_ID_NUM))
       {
        if((Port_ChannelConf[i].Pin_Index ==PC4_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PC5_PIN_ID_NUM))
         {
 
             return;   /*return without any action*/
         }
         /*Do nothing PD{0:3]are by default configured. */
       }
       else
       {
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);         
       CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,Port_ChannelConf[i].Pin_Index);    /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register to choose GPIO */    
       *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x0000002F << (Port_ChannelConf[i].Pin_Index * 4));    /* SET the PMCx bits for this pin to choose GPIO*/ 
       }
       break;
     case PORT_PIN_MODE_QEI:     /*case of using the pin for QEI mode...if it is supported on the pin*/     
       
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);         
       CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,Port_ChannelConf[i].Pin_Index);    /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register to choose GPIO */    
       *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x0000006F << (Port_ChannelConf[i].Pin_Index * 4));    /* SET the PMCx bits for this pin to choose GPIO*/ 
       break;
     case PORT_PIN_MODE_I2C:    /*case of using the pin for I2C mode...if it is supported on the pin*/ 
       if((Port_ChannelConf[i].Pin_Index == PF0_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PF3_PIN_ID_NUM))
       {
   
          return;   /*return without any action*/
       }
       else
       {
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);         
       CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,Port_ChannelConf[i].Pin_Index);    /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register to choose GPIO */    
       *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x0000003F << (Port_ChannelConf[i].Pin_Index * 4));    /* SET the PMCx bits for this pin to choose GPIO*/ 
       }       
       break;
       
    }
    
    if(Port_ChannelConf[i].Pin_Direction == PORT_PIN_IN)
    {
     CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) ,(Port_ChannelConf[i].Pin_Index));               /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
     
        if(Port_ChannelConf[i].Pin_Resistance == PullUp)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);       /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
        }
        else if(Port_ChannelConf[i].Pin_Resistance == PullDown)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);     /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
        }
        else
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);     /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);   /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
        }
    }
    else
    {
     SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) ,(Port_ChannelConf[i].Pin_Index));                 /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
   
       if(Port_ChannelConf[i].Pin_Value == STD_HIGH)
        {
           SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) ,(Port_ChannelConf[i].Pin_Index));  
        }
       else
       {
          CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) ,(Port_ChannelConf[i].Pin_Index));
       }
        
    }   
    
  }
}
/************************************************************************************
* Service Name: Port_GetVersionInfo
* Service ID[hex]: 0x03
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): VersionInfo - Pointer to where to store the version information of this module.
* Return value: None
* Description: Function to get the version information of this module.
************************************************************************************/
#if (PORT_VERSION_INFO_API == STD_ON)
void Port_GetVersionInfo(Std_VersionInfoType *versioninfo)    
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
        
       if(Port_Status == PORT_INITIALIZED){   
            		/* Report to DET  */
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_GET_VERSION_INFO_SID,PORT_E_UNINIT);
                return;
           }   
	/* Check if input pointer is not Null pointer */
	else if(NULL_PTR == versioninfo)
	{
		/* Report to DET  */
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_POINTER);
                return;
	}
	else
#endif /* (DIO_DEV_ERROR_DETECT == STD_ON) */
	{
		/* Copy the vendor Id */
		versioninfo->vendorID = (uint16)PORT_VENDOR_ID;
		/* Copy the module Id */
		versioninfo->moduleID = (uint16)PORT_MODULE_ID;
		/* Copy Software Major Version */
		versioninfo->sw_major_version = (uint8)PORT_SW_MAJOR_VERSION;
		/* Copy Software Minor Version */
		versioninfo->sw_minor_version = (uint8)PORT_SW_MINOR_VERSION;
		/* Copy Software Patch Version */
		versioninfo->sw_patch_version = (uint8)PORT_SW_PATCH_VERSION;
	}
}
#endif
/************************************************************************************
* Service Name:Port_SetPinDirection
* Service ID[hex]: 0x01
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): Pin ID-Direction
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Sets the port pin direction.
************************************************************************************/
#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
/*Function Sets the port pin direction*/
void Port_SetPinDirection( Port_PinType Pin, Port_PinDirectionType Direction )
{
    volatile uint32 * PortGpio_Ptr = NULL_PTR;  /* point to the required Port Registers base address */
   if(Port_Status == PORT_NOT_INITIALIZED)
   {
     #if (PORT_DEV_ERROR_DETECT == STD_ON)
            		/* Report to DET  */
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_GET_VERSION_INFO_SID,PORT_E_UNINIT);
     #endif 
     return;         /*return without any action*/           
   }
   
   for(uint8 i=0;i<PORT_CONFIGURED_PINS;i++)   /*To loop on the no of configured pins Only*/
   {
      if(Port_ChannelConf[i].Pin_Index == Pin)
      {
        if(Port_ChannelConf[i].Pin_Changeable_Direction == STD_FALSE) 
        {
          #if (PORT_DEV_ERROR_DETECT == STD_ON)		/* Report to DET  */
               Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_GET_VERSION_INFO_SID, PORT_E_DIRECTION_UNCHANGEABLE);
          #endif   
          return;   /*return without any action*/
        }
        else
        {
          switch(Port_ChannelConf[i].Port_Index)
           {
    case 0:   PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
      break;
    case 1:   PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */  
      break;
    case 2:   PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
      break;
    case 3:   PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
      break;
    case 4:   PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
      break;
    case 5:   PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
      break;      
           }
 
        }
     
      }
      else if (Pin > 7) /*if wrong pin ID is given to the function */
      {
        #if (PORT_DEV_ERROR_DETECT == STD_ON)
            		/* Report to DET  */
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_GET_VERSION_INFO_SID,PORT_E_PARAM_PIN);
        #endif 
        return;         /*return without any action*/           
      }
      else if (((Port_ChannelConf[i].Port_Index ==4)&&(Pin >5))||(Port_ChannelConf[i].Port_Index==5)&&(Pin >4))  /*if wrong pin ID is entered...special cond for port E & F */
      {                                                           
                #if (PORT_DEV_ERROR_DETECT == STD_ON)
            		/* Report to DET  */
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_GET_VERSION_INFO_SID,PORT_E_PARAM_PIN);
        #endif 
        return;         /*return without any action*/ 
      }
     
    }  
   if(Direction == PORT_PIN_IN)
       {
           CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET),Pin);         /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */       
       }
    else
      {
          SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) ,Pin);           /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
      }
}
#endif

/************************************************************************************
* Service Name:Port_RefreshPortDirection
* Service ID[hex]: 0x02
* Sync/Async: Synchronous
* Reentrancy: Non-reentrant
* Parameters (in): None 
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Refreshes port Pin direction
************************************************************************************/
void Port_RefreshPortDirection( void )
{
  volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
  if(Port_Status == PORT_INITIALIZED){
    
    if(Port_ChannelConf == NULL_PTR)
    {
             /*Do nothing*/
    }
    else
    {           
       for(uint8 i=0;i<PORT_CONFIGURED_PINS;i++)   /*To loop on the no of configured pins Only*/
           {
             if(Port_ChannelConf[i].Pin_Changeable_Direction == STD_FALSE)
             {
               switch(Port_ChannelConf[i].Port_Index)
              {
             case 0:   PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
               break;
             case 1:   PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */  
               break;
             case 2:   PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
               break;
             case 3:   PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
               break;
             case 4:   PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
               break;
             case 5:   PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
               break;          
               }          
             if(Port_ChannelConf[i].Pin_Direction == PORT_PIN_IN)
               {
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) ,(Port_ChannelConf[i].Pin_Index));               /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
               }
             else
               {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) ,(Port_ChannelConf[i].Pin_Index));               /* SET the corresponding bit in the GPIODIR register to configure it as output pin */
               }
             }
             else     /*Port_ChannelConf[i].Pin_Changeable_Direction == STD_FALSE*/
             {
                   /*Do nothing*/
             }
           }
    }
  }
  else   /*Port_Status == PORT_INITIALIZED*/
  {
    #if (DIO_DEV_ERROR_DETECT == STD_ON)
            		/* Report to DET  */
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_GET_VERSION_INFO_SID,PORT_E_UNINIT);
    #endif           
  }
}
/************************************************************************************
* Service Name:Port_SetPinMode
* Service ID[hex]: 0x04
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): Port Pin ID number , New Port Pin mode to be set on port pin. 
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Sets the port pin mode
************************************************************************************/
#if (PORT_SET_PIN_MODE_API == STD_ON )
/*Function Sets the port pin mode. */
void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode) 
{
  volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
  if(Port_Status == PORT_NOT_INITIALIZED)
      {
             #if (PORT_DEV_ERROR_DETECT == STD_ON)		/* Report to DET  */
               Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_GET_VERSION_INFO_SID, PORT_E_UNINIT);
             #endif   
             return;   /*return without any action*/
      }
  
  for(uint8 i=0;i<PORT_CONFIGURED_PINS;i++)   /*To loop on the no of configured pins Only*/
    {
      if(Port_ChannelConf[i].Pin_Index == Pin)
      {
        if(Port_ChannelConf[i].Pin_Changeable_Mode == STD_FALSE) 
        {
          #if (PORT_DEV_ERROR_DETECT == STD_ON)		/* Report to DET  */
               Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_GET_VERSION_INFO_SID, PORT_E_MODE_UNCHANGEABLE);
          #endif   
          return;   /*return without any action*/
        }
        else     /*Port_ChannelConf[i].Pin_Changeable_Mode == STD_FALSE*/
       {
        switch(Port_ChannelConf[i].Port_Index)
         {
    case 0:   PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
      break;
    case 1:   PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */  
      break;
    case 2:   PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
      break;
    case 3:   PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
      break;
    case 4:   PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
      break;
    case 5:   PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
      break;      
         }         
        switch(Mode)
            {
    case PORT_PIN_MODE_GPIO_DIO:     /*case of using the pin as digital Gpio mode*/
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_ChannelConf[i].Pin_Index); 
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,Port_ChannelConf[i].Pin_Index);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
       *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_ChannelConf[i].Pin_Index * 4));     /* Clear the PMCx bits for this pin */
       break;
       
    case PORT_PIN_MODE_ADC:     /*case of using the pin for ADC mode*/   
    case PORT_PIN_MODE_GPIO_AIO:     /*case of using the pin as Analog I/O mode...IF the pin is suitable to be set as Analog*/
      if((Port_ChannelConf[i].Pin_Index == PB4_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PB5_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PD0_PIN_ID_NUM)
        ||(Port_ChannelConf[i].Pin_Index == PD1_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index == PD2_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PD3_PIN_ID_NUM)
        ||(Port_ChannelConf[i].Pin_Index ==PE0_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PE1_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PE2_PIN_ID_NUM)
        ||(Port_ChannelConf[i].Pin_Index ==PE3_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PE4_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PE5_PIN_ID_NUM))
      { 
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_ChannelConf[i].Pin_Index); 
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,Port_ChannelConf[i].Pin_Index);      /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);          /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register to choose GPIO */
       *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_ChannelConf[i].Pin_Index * 4));   /* Clear the PMCx bits for this pin to choose GPIO*/
      }
     else
      {
          #if (PORT_DEV_ERROR_DETECT == STD_ON)		/* Report to DET  */
               Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_INVALID_MODE);
          #endif   
          return;   /*return without any action*/
      }     
       break;
   
    case PORT_PIN_MODE_CAN:     /*case of using the pin for CAN mode...if it is supported*/
      
     if((Port_ChannelConf[i].Pin_Index == PA0_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PA1_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PB4_PIN_ID_NUM)\
       ||(Port_ChannelConf[i].Pin_Index == PB5_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PE5_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PE4_PIN_ID_NUM)\
       ||(Port_ChannelConf[i].Pin_Index == PF0_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PF3_PIN_ID_NUM))
      {
        if((Port_ChannelConf[i].Pin_Index == PF0_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PF3_PIN_ID_NUM))
        {
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);         
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,Port_ChannelConf[i].Pin_Index);    /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register to choose GPIO */
       *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x0000003F << (Port_ChannelConf[i].Pin_Index * 4));    /* SET the PMCx bits for this pin to choose GPIO*/

        }
        else{
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);         
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,Port_ChannelConf[i].Pin_Index);    /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register to choose GPIO */
       *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x0000008F << (Port_ChannelConf[i].Pin_Index * 4));    /* SET the PMCx bits for this pin to choose GPIO*/
            }
       }  
     else
      {
          #if (PORT_DEV_ERROR_DETECT == STD_ON)		/* Report to DET  */
               Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_INVALID_MODE);
          #endif   
          return;   /*return without any action*/
      }         
       break; 
     
     case PORT_PIN_MODE_DIO_GPT:     /*case of using the pin for GPT mode...if it is supported on the pin*/ 
       
      if((Port_ChannelConf[i].Pin_Index == PB0_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PB1_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PB2_PIN_ID_NUM)
       ||(Port_ChannelConf[i].Pin_Index == PB3_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PB4_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PB5_PIN_ID_NUM)
       ||(Port_ChannelConf[i].Pin_Index == PB6_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PB7_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PC4_PIN_ID_NUM)
       ||(Port_ChannelConf[i].Pin_Index == PC5_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PC6_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PC7_PIN_ID_NUM)
       ||(Port_ChannelConf[i].Pin_Index == PD0_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PD1_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PD2_PIN_ID_NUM)
       ||(Port_ChannelConf[i].Pin_Index == PD3_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PD4_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PD5_PIN_ID_NUM)
       ||(Port_ChannelConf[i].Pin_Index == PD6_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PD7_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PF0_PIN_ID_NUM)
       ||(Port_ChannelConf[i].Pin_Index == PF1_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PF2_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PF3_PIN_ID_NUM)  
       ||(Port_ChannelConf[i].Pin_Index == PF4_PIN_ID_NUM))   
       {      
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);         
       CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,Port_ChannelConf[i].Pin_Index);    /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register to choose GPIO */
       *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x0000007F << (Port_ChannelConf[i].Pin_Index * 4));    /* SET the PMCx bits for this pin to choose GPIO*/
       }       
      else
      {
          #if (PORT_DEV_ERROR_DETECT == STD_ON)		/* Report to DET  */
               Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_INVALID_MODE);
          #endif   
          return;   /*return without any action*/
      } 
       break;
    case PORT_PIN_MODE_PWM:     /*case of using the pin for PWM mode...if it is supported on the pin*/ 
      
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);         
       CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,Port_ChannelConf[i].Pin_Index);    /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register to choose GPIO */    
      if((Port_ChannelConf[i].Pin_Index == PB4_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PB5_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PB6_PIN_ID_NUM)\
       ||(Port_ChannelConf[i].Pin_Index == PB7_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PC4_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PC5_PIN_ID_NUM)\
       ||(Port_ChannelConf[i].Pin_Index == PD0_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PD1_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PE4_PIN_ID_NUM)\
       ||(Port_ChannelConf[i].Pin_Index ==PE5_PIN_ID_NUM))
      {
       *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x0000004F << (Port_ChannelConf[i].Pin_Index * 4));    /* SET the PMCx bits for this pin to choose GPIO*/
      }
      else if((Port_ChannelConf[i].Pin_Index == PA6_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PA7_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PF0_PIN_ID_NUM)\
       ||(Port_ChannelConf[i].Pin_Index == PF1_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PF2_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PF3_PIN_ID_NUM)\
       ||(Port_ChannelConf[i].Pin_Index == PF4_PIN_ID_NUM))
      {
       *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x0000005F << (Port_ChannelConf[i].Pin_Index * 4));    /* SET the PMCx bits for this pin to choose GPIO*/ 
      }
      else
      {
          #if (PORT_DEV_ERROR_DETECT == STD_ON)		/* Report to DET  */
               Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_INVALID_MODE);
          #endif   
          return;   /*return without any action*/
      } 
      break;
    case PORT_PIN_MODE_UART:     /*case of using the pin for UART mode...if it is supported on the pin*/
      
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);         
       CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,Port_ChannelConf[i].Pin_Index);    /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register to choose GPIO */    
       if ((Port_ChannelConf[i].Pin_Index == PD0_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PD1_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PD2_PIN_ID_NUM)\
       ||(Port_ChannelConf[i].Pin_Index == PD3_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PF0_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PF1_PIN_ID_NUM))
       {
          #if (PORT_DEV_ERROR_DETECT == STD_ON)		/* Report to DET  */
               Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_INVALID_MODE);
          #endif   
          return;   /*return without any action*/
       }
       else if ((Port_ChannelConf[i].Pin_Index == PC4_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PC5_PIN_ID_NUM))
       {
        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x0000002F << (Port_ChannelConf[i].Pin_Index * 4));    /* SET the PMCx bits for this pin to choose GPIO*/ 
       }
       else
       {
        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x0000001F << (Port_ChannelConf[i].Pin_Index * 4));    /* SET the PMCx bits for this pin to choose GPIO*/ 
       }     
       break;
     case PORT_PIN_MODE_SPI:     /*case of using the pin for SPI mode...if it is supported on the pin*/     
       if((Port_ChannelConf[i].Pin_Index == PD0_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PD1_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PD2_PIN_ID_NUM)\
       ||(Port_ChannelConf[i].Pin_Index == PD3_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PC4_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PC5_PIN_ID_NUM))
       {
         if((Port_ChannelConf[i].Pin_Index ==PC4_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PC5_PIN_ID_NUM))
         {
             #if (PORT_DEV_ERROR_DETECT == STD_ON)		/* Report to DET  */
               Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_INVALID_MODE);
             #endif   
             return;   /*return without any action*/
         }
         /*Do nothing PD{0:3]are by default configured */
       }
       else
       {
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);         
       CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,Port_ChannelConf[i].Pin_Index);    /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register to choose GPIO */    
       *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x0000002F << (Port_ChannelConf[i].Pin_Index * 4));    /* SET the PMCx bits for this pin to choose GPIO*/ 
       }
       break;
     case PORT_PIN_MODE_QEI:     /*case of using the pin for QEI mode...if it is supported on the pin*/     
       
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);         
       CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,Port_ChannelConf[i].Pin_Index);    /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register to choose GPIO */    
       *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x0000006F << (Port_ChannelConf[i].Pin_Index * 4));    /* SET the PMCx bits for this pin to choose GPIO*/ 
       break;
     case PORT_PIN_MODE_I2C:    /*case of using the pin for I2C mode...if it is supported on the pin*/ 
       if((Port_ChannelConf[i].Pin_Index == PF0_PIN_ID_NUM)||(Port_ChannelConf[i].Pin_Index ==PF3_PIN_ID_NUM))
       {
          #if (PORT_DEV_ERROR_DETECT == STD_ON)		/* Report to DET  */
               Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_INVALID_MODE);
          #endif   
          return;   /*return without any action*/
       }
       else
       {
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);         
       CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,Port_ChannelConf[i].Pin_Index);    /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ChannelConf[i].Pin_Index);            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register to choose GPIO */    
       *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x0000003F << (Port_ChannelConf[i].Pin_Index * 4));    /* SET the PMCx bits for this pin to choose GPIO*/ 
       }       
       break;
       
            }
        }
      }
     else if (Pin > 7) /*if wrong pin ID is given to the function */
      {
        #if (PORT_DEV_ERROR_DETECT == STD_ON)
            		/* Report to DET  */
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_GET_VERSION_INFO_SID,PORT_E_PARAM_PIN);
        #endif 
        return;         /*return without any action*/           
      }
      else if (((Port_ChannelConf[i].Port_Index ==4)&&(Pin >5))||(Port_ChannelConf[i].Port_Index==5)&&(Pin >4))
      {                                                            /*if wrong pin ID is entered...special cond for port E & F */
                #if (PORT_DEV_ERROR_DETECT == STD_ON)
            		/* Report to DET  */
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_GET_VERSION_INFO_SID,PORT_E_PARAM_PIN);
        #endif 
        return;         /*return without any action*/ 
      }

    }        
}
#endif
