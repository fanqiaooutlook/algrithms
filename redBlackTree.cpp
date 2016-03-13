//some important explain information has been written in English, I think if you know a little English, you can understand all of the source  code. 

#include <iostream>
using namespace std;
#define red 1
#define black 0
typedef struct rbNode * prbNode;
struct rbNode{
	double key;//节点核心信息
	int color;
	int number;
	prbNode lChild;
	prbNode rChild;
	prbNode parent;
	int size;//以当前节点为根的树的节点个数。
};
//红黑树的节点如此排列，左子树为小于等于根的节点
typedef struct rbTree * prbTree;
struct rbTree{
	prbNode root;
	prbNode stl;//指向哨兵节点
};

/**********对列操作*************/
typedef struct queue* pqueue;
//queue节点
struct queue
{
	prbNode n;
	bool t;//true表示本层的末尾，false表示是空白节点；同时n为NULL
	pqueue next;
};

//队列头节点
typedef struct queueHead* pqHead;
struct queueHead
{
	pqueue head;
	pqueue tail;
};

//初始化队列
pqHead initQueue()
{
	pqHead pqh=new queueHead;
	pqh->head=NULL;
	pqh->tail=NULL;
	return pqh;
}

//判断对列是否为空 ，空返回真
bool isNullQueue(pqHead pqh)
{
	if(pqh->head==NULL&&pqh->tail==NULL)
	{
		return true;
	}
	return false;
}

//入队
void enQueue(pqHead pqh, pqueue node)
{
	if(pqh->head==NULL&&pqh->tail==NULL)
	{
		pqh->head=node;
		pqh->tail=node;
	//	node->next=NULL;
	}
	else
	{		
		pqh->tail->next=node;
		pqh->tail=node;
	
	}
		node->next=NULL;
}
 
//出队
pqueue deQueue(pqHead pqh)
{
	pqueue node=pqh->head;

	pqh->head=pqh->head->next;
	if(pqh->head==NULL)
	{
		pqh->tail=NULL;
	}
	return node;
}
/***对列操作end**/




prbTree initRBTree(){//初始时建立空树，使指针指向sentinel节点。
	prbTree tree=new rbTree;

	prbNode sentinel=new rbNode;
	sentinel->color=black;
	sentinel->lChild=NULL;
	sentinel->rChild=NULL;
	sentinel->parent=NULL;//sentinel的三个指针都为NULL
	sentinel->size=0;
	sentinel->number=0;

	tree->root=sentinel; 
	tree->stl=sentinel;
	return tree;
}

void leftRotate(prbTree tree, prbNode x){
	prbNode y, b; 
	y=x->rChild;
	b=y->lChild;

	//维护节点间的指针
	y->lChild=x;
	y->parent=x->parent;
	x->parent=y;
	x->rChild=b;
	b->parent=x;

	
	if(y->parent!=tree->stl){//如果y的父亲不为空，即y不为根节点
		if(y->parent->key>=y->key){
			y->parent->lChild=y;
		}else{
			y->parent->rChild=y;
		}
	}else{
		tree->root=y;
	}
	
	//维护节点的size字段
	y->size=x->size;
	x->size=x->lChild->size+x->rChild->size+1;
}

void rightRotate(prbTree tree,prbNode y){
	prbNode x,b;
	x=y->lChild;
	b=x->rChild;

	x->rChild=y;
	x->parent=y->parent;
	y->parent=x;
	y->lChild=b;
	b->parent=y;

	//维护节点间的指针
	if(x->parent!=tree->stl){//如果y的父亲不为空，即x不为根节点
		if(x->parent->key>=x->key){
			x->parent->lChild=x;
		}else{
			x->parent->rChild=x;
		}
	}else{
		tree->root=x;
	}
	
	//维护节点的size字段
	x->size=y->size;
	y->size=y->lChild->size+y->rChild->size+1;
}

void insertFixup(prbTree tree,prbNode x){//插入一个节点后，对红黑树进行调整一保持红黑树性质。
	prbNode px=NULL;
	prbNode ppx=NULL;
	prbNode xuncle=NULL;
	while(1){//1
		px=x->parent;
		ppx=px->parent;
		if(px->color==black){//x为根节点的情况在insert中已考虑，所有px一定存在
			return;
		}else if(ppx!=tree->stl){
			if(px==ppx->lChild){//如果px是ppx的左儿子
				xuncle=ppx->rChild;
				if(xuncle->color==red){
					px->color=black;
					xuncle->color=black;
					ppx->color=red;
					x=ppx;
				}else{
					if(x==px->lChild){
						px->color=black;
						ppx->color=red;
						rightRotate(tree,ppx);
						break;
					}else{
						leftRotate(tree,px);					
						x=px;
					}
				}
			}else{////如果px是ppx的右儿子
				xuncle=ppx->lChild;
				if(xuncle->color==red){
					xuncle->color=black;
					px->color=black;
					ppx->color=red;
					x=ppx;
				}else{
					if(x==px->rChild){
						ppx->color=red;
						px->color=black;
						leftRotate(tree,ppx);
						break;
					}else{
						rightRotate(tree,px);
						x=px;
					}
				}
			}
		}else{
			break;
		}
	}//1end
	tree->root->color=black;
}

