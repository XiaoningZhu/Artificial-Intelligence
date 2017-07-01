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

//将字符串型数字转化为整形数字 
double str_to_num(const string& str)  
{  
    istringstream iss(str);  
    double num;  
    iss >> num;  
    return num;      
}  


map<string, int> voca_vec;                    //储存不重复词汇及其位置 
double testTF[4000] = {0};                    //一个测试文本的TF矩阵 
int counter = 0;                                  //不重复词汇的数量 
double emo_rate[246][6];                       // 训练集所有情绪概率 
int num_text=246;                              //训练集文本个数 
const int emo_num=6;                          //情绪标签种类个数 


//将训练集各部分分开，分别得到纯文本result2，情绪标签概率，同时得到不重复向量 
void depart_text()
{
	
	ifstream read_train("Dataset_train.csv");          //读取训练集
	ofstream write2("result2.txt");                    //去除编号和概率，得到纯文本 
	if(!read_train)    cout<<"can not open the file"<<endl;
	else
	{  
		map<string, int>::iterator it;
		int num = 0;
		
		string line;                            //储存读取的一行数据 
		
		getline(read_train, line);
	    while(!read_train.eof())
	    {
		    getline(read_train, line);
		    istringstream is(line);
		    
		    int k=0;
		    
		    //在每行中逐个读取字符，根据空格和逗号来分离训练集 
		    int len = line.length();
		    int len1;
		    //去除训练文本号 
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
		    
		    //得到文本中的词汇，并形成不重复向量 
		    string word;
		    for(;len1<len;len1++)
		    {
		    	if(line[len1]==' ') 
		    	{
		    		word = line.substr(len2, len1-len2);   //单词 
		    		write2<<word<<' ';
		    		it = voca_vec.find(word);       //确定词汇是否已被储存在不重复词汇向量中 
	    			if(it == voca_vec.end())
	    			{
	    				voca_vec.insert(pair<string, int>(word, counter));  //在末尾插入词汇 
	    				counter ++;
	    				
	    			}
	    			len2=len1+1;
		    	}
		    	else if(k==1)
				{
				
					if(line[len1]==',')
		    		{
		    			k++;
		    			word = line.substr(len2, len1-len2); //单词 
		    			write2<<word<<' ';
		    			it = voca_vec.find(word);       //确定词汇是否已被储存在不重复词汇向量中 
	    				if(it == voca_vec.end())
	    				{
	    					voca_vec.insert(pair<string, int>(word, counter));  //在末尾插入词汇 
	    					counter ++;
	    				
	    				}
	    			len2=len1+1;
		    		}
		    	}
		    	else break;
		    }
		    
		    
		    //得到并储存所有训练文本的情绪标签概率 
		    int j=0;
		    for(; len1<len;len1++)
		    {
		    	    if(line[len1]==',')
		    		{
		    			
		    			word = line.substr(len2, len1-len2);

		    			emo_rate[num][j] = str_to_num(word);   //储存情绪标签概率 

	    			    len2=len1+1;
	    			    j++;
		    		}
		    		
		    		if(len1==len-1)
		    		{
		    			word = line.substr(len2, len1-len2+1);
		    			
		    			emo_rate[num][j] = str_to_num(word);   //储存情绪标签概率 
		    			
						j++;
		    		}
		    }
		    num++;
		    write2<<'\n';
		}
	}

}


