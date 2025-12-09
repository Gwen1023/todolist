#include "ui.h"
#include "todolist.h"
#include "backend.h"
#include "storage.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
    #define CLEAR_SCREEN() system("cls")
#else
    #define CLEAR_SCREEN() system("clear")
#endif

const char* statusToString(TaskStatus status) {
    switch (status) {
        case PENDING: return "待办";
        case IN_PROGRESS: return "进行中";
        case COMPLETED: return "已完成";
        default: return "未知";
    }
}

void trim_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

void showTaskList(TaskList *list) {
    CLEAR_SCREEN();
    printf("任务列表\n");
    printf("-----------------------------------------\n");
    
    if (list == NULL || list->head == NULL) {
        printf("当前没有任务！\n");
        printf("按回车键返回\n");
        getchar();
        return;
    }
    
    printf("ID  |  %-22s  |  状态    \n", "任务标题");
    printf("-----------------------------------------\n");
    
    Task *current = list->head;
    while (current != NULL) {
        printf("%-3d |  %-22s  |  %s\n", current->id, current->title, statusToString(current->status));
        current = current->next;
    }
    
    printf("-----------------------------------------\n");
    printf("任务总数: %d\n", list->count);
    printf("输入任务ID查看详情，或按回车返回: ");
    
    char input[20];
    fgets(input, sizeof(input), stdin);
    trim_newline(input);
    
    if (strlen(input) > 0) {
        int id = atoi(input);
        if (id > 0) {
            Task *task = findTaskById(list, id);
            if (task != NULL) {
                showTaskDetail(task);
            } else {
                printf("未找到ID为 %d 的任务！\n", id);
                printf("按回车键继续...");
                getchar();
            }
        }
    }
}

void showTaskDetail(Task *task) {
    if (task == NULL) return;
    
    CLEAR_SCREEN();  
    printf("任务详情\n");
    printf("-----------------------------------------\n");
    printf("任务ID: %d\n", task->id);
    printf("任务标题: %s\n", task->title);
    printf("任务描述: %s\n", task->description);
    printf("任务状态: %s\n", statusToString(task->status));
    
    char createTimeStr[100];
    strftime(createTimeStr, sizeof(createTimeStr), "%Y-%m-%d %H:%M:%S", localtime(&task->create_time));
    printf("创建时间: %s\n", createTimeStr);
    
    printf("截止时间: ");
    if (task->due_time != 0) {
        char dueTimeStr[100];
        strftime(dueTimeStr, sizeof(dueTimeStr), "%Y-%m-%d %H:%M:%S", localtime(&task->due_time));
        printf("%s\n", dueTimeStr);
    } else {
        printf("未设置\n");
    }
    
    printf("-----------------------------------------\n");
    printf("按回车键返回");
    getchar();
}

int addTaskUI(TaskList *list) {
    CLEAR_SCREEN();
    printf("添加新任务\n");
    printf("-----------------------------------------\n");
    
    char title[MAX_TITLE_LENGTH];
    char description[MAX_DESCRIPTION_LENGTH];
    char dueDate[50];
    
    printf("请输入任务标题: ");
    fgets(title, sizeof(title), stdin);
    trim_newline(title);
    
    printf("请输入任务描述: ");
    fgets(description, sizeof(description), stdin);
    trim_newline(description);
    
    printf("请输入截止日期(YYYY-MM-DD HH:MM): ");
    fgets(dueDate, sizeof(dueDate), stdin);
    trim_newline(dueDate);
    
    time_t due_time = 0;
    if (strlen(dueDate) > 0) {
        struct tm tm_time = {0};
        int year, month, day, hour, minute;
        
        if (sscanf(dueDate, "%d-%d-%d %d:%d", &year, &month, &day, &hour, &minute) == 5) {
            tm_time.tm_year = year - 1900;
            tm_time.tm_mon = month - 1;
            tm_time.tm_mday = day;
            tm_time.tm_hour = hour;
            tm_time.tm_min = minute;
            tm_time.tm_isdst = -1;
            
            due_time = mktime(&tm_time);
            printf("截止日期设置为: %s\n", dueDate);
        } else {
            printf("日期格式错误，截止时间将不设置。\n");
        }
    } else {
        printf("未设置截止日期。\n");
    }
    
    int id = list->count + 1;
    
    Task *task = createTask(id, title, description, due_time);
    if (task != NULL && addTask(list, task) == 0) {
        printf("任务添加成功！任务ID: %d\n", id);
        printf("按回车键继续...");
        getchar();
        return 0;
    } else {
        printf("任务添加失败！\n");
        printf("按回车键继续...");
        getchar();
        return -1;
    }
}

