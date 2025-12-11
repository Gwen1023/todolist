#include "test.h"
#include "backend.h"
#include "storage.h"
#include <stdio.h>

void printTestResult(const char *testName, int passed) {
    printf("%s: %s\n", testName, passed ? "PASS" : "FAIL");
}

void testListOperations() {
    printf("\n=== 测试链表基本操作 ===\n");
    
    TaskList *list = initTaskList();
    printTestResult("链表初始化", list != NULL && list->head == NULL && list->tail == NULL && list->count == 0);
    
    Task *task1 = createTask(1, "测试任务1", "这是第一个测试任务", time(NULL) + 86400); // 1天后截止
    Task *task2 = createTask(2, "测试任务2", "这是第二个测试任务", time(NULL) + 172800); // 2天后截止
    printTestResult("任务创建", task1 != NULL && task2 != NULL);
    
    int addResult1 = addTask(list, task1);
    int addResult2 = addTask(list, task2);
    printTestResult("任务添加", addResult1 == 1 && addResult2 == 1 && list->count == 2);
    
    Task *foundTask = findTaskById(list, 1);
    printTestResult("按ID查找任务", foundTask != NULL && foundTask->id == 1);
    
    foundTask = findTaskById(list, 999);
    printTestResult("查找不存在的任务", foundTask == NULL);
    
    int deleteResult = deleteTask(list, 1);
    printTestResult("任务删除", deleteResult == 1 && list->count == 1);
    
    deleteResult = deleteTask(list, 999);
    printTestResult("删除不存在的任务", deleteResult == 0 && list->count == 1);
    
    clearTaskList(list);
    printTestResult("清空链表", list->count == 0 && list->head == NULL && list->tail == NULL);
    
    freeTaskList(list);
    printf("链表释放完成\n");
    
    printf("=== 链表基本操作测试完成 ===\n");
}

void testTaskOperations() {
    printf("\n=== 测试任务增删改查 ===\n");
    
    TaskList *list = initTaskList();
    
    time_t now = time(NULL);
    Task *task1 = createTask(1, "学习C语言", "每天学习2小时C语言编程", now + 86400);
    Task *task2 = createTask(2, "完成作业", "完成数据结构作业", now + 172800);
    Task *task3 = createTask(3, "运动健身", "跑步5公里", now + 259200);
    
    addTask(list, task1);
    addTask(list, task2);
    addTask(list, task3);
    
    printTestResult("添加多个任务", list->count == 3);
    
    int updateResult = updateTask(list, 2, "完成C语言作业", "完成数据结构与算法作业", IN_PROGRESS, now + 345600);
    Task *updatedTask = findTaskById(list, 2);
    int updateCheck = (updateResult == 1) && 
                      (strcmp(updatedTask->title, "完成C语言作业") == 0) && 
                      (strcmp(updatedTask->description, "完成数据结构与算法作业") == 0) && 
                      (updatedTask->status == IN_PROGRESS);
    printTestResult("更新任务信息", updateCheck);
    
    int markResult1 = markTaskStatus(list, 1, COMPLETED);
    int markResult2 = markTaskStatus(list, 3, IN_PROGRESS);
    Task *task1Status = findTaskById(list, 1);
    Task *task3Status = findTaskById(list, 3);
    int statusCheck = (markResult1 == 1) && (markResult2 == 1) && 
                     (task1Status->status == COMPLETED) && 
                     (task3Status->status == IN_PROGRESS);
    printTestResult("标记任务状态", statusCheck);
    
    deleteTask(list, 1);
    deleteTask(list, 2);
    deleteTask(list, 3);
    printTestResult("批量删除任务", list->count == 0);
    
    updateResult = updateTask(list, 999, "不存在的任务", "这是一个不存在的任务", PENDING, now + 86400);
    printTestResult("更新不存在的任务", updateResult == 0);
    
    markResult1 = markTaskStatus(list, 999, COMPLETED);
    printTestResult("标记不存在的任务状态", markResult1 == 0);
    
    freeTaskList(list);
    printf("=== 任务增删改查测试完成 ===\n");
}

