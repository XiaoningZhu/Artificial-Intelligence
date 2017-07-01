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


map<string, int> voca_vec;                    //���治�ظ��ʻ㼰��λ�� 
double testTF[4000] = {0};                    //һ�������ı���TF���� 
int counter = 0;                                  //���ظ��ʻ������ 
double emo_rate[246][6];                       // ѵ���������������� 
int num_text=246;                              //ѵ�����ı����� 
const int emo_num=6;                          //������ǩ������� 


//��ѵ���������ַֿ����ֱ�õ����ı�result2��������ǩ���ʣ�ͬʱ�õ����ظ����� 
void depart_text()
{
	
	ifstream read_train("Dataset_train.csv");          //��ȡѵ����
	ofstream write2("result2.txt");                    //ȥ����ź͸��ʣ��õ����ı� 
	if(!read_train)    cout<<"can not open the file"<<endl;
	else
	{  
		map<string, int>::iterator it;
		int num = 0;
		
		string line;                            //�����ȡ��һ������ 
		
		getline(read_train, line);
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


//�õ�һ�������ı�������������ǩ���� 
double* get_testRate(double *testTF)              //testTFΪһ��ѵ���ı���TF���� 
{
  
	double TF[4000] = {0};                          //һ��ѵ���ı���TF����                            //����ѵ���ı�������ı��ľ��� 
	
	string emotion, emo_temp;
	double *emo_rate_test = new double[emo_num];            //����ÿ�������ı���������ǩ���� 
	for(int i=0; i<emo_num; i++) emo_rate_test[i]=0;
	int c=0;
	
	ifstream read_train("result2.txt");          //��ȡ���Լ����ı� 
	if(!read_train)    cout<<"can not open the file"<<endl;
	else
	{
		
		string line;                                    //�����ȡ��һ������ 
		map<string, int>::iterator it;
		
	    while(!read_train.eof())
	    {

	    	for(int i=0; i<4000; i++) TF[i]=0;
	    	
	    	getline(read_train, line);
		    istringstream is(line);
	    	string word;     
	    	
	    	int tot_num=0;
			//�õ�ÿ��ѵ���ı���TF����                   
	    	while(is>>word)                 //�����ȡ�ʻ� 
	    	{
	    			
	    		  //�ҵ��˴ʻ��ڴʻ���е�λ�ò���onehot����ֵΪ1 
	    		it = voca_vec.find(word);
	    		int i = it->second;          //�ҵ��ôʻ��ڲ��ظ��������λ�� 
	    		TF[i]++;
	    		tot_num++;
	    		
	    	}
	    	
	    	//�����и�˹ƽ����ÿ��TF���� 
	    	for(int i=0; i<counter; i++)
	    	{
	    		TF[i]=(TF[i]+1)/(tot_num+counter);
	    	}


            //����ÿ�ֱ�ǩ�ĸ��� 
	    	double temp_rate = 1.000000;
	    	for(int i=0; i<counter; i++)
	        {
	    		for(int k=0; k<testTF[i]; k++)
	    		{
	    			temp_rate = TF[i]*temp_rate;  //�۳�ÿ�����ʵĸ��� 
	    			
	    		}
	    	}
	    	for(int i=0; i<emo_num; i++)//��������� 
	    	{
	    		emo_rate_test[i]+=temp_rate*emo_rate[c][i];
	    	}

	    	c++;
	    	
	    	if(c==num_text)
	    	{
	    		break;
	    	}
	        
	    }
	    
		
		//��һ��
		double sum1=0.000000;
		for(int j=0; j<emo_num;j++)  sum1=sum1+emo_rate_test[j];
		
		for(int j=0; j<emo_num;j++) 
		{
			
			emo_rate_test[j]=emo_rate_test[j]/sum1;         //�����ı���ÿ����������    
		}
	}
	return emo_rate_test;
	
}


//������Լ�ÿ���������� 
void data_test()
{
	ofstream write("14353437_XiaoningZhu_regression.txt");  
	//write<<"id anger disgust fear joy sad surprise"<<endl;
	ifstream read_test("Dataset_test.csv");          //��ȡ���Լ�
	if(!read_test)    cout<<"can not open the file"<<endl;
	else
	{

		map<string, int>::iterator it;
		
		int c = 0;
		string line;                            //�����ȡ��һ������ 
		
		getline(read_test, line);
		
	    while(!read_test.eof())
	    {
			c++;
		    getline(read_test, line);
		    istringstream is(line);

		    for(int i=0; i<4000; i++) testTF[i]=0;  //һ�������ı���TF���� 
		    
		    int k=0;
		    int to_num=0;
		    
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
		    		
		    		//�ҵ��˴ʻ��ڴʻ���е�λ�ò���TF����ֵ�� 1 
	    		    it = voca_vec.find(word);
	    		                
	    		    if(it != voca_vec.end())          //�����ڲ��ظ��������ҵ��ôʻ� 
	    		    {
	    			    int i = it->second;          //�ҵ��ôʻ��ڲ��ظ��������λ�� 
	    			    testTF[i]++;
	    			    to_num++;
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
	    			    	testTF[i]++;
	    			    	to_num++;
	    		    	}
	    				len2=len1+1;
		    		}
		    	}
		    	else break;
		    	
		    }
   		
		    	double* r;                     //r����ÿ�������ı��������������� 
		    	r=get_testRate(testTF);          //�õ������ı���ǩ���� 
		    
		    	//write<<"test"<<c<<' ';
		    	for(int i=0; i<emo_num; i++)
		    	{
		    		write<<r[i]<<' ';      //�������ı�������ǩ����д���ı�
		    		
		   	 	}
		   		write<<endl;
	
		    
		
		}
	}
}

int main()
{

	depart_text();

	data_test();
	return 0;
}


