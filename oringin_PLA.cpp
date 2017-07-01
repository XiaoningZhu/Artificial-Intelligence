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
const int text_num=100;              //ѵ���ı����� 
const int dim=10001;                 //����ά�� 
const int test_num=100;              //�����ı����� 

int train_sample[text_num][dim];     //����ѵ������������ 

//��ȡѵ��������
void read_trainData()
{
	ifstream read_trainData("train_data.txt");          //��ȡѵ�������� 
	if(!read_trainData)    cout<<"can not open the file"<<endl;
	else
	{ 
	    int index_v=0;
		string line;                             
	    while(!read_trainData.eof())
	    {
		    getline(read_trainData, line);        //��ȡһ������
		    istringstream is(line);
		    
	    	int word; 
	    	train_sample[index_v][0]=1;
			int index_h=1;                      
	    	while(is>>word)                    //��ȡһ������ 
	    	{
	    		train_sample[index_v][index_h]=word; //������������ 
	    		index_h++;
	    	}
	    	index_v++;
	    }
	}
	

}

//��ȡѵ������ǩ 
int train_label[text_num]={0};       //����ѵ������ǩ 
void read_trainLabel()
{
	ifstream read_trainLabel("train_labels.txt");          //��ȡѵ������ǩ 
	if(!read_trainLabel)    cout<<"can not open the file"<<endl;
	else
	{ 
	    int index=0;
		string line;                             
		while(!read_trainLabel.eof())
	    {
		    getline(read_trainLabel, line);        //��ȡһ������
		    istringstream is(line);
		    
	    	int word;                       
	    	while(is>>word)                    //��ȡһ������ 
	    	{
	    		train_label[index]=word;       // ����ѵ������ǩ 
	    	} 
	    	index++;
	    }
	    
	    
	}
	
}

int weight[dim]={0};           //����Ȩ�� 
int update_num=0;              //���´���
//�õ�ѵ�����Ȩ�� 
void getWeight()
{
	
	int j=0;
	int flag=0;
	while(j<text_num)
	{
		int sum=0;
		for(int i=0; i<dim; i++)
		{
			sum=weight[i]*train_sample[j][i]+sum;  //����Ȩ�����ı�������˵Ľ�� 
		}
		
		//Ԥ���ǩ 
		int sign;
		if(sum>0) sign=1;
		else if(sum<0) sign=-1;
		else sign =0;

		if(sign!=train_label[j])   //����Ȩ�� 
		{
			update_num++;
			for(int i=0; i<dim; i++)
			{
				weight[i]=weight[i]+train_sample[j][i]*train_label[j];
				
			}
			
			flag=1;      //��־���º��ͷ��ʼ���� 
		}
		else flag=0;
		
		if(flag==1) j=0;
		else j++;
	}

	cout<<"Ȩ�ظ��´����� "<<update_num<<endl;

}

//��ȡ���Լ�����
int test_sample[test_num][dim];                //������Լ����� 
void read_testData()
{
	ifstream read_testData("test_data.txt");          //��ȡ���Լ����� 
	if(!read_testData)    cout<<"can not open the file"<<endl;
	else
	{ 
	    int index_v=0;
		string line;                             
	    while(!read_testData.eof())
	    {
		    getline(read_testData, line);        //��ȡһ������
		    istringstream is(line);
		    
	    	int word; 
	    	test_sample[index_v][0]=1;
			int index_h=1;                      
	    	while(is>>word)                    //��ȡһ������ 
	    	{
	    		test_sample[index_v][index_h]=word;   //������Լ����� 
	    		index_h++;
	    	}
	    	index_v++;
	    }
	}
}

//��ȡ���Լ��ı���ǩ 
int test_label[test_num]={0};
void read_testLabel()
{
	ifstream read_testLabel("test_labels.txt");          //����ѵ������ǩ 
	if(!read_testLabel)    cout<<"can not open the file"<<endl;
	else
	{ 
	    int index=0;
		string line;                             
	    while(!read_testLabel.eof())
	    {
		    getline(read_testLabel, line);        //��ȡһ������
		    istringstream is(line);
		    
	    	int word;                       
	    	while(is>>word)                    //��ȡһ������ 
	    	{
	    		test_label[index]=word;     //�����ǩ 
	    	}
	    	index++;
	    }
	}
	
	
}


double TP=0, FN=0, FP=0, TN=0;
double Accuracy=0, Precision=0, Recall=0, F1 =0;
//���� ����ָ��
void testLevel(int TF, int FN, int FP, int TN)
{
	Accuracy = (TP+TN)/(TP+FP+TN+FN);
	Precision = TP/(TP+FP);
	Recall = TP/(TP+FN);
	F1 = (2*Precision*Recall)/(Precision+Recall);
}

//��ѵ���õ�Ȩ�������Բ��Լ� 
void Test()
{
	for(int i=0; i<test_num; i++)
	{
		int sum=0;
		for(int j=0; j<dim; j++)
		{
			sum+=weight[j]*test_sample[i][j];     ////����Ȩ�����ı�������˵Ľ��
		}
		
		//Ԥ���ǩ 
		int sign;
		if(sum>0) sign =1;
		else if(sum<0) sign=-1;
		else 
		{
			if(i%2==0) sign=1;
			else sign=-1;
		}
		
		//����TP, FN, FP, TN 
		if(test_label[i]==1 && sign==1) TP++;
		if(test_label[i]==1 && sign==-1) FN++;
		if(test_label[i]==-1 && sign==1) FP++;
		if(test_label[i]==-1 && sign==-1) TN++;

	}
	
	testLevel(TP, FN, FP, TN);  //�������ָ�� 
	
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











