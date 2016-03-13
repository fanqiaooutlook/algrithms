//some important explain information has been written in English, I think if you know a little English, you can understand all of the source  code. 

#include <iostream>
using namespace std;
#define red 1
#define black 0
typedef struct rbNode * prbNode;
struct rbNode{
	double key;//�ڵ������Ϣ
	int color;
	int number;
	prbNode lChild;
	prbNode rChild;
	prbNode parent;
	int size;//�Ե�ǰ�ڵ�Ϊ�������Ľڵ������
};
//������Ľڵ�������У�������ΪС�ڵ��ڸ��Ľڵ�
typedef struct rbTree * prbTree;
struct rbTree{
	prbNode root;
	prbNode stl;//ָ���ڱ��ڵ�
};

/**********���в���*************/
typedef struct queue* pqueue;
//queue�ڵ�
struct queue
{
	prbNode n;
	bool t;//true��ʾ�����ĩβ��false��ʾ�ǿհ׽ڵ㣻ͬʱnΪNULL
	pqueue next;
};

//����ͷ�ڵ�
typedef struct queueHead* pqHead;
struct queueHead
{
	pqueue head;
	pqueue tail;
};

//��ʼ������
pqHead initQueue()
{
	pqHead pqh=new queueHead;
	pqh->head=NULL;
	pqh->tail=NULL;
	return pqh;
}

//�ж϶����Ƿ�Ϊ�� ���շ�����
bool isNullQueue(pqHead pqh)
{
	if(pqh->head==NULL&&pqh->tail==NULL)
	{
		return true;
	}
	return false;
}

//���
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
 
//����
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
/***���в���end**/




prbTree initRBTree(){//��ʼʱ����������ʹָ��ָ��sentinel�ڵ㡣
	prbTree tree=new rbTree;

	prbNode sentinel=new rbNode;
	sentinel->color=black;
	sentinel->lChild=NULL;
	sentinel->rChild=NULL;
	sentinel->parent=NULL;//sentinel������ָ�붼ΪNULL
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

	//ά���ڵ���ָ��
	y->lChild=x;
	y->parent=x->parent;
	x->parent=y;
	x->rChild=b;
	b->parent=x;

	
	if(y->parent!=tree->stl){//���y�ĸ��ײ�Ϊ�գ���y��Ϊ���ڵ�
		if(y->parent->key>=y->key){
			y->parent->lChild=y;
		}else{
			y->parent->rChild=y;
		}
	}else{
		tree->root=y;
	}
	
	//ά���ڵ��size�ֶ�
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

	//ά���ڵ���ָ��
	if(x->parent!=tree->stl){//���y�ĸ��ײ�Ϊ�գ���x��Ϊ���ڵ�
		if(x->parent->key>=x->key){
			x->parent->lChild=x;
		}else{
			x->parent->rChild=x;
		}
	}else{
		tree->root=x;
	}
	
	//ά���ڵ��size�ֶ�
	x->size=y->size;
	y->size=y->lChild->size+y->rChild->size+1;
}

