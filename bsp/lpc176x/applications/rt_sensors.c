
#include "rt_sensors.h"
//#include "adxl345.h"
//#include "lpc17xx_i2c.h"
//#include "keyscan.h"


ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t sensors_stack[ 1024 ];// �߳�ջ�ռ�
static struct rt_thread sensors_thread;// �߳�����



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
   
	    /*******������̬�߳�**********/
//	  rt_thread_t tid2;
//  
//    tid2 = rt_thread_create("sensor",
//    		rt_sensor_thread_entry, RT_NULL,
//    		512, RT_THREAD_PRIORITY_MAX/5, 20);
//    if (tid2 != RT_NULL) rt_thread_startup(tid2);

//    return 0;

	
	     /*******������̬�߳�**********/
	 rt_err_t result;
    // ����led_thread�̣߳���ں���Ϊled_thread_entry����ں�������ΪRT_NULL��ջ��ʼ��ַ��led_stack[0]��ջ��С��0x200�����ȼ���20��ʱ��Ƭ��5tick�� 
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
        rt_thread_startup(&sensors_thread);// ����sensors_thread��
    }

    return 0;
	
	
	
	
}


