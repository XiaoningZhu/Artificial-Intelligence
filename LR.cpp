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

const int text_num=583;              //ѵ���ı����� 
const int dim=11;                 //����ά�� 
const int test_num=100;              //�����ı����� 

int train_sample[text_num][dim];     //����ѵ������������ 

//��ȡѵ��������
void read_trainData()
{
	ifstream read_trainData("train.csv");          //��ȡѵ�������� 
	if(!read_trainData)    cout<<"can not open the file"<<endl;
	else
	{ 
	    int index_v=0;
		string line;                             
	    while(!read_trainData.eof())
	    {
		    getline(read_trainData, line);        //��ȡһ������
		    
	    	train_sample[index_v][0]=1;
			int index_h=1;         //
			int len = line.length();
			int start = 0;
			for(int i=0; i<len; i++)               
			{
				if(line[i]==',' || i==len-1)  //���ݶ��ŷ���һ�������е�ÿ������ 
				{
					int l = i-start+1;
					string word = line.substr(start,l);
					istringstream iss(word);  
    				int num;  
    				iss >> num; 
					train_sample[index_v][index_h]=num; //�����뿪�����ݴ����������� 
					index_h++;
					start = i+1;
				}
    			
			}
	    	index_v++;
	    }
	}
	

}

double weight[dim-1]={0};    //������º��Ȩ��
double weight_next[dim-1] = {0} ;
int update_num=0;      //���´���
int max_iter = 120;    //���������� 
double y = 0.02;          //the learning rate

//�õ�ѵ�����Ȩ��  
void getWeight()
{
	double dis = 2;          //ʹ����ѭ�� 
	while(dis > 0.1)   //��Ȩ��������ֹͣ����Ȩ�� 
	{
		update_num ++;
		for(int i=0; i<dim-1; i++)
		{
			weight[i] = weight_next[i];
		}
		double s[text_num]={0};     // weight * X Ȩ����ѵ���ı��ĳ˻� 
		for(int i=0; i< text_num; i++)
		{
			//����Ȩ�ط��� 
			for(int j=0; j<dim-1; j++)
			{
				s[i]+=weight[j]*train_sample[i][j];   
			}
			
			s[i] = (1/(1+exp(-1*s[i]))) - train_sample[i][dim-1];
			
		}
		
		//�����ݶ� 
		double grad[dim-1] = {0}; 
		for(int i=0; i<dim-1; i++)
		{
			for(int j=0; j<text_num; j++)
			{
				grad[i] = grad[i]+s[j]*train_sample[j][i];
			}
			
			weight_next[i] = weight[i]-y*grad[i];   //����Ȩ�� 
		}
		
		//��������Ȩ��֮���ŷ�Ͼ��룬�����ж��Ƿ����� 
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

//��ȡ���Լ�����
int test_sample[test_num][dim];                //������Լ����� 
void read_testData()
{
	ifstream read_testData("test.csv");          //��ȡ���Լ����� 
	if(!read_testData)    cout<<"can not open the file"<<endl;
	else
	{ 
	    int index_v=0;
		string line;                             
	    while(!read_testData.eof())
	    {
		    getline(read_testData, line);        //��ȡһ������
		    
	    	test_sample[index_v][0]=1;
			int index_h=1;       
			int len = line.length();
			int start = 0;
			for(int i=0; i<len; i++)               
			{
				if(line[i]==',' || i==len-1)   ////���ݶ��ŷ���һ�������е�ÿ������
				{
					int l = i-start+1;
					string word = line.substr(start,l);
					istringstream iss(word);  
    				int num;  
    				iss >> num; 
					test_sample[index_v][index_h]=num; //���������ݴ����������� 
					index_h++;
					start = i+1;
				}
    			
			}

	    	index_v++;
	    }
	}
	
}

//��ѵ���õ�Ȩ�������Բ��Լ� 
void Test()
{
	int TP=0, FN=0, FP=0, TN=0;
	for(int i=0; i<test_num; i++)
	{
		int sum=0;        // weight * X Ȩ����ѵ���ı��ĳ˻�
		for(int j=0; j<dim-1; j++)
		{
			sum+=weight[j]*test_sample[i][j];  
		}
		
		double p = 1/(1+exp(-1*sum));   //�ж���ǩΪ1�ĸ��� 

		
		//�ж�Ԥ���ǩ 
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
	
	
	double Accuracy = double(TP+TN)/(TP+FP+TN+FN);   //�����ж���ȷ�� 
	double Recall = double(TP)/(TP+FN);
	double Precision = double(TP)/(TP+FP);
	double F1 = double(2*Precision*Recall)/(Precision+Recall);
	cout<<TP<<" "<<FN<<" "<<FP<<" "<<TN<<endl;
	cout<<"��ȷ��: "<<Accuracy<<endl;
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