int deleteTaskUI(TaskList *list) {
    CLEAR_SCREEN();
    printf("删除任务\n");
    printf("-----------------------------------------\n");
    
    Task *curr = list->head;
    if(!curr) {
        printf("当前没有任务！\n");
        printf("按回车键返回...");
        getchar();
        return -1;
    }

    while(curr) {
        printf("[%d] %s\n", curr->id, curr->title);
        curr = curr->next;
    }
    printf("-----------------------------------------\n");
    
    printf("请输入要删除的任务ID: ");
    char input[20];
    fgets(input, sizeof(input), stdin);
    int id = atoi(input);
    
    Task *task = findTaskById(list, id);
    if (task == NULL) {
        printf("未找到ID为 %d 的任务！\n", id);
        printf("按回车键返回...");
        getchar();
        return -1;
    }
    
    printf("确定要删除以下任务？\n");
    printf("ID: %d\n", task->id);
    printf("标题: %s\n", task->title);
    printf("确定要删除吗？(y/n): ");
    
    char confirm[10];
    fgets(confirm, sizeof(confirm), stdin);
    
    if (confirm[0] == 'y' || confirm[0] == 'Y') {
        if (deleteTask(list, id) == 0) {
            printf("任务删除成功！\n");
        } else {
            printf("任务删除失败！\n");
        }
    } else {
        printf("已取消删除操作。\n");
    }
    
    printf("按回车键继续...");
    getchar();
    return 0;
}

int updateTaskUI(TaskList *list) {
    CLEAR_SCREEN();
    printf("更新任务\n");
    printf("-----------------------------------------\n");
    
    if (list->count == 0) {
        printf("当前没有任务！\n");
        printf("按回车键返回...");
        getchar();
        return -1;
    }
    
    printf("请输入要更新的任务ID: ");
    char input[20];
    fgets(input, sizeof(input), stdin);
    int id = atoi(input);
    
    Task *task = findTaskById(list, id);
    if (task == NULL) {
        printf("未找到ID为 %d 的任务！\n", id);
        printf("按回车键返回...");
        getchar();
        return -1;
    }
    
    printf("当前任务信息:\n");
    printf("ID: %d, 标题: %s\n", task->id, task->title);
    printf("状态: %s\n", statusToString(task->status));
    
    char title[MAX_TITLE_LENGTH];
    char description[MAX_DESCRIPTION_LENGTH];
    char dueDate[50];
    char statusInput[10];
    
    printf("请输入新的任务标题 (留空表示不修改): ");
    fgets(title, sizeof(title), stdin);
    trim_newline(title);
    
    char *newTitle = (strlen(title) > 0) ? title : task->title;
    
    printf("请输入新的任务描述 (留空表示不修改): ");
    fgets(description, sizeof(description), stdin);
    trim_newline(description);
    
    char *newDescription = (strlen(description) > 0) ? description : task->description;
    
    printf("请输入新的状态 (1-待办, 2-进行中, 3-已完成, 0-不修改): ");
    fgets(statusInput, sizeof(statusInput), stdin);
    int statusChoice = atoi(statusInput);
    
    TaskStatus newStatus = task->status;
    if (statusChoice == 1) newStatus = PENDING;
    else if (statusChoice == 2) newStatus = IN_PROGRESS;
    else if (statusChoice == 3) newStatus = COMPLETED;
    
    printf("请输入新的截止日期 (格式: YYYY-MM-DD HH:MM 或留空表示不修改): ");
    fgets(dueDate, sizeof(dueDate), stdin);
    trim_newline(dueDate);
    
    time_t newDueTime = task->due_time;
    if (strlen(dueDate) > 0) {
        struct tm tm_time = {0};
        int year, month, day, hour, minute;
        if (sscanf(dueDate, "%d-%d-%d %d:%d", &year, &month, &day, &hour, &minute) == 5) {
            tm_time.tm_year = year - 1900;
            tm_time.tm_mon = month - 1;
            tm_time.tm_mday = day;
            tm_time.tm_hour = hour;
            tm_time.tm_min = minute;
            tm_time.tm_isdst = -1;
            newDueTime = mktime(&tm_time);
            printf("截止日期设置为: %s\n", dueDate);
        } else {
            printf("日期格式错误，截止时间将保持不变。\n");
        }
    }
    
    if (updateTask(list, id, newTitle, newDescription, newStatus, newDueTime) == 0) {
        printf("任务更新成功！\n");
    } else {
        printf("任务更新失败！\n");
    }
    
    printf("按回车键继续...");
    getchar();
    return 0;
}

int markTaskStatusUI(TaskList *list) {
    CLEAR_SCREEN();
    printf("标记任务状态\n");
    printf("-----------------------------------------\n");
    
    if (list->count == 0) {
        printf("当前没有任务！\n");
        printf("按回车键返回...");
        getchar();
        return -1;
    }
    
    printf("请输入要标记的任务ID: ");
    char input[20];
    fgets(input, sizeof(input), stdin);
    int id = atoi(input);
    
    Task *task = findTaskById(list, id);
    if (task == NULL) {
        printf("未找到ID为 %d 的任务！\n", id);
        printf("按回车键返回...");
        getchar();
        return -1;
    }
    
    printf("任务信息:\n");
    printf("ID: %d, 标题: %s\n", task->id, task->title);
    printf("当前状态: %s\n", statusToString(task->status));
    
    printf("请选择新的状态:\n");
    printf("1. 待办\n");
    printf("2. 进行中\n");
    printf("3. 已完成\n");
    printf("请输入选择: ");
    
    char statusInput[10];
    fgets(statusInput, sizeof(statusInput), stdin);
    int statusChoice = atoi(statusInput);
    
    TaskStatus newStatus;
    switch (statusChoice) {
        case 1: newStatus = PENDING; break;
        case 2: newStatus = IN_PROGRESS; break;
        case 3: newStatus = COMPLETED; break;
        default:
            printf("无效的选择！\n");
            printf("按回车键返回...");
            getchar();
            return -1;
    }
    
    if (markTaskStatus(list, id, newStatus) == 0) {
        printf("任务状态更新成功！\n");
    } else {
        printf("任务状态更新失败！\n");
    }
    
    printf("按回车键继续...");
    getchar();
    return 0;
}

