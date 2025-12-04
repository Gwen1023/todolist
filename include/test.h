#ifndef TEST_H
#define TEST_H

#include "todolist.h"

// 测试函数（E同学负责）
// 测试链表基本操作
void testListOperations();

// 测试任务增删改查
void testTaskOperations();

// 测试搜索和过滤功能
void testSearchAndFilter();

// 测试数据存储功能
void testStorageOperations();

// 运行所有测试
int runAllTests();

// 压力测试 - 大量任务
void stressTest();

#endif // TEST_H