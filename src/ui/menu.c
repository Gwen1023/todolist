// menu.c
#include "ui.h"
#include "todolist.h"
#include "backend.h"
#include "storage.h"
#include <stdio.h>
#include <string.h>

// 把表示状态的数字转化为字符串
const char* statusToString(TaskStatus status) {
    switch (status) {
        case PENDING:
            return "待办";
        case IN_PROGRESS:
            return "进行中";
        case COMPLETED:
            return "已完成";
        default:
            return "未知";
    }
}

// 显示任务列表
void showTaskList(TaskList *list) {
    system("cls");  
    printf("任务列表\n");
    printf("-----------------------------------------\n");
    
    // 检查链表是否为空
    if (list == NULL || list->head == NULL) {
        printf("当前没有任务！\n");
        printf("按回车键返回\n");
        getchar();
        return;
    }
    
    // 绘制表格
    printf("ID  |  任务标题              |  状态    \n");
    printf("-----------------------------------------\n");
    
    Task *current = list->head;
    while (current != NULL) {
        printf("%2d  |  %-22s  |  %s\n", current->id, current->title, statusToString(current->status));
        current = current->next;
    }
    
    printf("-----------------------------------------\n");
    printf("任务总数: %d\n", list->count);
    
    // 询问用户是否要查看某个任务的详细信息
    printf("输入任务ID查看详情，或按回车返回: ");
    
    char input[10];
    fgets(input, sizeof(input), stdin);
    
    // 如果用户输入了ID，显示任务详情
    int id = atoi(input);
    if (id > 0) {
        Task *task = findTaskById(list, id);
        if (task != NULL) {
            showTaskDetail(task);
        } else {
            printf("未找到ID为 %d 的任务！\n", id);
            getchar();
        }
    }
}

