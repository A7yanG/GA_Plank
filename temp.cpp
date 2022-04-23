#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
using namespace std;

typedef struct SequencePair
{
    vector<int> X;
    vector<int> Y;
    int fitness = 0;
};
static int num = 5;                  //木板数量(序列长度)
static vector<SequencePair> PopMain; //主种群
static vector<SequencePair> PopTemp; //临时种群
static int MainSize = 20;            //主种群大小
static int TempSize = MainSize / 2;  //临时种群大小
static float XoverPos = 0.1;             //杂交概率
static int MaxTryTime = 10;           //最大重试次数
static float Percentage=0.2;           //亲属比例

//输出序列对
void printSequencePair(SequencePair s)
{
    cout << "S.X: ";
    for (int i = 0; i < num; i++)
    {
        cout << s.X[i] << " ";
    }
    cout << endl
         << "S.Y: ";
    for (int i = 0; i < num; i++)
    {
        cout << s.Y[i] << " ";
    }
    cout << endl;
}
//计算两个序列之间的距离
int Calc_distance(vector<int> X, vector<int> Y)
{
    vector<int>::iterator iterX_1;
    vector<int>::iterator iterX_2;
    vector<int>::iterator iterY_1;
    vector<int>::iterator iterY_2;
    int C = 0;
    for (int i = 0; i < num; i++)
    {
        iterX_1 = find(X.begin(), X.end(), i);
        iterY_1 = find(Y.begin(), Y.end(), i);
        for (int j = i + 1; j < num; j++)
        {
            int flagX = 0; // X序列标志
            int flagY = 0; // X序列标志
            iterX_2 = find(X.begin(), X.end(), j);
            iterY_2 = find(Y.begin(), Y.end(), j);
            if (iterX_1 > iterX_2)
                flagX = 1;
            if (iterY_1 > iterY_2)
                flagY = 1;
            if (!(flagX ^ flagY)) //异或
                C++;
        }
    }
    return num * (num - 1) / 2 - C;
}
//计算两个序列对之间的距离
int Calc_SP_distance(SequencePair s1, SequencePair s2)
{
    int distance = 0;
    distance += Calc_distance(s1.X, s2.X) + Calc_distance(s1.Y, s2.Y);
    return distance;
}
// Swap交换,任选序列对中的一个序列,选取2个元素并交换之
void Swap(SequencePair &s, int index1, int index2)
{

    int temp = 0;
    if (rand() % 2)
    {
        temp = s.X[index1];
        s.X[index1] = s.X[index2];
        s.X[index2] = temp;
    }
    else
    {
        temp = s.Y[index1];
        s.Y[index1] = s.Y[index2];
        s.Y[index2] = temp;
    }
}
// Swap+交换,交换时对 2个序列相同位置的元素同时进行操作
void Swap_plus(SequencePair &s, int index1, int index2)
{
    int temp = 0;
    temp = s.X[index1];
    s.X[index1] = s.X[index2];
    s.X[index2] = temp;
    temp = s.Y[index1];
    s.Y[index1] = s.Y[index2];
    s.Y[index2] = temp;
}

//获得某个序列对的邻域
vector<SequencePair> Get_neighbor_field(SequencePair s)
{
    vector<SequencePair> v;
    SequencePair s1;
    for (int i = 0; i < num; i++)
    {
        for (int j = i + 1; j < num; j++)
        {
            s1 = s;
            Swap_plus(s1, i, j);
            v.push_back(s1);
        }
    }
    return v;
}
//获得某个邻域的最优解
SequencePair Get_best_solution_from_field(vector<SequencePair> Neighbor_field)
{
}

