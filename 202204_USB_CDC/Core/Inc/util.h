/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UTIL_H_
#define __UTIL_H_
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "main.h"
/* Private includes ----------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define MAX_LOOP_COUNT	100
#define MAX_COUNT       100000

#define DELAY_UNIT_MSEC			1
#define DELAY_UNIT_10MSEC		10
#define DELAY_UNIT_100MSEC	100
#define DELAY_UNIT_SEC			1000
#define DELAY_UNIT_MINUTE		60000

#define COUNT_WRITE     1
#define COUNT_READ      0

#define MAX_Q_SIZE		10

struct _COUNT_DELAY_COMPONENT_
{
	uint32_t BaseValue;
	uint32_t PastValue;
	uint8_t  Flag;
	uint8_t  Done;
};
typedef struct _COUNT_DELAY_
{
	uint8_t Step;
  uint8_t En;
  uint32_t Unit;
  uint32_t SetValue;
	struct _COUNT_DELAY_COMPONENT_ Comp;
} sDelay_TypeDef;


typedef struct _Q_STRUCT_
{
	uint8_t PutPoint;
	uint8_t GetPoint;
	int QueueBuff[MAX_Q_SIZE];
}
Q_STRUCT;
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

void fnLoopCountTimer(void);
uint32_t fnUtilTimer(uint8_t flag);
uint8_t fnDelayProc(sDelay_TypeDef *stcnt);
void fnQueueInitProc(Q_STRUCT *QueueRec);
int fnQueuePutProc(Q_STRUCT *QueueRec, int val);
int fnQueueGetProc(Q_STRUCT *QueueRec);
int fnQueueEqlDelProc(Q_STRUCT *QueueRec);
int fnRingQueuePutProc(Q_STRUCT *QueueRec, int val);
int fnQueueRingGetProc(Q_STRUCT *QueueRec);
#ifdef __cplusplus
}
#endif
#endif // ifndef __UTIL_H_
