#include "Infrared.h"			 //此文件中定义了单片机的一些特殊功能寄存器
//---全局变量---//
uchar Ir_Value[4];
uchar Time;
/***************************************************************
* 函 数 名         : delay
* 函数功能		   : 延时函数，i = 1时，大约延时10us
****************************************************************/
void delay(uint i){
	while(i--);
}
/****************************************************************
* 函数名         : Ir_INit()
* 函数功能		  : 初始化红外线接收
* 输入           : 无
* 输出         	 : 无
****************************************************************/
void Ir_Init(){
	IT0 = 1;  //下降沿触发
	EX0 = 1;  //打开中断0允许
	EA = 1;	//打开总中断
	Ir_IN  =  1; //初始化端口
}
/*****************************************************************
* 函数名             : Ir_Read()
* 函数功能		     : 读取红外数值的中断函数
* 输入               : 无
* 输出               : 无
***************************************************************/
void Ir_Read() interrupt 0{
	uchar j,k;
	uint err;           //错误判断 特征值
	Time = 0;
    //---初始信号(起始码)判断---//
	delay(700);	        //7ms
	if(Ir_IN == 0){
		err = 1000;		//1000*10us = 10ms,超过说明接收到错误的信号
		while((Ir_IN == 0)&&(err>0)){	    //等待9ms的低电平,高电平跳出
			delay(1);
			err--;
		}
		if(Ir_IN == 1){			      //如果9ms内有高电平(通讯正常)
			err = 500;
			while((Ir_IN == 1)&&(err>0)){//等待4.5ms的起始高电平过去
				delay(1);
				err--;
			}
    //---初始信号(起始码)判断结束---//
    //---信号处理(用户码、反码，数据码、反码 共计4×8位)开始---//
			for(k = 0;k<4;k++){		//共有4组数据
				for(j = 0;j<8;j++){	//接收一组数据
					err = 60;
					while((Ir_IN == 0)&&(err>0)){//等待信号前面的560us低电平
						delay(1);
						err--;
					}
					err = 500;
					while((Ir_IN == 1)&&(err>0)){//计算高电平时长，需0.565~1.69
						delay(10);	          //0.1ms
						Time++;
						err--;
						if(Time>30)           //通讯异常，函数终止
							return;
					}
					Ir_Value[k]>>= 1;	    //k表示第几组数据
					if(Time>= 8)		    //如果高电平出现大于565us，那么是1
						Ir_Value[k]|= 0x80;
					Time = 0;		//用完时间要重新赋值
                    //位接收结束
				}
                //字节接收结束
			}
            //数据接收结束
		}
		if(Ir_Value[2]!= ~Ir_Value[3])//
			return;
	}
}
/*****************************************************************
* 函数名             : Ir_Send()
* 函数功能		     : 发射红外信号
* 输入               : 32位红外信号
* 输出               : 无
***************************************************************/
void IR_Send(unsigned long dat)
{
	uchar num, num1, IR_CodeBuf[4];
	IR_CodeBuf[0] = dat&0xff;
	IR_CodeBuf[1] = (dat&0xff00)>>8;
	IR_CodeBuf[2] = (dat&0xff0000)>>16;
	IR_CodeBuf[3] = (dat&0xff000000)>>24;

	Ir_OUT = 1;
	delay(200);
	Ir_OUT = 0;
	delay(900);
	Ir_OUT = 1;
	delay(450);
	for(num  =  0; num < 4; num++)
	{
		for(num1  =  0; num1 < 8; num1++)
		{
			delay(56);//emit 0.56ms
			if (IR_CodeBuf[num] & 0x01)
			{
				delay(169);//delay 1.685ms
			}
			else
			{
				delay(56);//delay 0.56ms
			}
			IR_CodeBuf[num] >>=  1;
		}
	}
	delay(60);//emit 600us
}