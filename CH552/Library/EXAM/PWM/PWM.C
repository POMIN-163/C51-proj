/********************************** (C) COPYRIGHT *******************************
* File Name          : PWM.C
* Author             : WCH
* Version            : V1.3
* Date               : 2016/6/24
* Description        : CH559 PWM�ӿں���             				   
*******************************************************************************/
#include "..\DEBUG.C"                                                         //������Ϣ��ӡ
#include "..\DEBUG.H"

#pragma  NOAREGS

#define SetPWMClk(CK_SE) (PWM_CK_SE = CK_SE)                                  //��Ƶ,Ĭ��ʱ��Fsys            
#define SetPWMCycle(Cycle) (PWM_CYCLE = Cycle)                                //����ѭ������
#define SetPWM1Dat(dat) (PWM_DATA = dat)                                      //����PWM���ռ�ձ�
#define SetPWM2Dat(dat) (PWM_DATA2 = dat)
#define PWMPINAlter( )  { P4_DIR |= bPWM2_ | bPWM1_;PIN_FUNC |= bPWM1_PIN_X;} //����PWM����ӳ��

/*******************************************************************************
* Function Name  : InitPWM1(UINT8 polar)
* Description    : PWM1��ʼ������
* Input          : polar=0ѡ��Ĭ�ϵ͵�ƽ���ߵ�ƽ�����Ч;
                   polar=1ѡ��Ĭ�ϸߵ�ƽ���͵�ƽ�����Ч;
* Output         : None
* Return         : None
*******************************************************************************/
void  InitPWM1(UINT8 polar)
{
    PWM_CTRL &= ~bPWM_CLR_ALL;                                                //���FIFO�ͼ���                                                      
    PWM_CTRL &= ~bPWM_MOD_MFM;
    PWM_CTRL |=	bPWM_IE_END;                                                  //ʹ��PWM������������ж�
    PWM_CTRL |=	bPWM_OUT_EN;                                                  //PWM1���ʹ��
    PWM_CTRL  |= bPWM_IF_END;                                                 //������е��жϱ�־
    if(polar){
        PWM_CTRL |= bPWM_POLAR;                                               //�͵�ƽ��Ч
    }
    else{
        PWM_CTRL &= ~bPWM_POLAR;                                              //�ߵ�ƽ��Ч  
    }			
}

/*******************************************************************************
* Function Name  : InitPWM2(UINT8 polar)
* Description    : PWM��ʼ������
* Input          : polar=0ѡ��Ĭ�ϵ͵�ƽ���ߵ�ƽ�����Ч;
                   polar=1ѡ��Ĭ�ϸߵ�ƽ���͵�ƽ�����Ч;
* Output         : None
* Return         : None
*******************************************************************************/
void  InitPWM2(UINT8 polar)
{
    PWM_CTRL &= ~bPWM_CLR_ALL;                                                //���FIFO�ͼ���                                                      
    PWM_CTRL &= ~bPWM_MOD_MFM;
    PWM_CTRL |=	bPWM_IE_END;                                                  //ʹ��PWM������������ж�
    PWM_CTRL |= bPWM2_OUT_EN;                                                 //PWM2���ʹ��	
    PWM_CTRL  |= bPWM_IF_END;                                                 //������е��жϱ�־
    if(polar){
        PWM_CTRL |= bPWM2_POLAR;                                              //�͵�ƽ��Ч
    }
    else{
        PWM_CTRL &= ~bPWM2_POLAR;                                             //�ߵ�ƽ��Ч  
    }			
}

/*******************************************************************************
* Function Name  : PWMInterrupt(void)
* Description    : PWM�жϷ������   
*******************************************************************************/
void	PWMInterrupt( void ) interrupt INT_NO_PWM1 using 1                     //PWM1&2�жϷ������,ʹ�üĴ�����1
{
    if(PWM_CTRL & bPWM_IF_END)                                                 
    {
        PWM_CTRL |= bPWM_IF_END;
        printf("PWM_DATA  %02X\n",(UINT16)PWM_DATA);
    }                                                                
}

main( ) 
{
    mDelaymS(30);                                                              //�ϵ���ʱ,�ȴ��ڲ������ȶ�,�ؼ� 
//  CfgFsys( );  
    PORT_CFG &= ~bP2_OC;
    P2_DIR |= bPWM1 | bPWM2;                                                   //����PWM������������Ϊ�������	
    
    mInitSTDIO( );                                                             //����0,�������ڵ���
    printf("start ...\n");
#if 0
    PWMPINAlter( );                                                            //����ӳ��	
#endif
    SetPWMClk(12);                                                             //����PWM1&2��ʱ�ӷ�Ƶϵ��Ϊ12
    InitPWM1(1);                                                               //PWM1��ʼ�����͵�ƽ��Ч
    InitPWM2(0);                                                               //PWM2��ʼ�����ߵ�ƽ��Ч
    SetPWMCycle(100);                                                          //����ѭ������100
    IE_PWM1 = 1;                                                               //ʹ��PWM1�ж�
    SetPWM1Dat(50);                                                            //PWM1ռ�ձ�����50/100
    SetPWM2Dat(50);                                                            //PWM1ռ�ձ�����50/100
    EA = 1;                                                                    //�ж��ܿ���
    while(1);
}
