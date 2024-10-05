/* 
 * 作者：qILoongIp
 * 工作频率：33.1776Mhz
 * 每次重新进行串口通讯前建议先复位或重新上电
 * 当发现显示错位例如原本应该在屏幕顶部的内容跑到了屏幕的中部时断电等待一段时间后再上电即可
 */
#include <12864.h>
#include <Uart.h>
#include <STC32G.H>

unsigned char Uart_Recv;//用于接收串口信息
unsigned char row = 0,col = 0;

void Uart_Proc() interrupt 4
{
	//扫描是这样的，每接收一个字节就画一个字节，倘若一次性接收16*64个字节再画的话会导致速度变慢
	if(RI)				//检测串口1接收中断
	{
		Uart_Recv = SBUF;
		RI = 0;			//清除串口1接收中断请求位
		if(col == 0)
		{
			//行号和列号的初始值需参考手册
			LCD12864_WriteCmd(0x80 + (row % 32));//行号，上下半屏的起始行号均为0x80
			LCD12864_WriteCmd(0x80 + (row / 32) * 8);//列号，上半屏的列号从0x80开始，下半屏从0x88开始
		}
		LCD12864_WriteData(Uart_Recv);//将本次接收到的字节写入
		col++;
		if(col >= 16)
		{
			col = 0;
			row++;
		}
		if(row >= 64) 
		{
			row = 0;
		}
	}
}

void main()
{
	P0M1 = 0x00;
	P0M0 = 0x00;
	P4M1 = 0x00;
	P4M0 = 0x00;
	WTST = 0;
	//以上部分是STC32配置IO口和程序读取等待控制寄存器的操作
	LCD12864_Init();
	Uart1_Init();
	LCD12864_WriteCmd(0x34);	//打开扩充指令,这个指令执行的时候会有清屏的效果，所以不能频繁执行
	LCD12864_WriteCmd(0x36);	//绘图，这个指令测试下来只需要打开一次就可以一直进行绘画，不需要关闭后又重新打开
	while(1);
}