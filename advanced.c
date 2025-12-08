// D同学负责的高级功能实现
#include "todolist.h"
#include "backend.h"
#include <string.h>
#include <time.h>

// 根据标题搜索任务
TaskList* searchTaskByTitle(TaskList *list, const char *keyword) {
    // 参数检查
    if (list == NULL || keyword == NULL || strlen(keyword) == 0) {
        return NULL;
    }
    
    // 创建一个新的链表来存储搜索结果
    TaskList *resultList = initTaskList();
    if (resultList == NULL) {
        return NULL;
    }
    
    // 遍历原始链表
    Task *current = list->head;
    while (current != NULL) {
        // 在标题中查找关键词（不区分大小写）
        if (strstr(current->title, keyword) != NULL) {
            // 找到匹配任务，创建副本并添加到结果链表
            Task *copyTask = createTask(
                current->id,
                current->title,
                current->description,
                current->due_time
            );
            if (copyTask != NULL) {
                copyTask->status = current->status;
                copyTask->create_time = current->create_time;
                addTask(resultList, copyTask);
            }
        }
        current = current->next;
    }
    
    return resultList;
}


// 根据状态过滤任务
TaskList* filterTaskByStatus(TaskList *list, TaskStatus status) {
    // 参数检查
    if (list == NULL) {
        return NULL;
    }
    
    // 创建一个新的链表来存储过滤结果
    TaskList *resultList = initTaskList();
    if (resultList == NULL) {
        return NULL;
    }
    
    // 遍历原始链表
    Task *current = list->head;
    while (current != NULL) {
        // 检查任务状态是否匹配
        if (current->status == status) {
            // 找到匹配任务，创建副本并添加到结果链表
            Task *copyTask = createTask(
                current->id,
                current->title,
                current->description,
                current->due_time
            );
            if (copyTask != NULL) {
                copyTask->status = current->status;
                copyTask->create_time = current->create_time;
                addTask(resultList, copyTask);
            }
        }
        current = current->next;
    }
    
    return resultList;
}

// 交换两个任务节点的数据（保持链表结构不变）
static void swapTaskData(Task *a, Task *b) {
    if (a == NULL || b == NULL) return;
    
    // 保存a的next指针（临时存储）
    Task *temp_next = a->next;
    
    // 交换除next指针外的所有数据
    // 使用临时变量存储a的数据
    Task temp_task = *a;
    
    // 将b的数据复制到a
    a->id = b->id;
    strncpy(a->title, b->title, MAX_TITLE_LENGTH);
    strncpy(a->description, b->description, MAX_DESCRIPTION_LENGTH);
    a->status = b->status;
    a->create_time = b->create_time;
    a->due_time = b->due_time;
    
    // 将临时存储的a数据复制到b
    b->id = temp_task.id;
    strncpy(b->title, temp_task.title, MAX_TITLE_LENGTH);
    strncpy(b->description, temp_task.description, MAX_DESCRIPTION_LENGTH);
    b->status = temp_task.status;
    b->create_time = temp_task.create_time;
    b->due_time = temp_task.due_time;
    
    // 恢复正确的next指针
    a->next = temp_next;
    // b的next指针已经在原链表中正确指向下一个节点，不需要修改
}

// 按创建时间排序任务（使用冒泡排序）
void sortTaskByCreateTime(TaskList *list, int ascending) {
    // 参数检查
    if (list == NULL || list->head == NULL || list->count <= 1) {
        return;
    }
    
    int swapped;
    Task *current;
    Task *last = NULL;
    
    // 冒泡排序
    do {
        swapped = 0;
        current = list->head;
        
        while (current != NULL && current->next != last) {
            int shouldSwap = 0;
            
            if (ascending) {
                // 升序：创建时间早的在前
                shouldSwap = (current->create_time > current->next->create_time);
            } else {
                // 降序：创建时间晚的在前
                shouldSwap = (current->create_time < current->next->create_time);
            }
            
            if (shouldSwap) {
                // 交换节点数据（不交换指针）
                swapTaskData(current, current->next);
                swapped = 1;
            }
            
            current = current->next;
        }
        last = current;
    } while (swapped);
}

// 按截止时间排序任务（使用冒泡排序）
void sortTaskByDueTime(TaskList *list, int ascending) {
    // 参数检查
    if (list == NULL || list->head == NULL || list->count <= 1) {
        return;
    }
    
    int swapped;
    Task *current;
    Task *last = NULL;
    
    // 冒泡排序
    do {
        swapped = 0;
        current = list->head;
        
        while (current != NULL && current->next != last) {
            int shouldSwap = 0;
            
            if (ascending) {
                // 升序：截止时间早的在前
                shouldSwap = (current->due_time > current->next->due_time);
            } else {
                // 降序：截止时间晚的在前
                shouldSwap = (current->due_time < current->next->due_time);
            }
            
            if (shouldSwap) {
                // 交换节点数据（不交换指针）
                swapTaskData(current, current->next);
                swapped = 1;
            }
            
            current = current->next;
        }
        last = current;
    } while (swapped);
}


// 获取任务统计信息
void getTaskStatistics(TaskList *list, int *total, int *pending, int *inProgress, int *completed) {
    // 参数检查和初始化
    if (total != NULL) *total = 0;
    if (pending != NULL) *pending = 0;
    if (inProgress != NULL) *inProgress = 0;
    if (completed != NULL) *completed = 0;
    
    if (list == NULL || list->head == NULL) {
        return;
    }
    
    // 遍历链表统计任务状态
    Task *current = list->head;
    while (current != NULL) {
        if (total != NULL) (*total)++;
        
        // 统计各状态任务数量
        switch (current->status) {
            case PENDING:
                if (pending != NULL) (*pending)++;
                break;
            case IN_PROGRESS:
                if (inProgress != NULL) (*inProgress)++;
                break;
            case COMPLETED:
                if (completed != NULL) (*completed)++;
                break;
        }
        
        current = current->next;
    }
}
// 检查即将到期的任务
TaskList* getUpcomingTasks(TaskList *list, int days) {
    // 参数检查
    if (list == NULL) {
        return NULL;
    }
    
    // 创建结果链表
    TaskList *resultList = initTaskList();
    if (resultList == NULL) {
        return NULL;
    }
    
    // 计算截止时间阈值（当前时间 + 天数）
    time_t now;
    time(&now);
    time_t threshold = now + (days * 24 * 60 * 60);  // 转换为秒数
    
    // 遍历链表
    Task *current = list->head;
    while (current != NULL) {
        // 找出状态为待办或进行中，且截止时间在阈值内的任务
        if ((current->status == PENDING || current->status == IN_PROGRESS) && 
            current->due_time != 0 && current->due_time <= threshold) {
            
            // 创建副本并添加到结果链表
            Task *copyTask = createTask(
                current->id,
                current->title,
                current->description,
                current->due_time
            );
            if (copyTask != NULL) {
                copyTask->status = current->status;
                copyTask->create_time = current->create_time;
                addTask(resultList, copyTask);
            }
        }
        current = current->next;
    }
    
    return resultList;
}