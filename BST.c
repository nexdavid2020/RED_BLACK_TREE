#include <stdio.h>
#include <stdlib.h>

// BST 구현
// 학교 알고리즘 교재 수도코드 토대로 구현!

typedef int element;
typedef struct TreeNode{
    element key;            // 값
    struct TreeNode* left;  // 왼쪽 자식
    struct TreeNode* right; // 오른쪽 자식
    struct TreeNode* p;     // 부모
} TreeNode;


// 탐색하는 함수 (재귀 방식)
TreeNode* search(TreeNode* node, int key){

    TreeNode* x = node;
    if(x == NULL || key == x->key){
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

    while(x != NULL && x->key != key){
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
    temp->right = NULL;
    temp->left = NULL;
    temp->p = NULL;
    return temp;
}

// 최솟값 찾는 함수
TreeNode* minimum(TreeNode* node){
    TreeNode* x = node;
    
    // 맨 왼쪽 단말 노드를 찾으러 내려감
    while(x->left != NULL){
        x = x->left;
    }
    return x;
}

// 최댓값 찾는 함수
TreeNode* maximum(TreeNode* node){
    TreeNode* x = node;

    while(x->right != NULL){
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
        while(y != NULL && x == y->right){
            x = y;
            y = y->p;
        }
        return y;
    }
}

void tree_insert(TreeNode** root, TreeNode* z) {
    TreeNode* y = NULL;
    TreeNode* x = *root;

    // z의 적절한 위치를 찾기 위해 트리를 내려간다.
    while (x != NULL) {
        y = x;

        if (z->key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    z->p = y; // z의 부모를 y로 설정한다.

    if (y == NULL) {
        // 트리가 비어 있었다면 z가 root가 된다.
        *root = z;
    } else if (z->key < y->key) {
        // z가 y의 왼쪽 자식이 된다.
        y->left = z;
    } else {
        // z가 y의 오른쪽 자식이 된다.
        y->right = z;
    }
}

// TRANSPLANT 함수 구현
void transplant(TreeNode** root, TreeNode* u, TreeNode* v){
    if(u->p == NULL){
        *root = v;
    }else if(u == u->p->left){
        u->p->left = v;
    }else{
        u->p->right = v;
    }

    if(v != NULL){
        v->p = u->p;
    }
}

// 노드 삭제하는 함수 구현
void tree_delete(TreeNode** root, TreeNode* z){
    TreeNode* y = NULL;
    
    if(z->left == NULL){
        transplant(root, z, z->right); // 삭제하려는 z가 왼쪽 자식이 없으면, 오른쪽 서브트리를 z 위치로 격상시키는 부분
    }else if(z->right == NULL){
        transplant(root, z, z->left);  // 삭제하려는 z가 오른쪽 자식이 없으면, 왼쪽 서브트리를 z 위치로 격상시키는 부분
    }else{
        y = minimum(z->right);
        if(y != z->right){
            transplant(root, y, y->right);
            y->right = z->right;
            y->right->p = y;
        }
        transplant(root, z, y);
        y->left = z->left;
        y->left->p = y;
    }
}

void inorder_traversal(TreeNode** root){
    TreeNode* x = *root;

    if(x != NULL){
        inorder_traversal(&(x->left));
        printf("%d ", x->key);
        inorder_traversal(&(x->right));
    }
}

int arr[] = {12, 24, 3, 1, 9, 5, 16, 2, 29, 13, 7, 92, 36};

int main(void){

    TreeNode* T = new_node(10);

    for(int i=0; i<sizeof(arr)/sizeof(int); i++){
        TreeNode* z = new_node(arr[i]);

        tree_insert(&T, z);
    }

    inorder_traversal(&T); // 중위 순회
    printf("\n");

    TreeNode* temp = search(T, 12);

    tree_delete(&T, temp);
    inorder_traversal(&T);
    printf("\n");

    return 0;
}