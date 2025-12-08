#define _CRT_SECURE_NO_WARNINGS
#include "ui.h"
#include "todolist.h"
#include "backend.h"
#include "storage.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//该函数用于打印todolist项目的欢迎界面
void showWelcomeScreen(){
    //清理屏幕信息
    system("cls");

    printf("**********************************\n");
    printf("**********   TODOLIST   **********\n");
    printf("__________________________________\n");
    printf("  *欢迎使用您的专属任务管理系统*  \n");
}




// 显示任务统计信息
void showTaskStatistics(TaskList *list) {
    system("cls");
    
    // 显示标题
    printf("\n\n\t\t==========================================\n");
    printf("\t\t               任务统计信息\n");
    printf("\t\t==========================================\n\n");
    
    // 获取统计数据
    int total = 0, pending = 0, inProgress = 0, completed = 0;
    getTaskStatistics(list, &total, &pending, &inProgress, &completed);
    
    // 显示统计数据
    printf("\t\t任务总数:    %d\n", total);
    printf("\t\t待处理任务:  %d\n", pending);
    printf("\t\t进行中任务:  %d\n", inProgress);
    printf("\t\t已完成任务:  %d\n", completed);
    
    // 计算完成率
    printf("\t\t完成率:      ");
    if (total > 0) {
        float completionRate = (float)completed / total * 100;
        printf("%.1f%%\n", completionRate);
    } else {
        printf("0.0%%\n");
    }
    
    // 底部提示
    printf("\n\t\t        按回车键返回");
    
    getchar();
}

//该函数用于打印todolist的主菜单，使用者可以根据需要输入不同功能对应的数字进行使用
void showMainMenu(){
    system("cls");  // 清屏

    printf ("＝＝＝＝＝=＝＝主菜单＝＝=＝＝＝＝＝\n");
    printf ("1.查看所有任务　　　    2.创建新任务\n");
    printf ("3.删除任务             4.编辑任务\n");
    printf ("5.标记任务状态          6.显示任务统计\n");
    printf ("7.搜索任务　　　　      8.导出任务(CSV格式)\n");
    printf ("9.导入任务(CSV格式)    10. 保存并退出\n");
    printf ("        0.退出Todolist\n");
    printf ("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝\n");
    printf("请输入对应数字选择功能：");
}
// 处理用户输入的命令
int handleUserCommand(char *command, TaskList *list) {
    // 转换命令为整数
    int choice = atoi(command);
    
    // 根据用户选择执行相应操作
    switch (choice) {
        case 1:  // 查看所有任务
            showTaskList(TaskList *list);
            break;
            
        case 2:  // 创建新任务
            addTaskUI(TaskList *list);
            break;
            
            
        case 3:  // 删除任务
            deleteTaskUI(TaskList *list);
            break;
            
        case 4:  // 编辑任务
            updateTaskUI(TaskList *list);
            break;
            
        case 5:  // 标记任务状态
            markTaskStatusUI(TaskList *list);
            break;
            
        case 6:  // 显示任务统计
            showTaskStatistics(list);
            break;
            
        case 7:  // 搜索任务
            searchTaskUI(TaskList *list);
            break;
        
        case 8:  // 导入任务
            importTasksFromCSVUI(list);
            break; 

        case 9:  // 导出任务
            exportTasksToCSVUI(list);
            break;    

        case 10:  // 保存并退出
            saveTasksToFile(list, DEFAULT_FILENAME);
            printf("\n\t\t        ✓ 任务已保存！");
            return 0;  // 表示退出
            
        case 0:  // 退出程序
            return 0;  // 表示退出
            
        default:  // 无效命令
            printf("\n\t\t        ✗ 无效的命令，请重新输入！");
            break;
    }
    
    return 1;  // 表示继续运行
}

// 运行程序主循环
void runMainLoop(TaskList *list) {
    char command[10];  // 存储用户输入的命令
    int continueRunning = 1;  // 控制是否继续运行
    
    // 先尝试加载保存的任务
    TaskList *loadedList = loadTasksFromFile(DEFAULT_FILENAME);
    if (loadedList != NULL) {
        // 清空当前列表
        clearTaskList(list);
        
        // 将加载的任务复制到当前列表
        Task *current = loadedList->head;
        while (current != NULL) {
            Task *copyTask = createTask(
                current->id,
                current->title,
                current->description,
                current->due_time
            );
            if (copyTask != NULL) {
                copyTask->status = current->status;
                copyTask->create_time = current->create_time;
                addTask(list, copyTask);
            }
            current = current->next;
        }
        
        // 释放加载的列表（因为数据已经复制）
        freeTaskList(loadedList);
        
        printf("已加载保存的任务！\n");
    }
    
    // 主循环
    while (continueRunning) {
        // 显示主菜单
        showMainMenu();
        
        // 获取用户输入
        fgets(command, sizeof(command), stdin);
        
        // 处理用户命令
        continueRunning = handleUserCommand(command, list);
        
        // 如果不退出，等待用户按回车继续
        if (continueRunning) {
            printf("\n按回车键继续...");
            while (getchar() != '\n');  // 清空输入缓冲区
        }
    }
}

int main(){
    showWelcomeScreen();
    showMainMenu();
    handleUserCommand(char *command, TaskList *list);
    runMainLoop(TaskList *list);
    return 0;
}