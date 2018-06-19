
#include "rt_ethernet.h"


#include <lwip/netdb.h> /* 为了解析主机名，需要包含netdb.h头文件 */
#include <lwip/sockets.h> /* 使用BSD socket，需要包含sockets.h头文件 */
#include "lwip/netbuf.h"
#include "lwip/api.h"
#include "emac.h"
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t ethernet_stack[ 2048 ];// 线程栈空间
static struct rt_thread ethernet_thread;// 线程名称


#define BUFSZ	1024


rt_uint8_t  RJ45_LINK_FLAG;
uint8_t  link_flag;
uint8_t connect_flag=0;
static const char send_data[] = "This is TCP Client from RT-Thread.";
int tcpclient(const char* url, int port)
{
		 char *recv_data;
    struct hostent *host;
    int sock, bytes_received;
    struct sockaddr_in server_addr;

    /* 通过函数入口参数url获得host地址（如果是域名，会做域名解析） */
    host = gethostbyname(url);

    /* 分配用于存放接收数据的缓冲 */
    recv_data = rt_malloc(BUFSZ);
    if (recv_data == RT_NULL)
    {
        rt_kprintf("No memory\n");
        return 1;
    }

    /* 创建一个socket，类型是SOCKET_STREAM，TCP类型 */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        /* 创建socket失败 */
        rt_kprintf("Socket error\n");

        /* 释放接收缓冲 */
        rt_free(recv_data);
        return 1;
    }

    /* 初始化预连接的服务端地址 */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));
    /* 连接到服务端 */
    while (connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        /* 连接失败 */
			  connect_flag=0;
        rt_kprintf("Connect fail!\n");
        lwip_close(sock);

         /*释放接收缓冲 */
        rt_free(recv_data);
       return 1;
    }
	
		rt_kprintf("Connect OK!\n");
    connect_flag=1;
		
    while(1)
    {
        
				link_flag=Check_PHYStatus();   //检查网线连接情况
				if(link_flag!=1)
				{
					rt_kprintf("RJ45 break line!\n");
					lwip_close(sock);

						 /*释放接收缓冲 */
						rt_free(recv_data);
					 break ;
					}
					
			/* 从sock连接中接收最大BUFSZ - 1字节数据 */
        bytes_received = recv(sock, recv_data, BUFSZ - 1, 8);
        if (bytes_received > 0)		//如果接收到数据则返回
        {
					recv_data[bytes_received] = '\0';

					if (strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0)
					{
							/* 如果是首字母是q或Q，关闭这个连接 */
							lwip_close(sock);

							/* 释放接收缓冲 */
							rt_free(recv_data);
							break;
					}
					else
					{
							/* 在控制终端显示收到的数据 */
							rt_kprintf("\nRecieved data = %s " , recv_data);
					}

					/* 发送数据到sock连接 */
					if(send(sock,send_data,strlen(send_data), 0) < 0)
					{
							/* 发送失败，关闭这个连接 */
							rt_kprintf("\nSend error ");
							lwip_close(sock);

							/* 释放接收缓冲 */
							rt_free(recv_data);
							break;
					}
        }

				/* 发送心跳信号 */
        if(send(sock,"Tick",strlen("Tick"),0) < 0)
        {
            /* 如果发送不成功表示连接断开 关闭socket后重新连接 */
						rt_kprintf("\nTick error ");
            lwip_close(sock);

            /* 释放接收缓冲 */
            rt_free(recv_data);
            break;
        }
				else
					rt_thread_delay(RT_TICK_PER_SECOND);
    }

    return 1;
						 
}
#ifdef RT_USING_FINSH
#include <finsh.h>
/* 输出tcpclient函数到finsh shell中 */
FINSH_FUNCTION_EXPORT(tcpclient, startup tcp client);
#endif



void rt_ethernet_thread_entry(void *parameter)
{
  
	 rt_thread_delay( RT_TICK_PER_SECOND/2 );
		
//		rt_kprintf("LWIP ENTRY!\n");
//	  while(tcpclient("192.168.0.62",8080 ))
//		{
//			rt_thread_delay(RT_TICK_PER_SECOND * 4);
//                                               //要想办法解决没有插网线启动了 再插网线连不上的问题
//			link_flag=Check_PHYStatus();             //检查网线连接情况
//			if(link_flag!=1)
//			{
//					rt_kprintf("RJ45 break line!!!Please try to plug in the LAN cable\n");
//			}
//			else
//			{
//			   if(connect_flag==0&&RJ45_LINK_FLAG==1) NVIC_SystemReset(); //解决没有插网线启动了 再插网线连不上的问题
//				
//			}
//		 
//   }
	

}




int rt_hw_ethernet_init()
{
   
	
//	  rt_thread_t tid1;
//  
//    tid1 = rt_thread_create("ethernet",
//    		rt_ethernet_thread_entry, RT_NULL,
//    		2048, RT_THREAD_PRIORITY_MAX/4, 30);
//    if (tid1 != RT_NULL) rt_thread_startup(tid1);

//    return 0;
	
	
	  rt_err_t result;
    // 创建led_thread线程，入口函数为led_thread_entry，入口函数参数为RT_NULL，栈起始地址：led_stack[0]，栈大小：0x200，优先级：20，时间片：10tick。 
    result = rt_thread_init(&ethernet_thread,
                            "ethernet",
                            rt_ethernet_thread_entry,
                            RT_NULL,
                            (rt_uint8_t*)&ethernet_stack[0],
                            sizeof(ethernet_stack),
                            16,
                            15);

    if (result == RT_EOK)
    {
        rt_kprintf("rt_thread_startup...ethernet_thread....\r\n");
        rt_thread_startup(&ethernet_thread);// 启动ethernet_thread程
    }

    return 0;
	
	
	
	
	
	
}






