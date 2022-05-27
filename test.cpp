#include <iostream>
#include <ctime>
#include <vector>
#include <fstream>
#include <cstring>
#include <windows.h>
using namespace std;
//结构体
typedef unsigned char byte;

char FileName[] = "test.bmp";

#define FILENAME FileName

typedef struct pixColor
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
}PIXCOLOR,*PPIXCOLOR;

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
    // GeneratePopulation();
    // CalcFitness();
    // Select();
    // CrossOver();
    // Variation();
    BITMAPFILEHEADER bmf;
	BITMAPINFOHEADER bif;
	FILE* pf;
	
	//返回值为0表示打开成功，非0则为失败
	errno_t err = fopen_s(&pf,FILENAME, "rb");

	if (0 == err)
	{
		fread(&bmf, sizeof(bmf), 1, pf);
		fread(&bif, sizeof(bif), 1, pf);

		if (0x4d42 != bmf.bfType)
		{
			cout << "文件不是位图！" << endl;
			fclose(pf);
		}else if (24 != bif.biBitCount)
		{
			cout << "位图不是24位！" << endl;
			fclose(pf);
		}
		else
		{
			cout << "图片的宽度：" << bif.biWidth << endl;
			cout << "图片的高度：" << bif.biHeight << endl;

			PPIXCOLOR* ColorData = new PPIXCOLOR[bif.biHeight];

			//一行的字节数
			int lineCount = bif.biSizeImage / bif.biHeight;

			//存的时候从倒数第一行开始存，所以读的时候先往倒数第一行放
			//读颜色数据
			for (int i = bif.biHeight - 1; i >= 0; i--)
			{
				ColorData[i] = new PIXCOLOR[bif.biWidth];
				fread(ColorData[i], sizeof(PIXCOLOR), bif.biWidth, pf);
				//偏移  bif.biWidth * sizeof(PIXCOLOR)有多少个字节
				fseek(pf, lineCount - bif.biWidth * sizeof(PIXCOLOR), SEEK_CUR);
			}
			fclose(pf);

			for (int i = 0; i < bif.biHeight; i++)
			{
				for (int j = 0; j < bif.biWidth; j++)
				{
					printf("(%3d,%3d,%3d),", ColorData[i][j].r, ColorData[i][j].g, ColorData[i][j].b);
				}
				cout << endl;
				delete[] ColorData[i];
			}
			delete[] ColorData;
			ColorData = nullptr;
		}
	}

	system("pause");
    
    return 0;
}


void PlacePlanksInitialization(int tableLength,int tableWidth,int planks[],int plankNum)
