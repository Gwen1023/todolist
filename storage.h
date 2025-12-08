#ifndef STORAGE_H
#define STORAGE_H

#include "todolist.h"

// 数据存储函数（F同学负责）
// 保存任务列表到文件（二进制格式）
int saveTasksToFile(TaskList *list, const char *filename);

// 从文件加载任务列表（二进制格式）
TaskList* loadTasksFromFile(const char *filename);

// 导出任务到CSV格式
int exportTasksToCSV(TaskList *list, const char *filename);

// 从CSV格式导入任务
TaskList* importTasksFromCSV(const char *filename);

#endif // STORAGE_H