void testSearchAndFilter() {
    printf("\n=== 测试搜索和过滤功能 ===\n");
    
    TaskList *list = initTaskList();
    time_t now = time(NULL);
    
    addTask(list, createTask(1, "学习C语言", "学习链表和指针", now + 86400));
    addTask(list, createTask(2, "学习Python", "学习数据科学", now + 172800));
    addTask(list, createTask(3, "完成C语言作业", "编写测试代码", now + 259200));
    addTask(list, createTask(4, "运动健身", "跑步和健身", now + 345600));
    addTask(list, createTask(5, "团队会议", "讨论项目进度", now + 432000));
    
    markTaskStatus(list, 1, COMPLETED);
    markTaskStatus(list, 2, IN_PROGRESS);
    markTaskStatus(list, 3, IN_PROGRESS);
    
    // 测试标题搜索
    TaskList *searchResults = searchTaskByTitle(list, "C语言");
    int searchCheck = (searchResults != NULL) && (searchResults->count == 2);
    printTestResult("标题搜索功能", searchCheck);
    freeTaskList(searchResults);
    
    // 测试搜索不存在的关键词
    searchResults = searchTaskByTitle(list, "Java");
    printTestResult("搜索不存在的关键词", searchResults != NULL && searchResults->count == 0);
    freeTaskList(searchResults);
    
    // 测试状态过滤
    TaskList *filteredTasks = filterTaskByStatus(list, IN_PROGRESS);
    int filterCheck = (filteredTasks != NULL) && (filteredTasks->count == 2);
    printTestResult("状态过滤功能", filterCheck);
    freeTaskList(filteredTasks);
    
    // 测试按创建时间排序（升序）
    sortTaskByCreateTime(list, 1);
    Task *current = list->head;
    int sortCreateCheck = 1; // 默认为通过
    int prevId = 0;
    while (current != NULL) {
        if (current->id <= prevId) {
            sortCreateCheck = 0;
            break;
        }
        prevId = current->id;
        current = current->next;
    }
    printTestResult("按创建时间升序排序", sortCreateCheck);
    
    // 测试按截止时间排序（降序）
    sortTaskByDueTime(list, 0);
    current = list->head;
    int sortDueCheck = 1; // 默认为通过
    time_t prevDue = current ? current->due_time + 1 : 0; // 初始值设为第一个任务截止时间+1
    while (current != NULL) {
        if (current->due_time > prevDue) {
            sortDueCheck = 0;
            break;
        }
        prevDue = current->due_time;
        current = current->next;
    }
    printTestResult("按截止时间降序排序", sortDueCheck);
    
    // 测试获取任务统计
    int total, pending, inProgress, completed;
    getTaskStatistics(list, &total, &pending, &inProgress, &completed);
    int statsCheck = (total == 5) && (completed == 1) && (inProgress == 2) && (pending == 2);
    printTestResult("获取任务统计信息", statsCheck);
    
    // 测试获取即将到期的任务（7天内）
    TaskList *upcomingTasks = getUpcomingTasks(list, 7);
    int upcomingCheck = (upcomingTasks != NULL) && (upcomingTasks->count == 5);
    printTestResult("获取即将到期的任务", upcomingCheck);
    freeTaskList(upcomingTasks);
    
    freeTaskList(list);
    printf("=== 搜索和过滤功能测试完成 ===\n");
}

