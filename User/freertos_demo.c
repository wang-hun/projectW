/**
 ****************************************************************************************************
 * @file        freertos.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.4
 * @date        2022-01-04
 * @brief       FreeRTOS 任务挂起与恢复实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 战舰F103开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
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
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO         1
#define START_TASK_STACK_SIZE   128
TaskHandle_t    start_task_handler;
void start_task( void * pvParameters );

/* TASK1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK1_PRIO         2
#define TASK1_STACK_SIZE   128
TaskHandle_t    task1_handler;
void task1( void * pvParameters );

/* TASK2 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK2_PRIO         3
#define TASK2_STACK_SIZE   128
TaskHandle_t    task2_handler;
void task2( void * pvParameters );

/* TASK3 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK3_PRIO         4
#define TASK3_STACK_SIZE   128
TaskHandle_t    task3_handler;
void task3( void * pvParameters );
/******************************************************************************************************/


/**
 * @brief       FreeRTOS例程入口函数
 * @param       无
 * @retval      无
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
    taskENTER_CRITICAL();               /* 进入临界区 */
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
    taskEXIT_CRITICAL();                /* 退出临界区 */
}

/* 任务一，实现LED0每500ms翻转一次,并串口输出 */
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


/* 任务二，实现实现光照采集*/
void task2( void * pvParameters )
{
     while (1)
    { 
			  
     
			  lcd_show_string(30,  50, 200, 16, 16, "STM32\r\n", RED);
			  lcd_show_string(30, 110, 200, 16, 16, "Detection system startup\r\n", BLUE);
        lcd_show_string(30, 150, 200, 16, 16, "Degree of machine damage:\r\n", BLUE);
			adcx = lsens_get_val();
        lcd_show_xnum(30, 180, adcx, 3, 16, 0, BLUE); /* 显示ADC的值 */
		//	lcd_draw_circle(300,300,100,RED);
		//	lcd_fill(350,350,400,400,BLUE);
        vTaskDelay(500);
    }
}

/* 任务三，判断按键KEY0，按下KEY0删除task1，task2 */
void task3( void * pvParameters )
{
    uint8_t key = 0;
    while(1)
    {
        key = key_scan(0);
        
			  if(key == KEY0_PRES)
        {
					lcd_clear(BLUE);
          lcd_show_string(30,  50, 200, 16, 16, "STM32", RED);
			    lcd_show_string(30, 110, 200, 16, 16, "Detection system shutdown", BLUE);
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
运行结果：
按下key0，一个灯不闪了，再按key1，刚才不闪的小灯又恢复闪了
#endif
