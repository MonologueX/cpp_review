#pragma once 
#include <iostream>
#include <assert.h>
using namespace std;

template <typename K, typename V>
struct AVLTreeNode
{
	pair<K, V> m_kv;
	AVLTreeNode<K, V>* m_left = nullptr;
	AVLTreeNode<K, V>* m_right = nullptr;
	AVLTreeNode<K, V>* m_parent = nullptr;
	int m_bf = 0;
};

template <typename K, typename V>
class AVLTree
{
	typedef AVLTreeNode<K, V> Node;
public:
	// pair<itrrator, bool> Insert(pair<K, V>& kv)
	bool Insert(pair<K, V> kv)
	{
		if (m_root == nullptr)
		{
			m_root = new Node;
			m_root->m_kv = kv;
			return true;
		}
		Node* cur = m_root;
		Node* parent = nullptr;
		while (cur)
		{
			if (cur->m_kv.first > kv.first)
			{
				parent = cur;
				cur = cur ->m_left;
			}
			else if (cur->m_kv.first < kv.first)
			{
				parent = cur;
				cur = cur->m_right;
			}
			else
			{
				return false;
			}
		}
		cur = new Node;
		cur->m_kv = kv;
		if (parent->m_kv.first < kv.first)
		{
			parent->m_right = cur;
			cur->m_parent = parent;
		}
		else
		{
			parent->m_left = cur;
			cur->m_parent = parent;
		}
		// 调平衡因子
		// 左变 --buf, 右变++
		// |bf| == 1, 向上调整
		// |bf| == 0, 调整完成
		// |bf| == 2, 旋转

		while (parent)
		{
			if (cur == parent->m_left)
				--parent->m_bf;
			else
				++parent->m_bf;

			if (abs(parent->m_bf) == 1)
			{
				cur = parent;
				parent = parent->m_parent;
				continue;
			}
			else if (parent->m_bf == 0)
			{
				break;
			}
			else if (abs(parent->m_bf) == 2)
			{
				// 进行旋转
				// 调节当前子树，降低高度
				if (parent->m_bf == -2 && cur->m_bf == -1)
				{
					// 右单旋
					RotateR(parent);
				}
				else if (parent->m_bf == 2 && cur->m_bf == 1)
				{
					// 左单旋
					RotateL(parent);
				}
				else if (parent->m_bf == -2 && cur->m_bf == 1)
				{
					// 左右双旋
					Node* subL = parent->m_left;
					Node* subLR = subL->m_right;
					int bf = subLR->m_bf;
					RotateL(parent->m_left);
					RotateR(parent);
					subLR->m_bf = 0;
					if (bf == 1)
					{
						subL->m_bf = -1;
					}
					else if (bf == -1)
					{
						parent->m_bf = 1;
					}
				}
				else if (parent->m_bf == 2 && cur->m_bf == -1)
				{
					// 右左双旋
					Node* subR = parent->m_right;
					Node* subRL = subR->m_left;
					int bf = subRL->m_bf;
					RotateR(parent->m_right);
					RotateL(parent);
		
					if (bf == 1)
					{
						parent->m_bf = -1;
					}
					else if (bf == -1)
					{
						subR->m_bf = 1;
					}
				}
				break;
			}
			else
				assert(false);
		}
		return true;
	}

	void Inorder()
	{
		_Inorder(m_root);
		cout << endl;
	}

	bool IsAVLTree()
	{
		return _IsAVLTree(m_root);
	}

	~AVLTree()
	{}


private:

	int GetHeight(Node* root)
	{
		if (root == nullptr)
			return 0;
		int leftHeight = GetHeight(root->m_left);
		int rightHeight = GetHeight(root->m_right);
		return leftHeight > rightHeight ? (leftHeight + 1) : (rightHeight + 1);
	}

	bool _IsAVLTree(Node* root)
	{
		if (root == nullptr)
			return true;
		int leftHeight = GetHeight(root->m_left);
		int rightHeight = GetHeight(root->m_right);
		return abs(leftHeight - rightHeight) < 2 && _IsAVLTree(root->m_left) && _IsAVLTree(root->m_right);
	}

	void _Inorder(Node* root)
	{
		if (root == nullptr)
			return;
		_Inorder(root->m_left);
		cout << root->m_kv.first << " ";
		_Inorder(root->m_right);
	}

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

		if (parent == m_root)
		{
			subL->m_parent = nullptr;
			m_root = subL;
		}
		else
		{
			if (ppNode->m_left == parent)
				ppNode->m_left = subL;
			else
				ppNode->m_right = subL;
			subL->m_parent = ppNode;
		}
		subL->m_bf = parent->m_bf = 0;
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

		if (parent == m_root)
		{
			subR->m_parent = nullptr;
			m_root = subR;
		}
		else
		{
			if (ppNode->m_left == parent)
				ppNode->m_left = subR;
			else
				ppNode->m_right = subR;
			subR->m_parent = ppNode;
		}
		subR->m_bf = parent->m_bf = 0;
	}

private:
	Node* m_root = nullptr;
};

void Test()
{
	//int array[] = { 16, 3, 7, 11, 9, 26, 18, 14, 15 };
	int array[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 };
	AVLTree<int, int> t;
	for (auto e : array)
		t.Insert(make_pair(e, 0));
	t.Inorder();
	cout << t.IsAVLTree() << endl;
}
