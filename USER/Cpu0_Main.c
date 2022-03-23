/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		ADS v1.2.2
 * @Target core		TC377TP
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-11-23
 ********************************************************************************************************************/

#include "Cpu0_Main.h"
#include "headfile.h"
#include "math.h"


#pragma section all "cpu0_dsram"

float dataOutput1[14][14] = {0};
float dataOutput2[6][12][12] = {0};
float dataOutput3[6][6][6] = {0};
float dataOutput4[10]={0};
extern float dataInput[28][28];
extern float weightConv[6][3][3];
extern float weightFc1[10][216];

Tensor input = {
                .data = dataInput,
                .dims = {1, 1, 28, 28}};
Tensor output1 = {
                .data = dataOutput1,
                .dims = {1, 1, 14, 14}};
Tensor output2 = {
                .data = dataOutput2,
                .dims = {1, 6, 12, 12}};
Tensor output3 = {
                .data = dataOutput3,
                .dims = {1, 6, 6, 6}
                };
Tensor output4 = {
                .data = dataOutput4,
                .dims = {1, 1, 1, 10}};
Tensor conv = {
                .data = weightConv,
                .dims = {6, 1, 3, 3}};
Tensor fc1 = {
                .data = weightFc1,
                .dims = {1, 1, 10, 216}};



float relu(float x)
{
    if(x>0) return x;
    else return 0;
}

int core0_main(void)
{
	get_clk();

	IfxCpu_emitEvent(&g_cpuSyncEvent);
	IfxCpu_waitEvent(&g_cpuSyncEvent, 0xFFFF);
	enableInterrupts();

	while(TRUE)
	{
	    MaxPool1(2, 2, &input, &output1);

	    Conv(1, 1, &output1, &output2, &conv);

	    for(int i = 0; i < 864; i++)
        {
	        output2.data[i] = relu(output2.data[i]);
        }

	    MaxPool2(2, 2, &output2, &output3);

	    Dense(&output3, &output4, &fc1);

	    for(int i = 0; i < 10; i++)
        {
	        output4.data[i] = relu(output4.data[i]);
        }

//	    for(int j = 0; j < 10; j++)
//	    {
//	        printf("%f ", output4.data[j]);
//	    }
//	    printf("\n");
	}
}

#pragma section all restore
