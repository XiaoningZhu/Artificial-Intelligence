#include <iostream>
#include <fstream>
#include <cstring>
#include <string.h>
#include <sstream>
#include <map>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iomanip>

///////////////   K=1  //////////////////////

using namespace std;

//���ַ���������ת��Ϊ�������� 
double str_to_num(const string& str)  
{  
    istringstream iss(str);  
    double num;  
    iss >> num;  
    return num;      
}  


//k=6, 

map<string, int> voca_vec;                    //���治�ظ��ʻ㼰��λ�� 
int onehot[4000] = {0};                        //��ʱ����onehot�����һ��
int test[4000] = {0};                          //һ�������ı���onehot���� 
int counter = 0;                                  //���ظ��ʻ������ 
int K;                                         //  K����ֵ 
double emo_rate[246][6];                       // ѵ��������������ǩ ���� 
int num_text=246;                              //ѵ�����ı����� 


//��ѵ���������ַֿ����ֱ�õ����ı���������ǩ���ʣ�ͬʱ�õ����ظ����� 
void depart_text()
{
	
	ifstream read_train("Dataset_train.txt");          //��ȡѵ����
	ofstream write2("result2.txt");                    //ȥ����ź͸��ʣ��õ���д���ı� 
	if(!read_train)    cout<<"can not open the file"<<endl;
	else
	{  
		map<string, int>::iterator it;
		int num = 0;
		
		string line;                            //�����ȡ��һ������ 
	    while(!read_train.eof())
	    {
		    getline(read_train, line);
		    istringstream is(line);
		    
		    int k=0;
		    
		    //��ÿ���������ȡ�ַ������ݿո�Ͷ���������ѵ���� 
		    int len = line.length();
		    int len1;
		    //ȥ��ѵ���ı��� 
		    for(len1=0; len1<len; len1++)
		    {
		    	if(line[len1]==',')
		    	{
		    		k++;
		    		break;
		    	}
		    	    
		    }
		    
		    len1++;
		    int len2 = len1;
		    
		    //�õ��ı��еĴʻ㣬���γɲ��ظ����� 
		    string word;
		    for(;len1<len;len1++)
		    {
		    	if(line[len1]==' ') 
		    	{
		    		word = line.substr(len2, len1-len2);   //���� 
		    		write2<<word<<' ';
		    		it = voca_vec.find(word);       //ȷ���ʻ��Ƿ��ѱ������ڲ��ظ��ʻ������� 
	    			if(it == voca_vec.end())
	    			{
	    				voca_vec.insert(pair<string, int>(word, counter));  //��ĩβ����ʻ� 
	    				counter ++;
	    				
	    			}
	    			len2=len1+1;
		    	}
		    	else if(k==1)
				{
				
					if(line[len1]==',')
		    		{
		    			k++;
		    			word = line.substr(len2, len1-len2); //���� 
		    			write2<<word<<' ';
		    			it = voca_vec.find(word);       //ȷ���ʻ��Ƿ��ѱ������ڲ��ظ��ʻ������� 
	    				if(it == voca_vec.end())
	    				{
	    					voca_vec.insert(pair<string, int>(word, counter));  //��ĩβ����ʻ� 
	    					counter ++;
	    				
	    				}
	    			len2=len1+1;
		    		}
		    	}
		    	else break;
		    }
		    
		    
		    //�õ�����������ѵ���ı���������ǩ���� 
		    int j=0;
		    for(; len1<len;len1++)
		    {
		    	    if(line[len1]==',')
		    		{
		    			
		    			word = line.substr(len2, len1-len2);

		    			emo_rate[num][j] = str_to_num(word);   //����������ǩ���� 

	    			    len2=len1+1;
	    			    j++;
		    		}
		    		
		    		if(len1==len-1)
		    		{
		    			word = line.substr(len2, len1-len2+1);
		    			
		    			emo_rate[num][j] = str_to_num(word);   //����������ǩ���� 
		    			
						j++;
		    		}
		    }
		    num++;
		    write2<<'\n';
		}
	}

}


