#include "util.h"

/*******************************************************************************
uint32_t fnUtilTimer(uint8_t flag)

함수 설명    :
인자값 설명  :
반환 값 설명 :
*******************************************************************************/
#define SYSTICK_LOOP_TIMER			0
void fnLoopCountTimer(void)
{
	static uint32_t past;
	uint32_t now;
	uint32_t time;

	while(1)
	{
		now = HAL_GetTick();
		if(now >= past)
		{
			time = now - past;
		}
		else
		{
			time = (HAL_MAX_DELAY - past) + now;
		}
		if(time >= MAX_LOOP_COUNT)
		{
			past = HAL_GetTick();

			#if SYSTICK_LOOP_TIMER
			static uint8_t toggle;
			toggle = ~toggle;
			if(toggle)
			{
				//LED_TOG;
				//MAIN LOOP TEST
			}
			else
			{
				//LED_TOG;
				//MAIN LOOP TEST
			}
			#endif
			
			break;
		}
	}
}
/*******************************************************************************
uint32_t fnUtilTimer(uint8_t flag)

함수 설명    : SysTick에서 증가하는 카운터 값을 호출하여 READ/WRITE 한다
인자값 설명  : flag == COUNT_WRITE (1)이면 함수호출시 카운터값 증가
               flag == COUNT_READ  (0)이면 함수호출시 카운터값 읽기
반환 값 설명 : 현재 COUNT값 반환
*******************************************************************************/
uint32_t fnUtilTimer(uint8_t flag)
{
    static uint32_t Count = 0;
  
    if(flag)
    {
    	Count++;
    }
    if(Count >= MAX_COUNT)
    {
        Count = 0;
    }
    return Count;
}

/*******************************************************************************
uint16_t fnDelayProc(COUNT_DELAY *stcnt)

함수 설명    : COUNT_DELAY값을 넣어 fnUtilTimer에서 증가한 카운터 값을 비교하여
               설정된 카운터 만큼 Delay

인자값 설명  : stcnt.En   -> 사용(1) / 사용안함(0)
           stcnt.Done -> 카운트 종료(1) / 카운트 진행중(0)
           stcnt.Unit -> 카운트 단위(UNIT_MSEC, UNIT_100MSEC, UNIT_SEC)
           stcnt.SVal -> 카운트 설정값
           stcnt.PVal -> 카운트 비교값
           stcnt.CVal -> 카운트 증가값

반환 값 설명 : stcnt->Done -> 카운트 종료(1) / 카운트 진행중(0)
*******************************************************************************/
uint8_t fnDelayProc(sDelay_TypeDef *Delay)
{
	uint32_t past = 0;

	if(Delay->En == true)
	{
		Delay->En = false;
		Delay->Comp.BaseValue = HAL_GetTick();
		Delay->Comp.PastValue = 0;
		Delay->Comp.Flag = 1; //run start
	}

	if(Delay->Comp.Flag == 0)
	{
		Delay->Comp.Done = true;
		return Delay->Comp.Done;
	}

	past = HAL_GetTick();

	if(past >= Delay->Comp.BaseValue)
	{
		if( (past - Delay->Comp.BaseValue) != 0 )
		{
		  Delay->Comp.PastValue = (past - Delay->Comp.BaseValue) / (Delay->Unit);
		}
		else
		{
		  Delay->Comp.PastValue = (past - Delay->Comp.BaseValue);
		}
	}
	else 
	{
		Delay->Comp.PastValue = ((HAL_MAX_DELAY - Delay->Comp.BaseValue) + past) / (Delay->Unit);
	}

	if(Delay->SetValue <= Delay->Comp.PastValue)
	{
		Delay->Comp.Done = true; 
		Delay->Comp.Flag = 0;
	}
	else
	{
		Delay->Comp.Done = false; 
	}

	return Delay->Comp.Done;
}


/**
  * @brief
  * @param
  * @retval
  */
void fnQueueInitProc(Q_STRUCT *QueueRec)
{
	QueueRec->PutPoint = 0;
	QueueRec->GetPoint = 0;
}

/**
  * @brief
  * @param
  * @retval
  */
int fnQueuePutProc(Q_STRUCT *QueueRec, int val)
{
    // 데이터 풀
    if(QueueRec->PutPoint == MAX_Q_SIZE)
    {
        return -1;
    }
    QueueRec->QueueBuff[QueueRec->PutPoint] = val;
    QueueRec->PutPoint++;

    return val;
}

/**
  * @brief
  * @param
  * @retval
  */
int fnQueueGetProc(Q_STRUCT *QueueRec)
{
    int i, val;

    // 데이터 없음
    if (QueueRec->PutPoint == 0)
    {
        return -1;
    }
    val = QueueRec->QueueBuff[0];
    for (i = 1; i < QueueRec->PutPoint; i++)
    {
    	QueueRec->QueueBuff[i-1] = QueueRec->QueueBuff[i];
    }
    QueueRec->PutPoint--;
//  QueueEqlDelPro(QueueRec);  // 큐 버퍼에 같은 데이터 삭제
    return val;
}

/**
  * @brief 큐 버퍼에 같은 데이터 삭제
  * @param
  * @retval
  */
int fnQueueEqlDelProc(Q_STRUCT *QueueRec)
{
	int i, j, k, val;
	char Buff[MAX_Q_SIZE];

	// 데이터 없음
	if (QueueRec->PutPoint == 0)
	{
		return -1;
	}
	val = QueueRec->QueueBuff[0];
	k = 0;
	for (i = 0; i < QueueRec->PutPoint-1; i++)
	{
		if (QueueRec->QueueBuff[i] > -1)
		{
			Buff[k++] = QueueRec->QueueBuff[i];
		}
		for (j = 1; j < QueueRec->PutPoint; j++)
		{
			if (QueueRec->QueueBuff[i] == QueueRec->QueueBuff[j])
			{
				QueueRec->QueueBuff[j] = -1;
			}
		}
	}
	for(i = 0; i < MAX_Q_SIZE; i++)
	{
		QueueRec->QueueBuff[i] = Buff[i];
	}
	QueueRec->PutPoint -= k;

	return val;
}

/**
  * @brief
  * @param
  * @retval
  */
int fnRingQueuePutProc(Q_STRUCT *QueueRec, int val)
{
	// 데이터 PULL
	if (QueueRec->PutPoint == MAX_Q_SIZE)
    {
		QueueRec->PutPoint = 0;
    }
    QueueRec->QueueBuff[QueueRec->PutPoint] = val;
    QueueRec->PutPoint++;

    return val;
}

/**
  * @brief
  * @param
  * @retval
  */
int fnQueueRingGetProc(Q_STRUCT *QueueRec)
{
    int val;

    // 데이터 없음
    if (QueueRec->PutPoint == QueueRec->GetPoint)
    {
        return -1;
    }
    val = QueueRec->QueueBuff[QueueRec->GetPoint];
    QueueRec->GetPoint++;

    return val;
}

