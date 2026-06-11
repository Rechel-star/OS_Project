#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 10      // 最大文件数量
#define MAX_BLOCKS 50     // 模拟磁盘总块数
#define BLOCK_SIZE 64     // 每个磁盘块的大小（字节）

// 文件控制块 (目录项)
typedef struct {
    char filename[20];
    int start_block;      // 起始物理块号
    int num_blocks;       // 占用的块数
    int is_used;          // 0: 空闲, 1: 已使用
} FileEntry;

FileEntry directory[MAX_FILES];
int bit_map[MAX_BLOCKS];              // 空闲空间管理：位示图 (0: 空闲, 1: 已分配)
char virtual_disk[MAX_BLOCKS][BLOCK_SIZE]; // 模拟物理磁盘

// 初始化文件系统
void initFileSystem() {
    for (int i = 0; i < MAX_FILES; i++) {
        directory[i].is_used = 0;
    }
    for (int i = 0; i < MAX_BLOCKS; i++) {
        bit_map[i] = 0;
        memset(virtual_disk[i], 0, BLOCK_SIZE);
    }
    printf("系统提示: 文件系统初始化完成！(总容量: %d 块)\n", MAX_BLOCKS);
}

// 寻找连续的空闲磁盘块 (连续分配策略)
int findFreeBlocks(int required_blocks) {
    int count = 0;
    int start = -1;
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (bit_map[i] == 0) {
            if (count == 0) start = i;
            count++;
            if (count == required_blocks) return start;
        } else {
            count = 0;
        }
    }
    return -1; // 空间不足或没有连续空间
}

// 1. 创建文件
void createFile() {
    char name[20];
    int size, required_blocks;

    printf("\n请输入新文件名: ");
    scanf("%s", name);
    printf("请输入文件大小 (字节): ");
    scanf("%d", &size);

    // 计算需要的磁盘块数 (向上取整)
    required_blocks = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;

    // 检查目录是否有空位
    int dir_index = -1;
    for (int i = 0; i < MAX_FILES; i++) {
        if (directory[i].is_used == 0) {
            dir_index = i;
            break;
        }
    }

    if (dir_index == -1) {
        printf("错误: 目录已满，无法创建新文件！\n");
        return;
    }

    // 分配磁盘空间
    int start_block = findFreeBlocks(required_blocks);
    if (start_block == -1) {
        printf("错误: 磁盘连续空闲空间不足！\n");
        return;
    }

    // 更新位示图和目录项
    for (int i = start_block; i < start_block + required_blocks; i++) {
        bit_map[i] = 1;
    }

    strcpy(directory[dir_index].filename, name);
    directory[dir_index].start_block = start_block;
    directory[dir_index].num_blocks = required_blocks;
    directory[dir_index].is_used = 1;

    printf("成功: 文件 '%s' 创建完毕！分配了 %d 个盘块 (起始块: %d)。\n", name, required_blocks, start_block);
}

// 2. 写入文件
void writeFile() {
    char name[20];
    char content[256];
    printf("\n请输入要写入的文件名: ");
    scanf("%s", name);

    for (int i = 0; i < MAX_FILES; i++) {
        if (directory[i].is_used == 1 && strcmp(directory[i].filename, name) == 0) {
            printf("请输入要写入的内容 (不含空格): ");
            scanf("%s", content);

            // 简单模拟写入：将内容直接截断/塞入起始盘块
            strncpy(virtual_disk[directory[i].start_block], content, BLOCK_SIZE - 1);
            virtual_disk[directory[i].start_block][BLOCK_SIZE - 1] = '\0'; // 确保字符串结束

            printf("成功: 数据已写入文件 '%s'。\n", name);
            return;
        }
    }
    printf("错误: 找不到文件 '%s'。\n", name);
}

// 3. 读取文件
void readFile() {
    char name[20];
    printf("\n请输入要读取的文件名: ");
    scanf("%s", name);

    for (int i = 0; i < MAX_FILES; i++) {
        if (directory[i].is_used == 1 && strcmp(directory[i].filename, name) == 0) {
            printf("文件 '%s' 的内容为:\n", name);
            // 简单模拟读取起始盘块的内容
            printf("%s\n", virtual_disk[directory[i].start_block]);
            return;
        }
    }
    printf("错误: 找不到文件 '%s'。\n", name);
}

// 4. 删除文件
void deleteFile() {
    char name[20];
    printf("\n请输入要删除的文件名: ");
    scanf("%s", name);

    for (int i = 0; i < MAX_FILES; i++) {
        if (directory[i].is_used == 1 && strcmp(directory[i].filename, name) == 0) {
            // 回收磁盘块 (更新位示图)
            int start = directory[i].start_block;
            int num = directory[i].num_blocks;
            for (int j = start; j < start + num; j++) {
                bit_map[j] = 0;
                memset(virtual_disk[j], 0, BLOCK_SIZE); // 清空数据
            }
            
            // 释放目录项
            directory[i].is_used = 0;
            printf("成功: 文件 '%s' 已删除，已回收 %d 个盘块。\n", name, num);
            return;
        }
    }
    printf("错误: 找不到文件 '%s'。\n", name);
}

// 5. 显示目录与空间状态
void listDirectory() {
    int free_blocks = 0;
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (bit_map[i] == 0) free_blocks++;
    }

    printf("\n=== 根目录列表 ===\n");
    printf("文件名\t\t起始块\t块数\n");
    printf("--------------------------------\n");
    int count = 0;
    for (int i = 0; i < MAX_FILES; i++) {
        if (directory[i].is_used == 1) {
            printf("%s\t\t%d\t%d\n", directory[i].filename, directory[i].start_block, directory[i].num_blocks);
            count++;
        }
    }
    if (count == 0) printf("(空目录)\n");
    
    printf("\n磁盘状态: 剩余空闲块 %d/%d\n", free_blocks, MAX_BLOCKS);
}

int main() {
    int choice;
    initFileSystem();

    while (1) {
        printf("\n========== 简易文件系统 ==========\n");
        printf("1. 创建文件\n");
        printf("2. 写入文件\n");
        printf("3. 读取文件\n");
        printf("4. 删除文件\n");
        printf("5. 查看目录与空间状态\n");
        printf("0. 退出\n");
        printf("请选择操作: ");
        
        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1: createFile(); break;
            case 2: writeFile(); break;
            case 3: readFile(); break;
            case 4: deleteFile(); break;
            case 5: listDirectory(); break;
            case 0: printf("退出系统。\n"); return 0;
            default: printf("无效输入，请重试。\n");
        }
    }
    return 0;
}
