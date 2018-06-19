
#include "rt_ethernet.h"


#include <lwip/netdb.h> /* Ϊ�˽�������������Ҫ����netdb.hͷ�ļ� */
#include <lwip/sockets.h> /* ʹ��BSD socket����Ҫ����sockets.hͷ�ļ� */
#include "lwip/netbuf.h"
#include "lwip/api.h"
#include "emac.h"
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t ethernet_stack[ 2048 ];// �߳�ջ�ռ�
static struct rt_thread ethernet_thread;// �߳�����


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

    /* ͨ��������ڲ���url���host��ַ��������������������������� */
    host = gethostbyname(url);

    /* �������ڴ�Ž������ݵĻ��� */
    recv_data = rt_malloc(BUFSZ);
    if (recv_data == RT_NULL)
    {
        rt_kprintf("No memory\n");
        return 1;
    }

    /* ����һ��socket��������SOCKET_STREAM��TCP���� */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        /* ����socketʧ�� */
        rt_kprintf("Socket error\n");

        /* �ͷŽ��ջ��� */
        rt_free(recv_data);
        return 1;
    }

    /* ��ʼ��Ԥ���ӵķ���˵�ַ */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));
    /* ���ӵ������ */
    while (connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        /* ����ʧ�� */
			  connect_flag=0;
        rt_kprintf("Connect fail!\n");
        lwip_close(sock);

         /*�ͷŽ��ջ��� */
        rt_free(recv_data);
       return 1;
    }
	
		rt_kprintf("Connect OK!\n");
    connect_flag=1;
		
    while(1)
    {
        
				link_flag=Check_PHYStatus();   //��������������
				if(link_flag!=1)
				{
					rt_kprintf("RJ45 break line!\n");
					lwip_close(sock);

						 /*�ͷŽ��ջ��� */
						rt_free(recv_data);
					 break ;
					}
					
			/* ��sock�����н������BUFSZ - 1�ֽ����� */
        bytes_received = recv(sock, recv_data, BUFSZ - 1, 8);
        if (bytes_received > 0)		//������յ������򷵻�
        {
					recv_data[bytes_received] = '\0';

					if (strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0)
					{
							/* ���������ĸ��q��Q���ر�������� */
							lwip_close(sock);

							/* �ͷŽ��ջ��� */
							rt_free(recv_data);
							break;
					}
					else
					{
							/* �ڿ����ն���ʾ�յ������� */
							rt_kprintf("\nRecieved data = %s " , recv_data);
					}

					/* �������ݵ�sock���� */
					if(send(sock,send_data,strlen(send_data), 0) < 0)
					{
							/* ����ʧ�ܣ��ر�������� */
							rt_kprintf("\nSend error ");
							lwip_close(sock);

							/* �ͷŽ��ջ��� */
							rt_free(recv_data);
							break;
					}
        }

				/* ���������ź� */
        if(send(sock,"Tick",strlen("Tick"),0) < 0)
        {
            /* ������Ͳ��ɹ���ʾ���ӶϿ� �ر�socket���������� */
						rt_kprintf("\nTick error ");
            lwip_close(sock);

            /* �ͷŽ��ջ��� */
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
/* ���tcpclient������finsh shell�� */
FINSH_FUNCTION_EXPORT(tcpclient, startup tcp client);
#endif



void rt_ethernet_thread_entry(void *parameter)
{
  
	 rt_thread_delay( RT_TICK_PER_SECOND/2 );
		
//		rt_kprintf("LWIP ENTRY!\n");
//	  while(tcpclient("192.168.0.62",8080 ))
//		{
//			rt_thread_delay(RT_TICK_PER_SECOND * 4);
//                                               //Ҫ��취���û�в����������� �ٲ����������ϵ�����
//			link_flag=Check_PHYStatus();             //��������������
//			if(link_flag!=1)
//			{
//					rt_kprintf("RJ45 break line!!!Please try to plug in the LAN cable\n");
//			}
//			else
//			{
//			   if(connect_flag==0&&RJ45_LINK_FLAG==1) NVIC_SystemReset(); //���û�в����������� �ٲ����������ϵ�����
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
    // ����led_thread�̣߳���ں���Ϊled_thread_entry����ں�������ΪRT_NULL��ջ��ʼ��ַ��led_stack[0]��ջ��С��0x200�����ȼ���20��ʱ��Ƭ��10tick�� 
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
        rt_thread_startup(&ethernet_thread);// ����ethernet_thread��
    }

    return 0;
	
	
	
	
	
	
}