// 显示任务详情
void showTaskDetail(Task *task) {
    if (task == NULL) {
        return;
    }
    
    system("cls");  
    
    printf("任务详情\n");
    printf("-----------------------------------------\n");
    printf("任务ID: %d\n", task->id);
    printf("任务标题: %s\n", task->title);
    printf("任务描述: %s\n", task->description);
    printf("任务状态: %s\n", statusToString(task->status));
    
    // 格式化显示创建时间
    char createTimeStr[100];
    strftime(createTimeStr, sizeof(createTimeStr), "%Y-%m-%d %H:%M:%S", localtime(&task->create_time));
    printf("创建时间: %s\n", createTimeStr);
    
    // 格式化显示截止时间
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

// 添加任务的交互界面
int addTaskUI(TaskList *list) {
    system("cls");  
    
    printf("添加新任务\n");
    printf("-----------------------------------------\n");
    
    char title[MAX_TITLE_LENGTH];
    char description[MAX_DESCRIPTION_LENGTH];
    char dueDate[20];
    
    // 任务标题输入提示
    printf("请输入任务标题: ");
    fflush(stdin);  // 清空输入缓冲区
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = '\0';
    
    // 任务描述输入提示
    printf("请输入任务描述: ");
    fflush(stdin);  // 清空输入缓冲区
    fgets(description, sizeof(description), stdin);
    description[strcspn(description, "\n")] = '\0';
    
    // 任务截止日期输入提示
    printf("请输入截止日期(YYYY-MM-DD HH:MM): ");
    fflush(stdin);  // 清空输入缓冲区
    fgets(dueDate, sizeof(dueDate), stdin);
    dueDate[strcspn(dueDate, "\n")] = '\0';
    
    // 解析截止时间
    time_t due_time = 0;
    if (strlen(dueDate) > 0) {
        struct tm tm_time;
        if (strptime(dueDate, "%Y-%m-%d %H:%M", &tm_time) != NULL) {
            due_time = mktime(&tm_time);
            printf("截止日期设置为: %s\n", dueDate);
        } else {
            printf("日期格式错误，截止时间将不设置。\n");
        }
    } else {
        printf("未设置截止日期。\n");
    }
    
    // 生成任务ID
    int id = list->count + 1;
    
    // 创建并添加任务
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

// 删除任务的交互界面
int deleteTaskUI(TaskList *list) {
    system("cls");  // 清屏
    
    // 简化为纯文本界面
    printf("删除任务\n");
    printf("-----------------------------------------\n");
    
    // 先显示任务列表
    showTaskList(list);
    
    // 如果没有任务，直接返回
    if (list->count == 0) {
        printf("当前没有任务！\n");
        printf("按回车键返回...");
        getchar();
        return -1;
    }
    
    // 获取要删除的任务ID
    printf("请输入要删除的任务ID: ");
    
    char input[10];
    fgets(input, sizeof(input), stdin);
    int id = atoi(input);
    
    // 查找任务
    Task *task = findTaskById(list, id);
    if (task == NULL) {
        printf("未找到ID为 %d 的任务！\n", id);
        printf("按回车键返回...");
        getchar();
        return -1;
    }
    
    // 再次确认删除
    printf("确定要删除以下任务？\n");
    printf("ID: %d\n", task->id);
    printf("标题: %s\n", task->title);
    printf("确定要删除任务 '%s' 吗？(y/n): ", task->title);
    
    char confirm[2];
    fgets(confirm, sizeof(confirm), stdin);
    
    if (confirm[0] == 'y' || confirm[0] == 'Y') {
        if (deleteTask(list, id) == 0) {
            printf("任务删除成功！\n");
            printf("按回车键继续...");
            getchar();
            return 0;
        } else {
            printf("任务删除失败！\n");
            printf("按回车键继续...");
            getchar();
            return -1;
        }
    } else {
        printf("已取消删除操作。\n");
        printf("按回车键继续...");
        getchar();
        return 0;
    }
}

// 更新任务的交互界面
int updateTaskUI(TaskList *list) {
    system("cls");  
    printf("更新任务\n");
    printf("-----------------------------------------\n");
    
    // 如果没有任务，直接返回
    if (list->count == 0) {
        printf("当前没有任务！\n");
        printf("按回车键返回...");
        getchar();
        return -1;
    }
    
    // 获取要更新的任务ID
    printf("请输入要更新的任务ID: ");
    
    char input[10];
    fgets(input, sizeof(input), stdin);
    int id = atoi(input);
    
    // 查找任务
    Task *task = findTaskById(list, id);
    if (task == NULL) {
        printf("未找到ID为 %d 的任务！\n", id);
        printf("按回车键返回...");
        getchar();
        return -1;
    }
    
    // 显示当前任务信息
    printf("当前任务信息:\n");
    printf("ID: %d, 标题: %s\n", task->id, task->title);
    printf("状态: %s\n", statusToString(task->status));
    
    char title[MAX_TITLE_LENGTH];
    char description[MAX_DESCRIPTION_LENGTH];
    char dueDate[20];
    char statusInput[2];
    
    // 获取新的任务标题
    printf("请输入新的任务标题 (留空表示不修改): ");
    fflush(stdin);  // 清空输入缓冲区
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = '\0'; // 去除末尾的换行符
    
    // 如果留空，保持原标题
    char *newTitle = (strlen(title) > 0) ? title : task->title;
    
    // 获取新的任务描述（留空表示不修改）
    printf("请输入新的任务描述 (留空表示不修改): ");
    fflush(stdin);  
    fgets(description, sizeof(description), stdin);
    description[strcspn(description, "\n")] = '\0';
    
    // 如果留空，保持原描述
    char *newDescription = (strlen(description) > 0) ? description : task->description;
    
    // 获取新的状态
    printf("请输入新的状态 (1-待办, 2-进行中, 3-已完成, 0-不修改): ");
    fgets(statusInput, sizeof(statusInput), stdin);
    int statusChoice = atoi(statusInput);
    
    // 设置新状态
    TaskStatus newStatus = task->status;  // 默认为原状态
    if (statusChoice == 1) {
        newStatus = PENDING;
    } else if (statusChoice == 2) {
        newStatus = IN_PROGRESS;
    } else if (statusChoice == 3) {
        newStatus = COMPLETED;
    }
    
    // 获取新的截止时间
    printf("请输入新的截止日期 (格式: YYYY-MM-DD HH:MM 或留空表示不修改): ");
    fflush(stdin);  
    fgets(dueDate, sizeof(dueDate), stdin);
    // 去除末尾的换行符
    dueDate[strcspn(dueDate, "\n")] = '\0';
    
    // 解析新的截止时间
    time_t newDueTime = task->due_time;  // 默认为原截止时间
    if (strlen(dueDate) > 0) {
        struct tm tm_time;
        if (strptime(dueDate, "%Y-%m-%d %H:%M", &tm_time) != NULL) {
            newDueTime = mktime(&tm_time);
            printf("截止日期设置为: %s\n", dueDate);
        } else {
            printf("日期格式错误，截止时间将保持不变。\n");
        }
    } else {
        printf("未修改截止日期。\n");
    }
    
    // 更新任务
    if (updateTask(list, id, newTitle, newDescription, newStatus, newDueTime) == 0) {
        printf("任务更新成功！\n");
    } else {
        printf("任务更新失败！\n");
    }
    
    printf("按回车键继续...");
    getchar();
    return 0;
}

// 标记任务状态的交互界面
int markTaskStatusUI(TaskList *list) {
    system("cls");  
    printf("标记任务状态\n");
    printf("-----------------------------------------\n");
    
    // 如果没有任务，直接返回
    if (list->count == 0) {
        printf("当前没有任务！\n");
        printf("按回车键返回...");
        getchar();
        return -1;
    }
    
    // 获取要标记的任务ID
    printf("请输入要标记的任务ID: ");
    char input[10];
    fgets(input, sizeof(input), stdin);
    int id = atoi(input);
    
    // 查找任务
    Task *task = findTaskById(list, id);
    if (task == NULL) {
        printf("未找到ID为 %d 的任务！\n", id);
        printf("按回车键返回...");
        getchar();
        return -1;
    }
    
    // 显示当前任务信息
    printf("任务信息:\n");
    printf("ID: %d, 标题: %s\n", task->id, task->title);
    printf("当前状态: %s\n", statusToString(task->status));
    
    // 显示任务状态选择
    printf("请选择新的状态:\n");
    printf("1. 待办\n");
    printf("2. 进行中\n");
    printf("3. 已完成\n");
    printf("请输入选择: ");
    char statusInput[2];
    fgets(statusInput, sizeof(statusInput), stdin);
    int statusChoice = atoi(statusInput);
    
    // 设置新状态
    TaskStatus newStatus;
    switch (statusChoice) {
        case 1:
            newStatus = PENDING;
            break;
        case 2:
            newStatus = IN_PROGRESS;
            break;
        case 3:
            newStatus = COMPLETED;
            break;
        default:
            printf("无效的选择！\n");
            printf("按回车键返回...");
            getchar();
            return -1;
    }
    
    // 更新任务状态
    if (markTaskStatus(list, id, newStatus) == 0) {
        printf("任务状态更新成功！\n");
    } else {
        printf("任务状态更新失败！\n");
    }
    
    printf("按回车键继续...");
    getchar();
    return 0;
}

// 查找任务的交互界面
Task* searchTaskUI(TaskList *list) {
    system("cls");  
    printf("搜索任务\n");
    printf("-----------------------------------------\n");
    
    printf("请选择搜索方式:\n");
    printf("1. 根据标题搜索\n");
    printf("2. 根据状态过滤\n");
    printf("请输入选择 (1-2): ");
    
    char option[2];
    fgets(option, sizeof(option), stdin);
    int choice = atoi(option);
    
    TaskList *resultList = NULL;
    
    switch (choice) {
        case 1:  // 根据标题搜索
            {
                char keyword[MAX_TITLE_LENGTH];
                printf("请输入搜索关键词: ");
                fflush(stdin);  
                fgets(keyword, sizeof(keyword), stdin);
                keyword[strcspn(keyword, "\n")] = '\0'; // 去除末尾的换行符
                resultList = searchTaskByTitle(list, keyword);
            }
            break;
            
        case 2:  // 根据状态过滤
            {
                printf("请选择要过滤的状态:\n");
                printf("1. 待办\n");
                printf("2. 进行中\n");
                printf("3. 已完成\n");
                printf("请输入选择 (1-3): ");
                
                char statusInput[2];
                fgets(statusInput, sizeof(statusInput), stdin);
                int statusChoice = atoi(statusInput);
                
                TaskStatus status;
                switch (statusChoice) {
                    case 1:
                        status = PENDING;
                        break;
                    case 2:
                        status = IN_PROGRESS;
                        break;
                    case 3:
                        status = COMPLETED;
                        break;
                    default:
                        printf("无效的选择！\n");
                        printf("按回车键返回...");
                        getchar();
                        return NULL;
                }
                
                resultList = filterTaskByStatus(list, status);
            }
            break;
            
        default:
            printf("无效的选择！\n");
            printf("按回车键返回...");
            getchar();
            return NULL;
    }
    
    // 显示搜索结果
    system("cls");
    printf("搜索结果\n");
    printf("-----------------------------------------\n");
    
    if (resultList != NULL) {
        if (resultList->count == 0) {
            printf("没有找到匹配的任务！\n");
        } else {
            printf("共找到 %d 个匹配的任务:\n", resultList->count);
            
            // 显示结果列表
            Task *current = resultList->head;
            while (current != NULL) {
                printf("ID: %d, 标题: %s\n", current->id, current->title);
                printf("  状态: %s\n", statusToString(current->status));
                current = current->next;
            }
        }
        
        // 释放结果列表
        freeTaskList(resultList);
    }
    
    printf("按回车键返回...");
    getchar();
    return NULL;
}

// 导入任务的交互界面
void importTasksFromCSVUI(TaskList *list) {
    system("cls");      
    printf("导入任务\n");
    printf("-----------------------------------------\n");
    
    // 获取CSV文件路径
    printf("请输入CSV文件路径 (留空使用默认路径): ");
    
    char filename[256];
    fflush(stdin);  
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';
    
    // 如果留空，使用默认文件名
    const char *importFilename = (strlen(filename) > 0) ? filename : "import_tasks.csv";

    printf("正在从 %s 导入任务...", importFilename);
    
    // 导入任务
    TaskList *importedList = importTasksFromCSV(importFilename);
    
    // 清空之前的提示并显示导入结果
    printf("\r");
    for (int i = 0; i < 50; i++) {
        printf(" ");
    }
    printf("\r");
    
    if (importedList != NULL) {
        // 将导入的任务添加到当前列表
        int importedCount = importedList->count;
        Task *current = importedList->head;
        while (current != NULL) {
            int newId = list->count + 1;
            current->id = newId;            
            addTask(list, current);            
            current = current->next;
        }
        
        printf("成功导入 %d 个任务！\n", importedCount);        
        free(importedList);
    } else {
        printf("导入失败，请检查文件是否存在并且格式正确。\n");
    }
    
    // 显示当前任务总数
    printf("当前任务总数: %d\n", list->count);    
    printf("按回车键返回...");
    getchar();
}

// 导出任务的交互界面
void exportTasksToCSVUI(TaskList *list) {
    system("cls");      
    printf("导出任务\n");
    printf("-----------------------------------------\n");
    
    // 检查是否有任务可导出
    if (list == NULL || list->count == 0) {
        printf("当前没有任务可导出！\n");        
        printf("按回车键返回...");
        getchar();
        return;
    }
    
    // 获取CSV文件路径
    printf("请输入导出文件路径 (留空使用默认路径): ");
    
    char filename[256];
    fflush(stdin);  
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';
    
    // 如果留空，使用默认文件名
    const char *exportFilename = (strlen(filename) > 0) ? filename : "export_tasks.csv";
    
    printf("正在将任务导出到 %s...", exportFilename);
    
    // 导出任务
    int result = exportTasksToCSV(list, exportFilename);
    
    // 清空之前的提示并显示导出结果
    printf("\r");
    for (int i = 0; i < 50; i++) {
        printf(" ");
    }
    printf("\r");
    
    if (result == 0) {
        printf("成功导出 %d 个任务到 %s！\n", list->count, exportFilename);
    } else {
        printf("导出失败，请检查文件路径是否正确。\n");
    }
    
    // 显示导出提示信息
    printf("导出文件位置: %s\n", exportFilename);    
    printf("按回车键返回...");
    getchar();
}