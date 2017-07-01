#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <map>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iomanip>

///////////////   K=1  //////////////////////

using namespace std;

//����ѵ���ı�������ǩ�͸�ѵ���ı�������ı�������ࡣ
class Corr
{
	public:
		void setCorr(double dis1, string emo1)
		{
			dis=dis1;
			emo=emo1;
		}
		
		void sort1(Corr c[], int n)//���ݾ������� 
        {
        	for(int i=0; i<n-1; i++)
			{
				for(int j=i; j<n-1;j++)
				{
					if((c[i]).dis>(c[j]).dis)
					{
						Corr temp=c[j];
						c[j]=c[i];
						c[i]=temp;
					}
				}
			}
		}


		double dis;//���� 
		string emo;//������ǩ 
};


map<string, int> voca_vec;                    //���治�ظ��ʻ㼰���ڴʻ�������λ�� 
int onehot[4000] = {0};                        //��ʱ����onehot�����һ��
int test[4000] = {0};                          //һ�������ı���onehot���� 
int counter = 0;                                  //���ظ��ʻ������ 
Corr dis_emo[246];                            //����k������ѵ���ı�������ǩ�͸�ѵ���ı�������ı�������ࡣ 
int K;

//��ò��ظ������� 
int getVoca_vector()
{

	ifstream read_train("train.txt");          //��ȡѵ���� 
	if(!read_train)    cout<<"can not open the file"<<endl;
	else
	{   
		map<string, int>::iterator it;
		string line;                             
	    while(!read_train.eof())
	    {
		    getline(read_train, line);        //��ȡһ������
		    istringstream is(line);
	    	string word;                       
	    	int i=0;
	    	while(is>>word)                    //��ȡһ����
	    	{
	    		i++;
	    		if(i>=4)                      //ÿ�д���ʽ�ı���ʼ��ȡ 
	    		{
	    			it = voca_vec.find(word);       //ȷ���ʻ��Ƿ��ѱ������ڲ��ظ��ʻ������� 
	    			if(it == voca_vec.end())         //δ�������ڲ��ظ��ʻ������� 
	    			{
	    				voca_vec.insert(pair<string, int>(word, counter));  //�ڴʻ�����ĩβ����ʻ� 
	    				counter ++;          //���㲻�ظ��ʻ���� 
	    				
	    			}
	    			
	    		}
	    	}
	   }
	   
    }
    
    cout<<counter<<endl;
	
}

//�ó������ı�Ԥ��ı�ǩ 
string getPredict_Label(int *test)                   //testΪһ��������һ�������ı�onehot��������� 
{
	int onehot1[4000] = {0};                          //���ڴ���һ��ѵ���ı���onehot���� 
	
	string emotion, emo_temp;
	ofstream write("train_text.txt");          //��ȡ���Լ� 
	
	ifstream read_train("train.txt");          //��ȡ���Լ� 
	if(!read_train)    cout<<"can not open the file"<<endl;
	else
	{
		
		string line;                                    
		map<string, int>::iterator it;
		int j=0;
	    while(!read_train.eof())
	    {
	    	memset(onehot1, 0, 4000*sizeof(int));        
	    	getline(read_train, line);                   //��ȡ��һ���ı� 
		    istringstream is(line);
	    	string word; 
			int i=0;                              
	    	while(is>>word)                 //�����ȡ�ʻ� 
	    	{
	    		i ++;
	    		
	    		if(i==3) emo_temp = word;
	    		
	    		if(i>=4)
	    		{	
	    		    //�ҵ��˴ʻ��ڴʻ���е�λ�ò���onehot����ֵΪ1 
	    		    it = voca_vec.find(word);
	    		    int i = it->second;          //�ҵ��ôʻ��ڲ��ظ��������λ�� 
	    			onehot1[i] = 1;  
	    		}	
	    	}
	    	
	    	for(int jj=0; jj<counter; jj++)
	    	{
	    		write<<onehot1[jj]<<' ';
			}
			write<<'\n';
	    	
	        double sum = 0;
	    
	    	for(int k=0; k < counter; k++)
	    	{
	    		//sum = sum + pow((onehot1[k]-test[k]),2);//����ŷʽ����
				sum +=  abs(onehot1[k]-test[k]);
	    	}
	    	
	    	//sum = sqrt(sum);  //�õ�ŷʽ���� 
	    
	    	(dis_emo[j]).setCorr(sum, emo_temp); //����ѵ���ı�������ı��ľ���͸�ѵ���ı���Ӧ��������ǩ�������� 
	    	j++; 	
	    }
	    
	    
	    
	    (dis_emo[0]).sort1(dis_emo, 247);    //���ݾ������� 

	    j=0;
	    int number[6]={0};     //���ڼ�¼k��ѵ���ı�����������ǩ�ֱ��ж��ٸ� 
	    
	    //���������С��k���ı��и����ǩ�ֱ��ж��ٸ� 
	    for (int k=0; k<246; k++)
	    {

	    	if((dis_emo[k]).emo=="joy") number[0]++;
	    	if((dis_emo[k]).emo=="sad") number[1]++;
	    	if((dis_emo[k]).emo=="fear") number[2]++;
	    	if((dis_emo[k]).emo=="surprise") number[3]++;
	    	if((dis_emo[k]).emo=="anger") number[4]++;
	    	if((dis_emo[k]).emo=="disgust") number[5]++;
	    			
	    	if(j>=K-1) break;
	    	j++;
		}
		
		
		int temp = number[0];
		for(int k=1; k<6; k++)
		{
			if(number[k]>temp) temp = number[k];//�ҵ�k��ѵ���ı�������������ǩ�����ı�ǩ���� 
		}
		
		vector<string> emo;

 		string emotions[6]={"joy", "sad", "fear","surprise", "anger", "disgust"}; //��number[k]�и�����Ӧ��������ǩ 
 		
 		int total=0;
 		for(int i=0; i<6; i++)
 		{
 			if(temp==number[i])
 			{
 				emo.push_back(emotions[i]); //���������ļ�����ǩ���������� ��֮�����ѡȡһ����ǩ 
 				total++;                     //����һ���ж��ٸ��������ļ�����ǩ
 			}
 		}

        //�Ӷ���������ı�ǩ��ѡȡһ����ǩ 
		int n=rand()%(total);   
		emotion = emo[n];

		
	}
	
	
	
	return emotion;
	
}

