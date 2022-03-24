/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
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
float dataOutput3[3][10][10] = {0};
float dataOutput4[50]={0};
float dataOutput5[10]={0};
extern float dataInput[28][28];
extern float weightConv1[6][3][3];
extern float weightConv2[3][6][3][3];
extern float weightFc1[50][300];
extern float weightFc2[10][50];
extern float biasFc1[50];
extern float biasFc2[10];
extern float biasConv1[6];
extern float biasConv2[3];

int res = 0;

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
                .dims = {1, 3, 10, 10}
                };
Tensor output4 = {
                .data = dataOutput4,
                .dims = {1, 1, 1, 50}};
Tensor output5 = {
                .data = dataOutput5,
                .dims = {1, 1, 1, 10}};
Tensor conv1 = {
                .data = weightConv1,
                .dims = {6, 1, 3, 3}};
Tensor conv2 = {
                .data = weightConv2,
                .dims = {3, 6, 3, 3}};
Tensor fc1 = {
                .data = weightFc1,
                .dims = {1, 1, 50, 300}};
Tensor fc2 = {
                .data = weightFc2,
                .dims = {1, 1, 10, 50}};



float relu(float x)
{
    if(x>0) return x;
    else return 0;
}

int core0_main(void)
{
	get_clk();
	gpio_init(P22_3, GPI, 1, PULLUP);

	IfxCpu_emitEvent(&g_cpuSyncEvent);
	IfxCpu_waitEvent(&g_cpuSyncEvent, 0xFFFF);
	enableInterrupts();

	while(TRUE)
	{
	    MaxPool1(2, 2, &input, &output1);

	    Conv(1, 1, &output1, &output2, &conv1, biasConv1);

	    for(int i = 0; i < 864; i++)
        {
	        output2.data[i] = relu(output2.data[i]);
        }

	    Conv(1, 1, &output2, &output3, &conv2, biasConv2);

	    for(int i = 0; i < 300; i++)
        {
            output3.data[i] = relu(output3.data[i]);
        }

	    Dense(&output3, &output4, &fc1, biasFc1);

	    for(int i = 0; i < 50; i++)
        {
	        output4.data[i] = relu(output4.data[i]);
        }

	    Dense(&output4, &output5, &fc2, biasFc2);

	    res = FindMax(output5.data);

        printf("%d\n", res);
	}
}

#pragma section all restore
