#include <iostream>
#include <algorithm>

const float MAX = 1000000;
using namespace std;

void bitonic_sort(float *data, int N)
{
    cout<<"N = "<<N<<endl;
    int i, j, k;
    float temp;

        for (k = 2;k <= N;k = 2 * k)
        {
            for (j = k >> 1;j>0; j = j >> 1)
            {
                for (i = 0;i<N;i++)
                {
                    int ixj = i^j;

                    if ((ixj)>i)
                    {
                        if ((i&k) == 0 && data[i] > data[ixj]) {
                            temp = data[i];
                            data[i] = data[ixj];
                            data[ixj] = temp;
                        }

                        if ((i&k) != 0 && data[i] < data[ixj]) {
                            temp = data[i];
                            data[i] = data[ixj];
                            data[ixj] = temp;
                        }

                    }
                }
            }
        }

}
void bitonic_sort_fun(float* data,int left,int right)
{
    int len = right - left;
    int i;
    for(i=1;i<len;i = i<<1){}
    cout<<"i = "<<i<<endl;

    float *seg = new float[i];
    if(i>len)
    {
        for(int j=len;j<i;j++)
        {
            seg[j] = MAX;
        }
    }
    for(int index=left,j = 0;index<right;index++,j++)
    {
        seg[j] = data[index];
    }
    bitonic_sort(seg,i);
    for(int p=0;p<i;p++)
    {
        cout<<seg[p]<<" ";
    }
    cout<<endl;
    for(int counter=0;counter<len;counter++)
    {
        data[left+counter] = seg[counter];
    }

}

void segmentedBitonicSort(float* data, int* seg_id, int* seg_start, int
n, int m)
{
    for(int i=0;i<m;i++)
    {

        bitonic_sort_fun(data,seg_start[i],seg_start[i+1]);
    }

}

int main()
{

    float data[11]={0.8, 0.2, 0.4, 0.6, 0.5,20,20,8,6,-0.5,4};
    int seg_id[11]={0,   0,   1,   1,  1,2,2,2,2,2,2};
    int seg_start[4]={0,2,5,11};
    int n=11;
    int m=3;

    segmentedBitonicSort(data,seg_id,seg_start,n,m);
    for(int i=0;i<11;i++)
    {
        cout<<data[i]<<" ";
    }
    cout<<endl;
}