//��������ı�Ԥ����ȷ�ĸ��� 
double getCorrect_rate()
{
	double num= 0;
    double num1 = 0;
    ofstream write_Label("testLabel.txt");
	
	ifstream read_test("test.txt");          //��ȡ���Լ� 
	//ofstream write_test("E://onehot_test.txt");
	if(!read_test)    cout<<"can not open the file"<<endl;
	else
	{
		  
		string emotion;
		
		string line;                                 
		map<string, int>::iterator it;
	    while(!read_test.eof())
	    {
	    	num++;
	    	memset(test, 0, 4000*sizeof(int)); 
	    	getline(read_test, line);       //��ȡһ���ı� 
		    istringstream is(line);
	    	string word; 
			int i=0;                              
	    	while(is>>word)                 //�����ȡ�ʻ� 
	    	{
	    		i ++;
	    		
	    		if(i==3)
	    		emotion = word;           //�ò����ı���������ȷ�ı�ǩ 
	    		
	    		if(i>=4)
	    		{
	    			
	    		    //�ҵ��˴ʻ��ڴʻ���е�λ�ò���onehot����ֵΪ1 
	    		    it = voca_vec.find(word);
	    		                
	    		    if(it != voca_vec.end())          //�����ڲ��ظ��������ҵ��ôʻ� 
	    		    {
	    			    int i = it->second;          //�ҵ��ôʻ��ڲ��ظ��������λ�� 
	    			    test[i] = 1;
	    		    }
	    		    
	    		}
	    		
	    	}

	    	string emotion1;
	    
	    	emotion1 = getPredict_Label(test);   //�õ������ı�Ԥ��ı�ǩ 
	    	write_Label<<emotion1<<'\n';

	    	if(emotion== emotion1) num1++;      //Ԥ���ǩ��ȷ 
	    	
	    }
	    
	
	}
	
	cout<<"Ԥ����ȷ�ĸ���: "<<num1<<endl<<"�����ı��ܸ���Ϊ��"<<num<<endl;
	double correct_rate = num1/num;  //num1ΪԤ����ȷ�ĸ�����numΪ���в����ı��ĸ����� 
	
	return correct_rate;
}

int main()
{
	cout<<"K= ";

	cin>>K;
	
	
	getVoca_vector();
	
    double correct_rate = getCorrect_rate();
    
    cout<<"��ȷ��Ϊ: "<<fixed<<setprecision(8)<<correct_rate<<endl;
    
	
	
	return 0;
	
	
}