//判断两个序列对是否相等
bool Isequal(SequencePair s1, SequencePair s2)
{
    for (int i = 0; i < num; i++)
    {
        if (s1.X[i] != s2.X[i] || s1.Y[i] != s2.Y[i])
            return false;
    }
    return true;
}
//获得局部最优解
SequencePair Get_locally_best_solution(SequencePair s)
{
    while (1)
    {
        vector<SequencePair> Neighbor_field = Get_neighbor_field(s);    //获得邻域
        SequencePair s1 = Get_best_solution_from_field(Neighbor_field); //
        if (Isequal(s, s1))
            return s1;
        else
            s = s1;
    }
}
//生成随机序列
vector<int> Generate_Sequence()
{
    vector<int> temp;
    int index = rand() % num;
    for (int i = 0; i < num; i++)
    {
        int j = 0;
        while (j < i)
        {
            if (temp[j] == index)
            {
                j = 0;
                index = rand() % num;
                continue;
            }
            else
                j++;
        }
        temp.push_back(index);
    }
    return temp;
}
//生成主种群
void Generate_PopMain()
{
    for (int t = 0; t < MainSize; t++)
    {
        SequencePair s;
        s.X = Generate_Sequence();
        s.Y = Generate_Sequence();
        PopMain.push_back(s);
        cout<<"Pop"<<t+1<<":"<<endl;
        printSequencePair(s);
    }
}
//计算适应值
void Calc_fitness()
{
}
//从主种群中选出适应值靠前的一半设为PopTemp
void Generate_PopTemp(vector<SequencePair> s)
{
    vector<SequencePair> vec;
    //按照适应值排序
    for (int i = 0; i < num - 1; i++)
    { //控制n-1趟冒泡
        for (int j = 0; j < num - 1 - i; j++)
        {
            if (s[j].fitness > s[j + 1].fitness)
            {               //比较相邻的两个元素
                SequencePair tmp;    //临时变量
                tmp = s[j]; //交换
                s[j] = s[j + 1];
                s[j + 1] = tmp;
            }
        }
    }
    for (int i = MainSize/2; i < MainSize; i++)
        vec.push_back(s[i]);
    PopTemp=vec;
}
//在PopTemp中取两个个体G1、G2为父代
void Get_parents_from_PopTemp(SequencePair &s1,SequencePair &s2){
    int index1=rand() %TempSize;
    s1=PopTemp[index1];
    int index2=rand() %TempSize;
    while(index2==index1)
         index2=rand() %TempSize;
    s2=PopTemp[index2];
}
//按次序交叉
void Cross_by_order(SequencePair s1,SequencePair s2,SequencePair offspring1,SequencePair offspring2){
    //随机获得起止位置
    int start=rand()%num;
    int stop=rand()%num;
    while(start==stop)
        stop=rand() %num;
    if(stop>start)
    {
        int temp=stop;
        stop=start;
        start=temp;
    }
    vector<int> vec;
    std::vector<int>::iterator it;
    for(int i=0;i<num;i++)
    {
        it=find(s1.X.begin()+start,s1.X.begin()+stop,s2.X[i]);
        if(it==s1.X.begin()+stop)//查找失败
            vec.push_back(s2.X[i]);
    }
    int i=num-1;
    while(i>=0)
    {
        if(i>=start&&i<=stop)
        {
            i--;
            break;
        }

    }
}   

//检验
void Check(){
    int Relative_num=MainSize*Percentage;
    //随机获得亲属
    vector<int> Relatives;
    int index = rand() % MainSize;
    for (int i = 0; i < Relative_num; i++)
    {
        int j = 0;
        while (j < i)
        {
            if (Relatives[j] == index)
            {
                j = 0;
                index = rand() % MainSize;
                continue;
            }
            else
                j++;
        }
        Relatives.push_back(index);
    }

    int TryTime=0;
    while(TryTime<MaxTryTime)//2个子代不能与随机选择的亲戚保持确定的距离Thresh
    {
        
        TryTime++;
    }
}


//种群演化
void GA(){
    SequencePair G1,G2;
    Get_parents_from_PopTemp(G1,G2);
    int _random=rand()%101;
    if((float)_random/100>XoverPos)
    {
        GA();
    }
    else//杂交检验变异
    {
        
    }
}
int main()
{
    SequencePair SP;

    for (int i = 0; i < num; i++)
    {
        SP.X.push_back(i); //依次把整数数值添加到vb2的末尾
    }
    SP.Y.push_back(0);
    SP.Y.push_back(1);
    SP.Y.push_back(3);
    SP.Y.push_back(2);
    SP.Y.push_back(4);
    // vector<SequencePair> Neighbor_field=Get_neighbor_field(SP);
    // vector <SequencePair> ::iterator iter;
    // for(iter=Neighbor_field.begin();iter!=Neighbor_field.end();iter++)
    //     printSequencePair(*iter);
    srand((int)time(0));
    Generate_PopMain();
    Generate_PopTemp(PopMain);
    for(int i=0;i<TempSize;i++){
        printSequencePair(PopTemp[i]);
    }
    system("pause");
    return 0;
}
