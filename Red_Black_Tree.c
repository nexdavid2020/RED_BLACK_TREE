#include <stdio.h>
#include <stdlib.h>

// RED_BLACK_TREE를 BST를 기반하여
// C언어로 구현
// 학교 알고리즘 교재 수도코드 토대로 구현!

typedef int element;
typedef struct TreeNode{
    element key;            // 값
    struct TreeNode* left;  // 왼쪽 자식
    struct TreeNode* right; // 오른쪽 자식
    struct TreeNode* p;     // 부모
    char color;  // 'r' or 'b'
} TreeNode;

TreeNode NILL_Node; // 전역 변수로 NILL 노드 선언


// 탐색하는 함수 (재귀 방식)
TreeNode* search(TreeNode* node, int key){

    TreeNode* x = node;
    if(x == &NILL_Node || key == x->key){
        return x;
    }  

    if(key < x->key){
        return search(x->left, key);
    }else{
        return search(x->right, key);
    }
}

// 탐색하는 함수 (반복문 이용 방식)
TreeNode* iterative_search(TreeNode* node, int key){
    TreeNode* x = node;

    while(x != &NILL_Node && x->key != key){
        if(key < x->key){
            x = x->left;
        }else {
            x = x->right;
        }
    }
    return x;
}

TreeNode* new_node(int item){
    TreeNode* temp = (TreeNode*)malloc(sizeof(TreeNode)*1);
    temp->key = item;
    temp->right = &NILL_Node;
    temp->left = &NILL_Node;
    temp->p = &NILL_Node;
    temp->color = 'r'; // 기본은 RED로
    return temp;
}

// 레드블랙트리를 만드는 노드
TreeNode* make_red_black_tree(int key){

    NILL_Node.color = 'b';
    NILL_Node.key = -100000;
    NILL_Node.left = NULL;
    NILL_Node.right = NULL;
    NILL_Node.p = NULL;

    TreeNode* root = (TreeNode*)malloc(sizeof(TreeNode));
    root->key = key;
    root->color = 'b';
    root->p = &NILL_Node;
    root->left = &NILL_Node;
    root->right = &NILL_Node;
    
    return root;
}


// 최솟값 찾는 함수
TreeNode* minimum(TreeNode* node){
    TreeNode* x = node;
    
    // 맨 왼쪽 단말 노드를 찾으러 내려감
    while(x->left != &NILL_Node){
        x = x->left;
    }
    return x;
}

// 최댓값 찾는 함수
TreeNode* maximum(TreeNode* node){
    TreeNode* x = node;

    while(x->right != &NILL_Node){
        x = x->right;
    }
    return x;
}

// Successor 찾는 함수
TreeNode* successor(TreeNode* node){
    TreeNode* x = node;

    if(x->right != NULL){
        return minimum(x->right);
    }else{
        TreeNode* y = x->p; // x의 부모를 y가 가르키기
        while(y != &NILL_Node && x == y->right){
            x = y;
            y = y->p;
        }
        return y;
    }
}

void inorder_traversal(TreeNode** root){
    TreeNode* x = *root;

    if(x != &NILL_Node){
        inorder_traversal(&(x->left));
        printf("%d ", x->key);
        inorder_traversal(&(x->right));
    }
}

// 왼쪽 회전 함수
void left_rotate(TreeNode** root, TreeNode* x){ // x 기준으로 왼쪽으로 회전!

    TreeNode* y = x->right;
    x->right = y->left; 
    if(y->left != &NILL_Node){ 
        y->left->p = x;
    }
    y->p = x->p;

    if(x->p == &NILL_Node){ // x가 root였다는 것이니 root를 y로 바꿔야!
        *root = y; 
    
    }else if(x == x->p->left){
        x->p->left = y;
    }else{
        x->p->right = y;
    }

    y->left = x;
    x->p = y;

}

// 오른쪽 회전 함수
void right_rotate(TreeNode** root, TreeNode* x){ // x 기준으로 오른쪽으로 회전!
    
    TreeNode* y = x->left;
    x->left = y->right;

    if(y->right != &NILL_Node){
        y->right->p = x;
    }
    y->p = x->p;

    if(x->p == &NILL_Node){ // x가 root였다는 것이니 root를 y로 바꿔야!
        *root = y;
    }else if(x == x->p->left){
        x->p->left = y;
    }else {
        x->p->right = y;
    }
    y->right = x;
    x->p = y;
}