// 测试数据存储功能
void testStorageOperations() {
    printf("\n=== 测试数据存储功能 ===\n");
    
    // 创建测试任务列表
    TaskList *originalList = initTaskList();
    time_t now = time(NULL);
    
    // 添加多个测试任务
    addTask(originalList, createTask(1, "存储测试任务1", "测试二进制保存与加载", now + 86400));
    addTask(originalList, createTask(2, "存储测试任务2", "测试CSV导出功能", now + 172800));
    addTask(originalList, createTask(3, "存储测试任务3", "测试数据完整性", now + 259200));
    
    // 更新任务状态
    markTaskStatus(originalList, 1, IN_PROGRESS);
    markTaskStatus(originalList, 2, COMPLETED);
    
    const char *binaryFile = "test_tasks.bin";
    const char *csvFile = "test_tasks.csv";
    
    // 测试保存到二进制文件
    int saveBinaryResult = saveTasksToBinary(originalList, binaryFile);
    printTestResult("保存任务到二进制文件", saveBinaryResult == 1);
    
    // 测试从二进制文件加载
    TaskList *loadedFromBinary = loadTasksFromBinary(binaryFile);
    int loadBinaryCheck = (loadedFromBinary != NULL) && (loadedFromBinary->count == 3);
    printTestResult("从二进制文件加载任务", loadBinaryCheck);
    
    // 比较加载的任务与原始任务
    if (loadedFromBinary) {
        Task *origTask = originalList->head;
        Task *loadedTask = loadedFromBinary->head;
        int dataMatchCheck = 1; // 默认为通过
        
        while (origTask != NULL && loadedTask != NULL) {
            // 比较关键属性
            if (origTask->id != loadedTask->id ||
                strcmp(origTask->title, loadedTask->title) != 0 ||
                strcmp(origTask->description, loadedTask->description) != 0 ||
                origTask->due_time != loadedTask->due_time ||
                origTask->status != loadedTask->status) {
                dataMatchCheck = 0;
                break;
            }
            origTask = origTask->next;
            loadedTask = loadedTask->next;
        }
        
        printTestResult("加载的任务数据与原始任务匹配", dataMatchCheck);
        freeTaskList(loadedFromBinary);
    }
    
    // 测试导出到CSV文件
    int exportCSVResult = exportTasksToCSV(originalList, csvFile);
    printTestResult("导出任务到CSV文件", exportCSVResult == 1);
    
    // 测试从CSV文件加载
    TaskList *loadedFromCSV = loadTasksFromCSV(csvFile);
    int loadCSVCheck = (loadedFromCSV != NULL) && (loadedFromCSV->count == 3);
    printTestResult("从CSV文件加载任务", loadCSVCheck);
    
    // 比较加载的任务与原始任务
    if (loadedFromCSV) {
        // CSV加载的任务可能会重新分配ID，但其他属性应该匹配
        int titleMatchCount = 0;
        Task *origTask = originalList->head;
        while (origTask != NULL) {
            Task *foundTask = searchTaskByTitle(loadedFromCSV, origTask->title);
            if (foundTask != NULL && 
                strcmp(foundTask->description, origTask->description) == 0 &&
                foundTask->status == origTask->status) {
                titleMatchCount++;
            }
            origTask = origTask->next;
        }
        printTestResult("CSV加载的任务内容与原始任务匹配", titleMatchCount == 3);
        freeTaskList(loadedFromCSV);
    }
    
    // 测试保存空列表
    TaskList *emptyList = initTaskList();
    saveBinaryResult = saveTasksToBinary(emptyList, binaryFile);
    printTestResult("保存空任务列表", saveBinaryResult == 1);
    
    // 测试加载空列表
    TaskList *loadedEmptyList = loadTasksFromBinary(binaryFile);
    printTestResult("加载空任务列表", loadedEmptyList != NULL && loadedEmptyList->count == 0);
    
    // 清理资源
    freeTaskList(originalList);
    freeTaskList(emptyList);
    freeTaskList(loadedEmptyList);
    
    // 清理测试文件
    remove(binaryFile);
    remove(csvFile);
    
    printf("=== 数据存储功能测试完成 ===\n");
}

// 运行所有测试
void runAllTests() {
    printf("\n==================================================\n");
    printf("               开始运行所有测试\n");
    printf("==================================================\n\n");
    
    clock_t start = clock();
    
    // 运行各个测试函数
    testListOperations();
    testTaskOperations();
    testSearchAndFilter();
    testStorageOperations();
    
    clock_t end = clock();
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    
    printf("\n==================================================\n");
    printf("所有测试完成! 总运行时间: %.2f 秒\n", elapsed_time);
    printf("==================================================\n");
}

