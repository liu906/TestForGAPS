Bitonic sort实现   大连理工大学 刘旭同
=== 
a)算法描述
--------
按段读出数据，每段的数据复制到一个新的数组中，并且用FLT_MAX(浮点数所能表示的最大值)把新的数组长度填充到2^n,然后对这个新数组执行bitonic sort算法。</br>
根据题目，输入的各条数据格式有严格约束，所以未对数组长度等数据的合法性进行检查。</br>

b)尝试过和完成了的加分挑战
----------------
<strong>（以下条目是尝试并完成了的加分挑战）</strong></br>
 <span><strong>1.不递归：segmentedBitonicSort函数及其所调用的任何其他函数都不得直接或 
间接地进行递归。</strong></span>

我直接使用了这里的一个非递归实现的函数void bitonic_sort(float *data, int N)：</br>https://stackoverflow.com/questions/40207668/understanding-bitonic-sort-code

在 void segmentedBitonicSort(float* data, int* seg_id, int* seg_start, int
n, int m)函数里分段调用bitonic_sort_fun()函数
~~~cpp
void segmentedBitonicSort(float* data, int* seg_id, int* seg_start, int
n, int m)
{
    for(int i=0;i<m;i++)
    {
        bitonic_sort_fun(data,seg_start[i],seg_start[i+1]);
    }
}
~~~
在bitonic_sort_fun()里对长度不符合2^n的数组做填充处理，然后把数组传给bitonic_sort()

~~~cpp
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
    for(int counter=0;counter<len;counter++)
    {
        data[left+counter] = seg[counter];
    }

}
~~~
 <span><strong>2.不调用函数：segmentedBitonicSort不调用除标准库函数外的任何其他函数。</strong></span></br></br>
 把所有被segmentedBitonicSort调用的函数写进segmentedBitonicSort里面即可。</br>
 
 </br><span><strong>3.内存高效：segmentedBitonicSort及其所调用的任何其他函数都不得进行动态内存分配，包括malloc、new和静态定义的STL容器。</strong></span>
 </br></br>原本为了把长度不足2^n的数据填充到2^n,先算出了所需的最小的`2^n=full`，然后`float* seg = new float[full]`，再在多余位置填充`FLT_MAX`，它表示float所能表示的最大值。</br></br>
现在为了实现内存高效，不能使用`new`来开辟内存，所以直接`float* seg = new float[200000]`,缺点是这样一来每一段的最多只能包含200000个数据了。</br></br>

 <span><strong>5.不需内存：segmentedBitonicSort不调用任何函数（包括C/C++标准库函数)，不使用全局变量，所有局部变量都是int、float或指针类型，C++程序不使用new关键字。</strong></span></br></br>
不使用全局变量所以不能引入`#include<cfloat>`来使用`FLT_MAX`表示float范围内的最大值，所有出现FLT_MAX的地方都用`3.40282e+038`代替即可。</br></br>

 <span><strong>6、绝对鲁棒：在输入数据中包含NaN时（例如sqrt(-1.f)），保证除NaN以外的数据正确排序，NaN的个数保持不变。</strong></span></br></br>
 既然不能调用标准库函数`is_nan`来判断一个数是否是NaN，那我们自己写一个if语句来判断：</br></br>
 ~~~cpp
 int count_nan = 0; //计算该段内NaN的个数
 
 for(int index=left,j = 0;index<right;index++,j++)
    {
        /************处理NaN************/
        if(data[index]==data[index]) seg[j] = data[index];//不是NaN的情况
        else
        {
            seg[j] = 3.40282e+038; //如果是NaN，暂时把NaN赋值成FLT_MAX,等到排序结束后，在利用count_nan把这些数恢复成NaN
            count_nan++;
        }
    }
 ~~~



c)可以独立运行的源代码
---------------------
<em>为了产生一个NaN，引入了math.h，希望这样不违反“不需内存”规则</em>
~~~cpp
#include <iostream>
#include <math.h>
using namespace std;


