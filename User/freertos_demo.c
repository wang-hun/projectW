/**
 ****************************************************************************************************
 * @file        freertos.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.4
 * @date        2022-01-04
 * @brief       FreeRTOS ���������ָ�ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ս��F103������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */
#include "./BSP/ADC/adc3.h"
#include "freertos_demo.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./BSP/LED/led.h"
#include "./SYSTEM/usart/usart.h"
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
short adcx;
/******************************************************************************************************/
/*FreeRTOS����*/

/* START_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO         1
#define START_TASK_STACK_SIZE   128
TaskHandle_t    start_task_handler;
void start_task( void * pvParameters );

/* TASK1 ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define TASK1_PRIO         2
#define TASK1_STACK_SIZE   128
TaskHandle_t    task1_handler;
void task1( void * pvParameters );

/* TASK2 ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define TASK2_PRIO         3
#define TASK2_STACK_SIZE   128
TaskHandle_t    task2_handler;
void task2( void * pvParameters );

/* TASK3 ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define TASK3_PRIO         4
#define TASK3_STACK_SIZE   128
TaskHandle_t    task3_handler;
void task3( void * pvParameters );
/******************************************************************************************************/


/**
 * @brief       FreeRTOS������ں���
 * @param       ��
 * @retval      ��
 */
void freertos_demo(void)
{    

    xTaskCreate((TaskFunction_t         )   start_task,
                (char *                 )   "start_task",
                (configSTACK_DEPTH_TYPE )   START_TASK_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   START_TASK_PRIO,
                (TaskHandle_t *         )   &start_task_handler );
    vTaskStartScheduler();
}


void start_task( void * pvParameters )
{
    taskENTER_CRITICAL();               /* �����ٽ��� */
    xTaskCreate((TaskFunction_t         )   task1,
                (char *                 )   "task1",
                (configSTACK_DEPTH_TYPE )   TASK1_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK1_PRIO,
                (TaskHandle_t *         )   &task1_handler );
                
    xTaskCreate((TaskFunction_t         )   task2,
                (char *                 )   "task2",
                (configSTACK_DEPTH_TYPE )   TASK2_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK2_PRIO,
                (TaskHandle_t *         )   &task2_handler );
                
    xTaskCreate((TaskFunction_t         )   task3,
                (char *                 )   "task3",
                (configSTACK_DEPTH_TYPE )   TASK3_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK3_PRIO,
                (TaskHandle_t *         )   &task3_handler );
    vTaskDelete(NULL);
    taskEXIT_CRITICAL();                /* �˳��ٽ��� */
}

/* ����һ��ʵ��LED0ÿ500ms��תһ��,��������� */
void task1( void * pvParameters )
{
    uint32_t task1_num = 0;
    while(1)
    {
        printf("%d\r\n",adcx);
        LED0_TOGGLE();
        vTaskDelay(500);
    }
}


/* �������ʵ��ʵ�ֹ��ղɼ�*/
void task2( void * pvParameters )
{
     while (1)
    { 
			  
     
			  lcd_show_string(30,  50, 350, 24, 24, "Vehicle-Mounted Communication System", BLACK);
			  lcd_show_string(30, 90, 300, 24, 24, "Shell Damage Monitoring", BLACK);
        lcd_show_string(30,130,200,24,24, "System", BLACK);
			  lcd_draw_rectangle(30,400,200,600,BLUE);
				lcd_show_string(45,425,200,32,32,"The",BLACK);
				lcd_show_string(45,475,200,32,32,"fifth",BLACK);
				lcd_show_string(45,525,200,32,32,"group",BLACK);

				lcd_fill_circle(400,500,80,LIGHTBLUE);
			adcx = lsens_get_val();
        lcd_show_xnum(387, 487, adcx,2, 32, 0, RED); /* ��ʾADC��ֵ */
		//	lcd_draw_circle(300,300,100,RED);
		//	lcd_fill(350,350,400,400,BLUE);
        vTaskDelay(500);
    }
}

/* ���������жϰ���KEY0������KEY0ɾ��task1��task2 */
void task3( void * pvParameters )
{
    uint8_t key = 0;
    while(1)
    {
        key = key_scan(0);
        
			  if(key == KEY0_PRES)
        {
					lcd_clear(WHITE);
          lcd_show_string(75,  100, 400, 32, 32, "SYSTEM SHUTDOWN", BLACK);
					lcd_draw_rectangle(40,50,400,200,BLUE);
					lcd_draw_hline(40,225,360,LIGHTBLUE);lcd_draw_hline(40,226,360,LIGHTBLUE);lcd_draw_hline(40,227,360,LIGHTBLUE);lcd_draw_hline(40,228,360,LIGHTBLUE);
					lcd_show_string(55,450,300,24,24,"THANKS!",BLACK);
					lcd_draw_rectangle(40,350,170,600,BLUE);
					lcd_draw_rectangle(270,350,400,600,BLUE);
					lcd_show_string(285,450,200,24,24,"SEE YOU",BLACK);
          vTaskSuspend(task1_handler);
					vTaskSuspend(task2_handler);
        }else if(key == KEY1_PRES)
        {
					lcd_clear(WHITE);
           vTaskResume(task1_handler);
					 vTaskResume(task2_handler);
        }
        vTaskDelay(10);
    }
}


#if 0
���н����
����key0��һ���Ʋ����ˣ��ٰ�key1���ղŲ�����С���ָֻ�����
#endif