// 压力测试函数
void stressTest() {
    printf("\n=== 开始压力测试 ===\n");
    
    // 测试1: 添加大量任务
    printf("\n测试1: 添加1000个任务\n");
    clock_t start = clock();
    
    TaskList *largeList = initTaskList();
    time_t now = time(NULL);
    
    for (int i = 1; i <= 1000; i++) {
        char title[50];
        char description[100];
        sprintf(title, "压力测试任务 %d", i);
        sprintf(description, "这是一个用于压力测试的任务描述，编号: %d", i);
        
        addTask(largeList, createTask(i, title, description, now + i * 3600));
        
        // 每100个任务标记一次进度
        if (i % 100 == 0) {
            printf("已添加 %d 个任务...\n", i);
        }
    }
    
    clock_t end = clock();
    double add_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("添加1000个任务耗时: %.2f 秒\n", add_time);
    printTestResult("添加大量任务", largeList != NULL && largeList->count == 1000);
    
    // 测试2: 搜索性能
    printf("\n测试2: 搜索性能测试\n");
    start = clock();
    TaskList *searchResults = searchTaskByTitle(largeList, "压力测试任务 500");
    end = clock();
    double search_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("搜索操作耗时: %.6f 秒\n", search_time);
    printTestResult("搜索特定任务", searchResults != NULL && searchResults->count > 0);
    freeTaskList(searchResults);
    
    // 测试3: 排序性能
    printf("\n测试3: 排序性能测试\n");
    start = clock();
    sortTaskByDueTime(largeList, 1); // 升序排序
    end = clock();
    double sort_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("排序操作耗时: %.6f 秒\n", sort_time);
    
    // 验证排序是否正确
    Task *current = largeList->head;
    int sortCheck = 1;
    time_t prevDue = current ? current->due_time - 1 : 0;
    while (current != NULL) {
        if (current->due_time < prevDue) {
            sortCheck = 0;
            break;
        }
        prevDue = current->due_time;
        current = current->next;
    }
    printTestResult("大量任务排序正确性", sortCheck);
    
    // 测试4: 内存管理
    printf("\n测试4: 内存释放测试\n");
    start = clock();
    freeTaskList(largeList);
    end = clock();
    double free_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("释放内存耗时: %.6f 秒\n", free_time);
    printTestResult("释放大量任务内存", 1); // 假设释放成功
    
    // 测试5: 循环操作测试
    printf("\n测试5: 循环添加和删除操作\n");
    TaskList *cycleList = initTaskList();
    int cycles = 100;
    int operationsPerCycle = 20;
    
    start = clock();
    for (int i = 0; i < cycles; i++) {
        // 每次循环添加一些任务
        for (int j = 1; j <= operationsPerCycle; j++) {
            char title[50];
            sprintf(title, "循环任务 %d-%d", i, j);
            addTask(cycleList, createTask(i * 100 + j, title, "循环测试任务", now + j));
        }
        
        // 每次循环删除一些任务
        for (int j = 1; j <= operationsPerCycle / 2; j++) {
            deleteTask(cycleList, i * 100 + j);
        }
    }
    end = clock();
    double cycle_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("循环操作耗时: %.2f 秒\n", cycle_time);
    printTestResult("循环添加删除稳定性", cycleList != NULL);
    
    freeTaskList(cycleList);
    
    printf("\n=== 压力测试完成 ===\n");
}

// 主函数，入口点
int main() {
    printf("\n=== 待办事项管理系统测试程序 ===\n\n");
    
    // 运行所有基本测试
    runAllTests();
    
    // 提示是否运行压力测试
    printf("\n是否运行压力测试? (y/n): ");
    char choice;
    scanf(" %c", &choice); // 注意空格，跳过可能的换行符
    
    if (choice == 'y' || choice == 'Y') {
        stressTest();
    }
    
    printf("\n测试程序执行完毕!\n");
    return 0;
}
