#include <iostream>
#include <fstream>
#include "Executer.h"
#include "myString.h"

void createCase0File(const MyString& fileName) {
    std::ofstream ofs(fileName.c_str(), std::ios::binary);
    if (!ofs) {
        std::cerr << "Failed to open file for writing: " << fileName << std::endl;
        return;
    }

    int16_t N = 7;
    int16_t T = 0;
    int32_t args[] = {0, 1, 2,3,5,6,7};
    int32_t results[] = {0,3,3,3,4,4,0};

    ofs.write(reinterpret_cast<char*>(&N), sizeof(N));
    ofs.write(reinterpret_cast<char*>(&T), sizeof(T));
    ofs.write(reinterpret_cast<char*>(args), sizeof(args));
    ofs.write(reinterpret_cast<char*>(results), sizeof(results));
    ofs.close();
}

void createCase1File(const MyString& fileName) {
    std::ofstream ofs(fileName.c_str(), std::ios::binary);
    if (!ofs) {
        std::cerr << "Failed to open file for writing: " << fileName << std::endl;
        return;
    }

    int16_t N = 2;
    int16_t T = 1;
    int32_t args[] = {3,5};

    ofs.write(reinterpret_cast<char*>(&N), sizeof(N));
    ofs.write(reinterpret_cast<char*>(&T), sizeof(T));
    ofs.write(reinterpret_cast<char*>(args), sizeof(args));
    ofs.close();
}

void createCase2File(const MyString& fileName) {
    std::ofstream ofs(fileName.c_str(), std::ios::binary);
    if (!ofs) {
        std::cerr << "Failed to open file for writing: " << fileName << std::endl;
        return;
    }

    int16_t N = 4;
    int16_t T = 2;
    int32_t args[] = {0,5,6,7};

    ofs.write(reinterpret_cast<char*>(&N), sizeof(N));
    ofs.write(reinterpret_cast<char*>(&T), sizeof(T));
    ofs.write(reinterpret_cast<char*>(args), sizeof(args));
    ofs.close();
}


void createCase3File(const MyString& fileName) {
    std::ofstream ofs(fileName.c_str(), std::ios::binary);
    if (!ofs) {
        std::cerr << "Failed to open file for writing: " << fileName << std::endl;
        return;
    }

    int16_t N = 2;
    int16_t T = 3;
    const char* file1 = "func0.dat";
    const char* file2 = "func1.dat";

    ofs.write(reinterpret_cast<char*>(&N), sizeof(N));
    ofs.write(reinterpret_cast<char*>(&T), sizeof(T));
    ofs.write(file1, strlen(file1) + 1);  // Include the null terminator
    ofs.write(file2, strlen(file2) + 1);  // Include the null terminator
    ofs.close();
}

void createCase4File(const MyString& fileName) {
    std::ofstream ofs(fileName.c_str(), std::ios::binary);
    if (!ofs) {
        std::cerr << "Failed to open file for writing: " << fileName << std::endl;
        return;
    }

    int16_t N = 2;
    int16_t T = 4;
    const char* file1 = "func0.dat";
    const char* file2 = "func1.dat";

    ofs.write(reinterpret_cast<char*>(&N), sizeof(N));
    ofs.write(reinterpret_cast<char*>(&T), sizeof(T));
    ofs.write(file1, strlen(file1) + 1);  // Include the null terminator
    ofs.write(file2, strlen(file2) + 1);  // Include the null terminator
    ofs.close();
}


void testExecutor(const MyString& fileName) {
    try {
        Executer executor(fileName);

        std::cout << "Testing executeOverInterval for file: " << fileName.c_str() << std::endl;
        executor.executeOverInterval(0, 10);

        std::cout << "Testing userRequestedExecution for file: " << fileName.c_str() << std::endl;
        executor.userRequestedExecution();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    createCase0File("func0.dat");
    createCase1File("func1.dat");
    createCase2File("func2.dat");
    createCase3File("func3.dat");
    createCase4File("func4.dat");
   try {
        MyString fileName0("func0.dat");
       std::cout << "\nTesting Case 0:" << std::endl;
        testExecutor(fileName0);
    } catch (const std::exception& e) {
        std::cerr << "Failed to test Case 0: " << e.what() << std::endl;
   }

    try {
        MyString fileName1("func1.dat");
        std::cout << "\nTesting Case 1:" << std::endl;
        testExecutor(fileName1);
    } catch (const std::exception& e) {
        std::cerr << "Failed to test Case 1: " << e.what() << std::endl;
    }
//
    try {
        MyString fileName2("func2.dat");
        std::cout << "\nTesting Case 2:" << std::endl;
        testExecutor(fileName2);
    } catch (const std::exception& e) {
        std::cerr << "Failed to test Case 2: " << e.what() << std::endl;
    }

    try {
        MyString fileName3("func3.dat");
        std::cout << "\nTesting Case 3:" << std::endl;
        testExecutor(fileName3);
    } catch (const std::exception& e) {
        std::cerr << "Failed to test Case 3: " << e.what() << std::endl;
    }

    try {
        MyString fileName4("func4.dat");
        std::cout << "\nTesting Case 4:" << std::endl;
        testExecutor(fileName4);
    } catch (const std::exception& e) {
        std::cerr << "Failed to test Case 4: " << e.what() << std::endl;
    }


}