void segmentedBitonicSort(float* data, int* seg_id, int* seg_start, int
n, int m)
{
    for(int seg_i=0;seg_i<m;seg_i++)
    {
        int left = seg_start[seg_i];
        int right = seg_start[seg_i+1];

        int len = right - left;
        int full;
        for(full=1;full<len;full = full<<1){}
        cout<<"full = "<<full<<endl;

        //float *seg = new float[full];  每一段里面最多有200000个数字
        float seg[200000];

        if(full>len)
        {
            for(int j=len;j<full;j++)
            {
                //补充的值从全局变量MAX替换成3.40282e+38
                seg[j] = 3.40282e+038;
            }
        }
        int count_nan = 0;
        for(int index=left,j = 0;index<right;index++,j++)
        {
            /************处理NaN************/
            if(data[index]==data[index]) seg[j] = data[index];
            else
            {
                seg[j] = 3.40282e+038;
                count_nan++;
            }
        }
        /****************** 下面插入bitonic_sort ********************/
        int N = full;
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
                        if ((i&k) == 0 && seg[i] > seg[ixj]) {
                            temp = seg[i];
                            seg[i] = seg[ixj];
                            seg[ixj] = temp;
                        }

                        if ((i&k) != 0 && seg[i] < seg[ixj]) {
                            temp = seg[i];
                            seg[i] = seg[ixj];
                            seg[ixj] = temp;
                        }

                    }
                }
            }
        }
        /*******************************插入完毕*******************************/
        //bitonic_sort(seg,i);
        for(int p=0;p<full;p++)
        {
            cout<<seg[p]<<" ";
        }
        cout<<endl;
        for(int p=len-count_nan,counter=0;counter<count_nan;counter++,p++)
        {
            seg[p] = sqrt(-1.0);
        }
        for(int p=0;p<full;p++)
        {
            cout<<seg[p]<<" ";
        }
        cout<<endl;
        for(int counter=0;counter<len;counter++)
        {
            data[left+counter] = seg[counter];
        }
    }

}

int main()
{

    //float data[11]={0.8, 0.2, sqrt(-1.f), 0.6, 0.5,20,sqrt(-1.f),8,6,-0.5,4};
float data[8]={-8888888,sqrt(-1.f),sqrt(-1.f),-3333333,-1111111,0,0,3};
int seg_id[8]={0,0,0,0,1,1,1,1};
int seg_start[3]={0,4,8};
int n=8;
int m=2;

    segmentedBitonicSort(data,seg_id,seg_start,n,m);

    cout<<"\nres is :"<<endl;
    for(int i=0;i<n;i++)
    {
        cout<<data[i]<<" ";
    }
    cout<<endl;
}
~~~

d)测试数据
-------------
~~~
//case1:
float data[11]={0.8,0.2,7,sqrt(-1.f),0.5,20,7,8,sqrt(-1.f),-0.5,4};
int seg_id[11]={0, 0, 1, 1, 1, 2, 2, 2, 2, 2, 2};
int seg_start[4]={0,2,5,11};
int n=11;
int m=3;

res is :
0.2 0.8 0.5 7 nan -0.5 4 7 8 20 nan

//case2:
float data[2]={1/0.0,-1/0.0};
int seg_id[2]={0, 0};
int seg_start[2]={0,2};
int n=2;
int m=1;

res is :
-inf inf

//case3:
float data[5]={1111111,2222222,3333333,4444444,7777777};
int seg_id[5]={0,0,0,0,1};
int seg_start[3]={0,4,5};
int n=5;
int m=2;

res is :
1.11111e+006 2.22222e+006 3.33333e+006 4.44444e+006 7.77778e+006

//case4:
float data[5]={7777777,4444444,2222222,3333333,1111111};
int seg_id[5]={0,0,0,0,1};
int seg_start[3]={0,4,5};
int n=5;
int m=2;

res is :
2.22222e+006 3.33333e+006 4.44444e+006 7.77778e+006 1.11111e+006

//case5:

float data[5]={8888888,sqrt(-1.f),sqrt(-1.f),3333333,1111111};
int seg_id[5]={0,0,0,0,1};
int seg_start[3]={0,4,5};
int n=5;
int m=2;

res is :
3.33333e+006 8.88889e+006 nan nan 1.11111e+006

//case6:

float data[8]={-8888888,sqrt(-1.f),sqrt(-1.f),-3333333,-1111111,0,0,3};
int seg_id[8]={0,0,0,0,1,1,1,1};
int seg_start[3]={0,4,8};
int n=8;
int m=2;

res is :
-8.88889e+006 -3.33333e+006 nan nan -1.11111e+006 0 0 3
~~~

e)性能分析
----------------
双调排序的时间复杂度是n(logn)^2这样看来的话它的性能似乎还不如快速排序。
[但相较于传统的排序算法，排序网络真正的研究价值在于，假如有机器可以同时处理多个比较器，排序的速度将大幅度提高。
简单来说，它是一种可以并行计算的排序算法。(摘自百度百科)](https://baike.baidu.com/item/%E5%8F%8C%E8%B0%83%E6%8E%92%E5%BA%8F/9598017?fr=aladdin)

f)测试的起始和完成时间以及实际使用的时间
----------------------------
4月22日收到邮件之后大概看了一下bitonic sort算法的讲解就去复习今天的一门专业课考试了。今天从晚上6点开始查资料到编程、测试结束，大概用了将近3个小时。
写文档用了将近1小时。
