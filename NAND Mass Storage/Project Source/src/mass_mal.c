/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : mass_mal.c
* Author             : MCD Application Team
* Version            : V3.1.1
* Date               : 04/07/2010
* Description        : Medium Access Layer interface
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "platform_config.h"
#ifdef USE_STM3210E_EVAL
 #include "sdcard.h"
#else
 #include "msd.h"
#endif /* USE_STM3210E_EVAL */
#include "fsmc_nand.h"
#include "nand_if.h"
#include "mass_mal.h"
#include "stm32_eval.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define BlockSize              512 /* Block Size in Bytes */
//#define BlockSize            128 /* Block Size in Bytes */
#define BufferWordsSize      (BlockSize >> 2)

#define NumberOfBlocks1       2  /* For Multi Blocks operation (Read/Write) */
#define MultiBufferWordsSize ((BlockSize * NumberOfBlocks1) >> 2)


uint32_t Mass_Memory_Size[2];
uint32_t Mass_Block_Size[2];
uint32_t Mass_Block_Count[2];

uint32_t Mass_Memory_Size1;
uint32_t Mass_Block_Size1;
uint32_t Mass_Block_Count1;

__IO uint32_t Status = 0;
SD_CardInfo SDCardInfo;
uint32_t Buffer_Block_Tx[BufferWordsSize], Buffer_Block_Rx[BufferWordsSize];
uint32_t Buffer_MultiBlock_Tx[MultiBufferWordsSize], Buffer_MultiBlock_Rx[MultiBufferWordsSize];
unsigned char dis_mem=0;
//volatile TestStatus EraseStatus = FAILED, TransferStatus1 = FAILED, TransferStatus2 = FAILED;

void Get_Medium_Characteristics(void);

#ifdef USE_STM3210E_EVAL
SD_CardInfo SDCardInfo;
#endif




/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : MAL_Init
* Description    : Initializes the Media on the STM32
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_Init(uint8_t lun)
{
  uint16_t status = MAL_OK;

  switch (lun)
  {
    case 0:
#ifdef USE_STM3210E_EVAL
      Status = SD_Init();
      Status = SD_GetCardInfo(&SDCardInfo);
      Status = SD_SelectDeselect((uint32_t) (SDCardInfo.RCA << 16));
      Status = SD_EnableWideBusOperation(SDIO_BusWide_4b);
      Status = SD_SetDeviceMode(SD_DMA_MODE);
	  if (Status == SD_OK)
  {	 
     
	  
      //Status = SD_ReadMultiBlocks(0x00, Buffer_MultiBlock_Rx, BlockSize, NumberOfBlocks1);	
	 // printf("\r\nSD SDIO-4bit模式 测试TF卡读写成功！ \n ");
	 
  }
  
 
  if (Status == SD_OK)
  {	
    // Read block of 512 bytes from address 0 
    Status = SD_ReadBlock(0x00, Buffer_Block_Rx, BlockSize);   

  }

 
  
  

  if (Status == SD_OK)
  {	 
    // Check the corectness of written dada 
    

	printf("\r\nSD SDIO-4bit模式 测试TF卡读写成功！ \n ");
  }  
  
  

#else
      MSD_Init();
#endif
      break;
#ifdef USE_STM3210E_EVAL
    case 1:
	
      status = NAND_Init();
      break;
#endif
    default:
      return MAL_FAIL;
  }
  return status;
}

/*******************************************************************************
* Function Name  : MAL_Write
* Description    : Write sectors
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_Write(uint8_t lun, uint32_t Memory_Offset, uint32_t *Writebuff, uint16_t Transfer_Length)
{

  switch (lun)
  {
    case 0:
#ifdef USE_STM3210E_EVAL
      Status = SD_WriteBlock(Memory_Offset, Writebuff, Transfer_Length);
      if ( Status != SD_OK )
      {
        return MAL_FAIL;
      }      
#else
      MSD_WriteBlock((uint8_t*)Writebuff, Memory_Offset, Transfer_Length);
#endif
      break;
#ifdef USE_STM3210E_EVAL
    case 1:
      NAND_Write(Memory_Offset, Writebuff, Transfer_Length);
	  //printf("Transfer_Length_w = %d\r\n",Memory_Offset);
      break;
#endif
    default:
      return MAL_FAIL;
  }
  return MAL_OK;
}

/*******************************************************************************
* Function Name  : MAL_Read
* Description    : Read sectors
* Input          : None
* Output         : None
* Return         : Buffer pointer
*******************************************************************************/
uint16_t MAL_Read(uint8_t lun, uint32_t Memory_Offset, uint32_t *Readbuff, uint16_t Transfer_Length)
{

  switch (lun)
  {
    case 0:
#ifdef USE_STM3210E_EVAL
      Status = SD_ReadBlock(Memory_Offset, Readbuff, Transfer_Length);
      if ( Status != SD_OK )
      {
        return MAL_FAIL;
      }
#else
      MSD_ReadBlock((uint8_t*)Readbuff, Memory_Offset, Transfer_Length);
#endif
      break;
#ifdef USE_STM3210E_EVAL
    case 1:
      NAND_Read(Memory_Offset, Readbuff, Transfer_Length);
	  //printf("Transfer_Length_r = %d\r\n",Transfer_Length);
      break;
#endif
    default:
      return MAL_FAIL;
  }
  return MAL_OK;
}

