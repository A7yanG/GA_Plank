#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <numeric>
#include <random>
#include <cmath>
using namespace std;
typedef struct Plank
{
    int plank_Id;   //木板序号
    int x = 0;      //左上角x坐标
    int y = 0;      //左上角y坐标
    int length = 0; //长度
    int width = 0;  //宽度
};

static vector<Plank> Planks;        //木板数组
static const int N = 6;                 //木板数量
static const int PopSize = 20;            //种群大小
static vector<vector<int>> PopMain; //种群
static vector<double> Fit;          //适应度数组
static const double Cross_rate = 0.9;     //交叉概率
static const double Variation_rate = 0.1; //变异概率
static vector<int> BestIndividual;  //适应值最高的个体
static double Max_fitness = 0;      //最大适应度
static int PlankTable[120][120];

vector<Plank> Place(vector<int> vec);
vector<Plank> PlaceByHighLine(vector<int> vec);
//清空矩阵
void ClearPlankTable(){
    for(int i=0;i<120;i++)
    {
        for(int j=0;j<120;j++)
            PlankTable[i][j]=0;
    }
}
//将木板放入矩阵中
void SetPlankInTable(Plank p)
{
    for (int j = p.y; j < p.width + p.y; j++)
        for (int k = p.x; k < p.length + p.x; k++)
            PlankTable[j][k] = p.plank_Id + 1;
}

//输出木板矩阵
void printTable()
{
    ClearPlankTable();
    vector<Plank> _Planks = PlaceByHighLine(BestIndividual);
    for (int i = 0; i < N; i++)
        SetPlankInTable(_Planks[i]);
    for (int i = 0; i < 120; i++)
    {
        for (int j = 0; j < 120; j++)
            cout << PlankTable[i][j];
        cout << endl;
    }
}

//输出个体
void printIndividual(vector<int> vec)
{
    for (int i = 0; i < N; i++)
    {
        cout << vec[i] << " ";
    }
    cout << endl;
}
//输出木板
void printPlanks()
{
    cout << "Planks:" << endl;
    for (int i = 0; i < N; i++)
    {
        cout << "ID:" << Planks[i].plank_Id << " x:" << Planks[i].x << " y:" << Planks[i].y << " length:" << Planks[i].length << " width:" << Planks[i].width << endl;
    }
}
//输出种群
void printPopulation()
{
    for (int i = 0; i < PopSize; i++)
        printIndividual(PopMain[i]);
}
//生成木板
void Generate_Planks()
{
    for (int i = 0; i < N; i++)
    {
        Plank p;
        p.plank_Id = i;
        p.length = rand() % 21 + 20;
        p.width = rand() % 21 + 20;
        Planks.push_back(p);
    }
    printPlanks();
}
//生成个体
vector<int> Generate_Individual()
{
    vector<int> temp;
    for (int i = 0; i < N; i++)
    {
        int index = rand() % N;
        int j = 0;
        while (j < i)
        {
            if (abs(temp[j]) == index)
            {
                j = 0;
                index = rand() % N;
                continue;
            }
            else
                j++;
        }
        if (rand() % 2)
            index = -index;
        temp.push_back(index);
    }
    return temp;
}
//生成主种群
void Generate_PopMain()
{
    srand(int(time(0)));
    cout << "Population:" << endl;
    for (int t = 0; t < PopSize; t++)
    {
        vector<int> vec = Generate_Individual();
        PopMain.push_back(vec);
        cout << "Pop" << t << ":" << endl;
        printIndividual(vec);
    }
}
//最高水平线放置木板
vector<Plank> PlaceByHighLine(vector<int> vec)
{
    vector<Plank> _Planks = Planks;
    for (int t = 0; t < N; t++)
    {
        //将需要旋转的木板进行旋转
         if (vec[t] < 0)
        {
            vec[t] = -vec[t];
            int temp = _Planks[vec[t]].length;
            _Planks[vec[t]].length = _Planks[vec[t]].width;
            _Planks[vec[t]].width = temp;
        }
        int leftLength = 0;
        int start = 0;
        int failure_flag = 0;
        int success_flag = 0;
        int i = 0;//行数
        while (!success_flag)
        {
            leftLength = 0;
            int j = 0;
            while (j <= 120)
            {
                if (leftLength >= _Planks[vec[t]].length)//剩余长度>=木板长度
                {
                    failure_flag = 0;
                    start = j - leftLength;
                    for (int k = i + 1; k < i + _Planks[vec[t]].width; k++)//找到的剩余长度下方空间是否可用
                    {
                        if (PlankTable[k][start] != 0 || PlankTable[k][j - 1] != 0)//检测两条边是否被占用
                        {
                            failure_flag = 1;
                            j = start;
                            break;
                        }
                    }
                    if (failure_flag)//剩余长度下方空间不可用
                    {
                        leftLength = 0;
                        j++;
                        continue;
                    }
                    else//剩余长度下方空间可用
                    {
                        success_flag = 1;
                        _Planks[vec[t]].x = start;
                        _Planks[vec[t]].y = i;
                        break;
                    }
                }
                if (PlankTable[i][j] == 0)
                    leftLength++;
                else
                    leftLength = 0;
                j++;
            }
            i++;
            if(i>=120)
                break;
        }
        SetPlankInTable(_Planks[vec[t]]);
    }
    return _Planks;
}

