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


//���ַ���������ת��Ϊ�������� 
double str_to_num(const string& str)  
{  
    istringstream iss(str);  
    double num;  
    iss >> num;  
    return num;      
}

const int emo_num=3;                        //������ǩ������� 
map<string, int> voca_vec[emo_num];             //����ʴ����ظ��ʻ㼰���ڴʻ�������λ�� 
map<string, int> voca_vec_train;                //����ȫ��ѵ���Ͳ����ı����ظ��ʻ� 
int counter1= 0;                              //ȫ��ѵ���ı����ظ��ʻ������ 
int test_num = 1000;                         //ѵ���ı��� 
int train_num = 246;

double counter[emo_num] = {0.000000};      //ÿ���ʴ��Ĳ��ظ��ʻ���� 
int train_emo_num[emo_num]={0};               // ѵ���ı�ÿ��������ǩ�ܸ��� 
double emo_rate[emo_num]={0};                //ÿ������������� 
double tot_word_num[emo_num]={0.000000};    //ÿ���ʴ��Ĵʵ��ܸ��� 

//������в��ظ�������Ͳ��ظ��ʻ�������Լ�������� 
void getVoca_vector()
{

	ifstream read_train("train.txt");          //��ȡѵ���� 
	if(!read_train)    cout<<"can not open the file"<<endl;
	else
	{   
		map<string, int>::iterator it;
		string line;           
		getline(read_train, line);                  
	    while(!read_train.eof())
	    {
		    getline(read_train, line);        //��ȡһ������
		    istringstream is(line);
	    	string word;                       
	    	int i=0;
	    	int flag=0;
	    	while(is>>word)                    //��ȡһ����
	    	{
	    		i++;
	    		if(i==2)
	    		{
	    			string la[] = {"1", "2", "3", "4", "5", "6"};
	    			for(int i=0; i<emo_num; i++)  //�б�ͬ��ǩ�����ı� 
	    			{
	    				if(word==la[i])
	    				{
	    					train_emo_num[i]++;   //�ñ�ǩ�ı�������һ 
	    					flag=str_to_num(word); //��־�Ÿ��ı��ı�ǩ 
	    				}
	    			}

	    		}
	    		if(i>=4)                      //ȥ����ź� 
	    		{
	    			
	    			tot_word_num[flag-1]=tot_word_num[flag-1]+1.000000;  
	    			
	    			
	    			it = voca_vec_train.find(word);       //ȷ���ʻ��Ƿ��ѱ������ڲ��ظ��ʻ������� 
	    			if(it == voca_vec_train.end())         //δ�������ڲ��ظ��ʻ������� 
	    			{
	    				voca_vec_train.insert(pair<string, int>(word, counter1));  //�ڴʻ�����ĩβ����ʻ� 
	    				counter1 ++;          //����ѵ���ı����ظ��ʻ���� 
	    				
	    			}
	    			
	    			
	    			for(int i=0; i<emo_num; i++)
	    			{
	    				if(flag==i+1)
	    				{ 
	    					it = voca_vec[i].find(word);       //ȷ���ʻ��Ƿ��ѱ������� �ʴ� ���ظ��ʻ������� 
	    					if(it == voca_vec[i].end())         //δ�������ڲ��ظ��ʻ������� 
	    					{
	    						voca_vec[i].insert(pair<string, int>(word, counter[i]));  //�ڴʻ�����ĩβ����ʻ� 
	    						counter[i] = counter[i]+1.000000;          //����ʴ����ظ��ʻ���� 
	    					
	    					}
	    				}
	    			}		
	    			
	    		}
	    	}
	   }
	   
    }
    
	for(int i=0; i<emo_num; i++)
	{
		emo_rate[i]=(double)train_emo_num[i]/t_num;    //����ÿ������������� 
	}
	
	ifstream read_test("test.txt");          //��ȡ���Լ� 
	if(!read_test)    cout<<"can not open the file"<<endl;
	else
	{   
		map<string, int>::iterator it;
		string line;           
		getline(read_test, line);                  
	    while(!read_test.eof())
	    {
		    getline(read_test, line);        //��ȡһ������
		    istringstream is(line);
	    	string word;                       
	    	int i=0;
	    	int flag=0;
	    	while(is>>word)                    //��ȡһ����
	    	{
	    		i++;
	    		if(i>=4)                      //ȥ����ź� 
	    		{
	    			
	    			it = voca_vec_train.find(word);       //ȷ���ʻ��Ƿ��ѱ������ڲ��ظ��ʻ������� 
	    			if(it == voca_vec_train.end())         //δ�������ڲ��ظ��ʻ������� 
	    			{
	    				voca_vec_train.insert(pair<string, int>(word, counter1));  //�ڴʻ�����ĩβ����ʻ� 
	    				counter1 ++;          //����ѵ���ı����ظ��ʻ���� 
	    				
	    			}
	    		}
	    	}
	    }
	}
	

}