/*******************************************************************************
* Function Name  : MAL_GetStatus
* Description    : Get status
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_GetStatus (uint8_t lun)
{
#ifdef USE_STM3210E_EVAL
  NAND_IDTypeDef NAND_ID;
  uint32_t DeviceSizeMul = 0, NumberOfBlocks = 0;
#else
  uint32_t temp_block_mul = 0;
  sMSD_CSD MSD_csd;
  uint32_t DeviceSizeMul = 0;
#endif


  if (lun == 0)
  {
#ifdef USE_STM3210E_EVAL
    if (SD_Init() == SD_OK)
    {
      SD_GetCardInfo(&SDCardInfo);
      SD_SelectDeselect((uint32_t) (SDCardInfo.RCA << 16));
      DeviceSizeMul = (SDCardInfo.SD_csd.DeviceSizeMul + 2);

      if(SDCardInfo.CardType == SDIO_HIGH_CAPACITY_SD_CARD)
      {
        Mass_Block_Count[0] = (SDCardInfo.SD_csd.DeviceSize + 1) * 1024;
      }
      else
      {
        NumberOfBlocks  = ((1 << (SDCardInfo.SD_csd.RdBlockLen)) / 512);
        Mass_Block_Count[0] = ((SDCardInfo.SD_csd.DeviceSize + 1) * (1 << DeviceSizeMul) << (NumberOfBlocks/2));
      }
      Mass_Block_Size[0]  = 512;

      Status = SD_SelectDeselect((uint32_t) (SDCardInfo.RCA << 16)); 
      Status = SD_EnableWideBusOperation(SDIO_BusWide_4b); 
      if ( Status != SD_OK )
      {
        return MAL_FAIL;
      }
       
      Status = SD_SetDeviceMode(SD_DMA_MODE);         
      if ( Status != SD_OK )
      {
        return MAL_FAIL;
      } 
     
#else
    MSD_GetCSDRegister(&MSD_csd);
    DeviceSizeMul = MSD_csd.DeviceSizeMul + 2;
    temp_block_mul = (1 << MSD_csd.RdBlockLen)/ 512;
    Mass_Block_Count[0] = ((MSD_csd.DeviceSize + 1) * (1 << (DeviceSizeMul))) * temp_block_mul;
    Mass_Block_Size[0] = 512;
    Mass_Memory_Size[0] = (Mass_Block_Count[0] * Mass_Block_Size[0]);
#endif
      Mass_Memory_Size[0] = Mass_Block_Count[0] * Mass_Block_Size[0];
      //STM_EVAL_LEDOn(LED2);
	  if(dis_mem==0){						  //显示TF卡的容量	由于是周期性更新状态，所以，加了条件，只显示一次容量值
	  	//printf("\r\n Micro SD卡的容量是  %d MBytes\n" ,Mass_Memory_Size[0]/1024/1024);
      	dis_mem=1;
	  }
	  return MAL_OK;

#ifdef USE_STM3210E_EVAL
    }
#endif
  }
#ifdef USE_STM3210E_EVAL
  else
  {
    FSMC_NAND_ReadID(&NAND_ID);
    if (NAND_ID.Device_ID != 0 )
    {
	  //	printf("\r\n Device ID : %02X %02X" ,NAND_ID.Maker_ID,NAND_ID.Device_ID );
      /* only one zone is used */
      Mass_Block_Count[1] = NAND_ZONE_SIZE * NAND_BLOCK_SIZE * NAND_MAX_ZONE ;
      Mass_Block_Size[1]  = NAND_PAGE_SIZE;
      Mass_Memory_Size[1] = (Mass_Block_Count[1] * Mass_Block_Size[1]);
      return MAL_OK;
    }
  }
#endif
  //STM_EVAL_LEDOn(LED2);
  return MAL_FAIL;
}

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