// RB_insert_fixup
void RB_insert_fixup(TreeNode** root, TreeNode* z){
    TreeNode* y = NULL;

    while(z->p->color == 'r'){
        if(z->p == z->p->p->left){   // 만약 z의 부모가 z의 조부모의 왼쪽 자식이라면
            y = z->p->p->right; // z의 삼촌 (부모의 형제)
            if(y->color == 'r'){
                // case 1 부분 (삽입된 노드의 부모와 삼촌 모두 red 일 경우)
                // 부모와 삼촌을 black으로 바꾸고, 할아버지를 red로 바꾼 뒤 할아버지에서 다시 확인 시작!
                z->p->color = 'b';
                y->color = 'b';
                z->p->p->color = 'r';
                z = z->p->p;     // z를 z의 조부모로 갱신
            }else{ // 삼촌의 색이 black일 때
                
                if(z == z->p->right){ // z가 부모의 오른쪽 자식, 이 상황은 조부모와 z가 꺾여있는 상태! case 2
                    // case2 부분 (삽입된 red노드가 부모의 오른쪽 자녀, 부모도 red이고 조부모의 왼쪽 자녀)
                    // 삼촌은 black 인 경우! -> 부모를 기준으로 왼쪽으로 회전한 뒤 case3 ㄱㄱ
                    z = z->p; 
                    left_rotate(root, z); // z의 부모 기준으로 왼쪽 회전
                }
                // 이 부분이 case 3! (삽입된 red 노드가 부모의 왼쪽 자녀, 부모도 red이면서 조부모의 왼쪽 자녀)
                // 삼촌은 black인 경우! -> 부모와 할아버지의 색을 바꾼 후 할아버지 기준으로 오른쪽 회전!
                z->p->color = 'b';
                z->p->p->color = 'r';
                right_rotate(root, z->p->p);
            }   
        }
        else{ // same as line 177~197, but with 'right' and 'left' exchanged
            y = z->p->p->left; 
            if(y->color == 'r'){
                // case 1 부분 
                z->p->color = 'b';
                y->color = 'b';
                z->p->p->color = 'r';
                z = z->p->p;     
            }else{ 
                if(z == z->p->left){ 
                    // case2 부분
                    z = z->p; 
                    right_rotate(root, z); 
                }
                // 이 부분이 case 3! 
                z->p->color = 'b';
                z->p->p->color = 'r';
                left_rotate(root, z->p->p);
            }    
        }
    }

    (*root)->color = 'b'; // 루트 노드의 색을 black으로 바꾸기!

}   

