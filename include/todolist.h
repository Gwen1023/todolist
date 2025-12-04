#ifndef TODOLIST_H
#define TODOLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 任务状态枚举
typedef enum {
    PENDING = 0,     // 待完成
    IN_PROGRESS,     // 进行中
    COMPLETED        // 已完成
} TaskStatus;

// 任务结构体
typedef struct Task {
    int id;                  // 任务ID
    char title[100];         // 任务标题
    char description[500];   // 任务描述
    TaskStatus status;       // 任务状态
    time_t create_time;      // 创建时间
    time_t due_time;         // 截止时间
    struct Task *next;       // 指向下一个任务的指针
} Task;

// 任务链表结构体
typedef struct TaskList {
    Task *head;              // 链表头指针
    Task *tail;              // 链表尾指针
    int count;               // 任务数量
} TaskList;

// 共用常量定义
#define MAX_TITLE_LENGTH 100
#define MAX_DESCRIPTION_LENGTH 500
#define DEFAULT_DATA_FILE "tasks.dat"
#define DEFAULT_CSV_FILE "tasks.csv"

// 命令常量
#define COMMAND_ADD "add"
#define COMMAND_DELETE "delete"
#define COMMAND_UPDATE "update"
#define COMMAND_LIST "list"
#define COMMAND_SEARCH "search"
#define COMMAND_MARK "mark"
#define COMMAND_STATS "stats"
#define COMMAND_EXPORT "export"
#define COMMAND_IMPORT "import"
#define COMMAND_EXIT "exit"

#endif // TODOLIST_H