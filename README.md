# TodoList Project (C Language)

## 项目简介
这是一个基于 C 语言实现的命令行 TodoList 任务管理系统。

## 项目架构

```
TodoList/
├── include/              # 头文件
│   ├── todolist.h        # 核心数据结构定义
│   ├── ui.h              # 前端界面相关
│   ├── backend.h         # 后端逻辑相关
│   ├── storage.h         # 数据存储相关
|   └── test.h            # 测试相关
├── src/
│   ├── ui/
│   │   ├── main.c        # 主函数入口
│   │   └── menu.c        # 菜单显示和交互
│   ├── backend/
│   │   ├── task.c        # 任务操作
│   │   ├── list.c        # 链表操作
│   │   └── advanced.c    # 高级功能
│   ├── storage/
│   │   └── storage.c     # 文件读写
│   └── test/
│       └── test.c        # 测试代码
├── build/                # 编译输出目录
└── Makefile              # 编译配置
```