
#include "rt_sensors.h"
//#include "adxl345.h"
//#include "lpc17xx_i2c.h"
//#include "keyscan.h"


ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t sensors_stack[ 1024 ];// 线程栈空间
static struct rt_thread sensors_thread;// 线程名称



void rt_sensor_thread_entry(void *parameter)
{
  
//	I2C1_Init(0);
//  rt_kprintf("I2C1_Inited\t\n");
//	ADXL345_Init();
//	rt_thread_delay( RT_TICK_PER_SECOND/100 );
//	rt_kprintf("ADXL345_Init_ok!!!\t\n");
//	if(adxl345_Check())  rt_kprintf("adxl345_Check_ok!!!\t\n");
//	else rt_kprintf("adxl345_Check_failed!!\t\n");
	  while(1)
		{
			
			
			 rt_thread_delay( RT_TICK_PER_SECOND/2 );
      // show_adxl345_value();
		   
			
		}

}

int rt_hw_sensors_init()
{
   
	    /*******创建动态线程**********/
//	  rt_thread_t tid2;
//  
//    tid2 = rt_thread_create("sensor",
//    		rt_sensor_thread_entry, RT_NULL,
//    		512, RT_THREAD_PRIORITY_MAX/5, 20);
//    if (tid2 != RT_NULL) rt_thread_startup(tid2);

//    return 0;

	
	     /*******创建静态线程**********/
	 rt_err_t result;
    // 创建led_thread线程，入口函数为led_thread_entry，入口函数参数为RT_NULL，栈起始地址：led_stack[0]，栈大小：0x200，优先级：20，时间片：5tick。 
    result = rt_thread_init(&sensors_thread,
                            "sensors",
                            rt_sensor_thread_entry,
                            RT_NULL,
                            (rt_uint8_t*)&sensors_stack[0],
                            sizeof(sensors_stack),
                            18,
                            5);

    if (result == RT_EOK)
    {
        rt_thread_startup(&sensors_thread);// 启动sensors_thread程
    }

    return 0;
	
	
	
	
}