void insert(prbTree tree,prbNode x){
	x->size=1;
	if(tree->root==tree->stl){//如果树为空树，此时，root指向的是sentinel，sentinel的所有指针都为0.
		tree->root=x;
		x->color=black;
		x->lChild=tree->stl;
		x->rChild=tree->stl;
		x->parent=tree->stl;		
	}else{//树不为空树
		prbNode node1=tree->root;
		prbNode node2=NULL;
		while(node1!=tree->stl){///此处注意新插入的节点的size值要置为1
			node2=node1;
			node1->size=node1->size+1;
			if(x->key<=node1->key){				
				node1=node1->lChild;
			}else{
				node1=node1->rChild;
			}
		}

		//x->color=red;

		x->parent=node2;
		if(x->key<=node2->key){
			node2->lChild=x;
		}else{
			node2->rChild=x;
		}
		insertFixup(tree,x);
	}
//	tree->root->color=black;
}


void delFixup(prbTree tree, prbNode x)
{
	prbNode px=NULL;
	prbNode xuncle=NULL;
	while(x->color==black&&x!=tree->root)
	{//1
		px=x->parent;	
		if(x==px->lChild)//如果x为px的左孩子
		{//2
			xuncle=px->rChild;
			if(xuncle->color==red)//如果x的uncle节点为红色
			{
				leftRotate(tree, px);
				xuncle->color=black;
				px->color=red;
			}
			else if(xuncle->lChild->color==black&&xuncle->rChild->color==black)//如果xuncle为黑色且两个子女都为黑色
			{
				xuncle->color=red;
				x=px;
			}
			else if(xuncle->lChild->color==black&&xuncle->rChild->color==red)//如果xuncle所指的左孩子为黑色，右孩子为红色
			{
				leftRotate(tree, px);
				if(px->color==black)
				{
					xuncle->rChild->color=black;
				}
				break;
			}
			else if(xuncle->lChild->color==red&&xuncle->rChild->color==black)
			{
				xuncle->lChild->color=black;
				xuncle->color=red;
				rightRotate(tree, xuncle);
				x->parent=px;//当x为sentinel时左旋或右旋会使x的parent指针指错位置
			}
			else
			{
				xuncle->color=red;
				xuncle->lChild->color=black;
				xuncle->rChild->color=black;
			}
		}//2end
		else//x为px的右孩子
		{
			xuncle=px->lChild;
			if(xuncle->color==red)//
			{
				rightRotate(tree, px);
				xuncle->color=black;
				px->color=red;
			}
			else if(xuncle->lChild->color==black&&xuncle->rChild->color==black)
			{
				xuncle->color=red;
				x=px;
			}
			else if(xuncle->lChild->color==red&&xuncle->rChild->color==black)
			{
				leftRotate(tree, px);
				if(px->color==black)
				{
					xuncle->lChild->color=black;
				}
				break;
			}
			else if(xuncle->lChild->color==black&&xuncle->rChild->color==red)
			{
				xuncle->color=red;
				xuncle->rChild->color=black;
				leftRotate(tree, xuncle);
				x->parent=px;
			}
			else
			{
				xuncle->color=red;
				xuncle->lChild->color=black;
				xuncle->rChild->color=black;
			}
		}
	}//1end
	x->color=black;
}

