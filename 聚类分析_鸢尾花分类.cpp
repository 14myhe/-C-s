#include <iostream>
#include <string.h>
#include <fstream>//对打开的文件进行读写操作 
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <iomanip> 
#include <math.h>
using namespace std;

#define Data 150
#define k 3 //簇的个数

//最后的验证方法是完成归类之后才给各个簇进行赋种类值，判断正确率，选择正确率最高的那一种 

double min_dis(double a,double b)
{
    if(a<b)
    {
        return a;
    }
    else
    {
        return b;
    }
}


//类FLOWER
class Flower{
    public:
        //四个自变量
        double SpealLength=0;//花萼长度 
        double SpealWidth=0;//花萼宽度 
        double PetalLength=0;//花瓣长度 
        double PetalWidth=0;//花瓣宽度 
        int init_kinds=0;//初始的种类，这里做个规定 Iris-setosa--0，Iris-versicolor--1，Iris-virginica--2 
        //归纳出种类
        int kinds=0;
    public:
         void setSLength(double slen);
         void setSWidth(double swid);
         void setPWidth(double pwid );
         void setPLength(double plen);
         void setkinds(int kind); 
         double getSLength();
         double getSWidth();
         double getPLength();
         double getPWidth();
         int getkind();
}; 

void Flower::setSLength(double slen)
{
    SpealLength=slen;
}
void Flower::setSWidth(double swid)
{
    SpealWidth=swid;
}
void Flower::setPLength(double plen)
{
    PetalLength=plen;
}
void Flower::setPWidth(double pwid)
{
    PetalWidth=pwid;
}
void Flower::setkinds(int i)
{
    kinds=i;
}
double Flower::getSLength()
{
    return SpealLength;
}
double Flower::getSWidth()
{
    return SpealWidth;
}
double Flower::getPLength()
{
    return PetalLength;
}
double Flower::getPWidth()
{
    return PetalWidth;
}
int Flower::getkind()
{
    return kinds;
}


//花簇类
class flower_group{
    public:
        //四个变量用来存储该簇的中心点值
        double ave_SL=0;
        double ave_SW=0;
        double ave_PL=0;
        double ave_PW=0;
        int num=0;//这个类的元素个数 
        double e=0;//表示方差

};

