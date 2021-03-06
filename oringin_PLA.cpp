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
const int text_num=100;              //训练文本个数 
const int dim=10001;                 //向量维度 
const int test_num=100;              //测试文本个数 

int train_sample[text_num][dim];     //储存训练集所有数据 

//读取训练集数据
void read_trainData()
{
	ifstream read_trainData("train_data.txt");          //读取训练集数据 
	if(!read_trainData)    cout<<"can not open the file"<<endl;
	else
	{ 
	    int index_v=0;
		string line;                             
	    while(!read_trainData.eof())
	    {
		    getline(read_trainData, line);        //读取一行数据
		    istringstream is(line);
		    
	    	int word; 
	    	train_sample[index_v][0]=1;
			int index_h=1;                      
	    	while(is>>word)                    //读取一个特征 
	    	{
	    		train_sample[index_v][index_h]=word; //储存在数组里 
	    		index_h++;
	    	}
	    	index_v++;
	    }
	}
	

}

//读取训练集标签 
int train_label[text_num]={0};       //储存训练集标签 
void read_trainLabel()
{
	ifstream read_trainLabel("train_labels.txt");          //读取训练集标签 
	if(!read_trainLabel)    cout<<"can not open the file"<<endl;
	else
	{ 
	    int index=0;
		string line;                             
		while(!read_trainLabel.eof())
	    {
		    getline(read_trainLabel, line);        //读取一行数据
		    istringstream is(line);
		    
	    	int word;                       
	    	while(is>>word)                    //读取一个特征 
	    	{
	    		train_label[index]=word;       // 储存训练集标签 
	    	} 
	    	index++;
	    }
	    
	    
	}
	
}

int weight[dim]={0};           //储存权重 
int update_num=0;              //更新次数
//得到训练后的权重 
void getWeight()
{
	
	int j=0;
	int flag=0;
	while(j<text_num)
	{
		int sum=0;
		for(int i=0; i<dim; i++)
		{
			sum=weight[i]*train_sample[j][i]+sum;  //计算权重与文本数据相乘的结果 
		}
		
		//预测标签 
		int sign;
		if(sum>0) sign=1;
		else if(sum<0) sign=-1;
		else sign =0;

		if(sign!=train_label[j])   //更新权重 
		{
			update_num++;
			for(int i=0; i<dim; i++)
			{
				weight[i]=weight[i]+train_sample[j][i]*train_label[j];
				
			}
			
			flag=1;      //标志更新后从头开始遍历 
		}
		else flag=0;
		
		if(flag==1) j=0;
		else j++;
	}

	cout<<"权重更新次数： "<<update_num<<endl;

}

//读取测试集数据
int test_sample[test_num][dim];                //储存测试集数据 
void read_testData()
{
	ifstream read_testData("test_data.txt");          //读取测试集数据 
	if(!read_testData)    cout<<"can not open the file"<<endl;
	else
	{ 
	    int index_v=0;
		string line;                             
	    while(!read_testData.eof())
	    {
		    getline(read_testData, line);        //读取一行数据
		    istringstream is(line);
		    
	    	int word; 
	    	test_sample[index_v][0]=1;
			int index_h=1;                      
	    	while(is>>word)                    //读取一个特征 
	    	{
	    		test_sample[index_v][index_h]=word;   //储存测试集数据 
	    		index_h++;
	    	}
	    	index_v++;
	    }
	}
}

//读取测试集文本标签 
int test_label[test_num]={0};
void read_testLabel()
{
	ifstream read_testLabel("test_labels.txt");          //测试训练集标签 
	if(!read_testLabel)    cout<<"can not open the file"<<endl;
	else
	{ 
	    int index=0;
		string line;                             
	    while(!read_testLabel.eof())
	    {
		    getline(read_testLabel, line);        //读取一行数据
		    istringstream is(line);
		    
	    	int word;                       
	    	while(is>>word)                    //读取一个特征 
	    	{
	    		test_label[index]=word;     //储存标签 
	    	}
	    	index++;
	    }
	}
	
	
}


double TP=0, FN=0, FP=0, TN=0;
double Accuracy=0, Precision=0, Recall=0, F1 =0;
//计算 评测指标
void testLevel(int TF, int FN, int FP, int TN)
{
	Accuracy = (TP+TN)/(TP+FP+TN+FN);
	Precision = TP/(TP+FP);
	Recall = TP/(TP+FN);
	F1 = (2*Precision*Recall)/(Precision+Recall);
}

//用训练好的权重来测试测试集 
void Test()
{
	for(int i=0; i<test_num; i++)
	{
		int sum=0;
		for(int j=0; j<dim; j++)
		{
			sum+=weight[j]*test_sample[i][j];     ////计算权重与文本数据相乘的结果
		}
		
		//预测标签 
		int sign;
		if(sum>0) sign =1;
		else if(sum<0) sign=-1;
		else 
		{
			if(i%2==0) sign=1;
			else sign=-1;
		}
		
		//计算TP, FN, FP, TN 
		if(test_label[i]==1 && sign==1) TP++;
		if(test_label[i]==1 && sign==-1) FN++;
		if(test_label[i]==-1 && sign==1) FP++;
		if(test_label[i]==-1 && sign==-1) TN++;

	}
	
	testLevel(TP, FN, FP, TN);  //计算测评指标 
	
	cout<<"Accuracy: "<<Accuracy<<endl;
	cout<<"Recall: "<<Recall<<endl;
	cout<<"Precision: "<<Precision<<endl;
	cout<<"F1: "<<F1<<endl;
}




int main()
{
	read_trainData();
	read_trainLabel();
	getWeight();
	read_testData();
	read_testLabel();
	Test();
	return 0;
}











