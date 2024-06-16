#include <stdio.h>

typedef struct node{
	int key;
    struct node* left;
    struct node* right;
}NODE;

NODE* init_node(NODE * node, int data){
	node->key=data;
    node->left=NULL;
    node->right=NULL;
    return node;
 }

// 재귀문 구현
NODE *search1(NODE *node, int key){
    //찾고자 하는 값을 찾아봤는데 아예 없었을 경우 혹은 애초에 node 자체가 null이였을 경우
    if(!node)
        return NULL;
        
    //순환으로 탐색 중 원하는 값을 찾았을 때
    if(key == node->key)
        return node;
        
    //찾는 값보다 루트 키 값이 작은 경우 오른쪽 서브트리로 이동
    else if(key > node->key)
        search1(node->right, key);
    //찾는 값보다 루트 키 값이 큰 경우 왼쪽 서브트리로 이동
    else 
        search1(node->left, key);
}

// 반복문 구현
NODE *search2(NODE *node, int key){
	/*찾고자 하는 값을 찾아봤는데 아예 없었을 경우
    혹은 애초에 node 자체가 null 이었을 경우 whild 문을 탈출하게 된다*/
    while(node) //node가 NULL이 아니라면 일단 탐색을 계속 한다.
    {
    	//원하는 키 값을 발견했을 경우
        if(key == node->key)
        	return node; //해당 노드의 주소 반환
            
        //찾고자 하는 값보다 루트키 값이 더 큰 경우 왼쪽 서브트리로 이동
        else if(key < node->key)
        	node = node->left;
            
        //찾고자 하는 값보다 루트키 값이 더 작은 경우 오른쪽 서브트리로 이동
        else
        	node = node->right;
    }
    return NULL;
}

void insert_node(NODE *node, int key){
	//t는 탐색을 위한 포인터이다.
    NODE *p, *t; //p는 부모노드를 가리키는 포인터, t는 현재 노드 포인터
    NODE *n; //n은 새로운 노드
    
    t = *root; //처음 시작 시 현재 노드는 root 그 자체다.
    p = NULL; //root의 부모노드는 없다. root 자체가 시초
    
    //탐색을 먼저 수행한다.
    while(t){
    	if(key == t->key) return; //해당 키가 이미 트리에 존재할 경우 리턴한다.
        p = t; //부모 노드를 현재 노드로 설정한다. 새로운 루트노드
        //서브트리의 새로운 루트노드가 등장하는 것과 같은 원리다.
        
        if(key < p->key) //부모의 키 값과 비교
        	t = p->left; //현재 노드는 부모의 왼쪽 서브트리가 된다.
        else
        	t = p->right; //현재 노드는 부모의 오른쪽 서브트리가 된다.
    }
     //key가 트리안에 없으므로 삽입이 가능하다.
     
     //새로 삽입될 노드를 생성한다.
     n = (NODE*)malloc(sizeof(NODE));
     if(!n) return; //동적할당 실패했을 경우 return
     
     //데이터를 복사한다.
     if(p) // 부모노드가 존재할 경우
     {
     	if(key < p->key)
        	p->left = n;
        else
        	p->right = n;
     }
     else 
     	*root = n; //부모노드가 없으니 새로생긴 노드가 루트 노드가 된다.
}