//删除在二叉树中查找到的第一个值为z的节点
void del(prbTree tree, double z)
{
	prbNode node1=tree->root;//用于查找并表示被删除的节点
	prbNode temp=NULL;
	prbNode node2=NULL;
	prbNode x=NULL;
//	double temp=0.0;
	while(node1!=tree->stl&&node1->key!=z)
	{//1
//		node2=node1;
	//	node1->size=node1->size-1;
		if(node1->key>z)
		{
			node1=node1->lChild;
		}
		else if(node1->key<z)
		{
			node1=node1->rChild;
		}
		else
		{
			
		}
	}//1end
	if(node1==tree->stl)
	{
		cout<<"红黑树中没有key值为:"<<z<<"的节点。\n";
	}
	else
	{
		temp=node1;
		while(temp!=tree->stl)
		{
			temp->size=temp->size-1;
			temp=temp->parent;
		}
		if(node1->rChild!=tree->stl)//如果node1的右子树存在
		{
			//node1->size=node1->size-1;
			node2=node1->rChild;
			while(node2->lChild!=tree->stl)
			{
				node2->size=node2->size-1;
				node2=node2->lChild;
			}
			x=node2->rChild;//node2指向node1的后续节点
			//node1->key=node2->key;                                              
			node2->rChild->parent=node2->parent;
			if(node2->key>node2->parent->key)
			{
				node2->parent->rChild=node2->rChild;
			}
			else
			{
				node2->parent->lChild=node2->rChild;
			}
			node1->key=node2->key;  
			if(node2->color==black)
			{
				delFixup(tree, x);
			}
		}
		else//node1的右子树不存在
		{//1
			if(node1->lChild==tree->stl)//如果node1的左子树也不存在
			{
				if(node1!=tree->root)
				{//如果node1不为根
					//node1->parent->size=node1->size-1;
					x=tree->stl;
					if(node1->parent->key<node1->key)
					{
						node1->parent->rChild=tree->stl;
					}
					else
					{
						node1->parent->lChild=tree->stl;
					}
					x->parent=node1->parent;
					if(node1->color==black)
					{
						delFixup(tree, x);
					}
				}
				else
				{
					tree->root=tree->stl;
				}
			}
			else//如果node1的左子树存在
			{
				//如果node1为根节点
				if(node1==tree->root)
				{
					tree->root=node1->lChild;
					node1->lChild->parent=tree->stl;
					tree->root->color=black;
				}
				else
				{
					x=node1->lChild;//以x为根进行红黑调整
					if(node1->key>node1->parent->key)
					{
						node1->parent->rChild=node1->lChild;
					}
					else
					{
						node1->parent->lChild=node1->lChild;
					}
					node1->lChild->parent=node1->parent;
					if(node1->color==black)
					{
						delFixup(tree, x);
					}
				}
			}
		}//1end
	}		
}

void showRBTree2(prbTree tree)
{
	if(tree->root==tree->stl)
	{
		return;
	}
	cout<<"红黑树的节点信息为: 编号|key|color|size|父节点编号\n";

	pqHead pqh=initQueue();

	pqueue node=new queue;
	node->n=tree->root;

	enQueue(pqh, node);
	node=new queue;
	node->n=NULL;
	enQueue(pqh, node);

	pqueue temp=NULL;
	prbNode prbn=NULL;

	while(!isNullQueue(pqh))
	{
		temp=deQueue(pqh);		
		if(temp->n!=NULL)
		{
			if(temp->n->lChild!=tree->stl)
			{
				node=new queue;
				node->n=temp->n->lChild;
				enQueue(pqh, node);		
			}

			if(temp->n->rChild!=tree->stl)
			{
				node=new queue;
				node->n=temp->n->rChild;
				enQueue(pqh, node);
			}
			if(temp->n->rChild!=tree->stl||temp->n->lChild!=tree->stl)
			{
				node=new queue;
				node->n=NULL;
				enQueue(pqh, node);				
			}						
		}

			prbn=temp->n;
			if(prbn!=NULL)
			cout<<prbn->number<<"|"<<prbn->key<<"|"<<prbn->color<<"|"<<prbn->size<<"|"<<prbn->parent->number<<endl;	
			else
			cout<<"***"<<endl;
		
	}
}