void insertFixup(prbTree tree,prbNode x){//����һ���ڵ�󣬶Ժ�������е���һ���ֺ�������ʡ�
	prbNode px=NULL;
	prbNode ppx=NULL;
	prbNode xuncle=NULL;
	while(1){//1
		px=x->parent;
		ppx=px->parent;
		if(px->color==black){//xΪ���ڵ�������insert���ѿ��ǣ�����pxһ������
			return;
		}else if(ppx!=tree->stl){
			if(px==ppx->lChild){//���px��ppx�������
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
			}else{////���px��ppx���Ҷ���
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
	if(tree->root==tree->stl){//�����Ϊ��������ʱ��rootָ�����sentinel��sentinel������ָ�붼Ϊ0.
		tree->root=x;
		x->color=black;
		x->lChild=tree->stl;
		x->rChild=tree->stl;
		x->parent=tree->stl;		
	}else{//����Ϊ����
		prbNode node1=tree->root;
		prbNode node2=NULL;
		while(node1!=tree->stl){///�˴�ע���²���Ľڵ��sizeֵҪ��Ϊ1
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
		if(x==px->lChild)//���xΪpx������
		{//2
			xuncle=px->rChild;
			if(xuncle->color==red)//���x��uncle�ڵ�Ϊ��ɫ
			{
				leftRotate(tree, px);
				xuncle->color=black;
				px->color=red;
			}
			else if(xuncle->lChild->color==black&&xuncle->rChild->color==black)//���xuncleΪ��ɫ��������Ů��Ϊ��ɫ
			{
				xuncle->color=red;
				x=px;
			}
			else if(xuncle->lChild->color==black&&xuncle->rChild->color==red)//���xuncle��ָ������Ϊ��ɫ���Һ���Ϊ��ɫ
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
				x->parent=px;//��xΪsentinelʱ������������ʹx��parentָ��ָ��λ��
			}
			else
			{
				xuncle->color=red;
				xuncle->lChild->color=black;
				xuncle->rChild->color=black;
			}
		}//2end
		else//xΪpx���Һ���
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

//ɾ���ڶ������в��ҵ��ĵ�һ��ֵΪz�Ľڵ�
void del(prbTree tree, double z)
{
	prbNode node1=tree->root;//���ڲ��Ҳ���ʾ��ɾ���Ľڵ�
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
		cout<<"�������û��keyֵΪ:"<<z<<"�Ľڵ㡣\n";
	}
	else
	{
		temp=node1;
		while(temp!=tree->stl)
		{
			temp->size=temp->size-1;
			temp=temp->parent;
		}
		if(node1->rChild!=tree->stl)//���node1������������
		{
			//node1->size=node1->size-1;
			node2=node1->rChild;
			while(node2->lChild!=tree->stl)
			{
				node2->size=node2->size-1;
				node2=node2->lChild;
			}
			x=node2->rChild;//node2ָ��node1�ĺ����ڵ�
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
		else//node1��������������
		{//1
			if(node1->lChild==tree->stl)//���node1��������Ҳ������
			{
				if(node1!=tree->root)
				{//���node1��Ϊ��
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
			else//���node1������������
			{
				//���node1Ϊ���ڵ�
				if(node1==tree->root)
				{
					tree->root=node1->lChild;
					node1->lChild->parent=tree->stl;
					tree->root->color=black;
				}
				else
				{
					x=node1->lChild;//��xΪ�����к�ڵ���
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
	cout<<"������Ľڵ���ϢΪ: ���|key|color|size|���ڵ���\n";

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

//���Ӳ�������ڵ��ֵʱͬʱ���ӽڵ㣨�����սڵ㣩��ӣ������������ڵ���Ҫ�ڶ�β���ϻ���ڵ㣬��Ϊ��������нڵ������
void showRBTree3(prbTree tree)//show the tree in the shape of the tree(but I didn't add spaces to make it like a real tree.)
{
	if(tree->root==tree->stl)
	{
		cout<<"the tree is empty.\n";
		return;
	}
	//cout<<"������Ľڵ���ϢΪ: ���|key|color|size|���ڵ���\n";
	cout<<"the red-black-tree's node information is listed in form of the following: number|key|color|size|parent's number\n";
	pqHead pqh=initQueue();

	pqueue node=new queue;
	node->n=tree->root;
	enQueue(pqh, node);


	node=new queue;
	node->n=NULL;
	node->t=true;
	enQueue(pqh, node);//����������ź�

	pqueue temp=NULL;

	//bool isEnd=false;
	pqueue temp2=NULL;//���ڼ�����������Ѿ�����

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
			if(temp->t==true)//&&temp->n==NULL��ʾ��������������Ҫ������
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
				enQueue(pqh, node);//����������ź�
			}
			else//temp->t==false&&temp->n==NULL����ʾ���ڵ�Ϊ�սڵ�
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
		//�ж��Ƿ�Ҫ���ϻ��еı��

	}

}

void showRBTree(prbTree tree){//������������...show tree's node in the order of the "key" value from min to max.
	//cout<<"������Ľڵ���ϢΪ: ���|��ַ|key|color|size|parent|lChild|rChild\n";
	//cout<<"������Ľڵ���ϢΪ: ���|���|���ڵ���|key|color|size\n";
	cout<<"the red-black-tree's node information is listed in form of the following: index|number|parent's number|key|color|size\n";
	prbNode node1=tree->root;
	prbNode node2=NULL;
	int i=1;
	int j=0;
	if(tree->root==tree->stl){
		//cout<<"��Ϊ������"<<endl;
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
	int num=0;//������Ľڵ����Ŀ��
	prbNode node=NULL;
	double a=0;
	prbTree tree=initRBTree();

	//cout<<"�����������Ľڵ�����\n";
	cout<<"please input the number of node of the red-black-tree:\n";

	cin>>num;
	while(i<=num){
		node=new rbNode;
		node->color=red;
		node->size=1;
		node->lChild=tree->stl;
		node->rChild=tree->stl;
		node->parent=tree->stl;
		
		//cout<<"�����������ĵ�"<<i<<"���ڵ�:\n";
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
		//cout<<"������Ҫɾ���Ľڵ��keyֵ��";
		cout<<"please input the node going to be deleted:\n";
		cin>>a;
		del(tree, a);
		//cout<<"ɾ���ڵ�"<<a<<"�����Ϊ��\n";
		cout<<"after node being deleted, the tree is:\n";
		showRBTree(tree);
		showRBTree3(tree);
	}
	return 0;
}