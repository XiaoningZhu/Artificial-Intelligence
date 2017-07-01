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


//将字符串型数字转化为整形数字 
double str_to_num(const string& str)  
{  
    istringstream iss(str);  
    double num;  
    iss >> num;  
    return num;      
}

const int emo_num=3;                        //情绪标签种类个数 
map<string, int> voca_vec[emo_num];             //储存词袋不重复词汇及其在词汇向量表位置 
map<string, int> voca_vec_train;                //储存全部训练和测试文本不重复词汇 
int counter1= 0;                              //全部训练文本不重复词汇的数量 
int test_num = 1000;                         //训练文本数 
int train_num = 246;

double counter[emo_num] = {0.000000};      //每个词袋的不重复词汇个数 
int train_emo_num[emo_num]={0};               // 训练文本每种情绪标签总个数 
double emo_rate[emo_num]={0};                //每种情绪先验概率 
double tot_word_num[emo_num]={0.000000};    //每个词袋的词的总个数 

//获得所有不重复向量表和不重复词汇个数，以及先验概率 
void getVoca_vector()
{

	ifstream read_train("train.txt");          //读取训练集 
	if(!read_train)    cout<<"can not open the file"<<endl;
	else
	{   
		map<string, int>::iterator it;
		string line;           
		getline(read_train, line);                  
	    while(!read_train.eof())
	    {
		    getline(read_train, line);        //读取一行数据
		    istringstream is(line);
	    	string word;                       
	    	int i=0;
	    	int flag=0;
	    	while(is>>word)                    //读取一个词
	    	{
	    		i++;
	    		if(i==2)
	    		{
	    			string la[] = {"1", "2", "3", "4", "5", "6"};
	    			for(int i=0; i<emo_num; i++)  //判别不同标签类别的文本 
	    			{
	    				if(word==la[i])
	    				{
	    					train_emo_num[i]++;   //该标签文本个数加一 
	    					flag=str_to_num(word); //标志着该文本的标签 
	    				}
	    			}

	    		}
	    		if(i>=4)                      //去除序号后 
	    		{
	    			
	    			tot_word_num[flag-1]=tot_word_num[flag-1]+1.000000;  
	    			
	    			
	    			it = voca_vec_train.find(word);       //确定词汇是否已被储存在不重复词汇向量中 
	    			if(it == voca_vec_train.end())         //未被储存在不重复词汇向量中 
	    			{
	    				voca_vec_train.insert(pair<string, int>(word, counter1));  //在词汇向量末尾插入词汇 
	    				counter1 ++;          //计算训练文本不重复词汇个数 
	    				
	    			}
	    			
	    			
	    			for(int i=0; i<emo_num; i++)
	    			{
	    				if(flag==i+1)
	    				{ 
	    					it = voca_vec[i].find(word);       //确定词汇是否已被储存在 词袋 不重复词汇向量中 
	    					if(it == voca_vec[i].end())         //未被储存在不重复词汇向量中 
	    					{
	    						voca_vec[i].insert(pair<string, int>(word, counter[i]));  //在词汇向量末尾插入词汇 
	    						counter[i] = counter[i]+1.000000;          //计算词袋不重复词汇个数 
	    					
	    					}
	    				}
	    			}		
	    			
	    		}
	    	}
	   }
	   
    }
    
	for(int i=0; i<emo_num; i++)
	{
		emo_rate[i]=(double)train_emo_num[i]/t_num;    //计算每种情绪先验概率 
	}
	
	ifstream read_test("test.txt");          //读取测试集 
	if(!read_test)    cout<<"can not open the file"<<endl;
	else
	{   
		map<string, int>::iterator it;
		string line;           
		getline(read_test, line);                  
	    while(!read_test.eof())
	    {
		    getline(read_test, line);        //读取一行数据
		    istringstream is(line);
	    	string word;                       
	    	int i=0;
	    	int flag=0;
	    	while(is>>word)                    //读取一个词
	    	{
	    		i++;
	    		if(i>=4)                      //去除序号后 
	    		{
	    			
	    			it = voca_vec_train.find(word);       //确定词汇是否已被储存在不重复词汇向量中 
	    			if(it == voca_vec_train.end())         //未被储存在不重复词汇向量中 
	    			{
	    				voca_vec_train.insert(pair<string, int>(word, counter1));  //在词汇向量末尾插入词汇 
	    				counter1 ++;          //计算训练文本不重复词汇个数 
	    				
	    			}
	    		}
	    	}
	    }
	}
	

}


