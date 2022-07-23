#include<stdio.h>
#include<string.h>
#include<malloc.h>
#define OK 0
#define ERROR -1
typedef int State;
typedef int DataType;

typedef struct LinkList{
    DataType operatorAndData;
    struct LinkList *next;
}LinkList,*LinkNode;

typedef struct LinkStact{
    LinkNode top;
    int count;
}LinkStack;
State createStack(LinkStack *S) {
	S->count = -1;
    S->top = NULL;
    return OK;
}
//push
State push(LinkStack *S, DataType data) {
    LinkNode s;
    s = (LinkNode)malloc(sizeof(LinkList));
    s->operatorAndData = data;
    s->next = S->top;
    S->top = s;
    S->count++;
    return OK;
}
//pop
State pop(LinkStack *S,DataType *data) {
    LinkNode p;
    //判断栈空
    if (S->top == NULL || S->count==-1) {
        return ERROR;
    }
    *data = S->top->operatorAndData;
    p = S->top;
    S->top = S->top->next;
    S->count--;
    // printf("%c",p);
    free(p);
    return OK;
}

State peek(LinkStack *S,DataType *data){
    //判断栈空
    if (S->top == NULL || S->count==-1) {
        return ERROR;
    }
    *data = S->top->operatorAndData;
    return OK;
}