void RB_insert(TreeNode** root, TreeNode* z) {
    
    TreeNode* x = *root;
    TreeNode* y = &NILL_Node;

    // z의 적절한 위치를 찾기 위해 트리를 내려간다.
    while (x != &NILL_Node) {
        y = x;

        if (z->key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    z->p = y; // z의 부모를 y로 설정한다.

    if (y == &NILL_Node) {
        // 트리가 비어 있었다면 z가 root가 된다.
        *root = z;
    } else if (z->key < y->key) {
        // z가 y의 왼쪽 자식이 된다.
        y->left = z;
    } else {
        // z가 y의 오른쪽 자식이 된다.
        y->right = z;
    }

    z->left = &NILL_Node;
    z->right = &NILL_Node;
    z->color = 'r'; // the new node starts out red

    RB_insert_fixup(root, z); // 이걸로 이제 red black 트리의 조건들이 충족되는지 여부 체크하는 헬퍼함수
}

// TRANSPLANT 함수 구현
void RB_transplant(TreeNode** root, TreeNode* u, TreeNode* v){
    if(u->p == &NILL_Node){
        *root = v;
    }else if(u == u->p->left){
        u->p->left = v;
    }else{
        u->p->right = v;
    }

    v->p = u->p;
}

// RB_delete_fixup
void RB_delete_fixup(TreeNode** root, TreeNode* x){
    TreeNode* w = NULL;
    
    while(x != *root && x->color =='b'){
        
        if(x == x->p->left){ // 만약 x가 부모의 왼쪽 자식이고
            w = x->p->right; // w는 x의 형제

            if(w->color == 'r'){
                // case 1 - doubly black의 오른쪽 형제가 red일 때 
                w->color = 'b';
                x->p->color = 'r';
                left_rotate(root, x->p);
                w = x->p->right;
            }

            if(w->left->color == 'b' && w->right->color == 'b'){
                // case 2 - 형제의 두 자식이 다 black인 경우 
                w->color = 'r';
                x = x->p;
            
            }else{

                if(w->right->color == 'b'){
                    // case 3 - doubly black의 오른쪽 형제가 black, 그 형제의 왼쪽 자녀가 red, 오른쪽 자녀가 black 일 때
                    w->left->color = 'b';
                    w->color = 'r';
                    right_rotate(root, w);
                    w = w->p->right;
                }

                // 여기가 case 4!
                w->color = x->p->color;
                x->p->color = 'b';
                w->right->color = 'b';
                left_rotate(root, x->p);
                x = *root;
            }
        }
        else{  // same as lines 282 ~ 313, but with 'right' and 'left' exchanged
            w = x->p->left; // w는 x의 형제

            if(w->color == 'r'){
                // case 1 - doubly black의 오른쪽 형제가 red일 때 
                w->color = 'b';
                x->p->color = 'r';
                right_rotate(root, x->p);
                w = x->p->left;
            }

            if(w->right->color == 'b' && w->left->color == 'b'){
                // case 2 - 형제의 두 자식이 다 black인 경우 
                w->color = 'r';
                x = x->p;
            
            }else{

                if(w->left->color == 'b'){
                    // case 3 - doubly black의 오른쪽 형제가 black, 그 형제의 왼쪽 자녀가 red, 오른쪽 자녀가 black 일 때
                    w->right->color = 'b';
                    w->color = 'r';
                    left_rotate(root, w);
                    w = w->p->left;
                }

                // 여기가 case 4!
                w->color = x->p->color;
                x->p->color = 'b';
                w->left->color = 'b';
                right_rotate(root, x->p);
                x = *root;
            }
        }
    }

    x->color = 'b';
}

// 노드 삭제하는 함수 구현
void RB_delete(TreeNode** root, TreeNode* z){
    TreeNode* y = z;
    char y_original_color = y->color; // 이거 기억해두기! 

    TreeNode* x = NULL;
    
    if(z->left == &NILL_Node){
        x = z->right;
        RB_transplant(root, z, z->right); // 삭제하려는 z가 왼쪽 자식이 없으면, 오른쪽 서브트리를 z 위치로 이식시키는 부분
    
    }else if(z->right == &NILL_Node){
        x = z->left;
        RB_transplant(root, z, z->left);  // 삭제하려는 z가 오른쪽 자식이 없으면, 왼쪽 서브트리를 z 위치로 이식시키는 부분
    
    }else{
        y = minimum(z->right);
        y_original_color = y->color;
        x = y->right;

        if(y != z->right){
            RB_transplant(root, y, y->right);
            y->right = z->right;
            y->right->p = y;
        }else{
            x->p = y;
        }
        RB_transplant(root, z, y);
        y->left = z->left;
        y->left->p = y;
        y->color = z->color;
    }

    if(y_original_color == 'b'){  // 만약 레드-블랙트리의 속성을 어겼다면
        RB_delete_fixup(root, x); // fix up 하기
    }

    free(z); // 삭제하는 z노드 free 시켜주기
}


int arr[] = {12, 24, 3, 1, 9, 5, 16, 2, 29, 13, 7, 92, 36};

int main(void){

    TreeNode* T = make_red_black_tree(10); // 레드블랙 트리 처음 만들고 root노드에 10 넣기

    for(int i=0; i<sizeof(arr)/sizeof(int); i++){
        TreeNode* z = new_node(arr[i]);

        RB_insert(&T, z);
    }

    inorder_traversal(&T); // 중위 순회
    printf("\n");


    // 12 찾아 지우기!
    TreeNode* temp = search(T, 12);
    RB_delete(&T, temp);
    inorder_traversal(&T);
    printf("\n");

    // 29 찾아 지우기!
    temp = search(T, 29);
    RB_delete(&T, temp);
    inorder_traversal(&T);
    printf("\n");


    // 100 추가하기!
    TreeNode* new = new_node(100);
    RB_insert(&T, new);
    inorder_traversal(&T);
    printf("\n");

    // 28 추가하기!
    new = new_node(28);
    RB_insert(&T, new);
    inorder_traversal(&T);
    printf("\n");


    return 0;
}