//�õ�һ�������ı���ÿ��������ǩ���� 
double* get_testRate(int *test)
{
  
	
	int onehot1[4000] = {0};                          //һ��ѵ���ı���onehot����
	double dis[num_text];                              //����ѵ���ı�������ı��ľ��� 
	
	string emotion, emo_temp;
	//double emo_rate_test[6]={0};
	double *emo_rate_test = new double[6];            //����ÿ�������ı���������ǩ���� 
	for(int i=0; i<6; i++) emo_rate_test[i]=0;
	int c=0;
	
	ifstream read_train("result2.txt");          //��ȡ���Լ����ı� 
	if(!read_train)    cout<<"can not open the file"<<endl;
	else
	{
		
		string line;                                    //�����ȡ��һ������ 
		map<string, int>::iterator it;
		
		int k=0;
	    while(!read_train.eof())
	    {
	    	c++;

	    	for(int i=0; i<4000; i++) onehot1[i]=0;
	    	
	    	getline(read_train, line);
		    istringstream is(line);
	    	string word;     
			//�õ�ÿ��ѵ���ı���onehot����                   
	    	while(is>>word)                 //�����ȡ�ʻ� 
	    	{

	    			
	    		  //�ҵ��˴ʻ��ڴʻ���е�λ�ò���onehot����ֵΪ1 
	    		it = voca_vec.find(word);
	    		int i = it->second;          //�ҵ��ôʻ��ڲ��ظ��������λ�� 
	    		onehot1[i] = 1;
	    		
	    	}

	    	
	        double sum = 0.0000;
	        //�õ����� 
	    	for(int j=0; j < counter; j++)
	    	{
	    		sum = sum + pow((onehot1[j]-test[j]),2);
	    		//sum=sum+abs(onehot1[j]-test[j]);
	    	}
	    	sum = sqrt(sum);
	    	
	    	dis[k]=sum; //������� 
			k++;
		    	
	    }
	    
		//����������, index��¼��Ӧ��ѵ���ı����������ʵ�λ�� 
	    int index[num_text];
	    for(int j=0; j<num_text; j++)
	    {
	    	index[j]=j;
	    }
	    for(int j=0; j<num_text-1; j++)
	    {
	    	for(int i=j; i<num_text-1; i++)
	    	{
	    		if(dis[j]>dis[i]) 
	    		{
	    			swap(dis[j], dis[i]);
	    			swap(index[j], index[i]);
	    		}
	    	}
	    }   
	
		
		//��������ı����������� 
	    for(int j =0; j <6; j++)
	    {
	    	for(int i=0; i<K; i++)
			{
	    		emo_rate_test[j]= emo_rate[index[i]][j]/(dis[i])+(emo_rate_test[j]);
	    	}
	    		
		}
		//cout<<emo_rate_test[0]<<' ' <<emo_rate_test[2]<<endl;

		//��һ��
		double sum1=0.0000;
		for(int j=0; j<6;j++)  sum1=sum1+emo_rate_test[j];
		for(int j=0; j<6;j++) 
		{
			
			emo_rate_test[j]=emo_rate_test[j]/sum1;         //�����ı�����������       
		}
	}
	return emo_rate_test;
	
}


//����ѵ���� 
void data_test()
{
	ofstream write("14353437_XiaoningZhu_regression.txt");  
	write<<"id anger disgust fear joy sad surprise"<<endl;
	ifstream read_test("Dataset_validation.txt");          //��ȡѵ����
	if(!read_test)    cout<<"can not open the file"<<endl;
	else
	{
		map<string, int>::iterator it;
		
		int c = 0;
		string line;                            //�����ȡ��һ������ 
		
	    while(!read_test.eof())
	    {
			c++;
		    getline(read_test, line);
		    istringstream is(line);

		    for(int i=0; i<4000; i++) test[i]=0;
		    
		    int k=0;
		    
		    // ȥ�������ı���� 
		    int len = line.length();
		    int len1;
		    for(len1=0; len1<len; len1++)
		    {
		    	if(line[len1]==',')
		    	{
		    		k++;
		    		break;
		    	}
		    	    
		    }
		    
		    len1++;
		    int len2 = len1;
		    
		    //�õ������ı����� 
		    string word;
		    for(;len1<len;len1++)
		    {
		    	if(line[len1]==' ') 
		    	{
		    		word = line.substr(len2, len1-len2);
		    		
		    		//�ҵ��˴ʻ��ڴʻ���е�λ�ò���onehot����ֵΪ1 
	    		    it = voca_vec.find(word);
	    		                
	    		    if(it != voca_vec.end())          //�����ڲ��ظ��������ҵ��ôʻ� 
	    		    {
	    			    int i = it->second;          //�ҵ��ôʻ��ڲ��ظ��������λ�� 
	    			    test[i] = 1;
	    		    }
	    			len2=len1+1;
		    	}
		    	else if(k==1)
				{
				
					if(line[len1]==',')
		    		{
		    			k++;
		    			word = line.substr(len2, len1-len2);
		    			//�ҵ��˴ʻ��ڴʻ���е�λ�ò���onehot����ֵΪ1 �� �õ�onehot���� 
	    		    it = voca_vec.find(word);
	    		                
	    		    if(it != voca_vec.end())          //�����ڲ��ظ��������ҵ��ôʻ� 
	    		    {
	    			    int i = it->second;          //�ҵ��ôʻ��ڲ��ظ��������λ�� 
	    			    test[i] = 1;
	    		    }
	    			len2=len1+1;
		    		}
		    	}
		    	else break;
		    }
		    
		    double* r;                     //r����ÿ�������ı��������������� 
		    r=get_testRate(test);          //�õ������ı���ǩ���� 
		    
		    write<<"test"<<c<<' ';
		    for(int i=0; i<6; i++)
		    {
		    	write<<r[i]<<' ';      //�������ı�������ǩ����д���ı�
		    }
		    write<<endl;
		}
	}
}

int main()
{

    cout<<"K=";
    cin>>K;
	depart_text();

	data_test();
	return 0;
}


