#include "12864.h"
#include <intrins.h>
/*******************************************************************************
* 函 数 名         : Delay   					经测试最低可以只延时5微秒
* 函数功能       	 : 延时5微秒
*******************************************************************************/
void Delay(void)	//@33.1776MHz
{
	unsigned long edata i;

	_nop_();
	i = 40UL;
	while (i) i--;
}
 
/*******************************************************************************
* 函 数 名         : LCD12864_Busy
* 函数功能       	 : 检测LCD是否忙
* 输    出         : 1或0（1表示不忙，0表示忙）
*******************************************************************************/
uchar LCD12864_Busy(void)
{
  uchar i = 0;
  LCD12864_RS = 0;     //选择命令
  LCD12864_RW = 1;  //选择读取
  LCD12864_EN = 1;
  Delay();
  while((LCD12864_DATAPORT & 0x80) == 0x80)  //检测读取到的值
  {
    i++;
    if(i > 100)
    {
      LCD12864_EN = 0;
      return 0;     //超过等待时间返回0表示失败
    }
  }
  LCD12864_EN = 0;
  return 1;
}
 
/*******************************************************************************
* 函 数 名         : LCD12864_WriteCmd
* 函数功能         : 写命令
*******************************************************************************/
void LCD12864_WriteCmd(uchar cmd)
{
  uchar i;
  i = 0;
  while( LCD12864_Busy() == 0)
  {
    Delay();
    i++;
    if( i>100)
    {
      return;     //超过等待退出
    }  
  } 
  LCD12864_RS = 0;     //选择命令
  LCD12864_RW = 0;     //选择写入
  LCD12864_EN = 0;     //初始化使能端
  LCD12864_DATAPORT = cmd;   //放置数据
  LCD12864_EN = 1;       //写时序
  Delay();
  LCD12864_EN = 0;              
}
 
/*******************************************************************************
* 函 数 名         : LCD12864_WriteData
* 函数功能         : 写数据
*******************************************************************************/
void LCD12864_WriteData(uchar dat)
{
  uchar i;
  i = 0;
  while( LCD12864_Busy() == 0)
  {
    Delay();
    i++;
    if( i>100)
    {
      return;     //超过等待退出
    }  
  }
  LCD12864_RS = 1;     //选择数据
  LCD12864_RW = 0;     //选择写入
  LCD12864_EN = 0;     //初始化使能端
  LCD12864_DATAPORT = dat;   //放置数据
  LCD12864_EN = 1;       //写时序
  Delay();
  LCD12864_EN = 0;                    
}
 
/*******************************************************************************
* 函 数 名         : LCD12864_Init
* 函数功能         : 初始化
*******************************************************************************/
void LCD12864_Init()
{
  LCD12864_PSB = 1;    //选择并行输入
	LCD12864_RST = 0;    //复位
  LCD12864_RST = 1;    //复位
  LCD12864_WriteCmd(0x01);  //清除LCD12864的显示内容
	LCD12864_WriteCmd(0x02);
  LCD12864_WriteCmd(0x0c);  //显示开，关光标
  LCD12864_WriteCmd(0x80);
	//LCD12864_WriteCmd(0x34);	//打开扩充指令
	LCD12864_WriteCmd(0x30);  //选择基本指令操作
}
 
/*******************************************************************************
* 函 数 名         : LCD12864_Word
* 函数功能         : 打字，一行十六个，一共四行
*******************************************************************************/
void LCD12864_Word(uchar x, uchar y,uchar *word)
{
  uchar pos;
  uchar i=y;

  if(x == 0)     // 第一行的地址是80H
  {
    x = 0x80;
  }    
  else if(x == 1)  //第二行的地址是90H
  {
    x = 0x90;  
  }  
  else if(x == 2)  //第三行的地址是88H
  {
    x = 0x88;
  }  
  else if(x == 3)  //第三行的地址是98H
  {
    x = 0x98;
  }  
  pos = x + y;
  LCD12864_WriteCmd(pos);  
  while(word[i]!='\0')
  {
    if(i<16)    //判断是否在本行的显示范围内
    {
      LCD12864_WriteData(word[i]);
      i++;
    }  
   }
}
/*******************************************************************************
* 函 数 名         : LCD12864_Image       
* 函数功能				 :用于显示一整张图片
*******************************************************************************/
void LCD12864_Image(uchar *dat)
{
	uchar i,j;
	LCD12864_WriteCmd(0x34);	//打开扩充指令
	//上半屏
	for(i = 0;i < 32;i++)
	{
		LCD12864_WriteCmd(0x80 + i);//x
		LCD12864_WriteCmd(0x80);//y
		for(j = 0;j < 16;j++) LCD12864_WriteData(*dat++);
	}
	//下半屏
	for(i = 0;i < 32;i++)
	{
		LCD12864_WriteCmd(0x80 + i);//x
		LCD12864_WriteCmd(0x88);//y
		for(j = 0;j < 16;j++) LCD12864_WriteData(*dat++);
	}
	LCD12864_WriteCmd(0x36);	//绘图
	LCD12864_WriteCmd(0x30);	//回到基本指令
}
/*******************************************************************************
* 函 数 名         : LCD12864_cls       
* 函数功能				 :用于绘画时清屏，不可清除非绘画的显示，例如字
*******************************************************************************/
void LCD12864_cls()
{
	uchar i,j;
	LCD12864_WriteCmd(0x34);	//打开扩充指令
	//上半屏
	for(i = 0;i < 32;i++)
	{
		LCD12864_WriteCmd(0x80 + i);//x
		LCD12864_WriteCmd(0x80);//y
		for(j = 0;j < 16;j++) LCD12864_WriteData(0x00);
	}
	//下半屏
	for(i = 0;i < 32;i++)
	{
		LCD12864_WriteCmd(0x80 + i);//x
		LCD12864_WriteCmd(0x88);//y
		for(j = 0;j < 16;j++) LCD12864_WriteData(0x00);
	}
	LCD12864_WriteCmd(0x36);	//绘图
	LCD12864_WriteCmd(0x30);	//回到基本指令
}