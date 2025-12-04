#ifndef UI_H
#define UI_H

#include "todolist.h"

// 主界面函数（A同学负责）
// 显示欢迎界面
void showWelcomeScreen();

// 显示主菜单
void showMainMenu();

// 处理用户输入的命令
int handleUserCommand(char *command, TaskList *list);

// 运行程序主循环
void runMainLoop(TaskList *list);

// 菜单交互函数（B同学负责）
// 显示任务列表
void showTaskList(TaskList *list);

// 显示任务详情
void showTaskDetail(Task *task);

// 添加任务的交互界面
int addTaskUI(TaskList *list);

// 删除任务的交互界面
int deleteTaskUI(TaskList *list);

// 更新任务的交互界面
int updateTaskUI(TaskList *list);

// 标记任务状态的交互界面
int markTaskStatusUI(TaskList *list);

// 查找任务的交互界面
Task* searchTaskUI(TaskList *list);

// 显示任务统计
void showTaskStatistics(TaskList *list);

// 导入任务的交互界面
void importTasksFromCSVUI(TaskList *list);

#endif // UI_H