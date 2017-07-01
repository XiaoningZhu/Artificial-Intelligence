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

//储存训练文本情绪标签和该训练文本与测试文本距离的类。
class Corr
{
	public:
		void setCorr(double dis1, string emo1)
		{
			dis=dis1;
			emo=emo1;
		}
		
		void sort1(Corr c[], int n)//根据距离排序 
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


		double dis;//距离 
		string emo;//情绪标签 
};


map<string, int> voca_vec;                    //储存不重复词汇及其在词汇向量表位置 
int onehot[4000] = {0};                        //即时储存onehot矩阵的一行
int test[4000] = {0};                          //一个测试文本的onehot矩阵 
int counter = 0;                                  //不重复词汇的数量 
Corr dis_emo[246];                            //储存k个储存训练文本情绪标签和该训练文本与测试文本距离的类。 
int K;

//获得不重复向量表 
int getVoca_vector()
{

	ifstream read_train("train.txt");          //读取训练集 
	if(!read_train)    cout<<"can not open the file"<<endl;
	else
	{   
		map<string, int>::iterator it;
		string line;                             
	    while(!read_train.eof())
	    {
		    getline(read_train, line);        //读取一行数据
		    istringstream is(line);
	    	string word;                       
	    	int i=0;
	    	while(is>>word)                    //读取一个词
	    	{
	    		i++;
	    		if(i>=4)                      //每行从正式文本开始读取 
	    		{
	    			it = voca_vec.find(word);       //确定词汇是否已被储存在不重复词汇向量中 
	    			if(it == voca_vec.end())         //未被储存在不重复词汇向量中 
	    			{
	    				voca_vec.insert(pair<string, int>(word, counter));  //在词汇向量末尾插入词汇 
	    				counter ++;          //计算不重复词汇个数 
	    				
	    			}
	    			
	    		}
	    	}
	   }
	   
    }
    
    cout<<counter<<endl;
	
}

//得出测试文本预测的标签 
string getPredict_Label(int *test)                   //test为一个储存着一个测试文本onehot矩阵的数组 
{
	int onehot1[4000] = {0};                          //用于储存一个训练文本的onehot向量 
	
	string emotion, emo_temp;
	ofstream write("train_text.txt");          //读取测试集 
	
	ifstream read_train("train.txt");          //读取测试集 
	if(!read_train)    cout<<"can not open the file"<<endl;
	else
	{
		
		string line;                                    
		map<string, int>::iterator it;
		int j=0;
	    while(!read_train.eof())
	    {
	    	memset(onehot1, 0, 4000*sizeof(int));        
	    	getline(read_train, line);                   //读取的一个文本 
		    istringstream is(line);
	    	string word; 
			int i=0;                              
	    	while(is>>word)                 //逐个读取词汇 
	    	{
	    		i ++;
	    		
	    		if(i==3) emo_temp = word;
	    		
	    		if(i>=4)
	    		{	
	    		    //找到此词汇在词汇表中的位置并在onehot矩阵赋值为1 
	    		    it = voca_vec.find(word);
	    		    int i = it->second;          //找到该词汇在不重复向量表的位置 
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
	    		//sum = sum + pow((onehot1[k]-test[k]),2);//计算欧式距离
				sum +=  abs(onehot1[k]-test[k]);
	    	}
	    	
	    	//sum = sqrt(sum);  //得到欧式距离 
	    
	    	(dis_emo[j]).setCorr(sum, emo_temp); //将该训练文本与测试文本的距离和该训练文本对应的情绪标签放入类中 
	    	j++; 	
	    }
	    
	    
	    
	    (dis_emo[0]).sort1(dis_emo, 247);    //根据距离排序 

	    j=0;
	    int number[6]={0};     //用于记录k个训练文本六个情绪标签分别有多少个 
	    
	    //计算距离最小的k个文本中各类标签分别有多少个 
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
			if(number[k]>temp) temp = number[k];//找到k个训练文本中六个情绪标签中最多的标签个数 
		}
		
		vector<string> emo;

 		string emotions[6]={"joy", "sad", "fear","surprise", "anger", "disgust"}; //与number[k]中个数对应的情绪标签 
 		
 		int total=0;
 		for(int i=0; i<6; i++)
 		{
 			if(temp==number[i])
 			{
 				emo.push_back(emotions[i]); //将个数最多的几个标签都储存起来 ，之后随机选取一个标签 
 				total++;                     //计算一共有多少个个数最多的几个标签
 			}
 		}

        //从多个个数最多的标签中选取一个标签 
		int n=rand()%(total);   
		emotion = emo[n];

		
	}
	
	
	
	return emotion;
	
}

//计算测试文本预测正确的概率 
double getCorrect_rate()
{
	double num= 0;
    double num1 = 0;
    ofstream write_Label("testLabel.txt");
	
	ifstream read_test("test.txt");          //读取测试集 
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
	    	getline(read_test, line);       //读取一行文本 
		    istringstream is(line);
	    	string word; 
			int i=0;                              
	    	while(is>>word)                 //逐个读取词汇 
	    	{
	    		i ++;
	    		
	    		if(i==3)
	    		emotion = word;           //该测试文本所属的正确的标签 
	    		
	    		if(i>=4)
	    		{
	    			
	    		    //找到此词汇在词汇表中的位置并在onehot矩阵赋值为1 
	    		    it = voca_vec.find(word);
	    		                
	    		    if(it != voca_vec.end())          //若能在不重复向量表找到该词汇 
	    		    {
	    			    int i = it->second;          //找到该词汇在不重复向量表的位置 
	    			    test[i] = 1;
	    		    }
	    		    
	    		}
	    		
	    	}

	    	string emotion1;
	    
	    	emotion1 = getPredict_Label(test);   //得到测试文本预测的标签 
	    	write_Label<<emotion1<<'\n';

	    	if(emotion== emotion1) num1++;      //预测标签正确 
	    	
	    }
	    
	
	}
	
	cout<<"预测正确的个数: "<<num1<<endl<<"测试文本总个数为："<<num<<endl;
	double correct_rate = num1/num;  //num1为预测正确的个数，num为所有测试文本的个数。 
	
	return correct_rate;
}

int main()
{
	cout<<"K= ";

	cin>>K;
	
	
	getVoca_vector();
	
    double correct_rate = getCorrect_rate();
    
    cout<<"正确率为: "<<fixed<<setprecision(8)<<correct_rate<<endl;
    
	
	
	return 0;
	
	
}
