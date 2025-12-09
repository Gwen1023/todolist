#include "storage.h"
#include "todolist.h"
#include "backend.h"
#include <stdio.h>
#include <string.h>

//保存任务到文件(二进制格式)
int saveTasksToFile(TaskList *list, const char *filename)
{
    if(list == NULL || filename == NULL) return -1;//边界检查
    
    //打开文件以及检查 
    FILE *fp = fopen(filename, "wb");
    if(fp == NULL){
        printf("文件打开失败!\n");
        return -1;
    }

    fwrite(&list->count, sizeof(int), 1, fp);//写入任务个数
    
    Task* current = list->head;
    while(current != NULL){
        fwrite(current, sizeof(Task), 1, fp);
        current = current->next;
    }

    fclose(fp);
    return 0;
}

//从文件加载任务(二进制格式)
TaskList* loadTasksFromFile(const char *filename)
{
    if(filename == NULL) return NULL;//边界检查

    FILE *fp = fopen(filename, "rb");
    if(fp == NULL){
        pritnf("文件打开失败!\n");
        return NULL;
    }//打开文件(二进制形式)

    TaskList* list = initTaskList();
    if(list == NULL){
        fclose(fp);
        return NULL;
    }//创建任务链表

    int count;
    if(fread(&count, sizeof(int), 1, fp) != 1){
        fclose(fp);
        freeTaskList(list);
        return NULL;
    }//读取任务总数

    for (int i = 0; i < count; i++){
        Task* task = (Task*)malloc(sizeof(Task));
        if(task == NULL){
            fclose(fp);
            freeList(list);
            return NULL;
        }

        if(fread(task, sizeof(task), 1, fp) != 1){
            free(task);
            fclose(fp);
            freeList(list);
            return NULL;
        }
        task->next = NULL;
        addTask(list, task);
    }
    fclose(fp);
    return list;
}

int exportTasksToCSV(TaskList *list, const char *filename)
{
    if(list == NULL || filename == NULL) return -1;
    FILE *fp = fopen(filename, "w");
    if(fp == NULL){
        printf("无法打开文件 %s 进行写入！\n", filename);
        return -1;
    }

    fprintf(fp, "ID,标题,描述,状态,创建时间,截止时间\n");

    Task *current = list->head;
    while(current != NULL){
        char createTimeStr[100];
        strftime(createTimeStr, sizeof(createTimeStr), "%Y-%m-%d %H:%M:%S", localtime(&current->create_time));
        
        char dueTimeStr[100] = "";
        if(current->due_time != 0){
            strftime(dueTimeStr, sizeof(dueTimeStr), "%Y-%m-%d %H:%M:%S", localtime(&current->due_time));
        }

        const char *statusStr;
        switch(current->status){
            case PENDING:
                statusStr = "待办";
                break;
            case IN_PROGRESS:
                statusStr = "进行中";
                break;
            case COMPLETED:
                statusStr = "已完成";
                break;
            default :
            statusStr = "未知";
        }

        fprintf(file, "%d,%s,%s,%s,%s,%s\n", 
                current->id, 
                current->title, 
                current->description, 
                statusStr, 
                createTimeStr, 
                dueTimeStr);

        current = current->next;
    }

    fclose(fp);
    return 0;
}

TaskList *importTasksFromCSV(const char *filename){
    if(filename == NULL){
        return NULL;
    }

    FILE *fp = fopen(filename, "r");
    if(fp == NULL){
        printf("无法打开文件 %s 进行读取！\n", filename);
        return NULL;
    }

    TaskList *list = initTaskList();
    if(list == NULL){
        fclose(fp);
        return NULL;
    }

    char line[1024];
    if (fgets(line, sizeof(line), file) == NULL) {
        // 文件为空
        fclose(file);
        freeTaskList(list);
        return NULL;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        // 解析CSV行
        int id;
        char title[MAX_TITLE_LENGTH];
        char description[MAX_DESCRIPTION_LENGTH];
        char statusStr[20];
        char createTimeStr[100];
        char dueTimeStr[100];
        
        if (sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,],%s", &id, title, description, statusStr, createTimeStr, dueTimeStr) < 5) {
            // 解析失败，跳过此行
            continue;
        }

        TaskStatus status = PENDING;  // 默认状态
        if (strcmp(statusStr, "待办") == 0) {
            status = PENDING;
        } else if (strcmp(statusStr, "进行中") == 0) {
            status = IN_PROGRESS;
        } else if (strcmp(statusStr, "已完成") == 0) {
            status = COMPLETED;
        }

        // 解析创建时间
        time_t create_time;
        struct tm tm_time;
        if (strptime(createTimeStr, "%Y-%m-%d %H:%M:%S", &tm_time) != NULL) {
            create_time = mktime(&tm_time);
        } else {
            // 解析失败，使用当前时间
            time(&create_time);
        }
        
        // 解析截止时间
        time_t due_time = 0;
        if (strlen(dueTimeStr) > 0 && strptime(dueTimeStr, "%Y-%m-%d %H:%M:%S", &tm_time) != NULL) {
            due_time = mktime(&tm_time);
        }
        
        // 创建任务
        Task *task = createTask(id, title, description, due_time);
        if (task != NULL) {
            task->status = status;
            task->create_time = create_time;
            
            // 添加到链表
            addTask(list, task);
        }
    }
    
    // 关闭文件
    fclose(file);
    
    return list;
}