//放置木板，按照最低水平线原则
vector<Plank> Place(vector<int> vec)
{
    vector<Plank> _Planks = Planks;
    int leftLength = 120;
    int leftWidth = 120;
    int preY = 0;
    for (int i = 0; i < N; i++)
    {
        //将需要旋转的木板进行旋转
        if (vec[i] < 0)
        {
            vec[i] = -vec[i];
            int temp = _Planks[vec[i]].length;
            _Planks[vec[i]].length = _Planks[vec[i]].width;
            _Planks[vec[i]].width = temp;
        }
        //剩余长度>=木板长度并且剩余宽度>=木板宽度
        if (leftLength >= _Planks[vec[i]].length && (120 - preY) >= _Planks[vec[i]].width)
        {
            _Planks[vec[i]].x = 120 - leftLength;
            _Planks[vec[i]].y = preY;
            leftLength -= _Planks[vec[i]].length;
            if (leftWidth + _Planks[vec[i]].width > 120 - preY) //更新最低水平线
            {
                leftWidth = 120 - preY - _Planks[vec[i]].width;
            }
            continue;
        }
        //剩余长度<木板长度,换行,判断剩余宽度是否>=木板宽度
        else if (leftWidth >= _Planks[vec[i]].width)
        {
            leftLength = 120;
            preY = 120 - leftWidth;
            leftLength -= _Planks[vec[i]].length;
            leftWidth -= _Planks[vec[i]].width;
            _Planks[vec[i]].x = 0;
            _Planks[vec[i]].y = preY;
            continue;
        }
        else
        {
            break;
        }
    }
    return _Planks;
}
//计算外接矩形面积(适应度)
void Calc_Fitness(vector<Plank> _Planks)
{
    int max_length = 0;
    int max_width = 0;
    for (int i = 0; i < N; i++)
    {
        if (_Planks[i].x + _Planks[i].length > max_length)
            max_length = _Planks[i].x + _Planks[i].length;
        if (_Planks[i].y + _Planks[i].width > max_width)
            max_width = _Planks[i].y + _Planks[i].width;
    }
    Fit.push_back(double(100000.0 / (max_length * max_width)));
    // cout << "Area:" << double(100000.0/(max_length * max_width))<< endl;
}
//依据精英保留和轮盘赌选择
void Select()
{
    PopMain.push_back(BestIndividual); //保留适应度最高的个体
    double SumFit = 0;
    for (int i = 0; i < PopSize; i++)
        SumFit += Fit[i];
    default_random_engine e(time(0));
    uniform_real_distribution<double> u(0, SumFit);
    for (int t = 0; t < PopSize - 1; t++)
    {

        double temp = u(e);
        // cout<<"temp"<<t<<":"<<temp<<endl;
        double SumTemp = 0;
        for (int i = 0; i < PopSize; i++)
        {
            SumTemp += Fit[i];
            if (SumTemp >= temp)
            {
                PopMain.push_back(PopMain[i]);
                break;
            }
        }
    }
    // cout<<"before delete"<<endl;
    //删除之前的个体，进行种群更新
    vector<vector<int>>::iterator iter;
    for (iter = PopMain.begin(); PopMain.size() > PopSize;)
    {
        iter = PopMain.erase(iter);
    }
    // cout<<"After Select:"<<endl;
    //  for (int t = 0; t < PopSize; t++)
    //  {
    //      printIndividual(PopMain[t]);
    //  }
}
//按次序交叉
vector<int> Cross(vector<int> v1, vector<int> v2)
{
    // cout<<"parents:"<<endl;
    // printIndividual(v1);
    // printIndividual(v2);
    //随机获得起止位置
    int start = rand() % N;
    int stop = rand() % N;
    while (start == stop)
        stop = rand() % N;
    if (stop < start)
    {
        int temp = stop;
        stop = start;
        start = temp;
    }
    // cout<<"start:"<<start<<" stop:"<<stop<<endl;
    vector<int> vec(N);
    //将父代1起止部分的信息复制到子代的相同位置
    for (int i = start; i <= stop; i++)
        vec[i] = v1[i];
    for (int i = 0; i < N; i++)
    {
        if (v1[i] < 0)
            v1[i] = -v1[i];
    }
    //遍历父代2，将剩余信息按照顺序补齐
    vector<int>::iterator it;
    int temp = 0;
    for (int i = 0; i < N; i++)
    {
        while (temp >= start && temp <= stop)
        {
            temp++;
        }
        it = find(v1.begin() + start, v1.begin() + stop + 1, abs(v2[i]));
        if (it == v1.begin() + stop + 1) //查找失败
        {
            // cout<<v2[i]<<"search failed!"<<endl;
            vec[temp] = v2[i];
            temp++;
        }
    }
    return vec;
}
//变异
void Variation()
{
    for (int i = 1; i < PopSize; i++)
    {
        float r = rand() % 100 * 1.0 / 100;
        if (r < Variation_rate)
        {
            // cout<<"variationed"<<endl;
            int index1 = rand() % N;
            int index2 = rand() % N;
            while (index1 == index2)
                index2 = rand() % N;
            int temp = PopMain[i][index1];
            PopMain[i][index1] = PopMain[i][index2];
            PopMain[i][index2] = temp;
        }
    }
}
//获得当前适应度最高的个体
void Get_best_solution()
{
    for (int i = 0; i < PopSize; i++)
    {
        if (Fit[i] > Max_fitness)
        {
            Max_fitness = Fit[i];
            BestIndividual = PopMain[i];
        }
    }
}
int main()
{
    Generate_Planks();  //生成木板
    Generate_PopMain(); //生成种群
    int IterationNum = 50;
    while (IterationNum--)
    {
        // cout<<"Firness:"<<endl;
        Fit.clear();
        for (int i = 0; i < PopSize; i++) //计算适应度
        {
            ClearPlankTable();
            vector<Plank> vec = PlaceByHighLine(PopMain[i]);
            Calc_Fitness(vec);
        }
        Get_best_solution(); //获得当前最优解
        // cout << "Max_fitness:" << Max_fitness << endl;
        if (IterationNum == 1)
            break;
        Select();                            //选择
        for (int i = 0; i < PopSize; i += 2) //交叉
        {

            double r = rand() % 100 * 1.0 / 100;
            if (r < Cross_rate)
            {
                // cout<<"Crossed"<<endl;
                vector<int> offspring1 = Cross(PopMain[i], PopMain[i + 1]);
                vector<int> offspring2 = Cross(PopMain[i + 1], PopMain[i]);
                if (i != 0)
                    PopMain[i] = offspring2;
                PopMain[i + 1] = offspring1;
            }
        }
        // cout << "After Cross:" << endl;
        //  printPopulation();
        Variation(); //变异
        // cout<<"After Variation:"<<endl;
        // printPopulation();
    }
    cout << "Last Population:" << endl;
    printPopulation();
    cout << "bestsolution:" << endl;
    printIndividual(BestIndividual);
    cout << "Max_fitness:" << Max_fitness << endl;
    printTable();

    // vector<Plank> vec = PlaceByHighLine(PopMain[0]);
    // for (int i = 0; i < 120; i++)
    // {
    //     for (int j = 0; j < 120; j++)
    //         cout << PlankTable[i][j];
    //     cout << endl;
    // }
    system("pause");
    return 0;
}