class model{
    public:
    	Flower f[Data];//用于存储从文本提取出来的数据
        flower_group fl_g[k];//表示簇的个数
    public:
        void readData(); //将文本中的数据取出来
        void rand_first_select_center();//随机选取k个簇中心 
        void sort_flower(Flower *f);//将数据聚集
        int rebuild(flower_group *f);//迭代训练 
        double cal_rate();//计算准确率 
};
void model::readData()
{
    //取出样本数据
    char buffer[256];
    ifstream in("data.txt");
    if(!in.is_open())
    {
        cout<<"error file"<<endl;
    }
    int i=0;
    char *sign=",";
    char *p;
    double exm=0;
    while(!in.eof())
    {
        in.getline(buffer,100);
        //进行分割
        //1.花萼长度
        p=strtok(buffer,sign);
        sscanf(p,"%lf",&exm);
        f[i].setSLength(exm);
        //2.花萼宽度
        p=strtok(NULL,sign);
        sscanf(p,"%lf",&exm);
        f[i].setSWidth(exm);
        //3.花瓣长度
        p=strtok(NULL,sign);
        sscanf(p,"%lf",&exm);
        f[i].setPLength(exm);
        //4.花瓣宽度
        p=strtok(NULL,sign);
        sscanf(p,"%lf",&exm);
        f[i].setPWidth(exm);
        //5.种类
		p=strtok(NULL,sign);
        if(strcmp(p,"Iris-setosa")==0)
        {
        	f[i].init_kinds=0;
        }
        else if(strcmp(p,"Iris-versicolor")==0)
        {
        	f[i].init_kinds=1; 
        }
        else
        {
        	f[i].init_kinds=2;
        }
        
        i++;
    }
    
}
void model::rand_first_select_center()
{
    //随机选出k个簇作为中心点
    int l[k];
    for(int i=0;i<k;i++)
    {
        l[i]=rand()%150;
        fl_g[i].ave_SL=f[l[i]].SpealLength;
        fl_g[i].ave_SW=f[l[i]].SpealWidth;
        fl_g[i].ave_PL=f[l[i]].PetalLength;
        fl_g[i].ave_PW=f[l[i]].PetalWidth;
    }
   /* for(int i=0;i<k;i++)
    {
        cout<<l[i]<<" "<<fl_g[i].ave_SL<<" "<<fl_g[i].ave_SW<<" "<<fl_g[i].ave_PL<<" "<<fl_g[i].ave_PW<<endl;
    }*/
    sort_flower(f);

}
void model::sort_flower(Flower *f)
{
    //归类，给kinds赋值
    double k1,k2,k3,min_k;
    flower_group temp[k];
   //对于每一个元素都要计算值
    for(int i=0;i<Data;i++)
    {
        //计算差值(欧式距离)，用距离的平方来比较也是一样的，而且可以简化计算
        k1=(f[i].SpealLength-fl_g[0].ave_SL)*(f[i].SpealLength-fl_g[0].ave_SL)+(f[i].SpealWidth-fl_g[0].ave_SW)*(f[i].SpealWidth-fl_g[0].ave_SW)
            +(f[i].PetalLength-fl_g[0].ave_PL)*(f[i].PetalLength-fl_g[0].ave_PL)+(f[i].PetalWidth-fl_g[0].ave_PW)*(f[i].PetalWidth-fl_g[0].ave_PW);
        k2=(f[i].SpealLength-fl_g[1].ave_SL)*(f[i].SpealLength-fl_g[1].ave_SL)+(f[i].SpealWidth-fl_g[1].ave_SW)*(f[i].SpealWidth-fl_g[1].ave_SW)
            +(f[i].PetalLength-fl_g[1].ave_PL)*(f[i].PetalLength-fl_g[1].ave_PL)+(f[i].PetalWidth-fl_g[1].ave_PW)*(f[i].PetalWidth-fl_g[1].ave_PW);
        k3=(f[i].SpealLength-fl_g[2].ave_SL)*(f[i].SpealLength-fl_g[2].ave_SL)+(f[i].SpealWidth-fl_g[2].ave_SW)*(f[i].SpealWidth-fl_g[2].ave_SW)
            +(f[i].PetalLength-fl_g[2].ave_PL)*(f[i].PetalLength-fl_g[2].ave_PL)+(f[i].PetalWidth-fl_g[2].ave_PW)*(f[i].PetalWidth-fl_g[2].ave_PW);

        min_k=min_dis(min_dis(k1,k2),k3);
        if(min_k==k1)
        {
            f[i].kinds=0;
            temp[0].e+=k1;//方差的累加
            temp[0].num++;
            temp[0].ave_SL+=f[i].SpealLength;
            temp[0].ave_SW+=f[i].SpealWidth;
            temp[0].ave_PL+=f[i].PetalLength;
            temp[0].ave_PW+=f[i].PetalWidth;
        }else if(min_k==k2)
        {
            f[i].kinds=1;
            temp[1].e+=k2;
            temp[1].num++;
            temp[1].ave_SL+=f[i].SpealLength;
            temp[1].ave_SW+=f[i].SpealWidth;
            temp[1].ave_PL+=f[i].PetalLength;
            temp[1].ave_PW+=f[i].PetalWidth;
        }
        else
        {
            f[i].kinds=2;
            temp[2].e+=k3;
            temp[2].num++;
            temp[2].ave_SL+=f[i].SpealLength;
            temp[2].ave_SW+=f[i].SpealWidth;
            temp[2].ave_PL+=f[i].PetalLength;
            temp[2].ave_PW+=f[i].PetalWidth;
        }

    }
    //计算新的簇中心
  /*  for(int i=0;i<k;i++)
    {
        //簇中心不变就停止迭代
        fl_g[i].ave_SL=temp[i].ave_SL/temp[i].num;
        fl_g[i].ave_SW=temp[i].ave_SW/temp[i].num;
        fl_g[i].ave_PL=temp[i].ave_PL/temp[i].num;
        fl_g[i].ave_PW=temp[i].ave_PW/temp[i].num;
    }*/
    rebuild(temp);


} 
int model::rebuild(flower_group *temp)
{
    if(temp[0].e==fl_g[0].e&&temp[1].e==fl_g[1].e&&temp[2].e==fl_g[2].e)
    {
        //簇中心不改变
        cout<<"final"<<endl;
		for(int i=0;i<k;i++)
        {
            cout<<fl_g[i].ave_SL<<" "<<fl_g[i].ave_SW<<" "<<fl_g[i].ave_PL<<" "<<fl_g[i].ave_PW<<endl;
        }
    }
    else
    {
        cout<<"初始簇中心的值为："<<endl;
        for(int i=0;i<k;i++)
        {
            cout<<fl_g[i].ave_SL<<" "<<fl_g[i].ave_SW<<" "<<fl_g[i].ave_PL<<" "<<fl_g[i].ave_PW<<endl;
        }
        //簇中心需要更新
        for(int i=0;i<k;i++)
        {
            //簇中心不变就停止迭代
            fl_g[i].ave_SL=temp[i].ave_SL/temp[i].num;
            fl_g[i].ave_SW=temp[i].ave_SW/temp[i].num;
            fl_g[i].ave_PL=temp[i].ave_PL/temp[i].num;
            fl_g[i].ave_PW=temp[i].ave_PW/temp[i].num;
            fl_g[i].e=temp[i].e;
        }
        cout<<"更新后簇中心的值为："<<endl;
        for(int i=0;i<k;i++)
        {
            cout<<fl_g[i].ave_SL<<" "<<fl_g[i].ave_SW<<" "<<fl_g[i].ave_PL<<" "<<fl_g[i].ave_PW<<endl;
        }
        sort_flower(f);
         
    }
}
double model::cal_rate()//计算准确率 
{
	//这里做个规定 Iris-setosa--0，Iris-versicolor--1，Iris-virginica--2 
	//现在有三个簇，需要将这三个簇和三种类型对应起来 ,总共有六种组合 
	cout<<"将得到的簇进行类型的判断"<<endl;
	int c_num=0; 
	//第一种情况 
	cout<<"如果三个簇分别对应种类 Iris-setosa--0,Iris-versicolor--1,Iris-virginica--2"<<endl; 
	for(int i=0;i<Data;i++)
	{
	    if(f[i].init_kinds==f[i].kinds)
	    {
	    	c_num++;
	    }
	}
	cout<<"准确率为:"<<c_num*100/(Data*1.0)<<"%"<<endl;	
	
	
	//第二种情况 
	cout<<"如果三个簇分别对应种类 Iris-setosa--0,Iris-virginica--2,Iris-versicolor--1"<<endl; 
	for(int i=0;i<Data;i++)
	{
	    if(f[i].kinds==0)//第二个簇 
	    {
	    	f[i].kinds=0;
	    }else if(f[i].kinds==1)
	    {
	    	f[i].kinds=2; 
	    }
	    else if(f[i].kinds==2)
	    {
	    	f[i].kinds=1;
  	    }
  	}
	c_num=0;
	for(int i=0;i<Data;i++)
	{
	    if(f[i].init_kinds==f[i].kinds)
	    {
	    	c_num++;
	    }
	}
	cout<<"准确率为:"<<c_num*100/(Data*1.0)<<"%"<<endl;
	
	
	//恢复到初始状态
	for(int i=0;i<Data;i++)
	{
	    if(f[i].kinds==0)
	    {
	    	f[i].kinds=0;
	    }else if(f[i].kinds==1)
	    {
	    	f[i].kinds=2; 
	    }
	    else if(f[i].kinds==2)
	    {
	    	f[i].kinds=1;
  	    }
  	} 
	//第三种情况 
	cout<<"如果三个簇分别对应种类Iris-versicolor--1，Iris-setosa--0，Iris-virginica--2"<<endl; 
	for(int i=0;i<Data;i++)
	{
	    if(f[i].kinds==0) 
	    {
	    	f[i].kinds=1;
	    }else if(f[i].kinds==1)
	    {
	    	f[i].kinds=0; 
	    }else if(f[i].kinds==2)
	    {
	    	f[i].kinds=2;
	    }
	}
	c_num=0;
	for(int i=0;i<Data;i++)
	{
	    if(f[i].init_kinds==f[i].kinds)
	    {
	    	c_num++;
	    }
	}
	cout<<"准确率为:"<<c_num*100/(Data*1.0)<<"%"<<endl;

	//恢复到初始状态
	for(int i=0;i<Data;i++)
	{
	    if(f[i].kinds==0)
	    {
	    	f[i].kinds=1;
	    }else if(f[i].kinds==1)
	    {
	    	f[i].kinds=0; 
	    }
	    else if(f[i].kinds==2)
	    {
	    	f[i].kinds=2;
  	    }
  	} 
 
	//第四种情况 
	cout<<"如果三个簇分别对应种类Iris-versicolor--1，Iris-virginica--2，Iris-setosa--0，"<<endl; 
	for(int i=0;i<Data;i++)
	{
	    if(f[i].kinds==0) 
	    {
	    	f[i].kinds=1;
	    }else if(f[i].kinds==1)
	    {
	    	f[i].kinds=2; 
	    }
	    else if(f[i].kinds==2)
		{
			f[i].kinds=0;
		} 
	}
	c_num=0;
	for(int i=0;i<Data;i++)
	{
	    if(f[i].init_kinds==f[i].kinds)
	    {
	    	c_num++;
	    }
	}
	cout<<"准确率为:"<<c_num*100/(Data*1.0)<<"%"<<endl;


	//恢复到初始状态
	for(int i=0;i<Data;i++)
	{
	    if(f[i].kinds==0)//第二个簇 
	    {
	    	f[i].kinds=2;
	    }else if(f[i].kinds==1)
	    {
	    	f[i].kinds=0; 
	    }
	    else if(f[i].kinds==2)
	    {
	    	f[i].kinds=1;
  	    }
  	} 	 
	//第五种情况 
	cout<<"如果三个簇分别对应种类Iris-virginica--2，Iris-setosa--0，Iris-versicolor--1，"<<endl; 
	for(int i=0;i<Data;i++)
	{
	    if(f[i].kinds==0) 
	    {
	    	f[i].kinds=2;
	    }else if(f[i].kinds==1)
	    {
	    	f[i].kinds=0; 
	    }
	    else if(f[i].kinds==2)
		{
			f[i].kinds=1;
		} 
	}
	c_num=0;
	for(int i=0;i<Data;i++)
	{
	    if(f[i].init_kinds==f[i].kinds)
	    {
	    	c_num++;
	    }
	}
	cout<<"准确率为:"<<c_num*100/(Data*1.0)<<"%"<<endl;
	
	
	//恢复到初始状态
	for(int i=0;i<Data;i++)
	{
	    if(f[i].kinds==0)//第二个簇 
	    {
	    	f[i].kinds=1;
	    }else if(f[i].kinds==1)
	    {
	    	f[i].kinds=2; 
	    }
	    else if(f[i].kinds==2)
	    {
	    	f[i].kinds=0;
  	    }
  	} 	 
		//第六种情况 
    cout<<"如果三个簇分别对应种类Iris-virginica--2，Iris-versicolor--1，,Iris-setosa--0，"<<endl; 
	for(int i=0;i<Data;i++)
	{
	    if(f[i].kinds==0) 
	    {
	    	f[i].kinds=2;
	    }else if(f[i].kinds==1)
	    {
	    	f[i].kinds=1; 
	    }
	    else if(f[i].kinds==2)
		{
			f[i].kinds=0;
		} 
	}
	c_num=0;
	for(int i=0;i<Data;i++)
	{
	    if(f[i].init_kinds==f[i].kinds)
	    {
	    	c_num++;
	    }
	}
	cout<<"准确率为:"<<c_num*100/(Data*1.0)<<"%"<<endl;

}
int main()
{
    srand(time(0));
    model m;
    m.readData();//将文件中的数据读出来 
    m.rand_first_select_center();//随机选出簇中心
    cout<<"转换后的结果"<<endl;
    for(int i=0;i<Data;i++)
    {
        cout<<m.f[i].getSLength()<<" "<<m.f[i].getSWidth()<<" "<<m.f[i].getPLength()<<" "<<m.f[i].getPWidth()<<" 初始种类："<<m.f[i].init_kinds<<" 预测种类:"<<m.f[i].kinds<<endl;
    }
    m.cal_rate(); 
    system("pause"); 
    
}
