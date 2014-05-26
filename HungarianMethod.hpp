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
	vector<vector<pair<double,int> > > node;//���ü�״̬����,״̬:-1��ʶ��0Ԫ��,0��ʶΪ����0Ԫ��,1��ʶΪ��Բ,2��ʶΪɾ��
	vector<int> stateHead;//״̬ͷ,��Ǵ��ڻ�Ȧ����
	vector<pair<int,int> > zeroHead;//0Ԫ�ظ���ͷ(first:��, second:��)
	int iZeroNum;//����0����
	int iCZNum;//��Բ0�ĸ���
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

		Simplify(m_Node);//��Լ
		CountZero(m_Node);
		MarkZero(m_Node);//��0��״̬����
		rr = result;

		OutResult(m_Node);
	}

	void CallHM()
	{
		hmNode m_Node;
		//Inite( arr,m_Node,IsMin );
		TestInite(m_Node);
		Simplify(m_Node);//��Լ

		CountZero(m_Node);
		MarkZero(m_Node);//��0��״̬����
		OutResult(m_Node);
	}
private:
	int length;//��¼���󳤶�(������ͷ)
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
		//cout<<"�������㷨:"<<endl;
		//cout<<"���ָ������,����:1, ��Сָ����������:0"<<endl;
		//cin>>iIsMin;

		//while(iIsMin!=1&&iIsMin!=0)
		//{
		//	cout<<"������1��0:"<<endl;;
		//	cin>>iIsMin;
		//}
		//if( iIsMin == 1 )
		//	IsMin = false;

		//cout<<"�����빤����"<<endl;
		//cin>>iWorker;

		//while(iWorker<1 || iWorker>100)
		//{
		//	cout<<"������Ϸ�����:"<<endl;
		//	cin>>iWorker;
		//}

		//cout<<"�����빤����:"<<endl;
		//cin>>iJob;
		//while(iJob<1 || iJob>100)
		//{
		//	cout<<"������Ϸ�����:"<<endl;
		//	cin>>iJob;
		//}
		//vector<vector<double> > arr(iWorker,vector<double>(iJob,0.0f));

		//cout<<"������"<<iWorker<<"��"<<iJob<<"�еľ���,ͬһ�����Կո���,��ͬ�м��Իس��ָ�:"<<endl;
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

	//��ʼ����Ϣ
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

	//ͳ��0Ԫ�ظ���
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
					m_Node.node[i][j].second = 0;//��ʶ��λ�õ�ֵΪ0.
					m_Node.iZeroNum++;
				}
	};

	//����״����Ϣ
	void resetState( hmNode& m_Node )
	{
		for( int i = 0; i < length; ++i )
			for( int j = 0; j < length; ++j )
				m_Node.node[i][j].second = -1;
		CountZero(m_Node);
	}


	//��Լ
	void Simplify( hmNode& m_Node )
	{
		//�й�Լ
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

		//�й�Լ
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
		int zeroNum = m_Node.iZeroNum + 1;//���ڼ�¼�ϴε�0Ԫ�ظ���.

		while( m_Node.iZeroNum < zeroNum )//ֱ������0Ԫ�ز��ټ���
		{
			zeroNum = m_Node.iZeroNum;
			//���δ�����
			for( int row = 0; row < length; ++row )
			{//for3
				if( m_Node.zeroHead[row].first == 1 )//����ֻ��һ��0,��"��Բ"
				{//if3
					int col = 0;//���Ҹ��е�0Ԫ������λ��
					for( ; col < length; ++col )
						if( m_Node.node[row][col].second == 0 )
							break;
					m_Node.zeroHead[row].first--;
					m_Node.zeroHead[col].second--;
					m_Node.iZeroNum--;
					m_Node.node[row][col].second = 1;//��Բ
					m_Node.stateHead[row] = 1;//��ʶ���д��ڻ�Բ
					m_Node.iCZNum++;//��ȦԪ�ظ���

					if( m_Node.zeroHead[col].second > 0 )//����"��Բ"��0Ԫ���������ϵ�0Ԫ��"��ɾ����"
					{//if1
						for( int i = 0; i < length; ++i )
						{
							if( m_Node.node[i][col].second == 0 )
							{
								m_Node.node[i][col].second = 2;//ɾ��
								m_Node.zeroHead[i].first--;
								m_Node.zeroHead[col].second--;
								m_Node.iZeroNum--;
							}
						}
					}//end if1
				}//end if3
			}//end for3

			//���δ�����
			for( int col = 0; col < length; ++col )
			{//for3
				if( m_Node.zeroHead[col].second == 1 )//����ֻ��һ��0,��"��Բ"
				{//if3
					int row = 0;//���Ҹ��е�0Ԫ������λ��
					for( ; row < length; ++row )
						if( m_Node.node[row][col].second == 0 )
							break;
					m_Node.zeroHead[row].first--;
					m_Node.zeroHead[col].second--;
					m_Node.iZeroNum--;
					m_Node.node[row][col].second = 1;//��Բ
					m_Node.stateHead[row] = 1;//��ʶ���д��ڻ�Բ(ͬ���д���һ��)
					m_Node.iCZNum++;//��ԲԪ�ظ���

					if( m_Node.zeroHead[row].first > 0 )//����"��Բ"��0Ԫ���������ϵ�0Ԫ��"��ɾ����"
					{//if1
						for( int j = 0; j < length; ++j )
						{
							if( m_Node.node[row][j].second == 0 )
							{
								m_Node.node[row][j].second = 2;//ɾ��
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

		double minVal = 0;//��¼δ��ֱ�߸��ǵ���Сֵ
		bool firstTry = true;//������Сֵ����,��ʶ��һ��.
		//�Ӵ�"��"���к�δ��"��"�������ҵ���Сֵ
		for( int row = 0; row < length; ++row )
			if( rowMark[row] == 1 )
				for( int col = 0; col < length; ++col )
					if( colMark[col] == 0 )
					{
						if(firstTry)
						{
							minVal = m_Node.node[row][col].first;//  ��һ��ȡֵ
							firstTry = false;
						}
						else
						{
							if( m_Node.node[row][col].first < minVal )
								minVal = m_Node.node[row][col].first;
						}
					}

		//��"��"���м�ȥδ����Ԫ���еĵ���Сֵ
		for( int row = 0; row < length; ++row )
			if( rowMark[row] == 1)//��"��"���м�ȥ��Сֵ
				for( int col = 0; col < length; ++col )
					m_Node.node[row][col].first -= minVal;

		//��"��"���м���δ����Ԫ���е���Сֵ,�Ӷ���֤0Ԫ�ز���
		for( int col = 0; col < length; ++col )
			if( colMark[col] == 1 )//��"��"����
				for( int row = 0; row < length; ++row )
					m_Node.node[row][col].first += minVal;

		resetState(m_Node);
		MarkZero(m_Node);//��0��״̬����
	}

	void TestZeroNum( hmNode& m_Node )
	{
		if( m_Node.iZeroNum > 0 )
			MultiZero(m_Node);//���ö�0����.
		else
		{
			if( m_Node.iCZNum == length )//��Բ�������ھ������,������.
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
			if( m_Node.zeroHead[i].first > 0 && m_Node.zeroHead[i].first < tt)//�ҵ���������0������
			{
				tt = m_Node.zeroHead[i].first;
				row = i;				
			}
		}
		//for( ; row < length; ++row )
		//	if( m_Node.zeroHead[row].first > 0 )
		//		break;

		for( int col = 0; col < length; ++col )//���ζԸ���0Ԫ�ؽ���"��Բ"����
		{
			if( m_Node.node[row][col].second == 0 )
			{
				hmNode tempNode = m_Node;
				tempNode.node[row][col].second = 1;//��Բ
				tempNode.zeroHead[row].first--;
				tempNode.zeroHead[col].second--;
				tempNode.iZeroNum--;
				tempNode.iCZNum++;

				for( int col2 = 0; col2 < length; ++col2 )//��"��Բ"Ԫ�������е���������0Ԫ��ɾ��
				{
					if( tempNode.node[row][col2].second == 0 )
					{
						tempNode.node[row][col2].second = 2;// ��Ʋ
						tempNode.zeroHead[row].first--;
						tempNode.zeroHead[col2].second--;
						tempNode.iZeroNum--;
					}
				}

				for( int row2 = 0; row2 < length; ++row2 )//�ѻ�Բ�����е���������0Ԫ��ɾ��
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
		cout<<"�����"<< result.size()<<"�����:"<<endl;
		for( int i = 0; i < int(result.size()); ++i )
		{
			cout<<"��"<<i<<"�����:"<<endl;
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