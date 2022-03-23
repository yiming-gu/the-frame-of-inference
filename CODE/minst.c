#include "minst.h"




float Max(uint8 x, uint8 y, uint8 z, uint8 w);



void Conv(uint8 stride_h, uint8 stride_w, Tensor *input,Tensor *output, Tensor *filter)
{
    uint8 OC = output->dims[1];
    uint8 OH = output->dims[2];
    uint8 OW = output->dims[3];
    uint8 IH = input->dims[2];
    uint8 IW = input->dims[3];
    uint8 FC = filter->dims[1];
    uint8 FH = filter->dims[2];
    uint8 FW = filter->dims[3];
    uint8 oc,oh,ow;
    uint8 fc,fh,fw;
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
                output->data[output_idx] = val;
            }
        }
    }
}


void Dense(Tensor* input, Tensor* output, Tensor* filter)
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
    }
    // remain
    for (h = 0; h < h_remains; ++h)
    {
        int idx  = (H - h - 1) * W;
        float* f = filter->data + idx;
        float* o = output->data + H - h - 1;
        float* i = input->data;
        *o = 0;
        for (w = 0; w < W; ++w)
        {
            *o += ((*f) * (*i));

            f ++;
            i ++;
        }
    }

}



void MaxPool1(uint8 stride_h, uint8 stride_w, Tensor *input,Tensor *output)
{
    uint8 OC = output->dims[1];
    uint8 OH = output->dims[2];
    uint8 OW = output->dims[3];
    uint8 IH = input->dims[2];
    uint8 IW = input->dims[3];
    uint8 oc,oh,ow;
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

void MaxPool2(uint8 stride_h, uint8 stride_w, Tensor *input,Tensor *output)
{
    uint8 OC = output->dims[1];
    uint8 OH = output->dims[2];
    uint8 OW = output->dims[3];
    uint8 IH = input->dims[2];
    uint8 IW = input->dims[3];
    uint8 oc,oh,ow;
    for (oc = 0; oc < OC; ++oc)
    {
        for (oh = 0; oh < OH; ++oh)
        {
            for (ow = 0; ow < OW; ++ow)
            {
//                float val = 0.0f;
                int input_idx =
                        ow * stride_w +
                        oh * stride_h * IW +
                        oc * IH * IW;
                int output_idx =
                        ow +
                        oh * OW +
                        oc * OH * OW;
                output->data[output_idx] = Max(input->data[input_idx], input->data[input_idx+1], input->data[input_idx+IW], input->data[input_idx+IW+1]);
//                 = val;
            }
        }
    }
}



float Max(uint8 x, uint8 y, uint8 z, uint8 w)
{
    float res = x;
    if(y>res) res=y;
    if(z>res) res=z;
    if(w>res) res=w;
    return res;
}



