#include <iostream>
#include <string.h>
#include <fstream>//�Դ򿪵��ļ����ж�д���� 
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <iomanip> 
#include <math.h>
using namespace std;

#define Data 150
#define k 3 //�صĸ���

//������֤��������ɹ���֮��Ÿ������ؽ��и�����ֵ���ж���ȷ�ʣ�ѡ����ȷ����ߵ���һ�� 

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


//��FLOWER
class Flower{
    public:
        //�ĸ��Ա���
        double SpealLength=0;//���೤�� 
        double SpealWidth=0;//������ 
        double PetalLength=0;//���곤�� 
        double PetalWidth=0;//������ 
        int init_kinds=0;//��ʼ�����࣬���������涨 Iris-setosa--0��Iris-versicolor--1��Iris-virginica--2 
        //���ɳ�����
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


//������
class flower_group{
    public:
        //�ĸ����������洢�ôص����ĵ�ֵ
        double ave_SL=0;
        double ave_SW=0;
        double ave_PL=0;
        double ave_PW=0;
        int num=0;//������Ԫ�ظ��� 
        double e=0;//��ʾ����

};

class model{
    public:
    	Flower f[Data];//���ڴ洢���ı���ȡ����������
        flower_group fl_g[k];//��ʾ�صĸ���
    public:
        void readData(); //���ı��е�����ȡ����
        void rand_first_select_center();//���ѡȡk�������� 
        void sort_flower(Flower *f);//�����ݾۼ�
        int rebuild(flower_group *f);//����ѵ�� 
        double cal_rate();//����׼ȷ�� 
};
void model::readData()
{
    //ȡ����������
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
        //���зָ�
        //1.���೤��
        p=strtok(buffer,sign);
        sscanf(p,"%lf",&exm);
        f[i].setSLength(exm);
        //2.������
        p=strtok(NULL,sign);
        sscanf(p,"%lf",&exm);
        f[i].setSWidth(exm);
        //3.���곤��
        p=strtok(NULL,sign);
        sscanf(p,"%lf",&exm);
        f[i].setPLength(exm);
        //4.������
        p=strtok(NULL,sign);
        sscanf(p,"%lf",&exm);
        f[i].setPWidth(exm);
        //5.����
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
    //���ѡ��k������Ϊ���ĵ�
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
    //���࣬��kinds��ֵ
    double k1,k2,k3,min_k;
    flower_group temp[k];
   //����ÿһ��Ԫ�ض�Ҫ����ֵ
    for(int i=0;i<Data;i++)
    {
        //�����ֵ(ŷʽ����)���þ����ƽ�����Ƚ�Ҳ��һ���ģ����ҿ��Լ򻯼���
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
            temp[0].e+=k1;//������ۼ�
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
    //�����µĴ�����
  /*  for(int i=0;i<k;i++)
    {
        //�����Ĳ����ֹͣ����
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
        //�����Ĳ��ı�
        cout<<"final"<<endl;
		for(int i=0;i<k;i++)
        {
            cout<<fl_g[i].ave_SL<<" "<<fl_g[i].ave_SW<<" "<<fl_g[i].ave_PL<<" "<<fl_g[i].ave_PW<<endl;
        }
    }
    else
    {
        cout<<"��ʼ�����ĵ�ֵΪ��"<<endl;
        for(int i=0;i<k;i++)
        {
            cout<<fl_g[i].ave_SL<<" "<<fl_g[i].ave_SW<<" "<<fl_g[i].ave_PL<<" "<<fl_g[i].ave_PW<<endl;
        }
        //��������Ҫ����
        for(int i=0;i<k;i++)
        {
            //�����Ĳ����ֹͣ����
            fl_g[i].ave_SL=temp[i].ave_SL/temp[i].num;
            fl_g[i].ave_SW=temp[i].ave_SW/temp[i].num;
            fl_g[i].ave_PL=temp[i].ave_PL/temp[i].num;
            fl_g[i].ave_PW=temp[i].ave_PW/temp[i].num;
            fl_g[i].e=temp[i].e;
        }
        cout<<"���º�����ĵ�ֵΪ��"<<endl;
        for(int i=0;i<k;i++)
        {
            cout<<fl_g[i].ave_SL<<" "<<fl_g[i].ave_SW<<" "<<fl_g[i].ave_PL<<" "<<fl_g[i].ave_PW<<endl;
        }
        sort_flower(f);
         
    }
}
double model::cal_rate()//����׼ȷ�� 
{
	//���������涨 Iris-setosa--0��Iris-versicolor--1��Iris-virginica--2 
	//�����������أ���Ҫ���������غ��������Ͷ�Ӧ���� ,�ܹ���������� 
	cout<<"���õ��Ĵؽ������͵��ж�"<<endl;
	int c_num=0; 
	//��һ����� 
	cout<<"��������طֱ��Ӧ���� Iris-setosa--0,Iris-versicolor--1,Iris-virginica--2"<<endl; 
	for(int i=0;i<Data;i++)
	{
	    if(f[i].init_kinds==f[i].kinds)
	    {
	    	c_num++;
	    }
	}
	cout<<"׼ȷ��Ϊ:"<<c_num*100/(Data*1.0)<<"%"<<endl;	
	
	
	//�ڶ������ 
	cout<<"��������طֱ��Ӧ���� Iris-setosa--0,Iris-virginica--2,Iris-versicolor--1"<<endl; 
	for(int i=0;i<Data;i++)
	{
	    if(f[i].kinds==0)//�ڶ����� 
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
	cout<<"׼ȷ��Ϊ:"<<c_num*100/(Data*1.0)<<"%"<<endl;
	
	
	//�ָ�����ʼ״̬
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
	//��������� 
	cout<<"��������طֱ��Ӧ����Iris-versicolor--1��Iris-setosa--0��Iris-virginica--2"<<endl; 
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
	cout<<"׼ȷ��Ϊ:"<<c_num*100/(Data*1.0)<<"%"<<endl;

	//�ָ�����ʼ״̬
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
 
	//��������� 
	cout<<"��������طֱ��Ӧ����Iris-versicolor--1��Iris-virginica--2��Iris-setosa--0��"<<endl; 
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
	cout<<"׼ȷ��Ϊ:"<<c_num*100/(Data*1.0)<<"%"<<endl;


	//�ָ�����ʼ״̬
	for(int i=0;i<Data;i++)
	{
	    if(f[i].kinds==0)//�ڶ����� 
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
	//��������� 
	cout<<"��������طֱ��Ӧ����Iris-virginica--2��Iris-setosa--0��Iris-versicolor--1��"<<endl; 
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
	cout<<"׼ȷ��Ϊ:"<<c_num*100/(Data*1.0)<<"%"<<endl;
	
	
	//�ָ�����ʼ״̬
	for(int i=0;i<Data;i++)
	{
	    if(f[i].kinds==0)//�ڶ����� 
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
		//��������� 
    cout<<"��������طֱ��Ӧ����Iris-virginica--2��Iris-versicolor--1��,Iris-setosa--0��"<<endl; 
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
	cout<<"׼ȷ��Ϊ:"<<c_num*100/(Data*1.0)<<"%"<<endl;

}
int main()
{
    srand(time(0));
    model m;
    m.readData();//���ļ��е����ݶ����� 
    m.rand_first_select_center();//���ѡ��������
    cout<<"ת����Ľ��"<<endl;
    for(int i=0;i<Data;i++)
    {
        cout<<m.f[i].getSLength()<<" "<<m.f[i].getSWidth()<<" "<<m.f[i].getPLength()<<" "<<m.f[i].getPWidth()<<" ��ʼ���ࣺ"<<m.f[i].init_kinds<<" Ԥ������:"<<m.f[i].kinds<<endl;
    }
    m.cal_rate(); 
    system("pause"); 
    
}
