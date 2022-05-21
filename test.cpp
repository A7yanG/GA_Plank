#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <vector>
using namespace std;

//结构体
typedef unsigned char byte;
typedef struct Point
{
    byte x;
    byte y;
};
typedef struct Individual
{
    vector<Point> Points;
    double fitness;
};

//全局变量
vector<Individual> PopMain;
static int MainSize = 100;        //种群大小
static int IndividualSize = 2000; //个体大小，即撒点数量
static double CrossRate = 0.9;
static double VariationRate = 0.01;
//函数定义
void GeneratePopulation();
void GenerateIndividual(Individual &individual);
void CalcIndividualFitness(Individual &individual);
void CalcFitness();
void Select();
void CrossOver();
void Variation();
bool IfPointValid(Point point);


//函数实现
void PrintPopMain()
{
    for(int i=0;i<MainSize;i++)
    {
        cout<<"Individual"<<i<<":"<<endl;
        for(int j=0;j<IndividualSize;j++)
        {
            cout<<(int)PopMain[i].Points[j].x<<" "<<(int)PopMain[i].Points[j].y<<",";
        }
        cout<<endl;
    }
}
void GeneratePopulation()
{

    srand(int(time(0)));
    PopMain.clear();
    for (int i = 0; i < MainSize; i++)
    {

        Individual individual;
        PopMain.push_back(individual);
        GenerateIndividual(PopMain[i]);
    }
}
void GenerateIndividual(Individual &individual)
{
    for (int i = 0; i < IndividualSize; i++)
    {
        Point point;
        point.x = rand() % 256;
        point.y = rand() % 256;
        individual.Points.push_back(point);
    }
}
void CalcIndividualFitness(Individual &individual)
{
    int validPoint = 0;
    for (int i = 0; i < IndividualSize; i++)
    {
        if (IfPointValid(individual.Points[i]))
        {
            validPoint++;
        }
    }
    individual.fitness = validPoint;
}
void CalcFitness()
{
    for (int i = 0; i < MainSize; i++)
    {
        CalcIndividualFitness(PopMain[i]);
    }
}
void Select()
{
    vector<Individual> PopTemp=PopMain;
    PopMain.clear();
    int SumFitness = 0;
    for (int i = 0; i < MainSize; i++)
        SumFitness += PopTemp[i].fitness;
    for (int t = 0; t < MainSize; t++) //轮盘赌
    {
        int temp = rand() % SumFitness; //随机生成数
        int SumTemp = 0;
        for (int i = 0; i < MainSize; i++)
        {
            SumTemp += PopTemp[i].fitness;
            if (SumTemp >= temp) //累加值>=随机生成的值
            {
                PopMain.push_back(PopTemp[i]);
                break;
            }
        }
    }
}
void CrossOver()
{
    for (int i = 0; i < MainSize; i+=2)
    {
        double r = rand() % 100 * 1.0 / 100;
        if (r < CrossRate)
        {
            int start = rand() % IndividualSize;
            int stop = rand() % IndividualSize;
            while (start == stop)
                stop = rand() % IndividualSize;
            if (stop < start)
            {
                int temp = stop;
                stop = start;
                start = temp;
            }
            for (int j = start; j <= stop; j++)
            {
                Point p1 = PopMain[i].Points[j];
                PopMain[i].Points[j] = PopMain[i + 1].Points[j];
                PopMain[i + 1].Points[j] = p1;
            }
        }
    }
}
void Variation()
{
    for (int i = 0; i < MainSize; i++)
    {
        double r1 = rand() % 100 * 1.0 / 100;
        if (r1 < VariationRate)
        {
            //cout<<i<<"变异了"<<endl;
            int r = rand() % (16 * IndividualSize);
            int index = r / 16; //变异点的下标
            int bitindex = r % 16 % 8;//变异点的位
            //cout<<"index:"<<index<<" bit:"<<bitindex<<endl;
            //cout<<(int)PopMain[i].Points[index].x<<' '<<(int)PopMain[i].Points[index].y<<endl;
            if (r % 16 / 8) //变异y
            {
                PopMain[i].Points[index].y^= (1 << bitindex);
            }
            else
            {
                PopMain[i].Points[index].x^= (1 << bitindex);
            }
            //cout<<(int)PopMain[i].Points[index].x<<' '<<(int)PopMain[i].Points[index].y<<endl;
        }
    }
}
bool IfPointValid(Point point)
{
    return true;
}

int main()
{
    GeneratePopulation();
    CalcFitness();
    Select();
    CrossOver();
    Variation();
    return 0;
}
