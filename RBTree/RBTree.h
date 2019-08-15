nclude <iostream>
#include <time.h>
using namespace std;

enum Color
{
	RED,
	BLACK
};

template <typename K, typename V>
struct RBTreeNode
{
	RBTreeNode<K, V>* m_left = nullptr;
	RBTreeNode<K, V>* m_right = nullptr;
	RBTreeNode<K, V>* m_parent = nullptr;
	pair<K, V> m_kv;
	Color m_color = RED;
};

template <typename K, typename V>
class RBTree
{
	typedef RBTreeNode<K, V> Node;
	typedef Node* PNode;
public:
	RBTree()
	{
		m_header = new Node;
		m_header->m_parent = nullptr;
		m_header->m_left = m_header;
		m_header->m_right = m_header;
	}

	bool Insert(const pair<K, V>& kv)
	{
		if (nullptr == m_header->m_parent)
		{
			PNode root = new Node;
			root->m_kv = kv;
			root->m_color = BLACK;
			root->m_parent = m_header;
			m_header->m_parent = root;
			m_header->m_left = root;
			m_header->m_right = root;
			return true;
		}

		PNode root = m_header->m_parent;
		PNode cur = root;
		PNode parent = nullptr;

		while (cur)
		{
			parent = cur;
			if (cur->m_kv.first < kv.first)
				cur = cur->m_right;
			else if (cur->m_kv.first > kv.first)
				cur = cur->m_left;
			else
				return false;
		}
		cur = new Node;
		cur->m_kv = kv;
		if (parent->m_kv.first < kv.first)
			parent->m_right = cur;
		else
			parent->m_left = cur;
		cur->m_parent = parent;

		// 更新颜色
		// cur 为红, p 为红, g 为黑, u 存在且为红
		// g 为 root, 将 g 修改为黑
		// g 不为 root, 修改 p、u 为黑, g 为红, 继续向上调整
		while (cur != root && RED == cur->m_parent->m_color)
		{
			PNode parent = cur->m_parent;
			PNode grandFather = cur->m_parent->m_parent;
			if (parent == grandFather->m_left)
			{
				PNode uncle = grandFather->m_right;
				if (uncle && RED == uncle->m_color)
				{
					parent->m_color = BLACK;
					uncle->m_color = BLACK;
					grandFather->m_color = RED;
					cur = grandFather;
				}
				else
				{
					// 情况三
					if (cur == parent->m_right)
					{
						RotateL(parent);
						swap(parent, cur);
					}
					// cur 为红, p为红, g为黑, u不存在、u存在且为黑
					// u 不存在, cur 一定是新插入的结点
					// u 存在, u 的颜色是黑, cur 一定不是新插入节点
					grandFather->m_color = RED;
					parent->m_color = BLACK;
					RotateR(grandFather);
					break;
				}
			}
			else
			{
				PNode uncle = grandFather->m_left;
				if (uncle && RED == uncle->m_color)
				{
					parent->m_color = BLACK;
					uncle->m_color = BLACK;
					grandFather->m_color = RED;
					cur = grandFather;
				}
				else
				{
					if (cur == parent->m_left)
					{
						RotateR(parent);
						swap(parent, cur);
					}

					grandFather->m_color = RED;
					parent->m_color = BLACK;
					RotateL(grandFather);
					break;
				}
			}
		}

		m_header->m_parent->m_color = BLACK;
		m_header->m_left = LeftMost();
		m_header->m_right = RightMost();
		return true;
	}
	void Inorder()
	{
		_Inorder(GetRoot());
		cout << endl;
	}

	~RBTree()
	{
		Destroy(GetRoot());
	}