//计算一个测试文本的标签类别为label的概率 
double getPredict_Label(vector<string> vec, int label)           //vec为传入的一个测试文本    
{
   
	ifstream read_train("train.txt");          //读取训练集 
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
	    	
		    getline(read_train, line);        //读取一行数据
		    istringstream is(line);
	    	string word;                       
	    	int i=0;
	    	int flag=0;
	    	while(is>>word)                    //读取一个词
	    	{
	    		i++;
	    		if(i==2)
	    		{
	    			if(str_to_num(word)==label)
	    			{
	    				flag=1;    //该文本 标签为与传入label 相同，选定该文本 
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
	    					k_num[i]=k_num[i]+1.000000;  //计算测试文本中每个单词在词袋中出现的次数 
	    					
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
	    	e_rate = e_rate*((double)(k_num[i]+1.000000)/(double)(tot_word_num[label-1]+counter1)); //计算有高斯平滑的未乘先验概率的数值 
			//e_rate = e_rate*((double)(k_num[i])/(double)(tot_word_num[label-1])); 	    	

	    }

	    e_rate = e_rate*emo_rate[label-1];  //乘先验概率得到标签类别为label的概率 

	    return e_rate;
	}
}


//计算测试文本预测正确的概率 
double getCorrect_rate()
{
	double num= 0;
    double num1 = 0;
	
	ifstream read_test("test.txt");          //读取测试集 
	//cout<<"预测正确的测试文本标签为："<<endl; 

	if(!read_test)    cout<<"can not open the file"<<endl;
	else
	{	     
		string emotion;
		string line;   
		getline(read_test, line);       //测试集去除开头一行                             
		map<string, int>::iterator it;
	    while(!read_test.eof())
	    {
	    	vector<string> vec;
	    	num++;
	    	getline(read_test, line);       //读取测试集一行文本 
		    istringstream is(line);
	    	string word; 
			int i=0;
			int label;                              
	    	while(is>>word)                 //逐个读取词汇 
	    	{
	    		i ++;
	    		
	    		if(i==2)
	    		label = str_to_num(word);           //该测试文本所属的正确的标签
	    		
	    		if(i>=4)
				{
					//在词汇表中找该词汇 
	    		    //it = voca_vec_train.find(word);
	    		                
	    		    //if(it != voca_vec_train.end())          //若能在不重复向量表找到该词汇 
	    		    //{
	    			//    int i = it->second;          //找到该词汇在不重复向量表的位置 
	    			//    test[i] = 1;
	    			    vec.push_back(word);   //将文本放入vec中 
	    			    
	    		    //}

	    		}
	    		
	    	}

	    	double testRate[emo_num];
	    
	    	//计算该测试文本每种情绪标签的概率 
	        for(int i=0; i<emo_num; i++)
	        {
	        	testRate[i] = getPredict_Label(vec, i+1);   //得到测试文本预测的标签概率 
	        }
	        
			// 找到概率最大的情绪标签 
            int testlabel=1;               //记录概率最大的情绪标签  
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
				num1++;    //预测正确的测试文本个数 
	    	}
	    	
	
	}
	cout<<endl;
	cout<<num1<<' ';
	cout<<num<<endl;
	//cout<<"测正确的测试文本个数为： "<<num1<<endl;
    double correct_rate = num1/num;   //得到正确率 
	
	return correct_rate;
	}
}

int main()
{
	getVoca_vector();
	
    double correct_rate = getCorrect_rate();
    
    cout<<"正确率为: "<<fixed<<setprecision(8)<<correct_rate<<endl;
    
	
	return 0;
}