Task* searchTaskUI(TaskList *list) {
    CLEAR_SCREEN();
    printf("搜索任务\n");
    printf("-----------------------------------------\n");
    
    printf("请选择搜索方式:\n");
    printf("1. 根据标题搜索\n");
    printf("2. 根据状态过滤\n");
    printf("请输入选择 (1-2): ");
    
    char option[10];
    fgets(option, sizeof(option), stdin);
    int choice = atoi(option);
    
    TaskList *resultList = NULL;
    
    switch (choice) {
        case 1: {
            char keyword[MAX_TITLE_LENGTH];
            printf("请输入搜索关键词: ");
            fgets(keyword, sizeof(keyword), stdin);
            trim_newline(keyword);
            
            resultList = searchTaskByTitle(list, keyword);
            break;
        }
        case 2: {
            printf("请选择要过滤的状态:\n");
            printf("1. 待办\n");
            printf("2. 进行中\n");
            printf("3. 已完成\n");
            printf("请输入选择 (1-3): ");
            
            char statusInput[10];
            fgets(statusInput, sizeof(statusInput), stdin);
            int statusChoice = atoi(statusInput);
            
            TaskStatus status;
            switch (statusChoice) {
                case 1: status = PENDING; break;
                case 2: status = IN_PROGRESS; break;
                case 3: status = COMPLETED; break;
                default:
                    printf("无效的选择！\n");
                    printf("按回车键返回...");
                    getchar();
                    return NULL;
            }
            resultList = filterTaskByStatus(list, status);
            break;
        }
        default:
            printf("无效的选择！\n");
            printf("按回车键返回...");
            getchar();
            return NULL;
    }
    
    CLEAR_SCREEN();
    printf("搜索结果\n");
    printf("-----------------------------------------\n");
    
    if (resultList != NULL) {
        if (resultList->count == 0) {
            printf("没有找到匹配的任务！\n");
        } else {
            printf("共找到 %d 个匹配的任务:\n", resultList->count);
            Task *current = resultList->head;
            while (current != NULL) {
                printf("ID: %d, 标题: %s\n", current->id, current->title);
                printf("  状态: %s\n", statusToString(current->status));
                current = current->next;
            }
        }
        freeTaskList(resultList);
    }
    
    printf("按回车键返回...");
    getchar();
    return NULL;
}

void importTasksFromCSVUI(TaskList *list) {
    CLEAR_SCREEN();
    printf("导入任务\n");
    printf("-----------------------------------------\n");
    
    printf("请输入CSV文件路径 (留空使用默认路径): ");
    
    char filename[256];
    fgets(filename, sizeof(filename), stdin);
    trim_newline(filename);
    
    const char *importFilename = (strlen(filename) > 0) ? filename : "import_tasks.csv";
    
    printf("正在从 %s 导入任务...\n", importFilename);
    
    TaskList *importedList = importTasksFromCSV(importFilename);
    
    if (importedList != NULL) {
        int importedCount = 0;
        Task *current = importedList->head;
        while (current != NULL) {
            Task *nextTask = current->next;
            
            int newId = list->count + importedCount + 1;
            current->id = newId;
            
            addTask(list, current);
            importedCount++;
            
            current = nextTask;
        }
        
        free(importedList);
        printf("成功导入 %d 个任务！\n", importedCount);
    } else {
        printf("导入失败，请检查文件是否存在并且格式正确。\n");
    }
    
    printf("当前任务总数: %d\n", list->count);
    printf("按回车键返回...");
    getchar();
}

void exportTasksToCSVUI(TaskList *list) {
    CLEAR_SCREEN();
    printf("导出任务\n");
    printf("-----------------------------------------\n");
    
    if (list == NULL || list->count == 0) {
        printf("当前没有任务可导出！\n");
        printf("按回车键返回...");
        getchar();
        return;
    }
    
    printf("请输入导出文件路径 (留空使用默认路径): ");
    
    char filename[256];
    fgets(filename, sizeof(filename), stdin);
    trim_newline(filename);
    
    const char *exportFilename = (strlen(filename) > 0) ? filename : "export_tasks.csv";
    
    printf("正在将任务导出到 %s...\n", exportFilename);
    
    int result = exportTasksToCSV(list, exportFilename);
    
    if (result == 0) {
        printf("成功导出 %d 个任务到 %s！\n", list->count, exportFilename);
    } else {
        printf("导出失败，请检查文件路径是否正确。\n");
    }
    
    printf("按回车键返回...");
    getchar();
}