//出队并输出本节点的值时同时将子节点（包括空节点）入队，如果遇到换层节点则要在对尾加上换层节点，因为本层的所有节点已入队
void showRBTree3(prbTree tree)//show the tree in the shape of the tree(but I didn't add spaces to make it like a real tree.)
{
	if(tree->root==tree->stl)
	{
		cout<<"the tree is empty.\n";
		return;
	}
	//cout<<"红黑树的节点信息为: 编号|key|color|size|父节点编号\n";
	cout<<"the red-black-tree's node information is listed in form of the following: number|key|color|size|parent's number\n";
	pqHead pqh=initQueue();

	pqueue node=new queue;
	node->n=tree->root;
	enQueue(pqh, node);


	node=new queue;
	node->n=NULL;
	node->t=true;
	enQueue(pqh, node);//本层结束的信号

	pqueue temp=NULL;

	//bool isEnd=false;
	pqueue temp2=NULL;//用于检测是整个树已经输完

	prbNode prbn=NULL;	
	while(!isNullQueue(pqh))
	{
		temp=deQueue(pqh);
		prbn=temp->n;
		if(temp->n!=NULL)
		{
			cout<<" {"<<prbn->number<<"|"<<prbn->key<<"|"<<prbn->color<<"|"<<prbn->size<<"|"<<prbn->parent->number<<"} ";	
			if(temp->n->lChild!=tree->stl)
			{
				node=new queue;
				node->n=temp->n->lChild;
				enQueue(pqh, node);		
			}
			else
			{
				node=new queue;
				node->n=NULL;
				enQueue(pqh, node);				
			}

			if(temp->n->rChild!=tree->stl)
			{
				node=new queue;
				node->n=temp->n->rChild;
				enQueue(pqh, node);
			}
			else
			{
				node=new queue;
				node->n=NULL;
				enQueue(pqh, node);			
			}			
		}
		else
		{
			if(temp->t==true)//&&temp->n==NULL表示本层的输出结束。要换行了
			{
				cout<<"\n";

				
				temp2=temp->next;
				while(temp2!=NULL)
				{
					if(temp2->n!=NULL)
					{
						break;
					}
					temp2=temp2->next;
				}
				if(temp2==NULL)
				{
					return;
				}

				node=new queue;
				node->n=NULL;
				node->t=true;
				enQueue(pqh, node);//本层结束的信号
			}
			else//temp->t==false&&temp->n==NULL，表示本节点为空节点
			{
				cout<<" {*|*|*|*|*} ";

				node=new queue;
				node->n=NULL;
				enQueue(pqh, node);	

				node=new queue;
				node->n=NULL;
				enQueue(pqh, node);	
			}
		}
		//判断是否要加上换行的标记

	}

}

void showRBTree(prbTree tree){//中序遍历红黑树...show tree's node in the order of the "key" value from min to max.
	//cout<<"红黑树的节点信息为: 序号|地址|key|color|size|parent|lChild|rChild\n";
	//cout<<"红黑树的节点信息为: 序号|编号|父节点编号|key|color|size\n";
	cout<<"the red-black-tree's node information is listed in form of the following: index|number|parent's number|key|color|size\n";
	prbNode node1=tree->root;
	prbNode node2=NULL;
	int i=1;
	int j=0;
	if(tree->root==tree->stl){
		//cout<<"树为空树。"<<endl;
		cout<<"tree is empty!\n";
		return;
	}
	while(1){//1
		if(node1->lChild!=tree->stl&&j==0){
			node1=node1->lChild;
		}else{
			//cout<<"               "<<i<<"|"<<node1<<"|"<<node1->key<<"|"<<node1->color<<"|"<<node1->size<<"|"<<node1->parent<<"|"<<node1->lChild<<"|"<<node1->rChild<<endl;
			cout<<"               "<<i<<"|"<<node1->number<<"|"<<node1->parent->number<<"|"<<node1->key<<"|"<<node1->color<<"|"<<node1->size<<endl;
			
			j=0;
			i++;
			if(node1->rChild!=tree->stl){
				node1=node1->rChild;				
			}else{
				while(true){//2
					node2=node1;
					node1=node1->parent;
					if(node2==node1->lChild){
						j=1;
						break;
					}
					if((node2!=node1->lChild)&&(node1->parent==tree->stl || node1==tree->stl)){
						return;
					}				
				}//2				
			}			
		}
	}//1end
}

int main(){
	int i=1;
	int num=0;//待输入的节点的数目。
	prbNode node=NULL;
	double a=0;
	prbTree tree=initRBTree();

	//cout<<"请输入红黑树的节点数：\n";
	cout<<"please input the number of node of the red-black-tree:\n";

	cin>>num;
	while(i<=num){
		node=new rbNode;
		node->color=red;
		node->size=1;
		node->lChild=tree->stl;
		node->rChild=tree->stl;
		node->parent=tree->stl;
		
		//cout<<"请输入红黑树的第"<<i<<"个节点:\n";
		cout<<"please input the red-black-tree's "<<i<<"th node:\n";
		cin>>a;
		node->key=a;
		node->number=i;
		i++;
		insert(tree,node);		
	}
	showRBTree(tree);
	showRBTree3(tree);
	while(true){
		//cout<<"请输入要删除的节点的key值：";
		cout<<"please input the node going to be deleted:\n";
		cin>>a;
		del(tree, a);
		//cout<<"删除节点"<<a<<"后的树为：\n";
		cout<<"after node being deleted, the tree is:\n";
		showRBTree(tree);
		showRBTree3(tree);
	}
	return 0;
}