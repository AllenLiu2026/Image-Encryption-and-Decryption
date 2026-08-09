#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <conio.h> // 用于暂停

// 颜色输出宏（Windows专属，更友好）
#define RED_TEXT "\033[31m"
#define GREEN_TEXT "\033[32m"
#define YELLOW_TEXT "\033[33m"
#define RESET_TEXT "\033[0m"

// 修复Windows路径分隔符（\转/）
std::string fixPath(std::string path) {
    for (size_t i = 0; i < path.size(); i++) {
        if (path[i] == '\\') path[i] = '/';
    }
    return path;
}

// 核心：异或加密/解密函数（加密解密通用）
bool xorEncryptDecrypt(const std::string& inputPath, const std::string& outputPath, int key) {
    std::string fixedIn = fixPath(inputPath);
    std::string fixedOut = fixPath(outputPath);

    // 二进制模式打开文件
    std::ifstream inFile(fixedIn, std::ios::binary);
    if (!inFile.is_open()) {
        std::cerr << RED_TEXT << "[错误] 输入文件无法打开！原因可能是：" << RESET_TEXT << std::endl;
        std::cerr << "  1. 路径错误（检查是否输错）" << std::endl;
        std::cerr << "  2. 文件被其他软件占用（如画图、浏览器）" << std::endl;
        std::cerr << "  3. 无访问权限（建议复制到桌面重试）" << std::endl;
        return false;
    }

    std::ofstream outFile(fixedOut, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << RED_TEXT << "[错误] 输出文件无法创建！原因可能是：" << RESET_TEXT << std::endl;
        std::cerr << "  1. 保存路径不存在（如文件夹未创建）" << std::endl;
        std::cerr << "  2. 无写入权限（建议保存到桌面）" << std::endl;
        inFile.close();
        return false;
    }

    // 逐字节异或处理
    char byte;
    long long processedBytes = 0;
    while (inFile.read(&byte, sizeof(char))) {
        byte ^= key; // 核心异或运算
        outFile.write(&byte, sizeof(char));
        processedBytes++;
    }

    inFile.close();
    outFile.close();
    std::cout << GREEN_TEXT << "[成功] 处理完成！共处理 " << processedBytes << " 字节" << RESET_TEXT << std::endl;
    return true;
}

// 显示主菜单
void showMenu() {
    system("cls"); // 清屏（Windows）
    std::cout << "=====================================" << std::endl;
    std::cout << "          图片加密解密工具 V2.0       " << std::endl;
    std::cout << "=====================================" << std::endl;
    std::cout << "            1. 图片加密               " << std::endl;
    std::cout << "            2. 图片解密               " << std::endl;
    std::cout << "            0. 退出程序               " << std::endl;
    std::cout << "=====================================" << std::endl;
    std::cout << "请选择操作（输入数字0-2）：";
}

// 加密流程
void encryptProcess() {
    std::string imgPath, outputPath;
    int key;

    std::cout << "\n===== 图片加密 =====" << std::endl;
    std::cout << "提示：路径支持复制粘贴，示例：C:\\1.png" << std::endl;
    
    std::cout << "请输入待加密图片路径：";
    std::cin >> imgPath;
    
    std::cout << "请输入加密后保存路径：";
    std::cin >> outputPath;
    
    std::cout << "请输入加密密钥（任意整数，如123，务必记住！）：";
    std::cin >> key;

    std::cout << YELLOW_TEXT << "[提示] 正在加密图片...（请勿关闭程序）" << RESET_TEXT << std::endl;
    xorEncryptDecrypt(imgPath, outputPath, key);
}

// 解密流程
void decryptProcess() {
    std::string imgPath, outputPath;
    int key;

    std::cout << "\n===== 图片解密 =====" << std::endl;
    std::cout << "提示：解密密钥必须和加密时一致，否则图片会损坏！" << std::endl;
    std::cout << "请输入待解密图片路径：";
    std::cin >> imgPath;
    
    std::cout << "请输入解密后保存路径：";
    std::cin >> outputPath;
    
    std::cout << "请输入解密密钥（和加密时相同）：";
    std::cin >> key;

    std::cout << YELLOW_TEXT << "[提示] 正在解密图片...（请勿关闭程序）" << RESET_TEXT << std::endl;
    xorEncryptDecrypt(imgPath, outputPath, key);
}

int main() {
    int choice;
    while (true) {
        showMenu();
        std::cin >> choice;

        switch (choice) {
            case 1:
                encryptProcess();
                break;
            case 2:
                decryptProcess();
                break;
            case 0:
                std::cout << GREEN_TEXT << "\n[提示] 程序已退出，感谢使用！" << RESET_TEXT << std::endl;
                return 0;
            default:
                std::cerr << RED_TEXT << "\n[错误] 输入无效！请输入0、1或2" << RESET_TEXT << std::endl;
                break;
        }

        // 操作完成后暂停，按任意键返回菜单
        std::cout << "\n按任意键返回主菜单...";
        _getch(); // 等待按键（不回显）
    }

    return 0;
}
