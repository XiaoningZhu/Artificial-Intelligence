#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <map>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

const int text_num=583;              //训练文本个数 
const int dim=11;                 //向量维度 
const int test_num=100;              //测试文本个数 

int train_sample[text_num][dim];     //储存训练集所有数据 

//读取训练集数据
void read_trainData()
{
	ifstream read_trainData("train.csv");          //读取训练集数据 
	if(!read_trainData)    cout<<"can not open the file"<<endl;
	else
	{ 
	    int index_v=0;
		string line;                             
	    while(!read_trainData.eof())
	    {
		    getline(read_trainData, line);        //读取一行数据
		    
	    	train_sample[index_v][0]=1;
			int index_h=1;         //
			int len = line.length();
			int start = 0;
			for(int i=0; i<len; i++)               
			{
				if(line[i]==',' || i==len-1)  //根据逗号分离一行数据中的每个数据 
				{
					int l = i-start+1;
					string word = line.substr(start,l);
					istringstream iss(word);  
    				int num;  
    				iss >> num; 
					train_sample[index_v][index_h]=num; //将分离开的数据储存在数组里 
					index_h++;
					start = i+1;
				}
    			
			}
	    	index_v++;
	    }
	}
	

}

double weight[dim-1]={0};    //储存更新后的权重
double weight_next[dim-1] = {0} ;
int update_num=0;      //更新次数
int max_iter = 120;    //最大迭代次数 
double y = 0.02;          //the learning rate

//得到训练后的权重  
void getWeight()
{
	double dis = 2;          //使进入循环 
	while(dis > 0.1)   //若权重收敛则停止更新权重 
	{
		update_num ++;
		for(int i=0; i<dim-1; i++)
		{
			weight[i] = weight_next[i];
		}
		double s[text_num]={0};     // weight * X 权重与训练文本的乘积 
		for(int i=0; i< text_num; i++)
		{
			//计算权重分数 
			for(int j=0; j<dim-1; j++)
			{
				s[i]+=weight[j]*train_sample[i][j];   
			}
			
			s[i] = (1/(1+exp(-1*s[i]))) - train_sample[i][dim-1];
			
		}
		
		//计算梯度 
		double grad[dim-1] = {0}; 
		for(int i=0; i<dim-1; i++)
		{
			for(int j=0; j<text_num; j++)
			{
				grad[i] = grad[i]+s[j]*train_sample[j][i];
			}
			
			weight_next[i] = weight[i]-y*grad[i];   //更新权重 
		}
		
		//计算两次权重之间的欧氏距离，进而判断是否收敛 
		dis = 0;
		for(int i=0; i<dim-1; i++)
		{
			
			dis = dis + pow(y*grad[i], 2);        
		}
		
		dis = sqrt(dis);
		
		
		//cout<<"dis"<<dis<<endl;
	}
	cout<<"update_num"<<update_num<<endl;
}

//读取测试集数据
int test_sample[test_num][dim];                //储存测试集数据 
void read_testData()
{
	ifstream read_testData("test.csv");          //读取测试集数据 
	if(!read_testData)    cout<<"can not open the file"<<endl;
	else
	{ 
	    int index_v=0;
		string line;                             
	    while(!read_testData.eof())
	    {
		    getline(read_testData, line);        //读取一行数据
		    
	    	test_sample[index_v][0]=1;
			int index_h=1;       
			int len = line.length();
			int start = 0;
			for(int i=0; i<len; i++)               
			{
				if(line[i]==',' || i==len-1)   ////根据逗号分离一行数据中的每个数据
				{
					int l = i-start+1;
					string word = line.substr(start,l);
					istringstream iss(word);  
    				int num;  
    				iss >> num; 
					test_sample[index_v][index_h]=num; //分离后的数据储存在数组里 
					index_h++;
					start = i+1;
				}
    			
			}

	    	index_v++;
	    }
	}
	
}

//用训练好的权重来测试测试集 
void Test()
{
	int TP=0, FN=0, FP=0, TN=0;
	for(int i=0; i<test_num; i++)
	{
		int sum=0;        // weight * X 权重与训练文本的乘积
		for(int j=0; j<dim-1; j++)
		{
			sum+=weight[j]*test_sample[i][j];  
		}
		
		double p = 1/(1+exp(-1*sum));   //判定标签为1的概率 

		
		//判定预测标签 
		int sign;
		if(p>0.5) sign =1;         
		else if(p==0.5)
		{
			//cout<<"i "<<i<<endl; 
			if(i%2==0) sign=1;
			else sign=0;
		}
		else 
		{
			sign = 0;
		}
	
		
		if(sign == 1 && test_sample[i][dim-1]==1) TP++;
		if(sign == 0 && test_sample[i][dim-1]==1) FN++;
		if(sign == 1 && test_sample[i][dim-1]==0) FP++;
		if(sign == 0 && test_sample[i][dim-1]==0) TN++;
	}
	
	
	double Accuracy = double(TP+TN)/(TP+FP+TN+FN);   //计算判定正确率 
	double Recall = double(TP)/(TP+FN);
	double Precision = double(TP)/(TP+FP);
	double F1 = double(2*Precision*Recall)/(Precision+Recall);
	cout<<TP<<" "<<FN<<" "<<FP<<" "<<TN<<endl;
	cout<<"正确率: "<<Accuracy<<endl;
	cout<<"Recall: "<<Recall<<endl;
	cout<<"Precision: "<<Precision<<endl;
    cout<<"F1: "<<F1<<endl;
	
}



int main()
{
	read_trainData();
	getWeight();
	read_testData();
	Test();
	return 0;
}
