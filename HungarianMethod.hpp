#include <vector>
#include <utility>
#include <list>
#include <algorithm>
#include <iostream>

using std::vector;
using std::max_element;
using std::pair;
using std::list;
using std::cout;
using std::endl;
using std::cin;

typedef struct hmNode
{
	vector<vector<pair<double,int> > > node;//费用及状态矩阵,状态:-1标识非0元素,0标识为自由0元素,1标识为画圆,2标识为删除
	vector<int> stateHead;//状态头,标记存在画圈的行
	vector<pair<int,int> > zeroHead;//0元素个数头(first:行, second:列)
	int iZeroNum;//自由0个数
	int iCZNum;//画圆0的个数
}hmNode;


class HungarianMethod
{
public:
	HungarianMethod(){};
	~HungarianMethod() {};

	void CallHM( vector<vector<double> >& arr, vector<vector<int> >& rr, bool IsMin = true )
	{
		hmNode m_Node;
		Inite( arr,m_Node,IsMin );
		//TestInite(m_Node);

		Simplify(m_Node);//规约
		CountZero(m_Node);
		MarkZero(m_Node);//对0的状态划分
		rr = result;

		OutResult(m_Node);
	}

	void CallHM()
	{
		hmNode m_Node;
		//Inite( arr,m_Node,IsMin );
		TestInite(m_Node);
		Simplify(m_Node);//规约

		CountZero(m_Node);
		MarkZero(m_Node);//对0的状态划分
		OutResult(m_Node);
	}
private:
	int length;//纪录矩阵长度(包括表头)
	int m_iPerson;
	int m_iJob;
	vector<vector<int> > result;

private:
	void TestInite( hmNode& m_Node )
	{
		//int iIsMin = 0;
		//bool IsMin = true;
		//int iWorker = 0;
		//int iJob = 0;		
		//cout<<"匈牙利算法:"<<endl;
		//cout<<"最大指派问题,输入:1, 最小指派问题输入:0"<<endl;
		//cin>>iIsMin;

		//while(iIsMin!=1&&iIsMin!=0)
		//{
		//	cout<<"请输入1或0:"<<endl;;
		//	cin>>iIsMin;
		//}
		//if( iIsMin == 1 )
		//	IsMin = false;

		//cout<<"请输入工人数"<<endl;
		//cin>>iWorker;

		//while(iWorker<1 || iWorker>100)
		//{
		//	cout<<"请输入合法数据:"<<endl;
		//	cin>>iWorker;
		//}

		//cout<<"请输入工作数:"<<endl;
		//cin>>iJob;
		//while(iJob<1 || iJob>100)
		//{
		//	cout<<"请输入合法数据:"<<endl;
		//	cin>>iJob;
		//}
		//vector<vector<double> > arr(iWorker,vector<double>(iJob,0.0f));

		//cout<<"请输入"<<iWorker<<"行"<<iJob<<"列的矩阵,同一行内以空格间隔,不同行间以回车分隔:"<<endl;
		//for(int row=0; row < iWorker; row++)
		//	for( int col=0; col < iJob; col++)
		//	{
		//		scanf_s("%f",&arr[row][col]);
		//	}
		vector<vector<double> > arr;
		vector<double> a1;
		a1.push_back(4),a1.push_back(5),a1.push_back(7),a1.push_back(3),a1.push_back(6);
		arr.push_back(a1);a1.clear();
		a1.push_back(1),a1.push_back(3),a1.push_back(5),a1.push_back(8),a1.push_back(4);
		arr.push_back(a1);a1.clear();
		a1.push_back(2),a1.push_back(6),a1.push_back(5),a1.push_back(7),a1.push_back(2);
		arr.push_back(a1);a1.clear();
		a1.push_back(3),a1.push_back(5),a1.push_back(6),a1.push_back(3),a1.push_back(6);
		arr.push_back(a1);a1.clear();
		a1.push_back(9),a1.push_back(3),a1.push_back(4),a1.push_back(3),a1.push_back(4);
		arr.push_back(a1);
		bool IsMin = true;
		Inite(arr,m_Node,IsMin);
	}

	//初始化信息
	void Inite( vector<vector<double> >& arr,hmNode& m_Node, bool IsMin )
	{
		m_iPerson = int(arr.size());
		/*m_iJob = 0;
		if( m_iPerson != 0 )*/
			m_iJob = int(arr.front().size());
		length = m_iPerson > m_iJob?m_iPerson:m_iJob;
		m_Node.node = vector<vector<pair<double,int> > >(length,vector<pair<double,int> >(length,pair<double,int>(0.0f,-1)));
		if( IsMin )
			for( int i = 0; i < m_iPerson; ++i )
				for( int j = 0; j < m_iJob; ++j )
					m_Node.node[i][j].first = arr[i][j];
		else
		{
			double maxVal = arr[0][0];
			for( int i = 0; i < m_iPerson; ++i )
			{
				double maxCol = *max_element( arr[i].begin(), arr[i].end() );
				if( maxVal < maxCol )
					maxVal = maxCol;
			}

			for( int i = 0; i < m_iPerson; ++i )
				for( int j = 0; j < m_iJob; ++j )
					m_Node.node[i][j].first = maxVal - arr[i][j];
		}
	};