//得到一个测试文本的所有情绪标签概率 
double* get_testRate(double *testTF)              //testTF为一个训练文本的TF矩阵 
{
  
	double TF[4000] = {0};                          //一个训练文本的TF矩阵                            //储存训练文本与测试文本的距离 
	
	string emotion, emo_temp;
	double *emo_rate_test = new double[emo_num];            //储存每个测试文本的情绪标签概率 
	for(int i=0; i<emo_num; i++) emo_rate_test[i]=0;
	int c=0;
	
	ifstream read_train("result2.txt");          //读取测试集纯文本 
	if(!read_train)    cout<<"can not open the file"<<endl;
	else
	{
		
		string line;                                    //储存读取的一行数据 
		map<string, int>::iterator it;
		
	    while(!read_train.eof())
	    {

	    	for(int i=0; i<4000; i++) TF[i]=0;
	    	
	    	getline(read_train, line);
		    istringstream is(line);
	    	string word;     
	    	
	    	int tot_num=0;
			//得到每个训练文本的TF向量                   
	    	while(is>>word)                 //逐个读取词汇 
	    	{
	    			
	    		  //找到此词汇在词汇表中的位置并在onehot矩阵赋值为1 
	    		it = voca_vec.find(word);
	    		int i = it->second;          //找到该词汇在不重复向量表的位置 
	    		TF[i]++;
	    		tot_num++;
	    		
	    	}
	    	
	    	//计算有高斯平滑的每个TF矩阵 
	    	for(int i=0; i<counter; i++)
	    	{
	    		TF[i]=(TF[i]+1)/(tot_num+counter);
	    	}


            //计算每种标签的概率 
	    	double temp_rate = 1.000000;
	    	for(int i=0; i<counter; i++)
	        {
	    		for(int k=0; k<testTF[i]; k++)
	    		{
	    			temp_rate = TF[i]*temp_rate;  //累乘每个单词的概率 
	    			
	    		}
	    	}
	    	for(int i=0; i<emo_num; i++)//乘先验概率 
	    	{
	    		emo_rate_test[i]+=temp_rate*emo_rate[c][i];
	    	}

	    	c++;
	    	
	    	if(c==num_text)
	    	{
	    		break;
	    	}
	        
	    }
	    
		
		//归一化
		double sum1=0.000000;
		for(int j=0; j<emo_num;j++)  sum1=sum1+emo_rate_test[j];
		
		for(int j=0; j<emo_num;j++) 
		{
			
			emo_rate_test[j]=emo_rate_test[j]/sum1;         //测试文本的每种情绪概率    
		}
	}
	return emo_rate_test;
	
}


//计算测试集每种情绪概率 
void data_test()
{
	ofstream write("14353437_XiaoningZhu_regression.txt");  
	//write<<"id anger disgust fear joy sad surprise"<<endl;
	ifstream read_test("Dataset_test.csv");          //读取测试集
	if(!read_test)    cout<<"can not open the file"<<endl;
	else
	{

		map<string, int>::iterator it;
		
		int c = 0;
		string line;                            //储存读取的一行数据 
		
		getline(read_test, line);
		
	    while(!read_test.eof())
	    {
			c++;
		    getline(read_test, line);
		    istringstream is(line);

		    for(int i=0; i<4000; i++) testTF[i]=0;  //一个测试文本的TF矩阵 
		    
		    int k=0;
		    int to_num=0;
		    
		    // 去除测试文本序号 
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
		    
		    //得到测试文本单词 
		    string word;
		    for(;len1<len;len1++)
		    {
		    	if(line[len1]==' ') 
		    	{
		    		word = line.substr(len2, len1-len2);
		    		
		    		//找到此词汇在词汇表中的位置并在TF矩阵赋值加 1 
	    		    it = voca_vec.find(word);
	    		                
	    		    if(it != voca_vec.end())          //若能在不重复向量表找到该词汇 
	    		    {
	    			    int i = it->second;          //找到该词汇在不重复向量表的位置 
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
		    			//找到此词汇在词汇表中的位置并在onehot矩阵赋值为1 ， 得到onehot向量 
	    		    	it = voca_vec.find(word);
	    		                
	    		    	if(it != voca_vec.end())          //若能在不重复向量表找到该词汇 
	    		    	{
	    			    	int i = it->second;          //找到该词汇在不重复向量表的位置 
	    			    	testTF[i]++;
	    			    	to_num++;
	    		    	}
	    				len2=len1+1;
		    		}
		    	}
		    	else break;
		    	
		    }
   		
		    	double* r;                     //r储存每个测试文本的六个情绪概率 
		    	r=get_testRate(testTF);          //得到测试文本标签概率 
		    
		    	//write<<"test"<<c<<' ';
		    	for(int i=0; i<emo_num; i++)
		    	{
		    		write<<r[i]<<' ';      //将测试文本情绪标签概率写入文本
		    		
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