//����һ�������ı��ı�ǩ���Ϊlabel�ĸ��� 
double getPredict_Label(vector<string> vec, int label)           //vecΪ�����һ�������ı�    
{
   
	ifstream read_train("train.txt");          //��ȡѵ���� 
	if(!read_train)    cout<<"can not open the file"<<endl;
	else
	{   
		int len=vec.size();    
		
		double k_num[1000]={0.000000};
		map<string, int>::iterator it;
		string line;           
		getline(read_train, line);                  
	    while(!read_train.eof())
	    {
	    	
		    getline(read_train, line);        //��ȡһ������
		    istringstream is(line);
	    	string word;                       
	    	int i=0;
	    	int flag=0;
	    	while(is>>word)                    //��ȡһ����
	    	{
	    		i++;
	    		if(i==2)
	    		{
	    			if(str_to_num(word)==label)
	    			{
	    				flag=1;    //���ı� ��ǩΪ�봫��label ��ͬ��ѡ�����ı� 
	    			}
	    			else flag=0;
	    		}
	    		
	    		if(i>=4)
				{
					if(flag==1)
	    			{
	    			
	    			for(int i=0; i<len; i++)
	    			{
	    				if(word==vec[i])
	    				{
	    					k_num[i]=k_num[i]+1.000000;  //��������ı���ÿ�������ڴʴ��г��ֵĴ��� 
	    					
	    				}
	    				
	    			}
	    			}
				}
	    		
	    	}
	    }
	    
	    double e_rate=1.000000;
	    for(int i=0; i<len; i++)
	    {
	    	//e_rate = e_rate*((double)(k_num[i]+0.100)/(double)(tot_word_num[label-1]+counter1*0.100));
	    	e_rate = e_rate*((double)(k_num[i]+1.000000)/(double)(tot_word_num[label-1]+counter1)); //�����и�˹ƽ����δ��������ʵ���ֵ 
			//e_rate = e_rate*((double)(k_num[i])/(double)(tot_word_num[label-1])); 	    	

	    }

	    e_rate = e_rate*emo_rate[label-1];  //��������ʵõ���ǩ���Ϊlabel�ĸ��� 

	    return e_rate;
	}
}


//��������ı�Ԥ����ȷ�ĸ��� 
double getCorrect_rate()
{
	double num= 0;
    double num1 = 0;
	
	ifstream read_test("test.txt");          //��ȡ���Լ� 
	//cout<<"Ԥ����ȷ�Ĳ����ı���ǩΪ��"<<endl; 

	if(!read_test)    cout<<"can not open the file"<<endl;
	else
	{	     
		string emotion;
		string line;   
		getline(read_test, line);       //���Լ�ȥ����ͷһ��                             
		map<string, int>::iterator it;
	    while(!read_test.eof())
	    {
	    	vector<string> vec;
	    	num++;
	    	getline(read_test, line);       //��ȡ���Լ�һ���ı� 
		    istringstream is(line);
	    	string word; 
			int i=0;
			int label;                              
	    	while(is>>word)                 //�����ȡ�ʻ� 
	    	{
	    		i ++;
	    		
	    		if(i==2)
	    		label = str_to_num(word);           //�ò����ı���������ȷ�ı�ǩ
	    		
	    		if(i>=4)
				{
					//�ڴʻ�����Ҹôʻ� 
	    		    //it = voca_vec_train.find(word);
	    		                
	    		    //if(it != voca_vec_train.end())          //�����ڲ��ظ��������ҵ��ôʻ� 
	    		    //{
	    			//    int i = it->second;          //�ҵ��ôʻ��ڲ��ظ��������λ�� 
	    			//    test[i] = 1;
	    			    vec.push_back(word);   //���ı�����vec�� 
	    			    
	    		    //}

	    		}
	    		
	    	}

	    	double testRate[emo_num];
	    
	    	//����ò����ı�ÿ��������ǩ�ĸ��� 
	        for(int i=0; i<emo_num; i++)
	        {
	        	testRate[i] = getPredict_Label(vec, i+1);   //�õ������ı�Ԥ��ı�ǩ���� 
	        }
	        
			// �ҵ���������������ǩ 
            int testlabel=1;               //��¼��������������ǩ  
            double temp=testRate[0];
	    	for(int i=1; i<emo_num; i++)
	    	{
	    		if(temp<testRate[i])
	    		{
	    			testlabel = i+1;
	    			temp=testRate[i];
	    		}
	    	}
	    	
	    	cout<<testlabel<<' ';
			//cout<<' '<<label<<endl;
	    	
	    	if(testlabel == label) 
			{
				//cout<<label<<' ';
				num1++;    //Ԥ����ȷ�Ĳ����ı����� 
	    	}
	    	
	
	}
	cout<<endl;
	cout<<num1<<' ';
	cout<<num<<endl;
	//cout<<"����ȷ�Ĳ����ı�����Ϊ�� "<<num1<<endl;
    double correct_rate = num1/num;   //�õ���ȷ�� 
	
	return correct_rate;
	}
}

int main()
{
	getVoca_vector();
	
    double correct_rate = getCorrect_rate();
    
    cout<<"��ȷ��Ϊ: "<<fixed<<setprecision(8)<<correct_rate<<endl;
    
	
	return 0;
}