	//统计0元素个数
	void CountZero( hmNode& m_Node )
	{
		m_Node.stateHead = vector<int>(length,0);
		m_Node.zeroHead = vector<pair<int,int> >(length,pair<int,int>(0,0) );
		m_Node.iZeroNum = 0;
		m_Node.iCZNum = 0;
		for( int i = 0; i < length; ++i )
			for( int j = 0; j < length; ++j )
				if( m_Node.node[i][j].first == 0 )
				{
					m_Node.zeroHead[i].first++;
					m_Node.zeroHead[j].second++;
					m_Node.node[i][j].second = 0;//标识该位置的值为0.
					m_Node.iZeroNum++;
				}
	};

	//重置状体信息
	void resetState( hmNode& m_Node )
	{
		for( int i = 0; i < length; ++i )
			for( int j = 0; j < length; ++j )
				m_Node.node[i][j].second = -1;
		CountZero(m_Node);
	}


	//规约
	void Simplify( hmNode& m_Node )
	{
		//行规约
		for(int i = 0; i < length;++i)
		{
			double fMin = m_Node.node[i][0].first;
			for( int j = 1; j < length; ++j )
				if( m_Node.node[i][j].first < fMin )
					fMin = m_Node.node[i][j].first;
			if( fMin == 0 )
				continue;
			for( int j = 0; j < length; ++j )
				m_Node.node[i][j].first -= fMin;
		}

		//列规约
		for(int j = 0; j < length; ++j)
		{
			double fMin = m_Node.node[0][j].first;
			for(int i = 1; i < length; ++i)
				if(m_Node.node[i][j].first < fMin)
					fMin = m_Node.node[i][j].first;
			if( fMin == 0 )
				continue;
			for(int i = 0; i < length; ++i)
				m_Node.node[i][j].first -= fMin;
		}
	};

	void GetResult(hmNode& m_Node)
	{
		vector<int> arr(length,0);
		for( int row = 0; row < length; ++row )
			for( int col = 0; col < length; ++col )
				if( m_Node.node[row][col].second == 1 )
					arr[row] = col;
		result.push_back(arr);
	};

	void  MarkZero( hmNode& m_Node )
	{
		int zeroNum = m_Node.iZeroNum + 1;//用于纪录上次的0元素个数.

		while( m_Node.iZeroNum < zeroNum )//直到自由0元素不再减少
		{
			zeroNum = m_Node.iZeroNum;
			//依次处理行
			for( int row = 0; row < length; ++row )
			{//for3
				if( m_Node.zeroHead[row].first == 1 )//该行只有一个0,则"画圆"
				{//if3
					int col = 0;//查找该行的0元素所在位置
					for( ; col < length; ++col )
						if( m_Node.node[row][col].second == 0 )
							break;
					m_Node.zeroHead[row].first--;
					m_Node.zeroHead[col].second--;
					m_Node.iZeroNum--;
					m_Node.node[row][col].second = 1;//画圆
					m_Node.stateHead[row] = 1;//标识该行存在画圆
					m_Node.iCZNum++;//画圈元素个数

					if( m_Node.zeroHead[col].second > 0 )//将所"画圆"的0元素所在列上的0元素"画删除线"
					{//if1
						for( int i = 0; i < length; ++i )
						{
							if( m_Node.node[i][col].second == 0 )
							{
								m_Node.node[i][col].second = 2;//删除
								m_Node.zeroHead[i].first--;
								m_Node.zeroHead[col].second--;
								m_Node.iZeroNum--;
							}
						}
					}//end if1
				}//end if3
			}//end for3

			//依次处理列
			for( int col = 0; col < length; ++col )
			{//for3
				if( m_Node.zeroHead[col].second == 1 )//该列只有一个0,则"画圆"
				{//if3
					int row = 0;//查找该行的0元素所在位置
					for( ; row < length; ++row )
						if( m_Node.node[row][col].second == 0 )
							break;
					m_Node.zeroHead[row].first--;
					m_Node.zeroHead[col].second--;
					m_Node.iZeroNum--;
					m_Node.node[row][col].second = 1;//画圆
					m_Node.stateHead[row] = 1;//标识该行存在画圆(同上行处理一样)
					m_Node.iCZNum++;//画圆元素个数

					if( m_Node.zeroHead[row].first > 0 )//将所"画圆"的0元素所在列上的0元素"画删除线"
					{//if1
						for( int j = 0; j < length; ++j )
						{
							if( m_Node.node[row][j].second == 0 )
							{
								m_Node.node[row][j].second = 2;//删除
								m_Node.zeroHead[row].first--;
								m_Node.zeroHead[j].second--;
								m_Node.iZeroNum--;
							}
						}
					}//end if1
				}//end if3
			}//end for3
		}//end while

		TestZeroNum(m_Node);
	};

