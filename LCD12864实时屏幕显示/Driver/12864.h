#ifndef __LCD12864_H
#define __LCD12864_H
 
//---包含头文件---//
#include <STC32G.H>
 
//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif
 
#ifndef uint 
#define uint unsigned int
#endif
//---定义使用的IO口---//
 
#define LCD12864_DATAPORT P0      	   //数据IO口
 
sbit LCD12864_RS  =  P4^0;             //（数据命令）寄存器选择输入 
sbit LCD12864_RW  =  P4^1;             //液晶读/写控制
sbit LCD12864_EN  =  P4^2;             //液晶使能控制
sbit LCD12864_PSB =  P4^3;         		 //串/并方式控制
sbit LCD12864_RST =  P4^4;             //复位端
//---声明全局函数---//
void Delay();
uchar LCD12864_Busy();
void LCD12864_WriteCmd(uchar cmd);
void LCD12864_WriteData(uchar dat);
void LCD12864_Init();
void LCD12864_Word(uchar x, uchar y,uchar *word);
void LCD12864_Image(uchar *dat);
void LCD12864_cls();
 
#endif