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


//k=6, 

map<string, int> voca_vec;                    //储存不重复词汇及其位置 
int onehot[4000] = {0};                        //即时储存onehot矩阵的一行
int test[4000] = {0};                          //一个测试文本的onehot矩阵 
int counter = 0;                                  //不重复词汇的数量 
int K;                                         //  K的数值 
double emo_rate[246][6];                       // 训练集所有情绪标签 概率 
int num_text=246;                              //训练集文本个数 


//将训练集各部分分开，分别得到纯文本，情绪标签概率，同时得到不重复向量 
void depart_text()
{
	
	ifstream read_train("Dataset_train.txt");          //读取训练集
	ofstream write2("result2.txt");                    //去除编号和概率，得到和写纯文本 
	if(!read_train)    cout<<"can not open the file"<<endl;
	else
	{  
		map<string, int>::iterator it;
		int num = 0;
		
		string line;                            //储存读取的一行数据 
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


//得到一个测试文本的每个情绪标签概率 
double* get_testRate(int *test)
{
  
	
	int onehot1[4000] = {0};                          //一个训练文本的onehot矩阵
	double dis[num_text];                              //储存训练文本与测试文本的距离 
	
	string emotion, emo_temp;
	//double emo_rate_test[6]={0};
	double *emo_rate_test = new double[6];            //储存每个测试文本的情绪标签概率 
	for(int i=0; i<6; i++) emo_rate_test[i]=0;
	int c=0;
	
	ifstream read_train("result2.txt");          //读取测试集纯文本 
	if(!read_train)    cout<<"can not open the file"<<endl;
	else
	{
		
		string line;                                    //储存读取的一行数据 
		map<string, int>::iterator it;
		
		int k=0;
	    while(!read_train.eof())
	    {
	    	c++;

	    	for(int i=0; i<4000; i++) onehot1[i]=0;
	    	
	    	getline(read_train, line);
		    istringstream is(line);
	    	string word;     
			//得到每个训练文本的onehot向量                   
	    	while(is>>word)                 //逐个读取词汇 
	    	{

	    			
	    		  //找到此词汇在词汇表中的位置并在onehot矩阵赋值为1 
	    		it = voca_vec.find(word);
	    		int i = it->second;          //找到该词汇在不重复向量表的位置 
	    		onehot1[i] = 1;
	    		
	    	}

	    	
	        double sum = 0.0000;
	        //得到距离 
	    	for(int j=0; j < counter; j++)
	    	{
	    		sum = sum + pow((onehot1[j]-test[j]),2);
	    		//sum=sum+abs(onehot1[j]-test[j]);
	    	}
	    	sum = sqrt(sum);
	    	
	    	dis[k]=sum; //储存距离 
			k++;
		    	
	    }
	    
		//将距离排序, index记录对应的训练文本的情绪概率的位置 
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
	
		
		//计算测试文本的情绪概率 
	    for(int j =0; j <6; j++)
	    {
	    	for(int i=0; i<K; i++)
			{
	    		emo_rate_test[j]= emo_rate[index[i]][j]/(dis[i])+(emo_rate_test[j]);
	    	}
	    		
		}
		//cout<<emo_rate_test[0]<<' ' <<emo_rate_test[2]<<endl;

		//归一化
		double sum1=0.0000;
		for(int j=0; j<6;j++)  sum1=sum1+emo_rate_test[j];
		for(int j=0; j<6;j++) 
		{
			
			emo_rate_test[j]=emo_rate_test[j]/sum1;         //测试文本的情绪概率       
		}
	}
	return emo_rate_test;
	
}


//测试训练集 
void data_test()
{
	ofstream write("14353437_XiaoningZhu_regression.txt");  
	write<<"id anger disgust fear joy sad surprise"<<endl;
	ifstream read_test("Dataset_validation.txt");          //读取训练集
	if(!read_test)    cout<<"can not open the file"<<endl;
	else
	{
		map<string, int>::iterator it;
		
		int c = 0;
		string line;                            //储存读取的一行数据 
		
	    while(!read_test.eof())
	    {
			c++;
		    getline(read_test, line);
		    istringstream is(line);

		    for(int i=0; i<4000; i++) test[i]=0;
		    
		    int k=0;
		    
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
		    		
		    		//找到此词汇在词汇表中的位置并在onehot矩阵赋值为1 
	    		    it = voca_vec.find(word);
	    		                
	    		    if(it != voca_vec.end())          //若能在不重复向量表找到该词汇 
	    		    {
	    			    int i = it->second;          //找到该词汇在不重复向量表的位置 
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
		    			//找到此词汇在词汇表中的位置并在onehot矩阵赋值为1 ， 得到onehot向量 
	    		    it = voca_vec.find(word);
	    		                
	    		    if(it != voca_vec.end())          //若能在不重复向量表找到该词汇 
	    		    {
	    			    int i = it->second;          //找到该词汇在不重复向量表的位置 
	    			    test[i] = 1;
	    		    }
	    			len2=len1+1;
		    		}
		    	}
		    	else break;
		    }
		    
		    double* r;                     //r储存每个测试文本的六个情绪概率 
		    r=get_testRate(test);          //得到测试文本标签概率 
		    
		    write<<"test"<<c<<' ';
		    for(int i=0; i<6; i++)
		    {
		    	write<<r[i]<<' ';      //将测试文本情绪标签概率写入文本
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