	void MoveZero( hmNode& m_Node )
	{
		vector<int> rowMark(length,0);
		vector<int> colMark(length,0);
		for( int row = 0; row <length; ++row )
			if( m_Node.stateHead[row] == 0 )
				rowMark[row] = 1;

		bool juge = true;
		while(juge)
		{
			juge = false;
			for( int row = 0; row < length; ++row )
				if( rowMark[row] == 1 )
					for( int col = 0; col < length; ++col )
						if( m_Node.node[row][col].second == 2 && colMark[col] != 1 )
						{
							colMark[col] = 1;
							juge = true;
						}

			for( int col = 0; col < length; ++col )
				if( colMark[col] == 1 )
					for( int row = 0; row < length; ++row )
						if( m_Node.node[row][col].second == 1 && rowMark[row] != 1 )
						{
							rowMark[row] = 1;
							juge = true;
						}

		}//end while

		double minVal = 0;//纪录未被直线覆盖的最小值
		bool firstTry = true;//用于最小值计算,标识第一次.
		//从打"√"的行和未打"√"的列中找到最小值
		for( int row = 0; row < length; ++row )
			if( rowMark[row] == 1 )
				for( int col = 0; col < length; ++col )
					if( colMark[col] == 0 )
					{
						if(firstTry)
						{
							minVal = m_Node.node[row][col].first;//  第一次取值
							firstTry = false;
						}
						else
						{
							if( m_Node.node[row][col].first < minVal )
								minVal = m_Node.node[row][col].first;
						}
					}

		//打"√"的行减去未覆盖元素中的的最小值
		for( int row = 0; row < length; ++row )
			if( rowMark[row] == 1)//打"√"的行减去最小值
				for( int col = 0; col < length; ++col )
					m_Node.node[row][col].first -= minVal;

		//打"√"的列加上未覆盖元素中的最小值,从而保证0元素不变
		for( int col = 0; col < length; ++col )
			if( colMark[col] == 1 )//打"√"的列
				for( int row = 0; row < length; ++row )
					m_Node.node[row][col].first += minVal;

		resetState(m_Node);
		MarkZero(m_Node);//对0的状态划分
	}

	void TestZeroNum( hmNode& m_Node )
	{
		if( m_Node.iZeroNum > 0 )
			MultiZero(m_Node);//调用多0处理.
		else
		{
			if( m_Node.iCZNum == length )//画圆个数等于矩阵阶数,输出结果.
			{
				GetResult(m_Node);
			}
			else
			{
				MoveZero(m_Node);
			}
		}
	};
	

	void MultiZero( hmNode& m_Node )
	{
		int row = 0;
		for( int i = 0; i < length; ++i )
		{
			int tt = length;
			if( m_Node.zeroHead[i].first > 0 && m_Node.zeroHead[i].first < tt)//找到最少自由0所在行
			{
				tt = m_Node.zeroHead[i].first;
				row = i;				
			}
		}
		//for( ; row < length; ++row )
		//	if( m_Node.zeroHead[row].first > 0 )
		//		break;

		for( int col = 0; col < length; ++col )//依次对该行0元素进行"画圆"测试
		{
			if( m_Node.node[row][col].second == 0 )
			{
				hmNode tempNode = m_Node;
				tempNode.node[row][col].second = 1;//画圆
				tempNode.zeroHead[row].first--;
				tempNode.zeroHead[col].second--;
				tempNode.iZeroNum--;
				tempNode.iCZNum++;

				for( int col2 = 0; col2 < length; ++col2 )//把"画圆"元素所在行的其它自由0元素删除
				{
					if( tempNode.node[row][col2].second == 0 )
					{
						tempNode.node[row][col2].second = 2;// 画撇
						tempNode.zeroHead[row].first--;
						tempNode.zeroHead[col2].second--;
						tempNode.iZeroNum--;
					}
				}

				for( int row2 = 0; row2 < length; ++row2 )//把画圆所在列的其它自由0元素删除
				{
					if( tempNode.node[row2][col].second == 0 )
					{
						tempNode.node[row2][col].second = 2;
						tempNode.zeroHead[row2].first--;
						tempNode.zeroHead[col].second--;
						tempNode.iZeroNum--;
					}
				}
				MarkZero(tempNode);
			}
		}
	};	

	void OutResult(hmNode &m_Node)
	{
		double value = 0.0;
		cout<<"求得了"<< result.size()<<"个结果:"<<endl;
		for( int i = 0; i < int(result.size()); ++i )
		{
			cout<<"第"<<i<<"个结果:"<<endl;
			for( int j = 0; j < int(result.front().size()); ++j )
			{
				cout<<result[i][j]<<" ";

				value += m_Node.node[i][j].first;
			}
			cout<<endl;
		}

		cout << "max value is : " << value << endl;

	};
};