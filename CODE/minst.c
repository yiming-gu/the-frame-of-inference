#include "minst.h"




float Max(float x, float y, float z, float w);



void Conv(uint8 stride_h, uint8 stride_w, Tensor *input,Tensor *output, Tensor *filter, float* bias)
{
    int OC = output->dims[1];
    int OH = output->dims[2];
    int OW = output->dims[3];
    int IH = input->dims[2];
    int IW = input->dims[3];
    int FC = filter->dims[1];
    int FH = filter->dims[2];
    int FW = filter->dims[3];
    int oc,oh,ow;
    int fc,fh,fw;
    for (oc = 0; oc < OC; ++oc)
    {
        for (oh = 0; oh < OH; ++oh)
        {
            for (ow = 0; ow < OW; ++ow)
            {
                float val = 0;
                int   base_ow = ow * stride_w;
                int   base_oh = oh * stride_h;
                int   base_ss = oc * FC * FH * FW;

                for (fc = 0; fc < FC; ++fc)
                {
                    for (fh = 0; fh < FH; ++fh)
                    {
                        for (fw = 0; fw < FW; ++fw)
                        {
                            int input_idx =
                                    (base_ow + fw) +
                                    (base_oh + fh) * IW +
                                    fc * IH * IW;
                            int filter_idx =
                                    fw +
                                    fh * FW +
                                    fc * FH * FW +
                                    base_ss;
                            val += (input->data[input_idx] * filter->data[filter_idx]);
                        }
                    }
                }
                int output_idx =
                        ow +
                        oh * OW +
                        oc * OH * OW;
                output->data[output_idx] = val + bias[oc];
            }
        }
    }
}


void Dense(Tensor* input, Tensor* output, Tensor* filter, float* bias)
{
    int H = filter->dims[2];
    int W = filter->dims[3];

    int h, w;
    int h_iters  = floor(H / 4);
    int h_remains = H % 4;

    // unroll
    for (h = 0; h < h_iters; ++h)
    {
        int idx0 = (h * 4 + 0) * W;
        int idx1 = (h * 4 + 1) * W;
        int idx2 = (h * 4 + 2) * W;
        int idx3 = (h * 4 + 3) * W;

        float* f0 = filter->data + idx0;
        float* f1 = filter->data + idx1;//取出四行
        float* f2 = filter->data + idx2;
        float* f3 = filter->data + idx3;

        float* o0 = output->data + h * 4 + 0;
        float* o1 = output->data + h * 4 + 1;
        float* o2 = output->data + h * 4 + 2;
        float* o3 = output->data + h * 4 + 3;

        float* b0 = bias + h * 4 + 0;
        float* b1 = bias + h * 4 + 1;
        float* b2 = bias + h * 4 + 2;
        float* b3 = bias + h * 4 + 3;

        float* ii = input->data;

        *o0 = 0;
        *o1 = 0;
        *o2 = 0;
        *o3 = 0;
        for (w = 0; w < W; ++w)
        {
            *o0 += ((*f0) * (*ii));
            *o1 += ((*f1) * (*ii));
            *o2 += ((*f2) * (*ii));
            *o3 += ((*f3) * (*ii));

            f0 ++;
            f1 ++;
            f2 ++;
            f3 ++;
            ii ++;
        }
        *o0 += (*b0);
        *o1 += (*b1);
        *o2 += (*b2);
        *o3 += (*b3);
    }
    // remain
    for (h = 0; h < h_remains; ++h)
    {
        int idx  = (H - h - 1) * W;
        float* f = filter->data + idx;
        float* o = output->data + H - h - 1;
        float* b = bias + H - h - 1;
        float* i = input->data;
        *o = 0;
        for (w = 0; w < W; ++w)
        {
            *o += ((*f) * (*i));

            f ++;
            i ++;
        }
        *o += (*b);
    }

}



void MaxPool1(uint8 stride_h, uint8 stride_w, Tensor *input,Tensor *output)
{
    int OC = output->dims[1];
    int OH = output->dims[2];
    int OW = output->dims[3];
    int IH = input->dims[2];
    int IW = input->dims[3];
    int oc,oh,ow;
    for (oc = 0; oc < OC; ++oc)
    {
        for (oh = 0; oh < OH; ++oh)
        {
            for (ow = 0; ow < OW; ++ow)
            {
                float val = 0.0f;
                int input_idx =
                        ow * stride_w +
                        oh * stride_h * IW +
                        oc * IH * IW;
                int output_idx =
                        ow +
                        oh * OW +
                        oc * OH * OW;
                val = Max(input->data[input_idx], input->data[input_idx+1], input->data[input_idx+IW], input->data[input_idx+IW+1]);
                output->data[output_idx] = val;

            }
        }
    }
}



float Max(float x, float y, float z, float w)
{
    float res = x;
    if(y>res) res=y;
    if(z>res) res=z;
    if(w>res) res=w;
    return res;
}

int FindMax(float* x)
{
    float res = x[0]; int a = 0;
    if(x[1]>res) {res=x[1]; a = 1;};
    if(x[2]>res) {res=x[2]; a = 2;};
    if(x[3]>res) {res=x[3]; a = 3;};
    if(x[4]>res) {res=x[4]; a = 4;};
    if(x[5]>res) {res=x[5]; a = 5;};
    if(x[6]>res) {res=x[6]; a = 6;};
    if(x[7]>res) {res=x[7]; a = 7;};
    if(x[8]>res) {res=x[8]; a = 8;};
    if(x[9]>res) {res=x[9]; a = 9;};
    return a;
}