	bool IsValidRBTree()
	{
		PNode pRoot = GetRoot();
		// 空树也是红黑树
		if (nullptr == pRoot)
			return true;
		// 检测根节点是否满足情况
		if (BLACK != pRoot->m_color)
		{
			cout << "违反红黑树性质二：根节点必须为黑色" << endl;
			return false;
		}
		// 获取任意一条路径中黑色节点的个数
		size_t blackCount = 0;
		PNode pCur = pRoot;
		while (pCur)
		{
			if (BLACK == pCur->m_color)
				blackCount++;
			pCur = pCur->m_left;
		}
		// 检测是否满足红黑树的性质，k用来记录路径中黑色节点的个数
		size_t k = 0;
		return _IsValidRBTree(pRoot, k, blackCount);
	}
	bool _IsValidRBTree(PNode pRoot, size_t k, const size_t blackCount)
	{
		if (nullptr == pRoot)
			return true;
		// 统计黑色节点的个数
		if (BLACK == pRoot->m_color)
			k++;
		// 检测当前节点与其双亲是否都为红色
		PNode pParent = pRoot->m_parent;
		if (pParent && RED == pParent->m_color && RED == pRoot->m_color)
		{
			cout << "违反性质三：没有连在一起的红色节点" << endl;
			return false;
		}
		// 如果pRoot是因子节点，检测当前路径中黑色节点的个数是否有问题
		if (nullptr == pRoot->m_left && nullptr == pRoot->m_right)
		{
			if (k != blackCount)
			{
				cout << "违反性质四：每条路径中黑色节点的个数必须相同" << endl;
				return false;
			}
		}
		return _IsValidRBTree(pRoot->m_left, k, blackCount) &&
			_IsValidRBTree(pRoot->m_right, k, blackCount);
	}

	/*bool IsVaildBRTree(PNode root, size_t k, const size_t blackCount)
	{
		if (root == nullptr)
		{
			return true;
			if (k != blanckCount)
			{
				cout << "4 error!!!" << endl;
				return false;
			}
			else
				return true;
		}
		if (root->m_color == BLACK)
			++k;
		else if (root->m_color == RED)
		{
			if (root->m_parent->m_color == RED)
			{
				cout << "3 error!!!" << endl;
				return false;
			}
		}
		else
		{
			assert(false);
		}
		return IsVaildBRTree(root->m_left, k, blanckCount) && IsVaildBRTree(root->m_right, k, blackCount);
	}*/
private:
	void RotateR(Node* parent)
	{
		Node* subL = parent->m_left;
		Node* subLR = subL->m_right;

		parent->m_left = subLR;
		if (subLR)
			subLR->m_parent = parent;

		subL->m_right = parent;
		Node* ppNode = parent->m_parent;
		parent->m_parent = subL;

		if (parent == m_header->m_parent)
		{
			subL->m_parent = nullptr;
			m_header->m_parent = subL;
		}
		else
		{
			if (ppNode->m_left == parent)
				ppNode->m_left = subL;
			else
				ppNode->m_right = subL;
			subL->m_parent = ppNode;
		}
	}

	void RotateL(Node* parent)
	{
		Node* subR = parent->m_right;
		Node* subRL = subR->m_left;

		parent->m_right = subRL;
		if (subRL)
			subRL->m_parent = parent;

		subR->m_left = parent;
		Node* ppNode = parent->m_parent;
		parent->m_parent = subR;

		if (parent == m_header->m_parent)
		{
			subR->m_parent = nullptr;
			m_header->m_parent = subR;
		}
		else
		{
			if (ppNode->m_left == parent)
				ppNode->m_left = subR;
			else
				ppNode->m_right = subR;
			subR->m_parent = ppNode;
		}
	}

	PNode& GetRoot()
	{
		return m_header->m_parent;
	}

	PNode LeftMost()
	{
		PNode cur = m_header->m_parent;
		while (cur && cur->m_left != nullptr)
		{
			cur = cur->m_left;
		}
		return cur;
	}
	PNode RightMost()
	{
		PNode cur = m_header->m_parent;
		while (cur && cur->m_right != nullptr)
		{
			cur = cur->m_right;
		}
		return cur;
	}
	void _Inorder(PNode root)
	{
		if (root)
		{
			_Inorder(root->m_left);
			cout << root->m_kv.first << " ";
			_Inorder(root->m_right);
		}
	}
	void Destroy(PNode& root)
	{
		if (root)
		{
			Destroy(root->m_left);
			Destroy(root->m_right);
			delete root;
			root = nullptr;
		}
	}
private:
	PNode m_header;
};

void Test()
{
	//int array[] = { 16, 3, 7, 11, 9, 26, 18, 14, 15 };
	RBTree<int, int> t;
	/*for (auto e : array)
	{
		t.Insert(make_pair(e, e));
	}*/
	int n = 0;
	while (cin >> n)
	{
		srand(time(0));
		while (n--)
		{
			t.Insert(make_pair(rand(), n));
		}
		t.Inorder();
		if (t.IsValidRBTree())
			cout << "IsValidRBTree" << endl;
		else 
			cout << "IsNotValidRBTree" << endl;
	}
}
