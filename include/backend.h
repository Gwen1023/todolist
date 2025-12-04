#ifndef BACKEND_H
#define BACKEND_H

#include "todolist.h"

// 链表基本操作函数（C同学负责）
// 初始化任务链表
TaskList* initTaskList();

// 添加任务到链表
int addTask(TaskList *list, Task *task);

// 根据ID查找任务
Task* findTaskById(TaskList *list, int id);

// 根据ID删除任务
int deleteTask(TaskList *list, int id);

// 清空任务链表
void clearTaskList(TaskList *list);

// 释放任务链表内存
void freeTaskList(TaskList *list);

// 任务操作函数（C同学负责）
// 创建新任务
Task* createTask(int id, const char *title, const char *description, time_t due_time);

// 更新任务信息
int updateTask(TaskList *list, int id, const char *title, const char *description, TaskStatus status, time_t due_time);

// 标记任务状态
int markTaskStatus(TaskList *list, int id, TaskStatus status);

// 高级功能函数（D同学负责）
// 根据标题搜索任务
TaskList* searchTaskByTitle(TaskList *list, const char *keyword);

// 根据状态过滤任务
TaskList* filterTaskByStatus(TaskList *list, TaskStatus status);

// 按创建时间排序任务
void sortTaskByCreateTime(TaskList *list, int ascending);

// 按截止时间排序任务
void sortTaskByDueTime(TaskList *list, int ascending);

// 获取任务统计信息
void getTaskStatistics(TaskList *list, int *total, int *pending, int *inProgress, int *completed);

// 检查即将到期的任务
TaskList* getUpcomingTasks(TaskList *list, int days);

#endif // BACKEND_H