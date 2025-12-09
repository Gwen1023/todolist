#ifndef BACKEND_H
#define BACKEND_H

#include "todolist.h"

// 链表基本操作函数（C同学负责）
// 初始化任务链表↓
TaskList* initTaskList(){
    TaskList *list = malloc(sizeof(TaskList));//使用malloc动态分配TaskList结构体内存（list就结构体在todolist.h的头文件里面已经写了）
    if(!list) return NULL;//检查内存是否分配成功，失败就返回NULL
    list->head = list->tail = NULL;//初始化链表头尾指针为NULL，表示空链表
    list->count = 0;//设置任务的计数器为0
    return list//返回创建的链表指针
};

// 添加任务到链表
int addTask(TaskList *list, Task *task){
    if (!list || !task) return -1;//参数有效性检查，任一为NULL则返回-1（失败）
    
    if (!list->head) {
        list->head = list->tail = task;//如果链表为空（head为NULL），新任务成为头尾节点
    } else {
        list->tail->next = task;
        list->tail = task;
    }//如果链表非空，将新任务添加到尾部，更新尾指针
    list->count++;
    return 0;
};

// 根据ID查找任务
Task* findTaskById(TaskList *list, int id){
    if(!list)return NULL;
    Task *current = list->head;//从链表头开始遍历
    while (current && current->id != id) {
        current = current->next;
    }//遍历链表，直到找到匹配ID或到达链表末尾
    return current;//返回找到的任务指针（找到）或NULL（未找到）
};

// 根据ID删除任务
int deleteTask(TaskList *list, int id){
    if (!list || !list->head) return -1;

    Task *current = list->head, *prev = NULL;//定义当前节点和前驱节点指针
    while (current && current->id != id) {
        prev = current;
        current = current->next;//遍历查找要删除的任务
    }
    if (!current) return -1;//如果未找到，返回错误
    
    if (prev) {
        prev->next = current->next;
    } else {
        list->head = current->next;
    }//调整链表连接：如果删除的是头节点，更新head；否则更新前驱节点的next
    
    if (current == list->tail) {
        list->tail = prev;
    }//如果删除的是尾节点，更新tail指针
    
    free(current);//释放被删除任务的内存
    list->count--;//任务计数器减1
    return 0;
};

// 清空任务链表
void clearTaskList(TaskList *list){
    if (!list) return;//检查链表有效性
    Task *current = list->head;
    while (current) {
        Task *next = current->next;
        free(current);
        current = next;
    }//遍历链表，释放每个任务节点的内存（从表头开始）
    list->head = list->tail = NULL;//重置头尾指针为NULL
    list->count = 0;//任务计数器归零
};

// 释放任务链表内存
void freeTaskList(TaskList *list){
    if (!list) return;
    clearTaskList(list);//调用clearTaskList清空链表内容
    free(list);//释放链表头结构体内存
};

// 任务操作函数（C同学负责）
// 创建新任务
Task* createTask(int id, const char *title, const char *description, time_t due_time){
    Task *task = malloc(sizeof(Task));
    if(!task)return NULL;//分配任务内存并检查分配结果，同上

    task->id = id;
    strncpy(task->title,title ? title : "",99);//strncpy的用法：把括号内第二个前【第三个数字】赋值到第一个。三元运算符是否符合条件选择第一个还是第二个。
    strncpy(task->description, description ? description : "", 499);
    task->description[499] = '\0';
    task->status = PENDING;//设置任务状态为PENDING（待完成）
    task->create_time = time(NULL);//设置创建时间为当前系统时间，time(NULL)是time.h库里面系统时间的意思
    task->due_time = due_time;//设置截止时间
    task->next = NULL;//把最后一个设为null

    return task;//返回创建的任务
};

// 更新任务信息
int updateTask(TaskList *list, int id, const char *title, const char *description, TaskStatus status, time_t due_time){
    // 查找要更新的任务
    Task *task = findTaskById(list, id);
    if (!task) {
        printf("错误：未找到ID为%d的任务\n", id);
        return -1;
    }
    
    // 更新标题（如果提供了新标题）
    if (title != NULL) {
        strncpy(task->title, title, 99);
        task->title[99] = '\0';
    }
    
    // 更新描述（如果提供了新描述）
    if (description != NULL) {
        strncpy(task->description, description, 499);
        task->description[499] = '\0';
    }
    
    // 更新状态
    task->status = status;
    
    // 更新截止时间
    task->due_time = due_time;
    
    printf("任务ID %d 更新成功\n", id);
    return 0;
};

// 标记任务状态
int markTaskStatus(TaskList *list, int id, TaskStatus status){
    Task *task = findTaskById(list, id);//调用findTaskById查找任务
    if (!task) return -1;
    task->status = status;//更新任务状态
    return 0;
};
