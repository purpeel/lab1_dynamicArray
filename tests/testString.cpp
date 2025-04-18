#include <gtest/gtest.h>
extern "C" {
    #include "../inc/stringTypeinfo.h"
}


class StringTypeInfoTest : public ::testing::Test {
    protected:
        char* testStr;
        elemPtr testPtr;
    
        void SetUp() override {
            testStr = strdup("test");
            testPtr = (elemPtr)testStr;
        }
    
        void TearDown() override {
            free(testStr);
        }
    };
    
    // Test stringGetSize()
    TEST_F(StringTypeInfoTest, GetSize) {
        size_t size = getStringTI()->getSize();
        EXPECT_EQ(size, sizeof(char*));
    }
    
    // Test deleteString()
    TEST_F(StringTypeInfoTest, DeleteString) {
        char* str = strdup("test\n");
        Exception result = getStringTI()->destruct((elemPtr)str);
        EXPECT_EQ(result, SUCCESSFUL_EXECUTION);
    }
    
    // Test copyString()
    TEST_F(StringTypeInfoTest, CopyString) {
        const char* source = "test string\n";
        // char* dest = nullptr;
        elemPtr destPtr = malloc(sizeof(char*));
        
        Exception result = getStringTI()->assign(&destPtr, (elemPtr) source);
        
        EXPECT_EQ(result, SUCCESSFUL_EXECUTION);
        EXPECT_STREQ((char*) destPtr, "test string\n");
        
        free(destPtr);
    }
    
    // Test compareString()
    TEST_F(StringTypeInfoTest, CompareString) {
        const char* str1 = "abc\n";
        const char* str2 = "def\n";
        const char* str3 = "abc\n";
        
        // Test equal strings
        EXPECT_EQ(getStringTI()->compare((elemPtr)str1, (elemPtr)str3), EQUAL);
        
        // Test less than
        EXPECT_EQ(getStringTI()->compare((elemPtr)str1, (elemPtr)str2), LESS);
        
        // Test greater than
        EXPECT_EQ(getStringTI()->compare((elemPtr)str2, (elemPtr)str1), GREATER);
        
        // Test different lengths
        const char* str4 = "abcd\n";
        EXPECT_EQ(getStringTI()->compare((elemPtr)str1, (elemPtr)str4), LESS);
        EXPECT_EQ(getStringTI()->compare((elemPtr)str4, (elemPtr)str1), GREATER);
    }
    
    // Test printString()
    TEST_F(StringTypeInfoTest, PrintString) {
        const char* str = "test string";
        testing::internal::CaptureStdout();
        getStringTI()->print((elemPtr)str);
        std::string output = testing::internal::GetCapturedStdout();
        EXPECT_EQ(output, "test string");
    }
    
    // Test inputString()
    TEST_F(StringTypeInfoTest, InputString) {
        elemPtr result = nullptr;
        const char* input = "test input";
        
        Exception status = getStringTI()->input(&result, input);
        
        EXPECT_EQ(status, SUCCESSFUL_EXECUTION);
        EXPECT_STREQ((char*)result, "test input");
        
        free(result);
    }
    
    // Test firstIsLower()
    TEST_F(StringTypeInfoTest, FirstIsLower) {
        const char* lowerFirst = "test";
        const char* upperFirst = "Test";
        const char* numberFirst = "1test";
        
        EXPECT_TRUE(getStringTI()->setForWhere[0]((elemPtr)lowerFirst));
        EXPECT_FALSE(getStringTI()->setForWhere[0]((elemPtr)upperFirst));
        EXPECT_FALSE(getStringTI()->setForWhere[0]((elemPtr)numberFirst));
    }
    
    // Test isDigit()
    TEST_F(StringTypeInfoTest, IsDigit) {
        const char* allDigits = "12345";
        const char* mixedString = "123abc";
        const char* noDigits = "abcde";
        
        EXPECT_TRUE(getStringTI()->setForWhere[1]((elemPtr)allDigits));
        EXPECT_FALSE(getStringTI()->setForWhere[1]((elemPtr)mixedString));
        EXPECT_FALSE(getStringTI()->setForWhere[1]((elemPtr)noDigits));
    }
    
    // Test isUpperCase()
    TEST_F(StringTypeInfoTest, IsUpperCase) {
        const char* allUpper = "TEST";
        const char* mixedCase = "TeSt";
        const char* allLower = "test";
        
        EXPECT_TRUE(getStringTI()->setForWhere[2]((elemPtr)allUpper));
        EXPECT_FALSE(getStringTI()->setForWhere[2]((elemPtr)mixedCase));
        EXPECT_FALSE(getStringTI()->setForWhere[2]((elemPtr)allLower));
    }
    
    // Test invertString()
    TEST_F(StringTypeInfoTest, InvertString) {
        char* str = strdup("Hello");
        
        Exception result = getStringTI()->setForMap[0]((elemPtr)str);
        
        EXPECT_EQ(result, SUCCESSFUL_EXECUTION);
        EXPECT_STREQ(str, "olleH");
        
        free(str);
    }
    
    // Test toLowerCase()
    TEST_F(StringTypeInfoTest, ToLowerCase) {
        char* str = strdup("HELLO");
        
        Exception result = getStringTI()->setForMap[1]((elemPtr)str);
        
        EXPECT_EQ(result, SUCCESSFUL_EXECUTION);
        EXPECT_STREQ(str, "hello");
        
        free(str);
    }
    
    // Test getFirstLiteral()
    TEST_F(StringTypeInfoTest, GetFirstLiteral) {
        char* str = strdup("Hello");
        
        Exception result = getStringTI()->setForMap[2]((elemPtr)str);
        
        EXPECT_EQ(result, SUCCESSFUL_EXECUTION);
        EXPECT_STREQ(str, "H");
        
        free(str);
    }
    
    // Test maxString()
    TEST_F(StringTypeInfoTest, MaxString) {
        const char* str1 = "abc";
        const char* str2 = "def";
        
        elemPtr result = getStringTI()->maximum((elemPtr)str1, (elemPtr)str2);
        EXPECT_EQ(result, (elemPtr)str2);
        
        result = getStringTI()->maximum((elemPtr)str2, (elemPtr)str1);
        EXPECT_EQ(result, (elemPtr)str2);
    }
    
    // Test minString()
    TEST_F(StringTypeInfoTest, MinString) {
        const char* str1 = "abc";
        const char* str2 = "def";
        
        elemPtr result = getStringTI()->minimum((elemPtr)str1, (elemPtr)str2);
        EXPECT_EQ(result, (elemPtr)str1);
        
        result = getStringTI()->minimum((elemPtr)str2, (elemPtr)str1);
        EXPECT_EQ(result, (elemPtr)str1);
    }
    
    // Test swapString()
    TEST_F(StringTypeInfoTest, SwapString) {
        char* str1 = strdup("first");
        char* str2 = strdup("second");
        elemPtr ptr1 = (elemPtr)str1;
        elemPtr ptr2 = (elemPtr)str2;
        
        Exception result = getStringTI()->swap(&ptr1, &ptr2);
        
        EXPECT_EQ(result, SUCCESSFUL_EXECUTION);
        EXPECT_STREQ((char*)ptr1, "second");
        EXPECT_STREQ((char*)ptr2, "first");
        
        free(str1);
        free(str2);
    }
    
    // Test getStringTI()
    TEST_F(StringTypeInfoTest, GetStringTI) {
        const TypeInfo* info = getStringTI();
        
        EXPECT_NE(info, nullptr);
        EXPECT_STREQ(info->typeName, "string");
        EXPECT_NE(info->swap, nullptr);
        EXPECT_NE(info->input, nullptr);
        EXPECT_NE(info->assign, nullptr);
        EXPECT_NE(info->compare, nullptr);
        EXPECT_NE(info->maximum, nullptr);
        EXPECT_NE(info->minimum, nullptr);
        EXPECT_NE(info->getSize, nullptr);
        EXPECT_NE(info->destruct, nullptr);
        EXPECT_NE(info->print, nullptr);
        
        // Verify map and where function arrays
        for(int i = 0; i < 3; i++) {
            EXPECT_NE(info->setForMap[i], nullptr);
            EXPECT_NE(info->setForWhere[i], nullptr);
        }
    }
    
    // Edge cases and error conditions
    TEST_F(StringTypeInfoTest, EdgeCases) {
        // Empty string
        char* emptyStr = strdup("");
        EXPECT_FALSE(getStringTI()->setForWhere[0]((elemPtr)emptyStr));
        EXPECT_TRUE(getStringTI()->setForWhere[1]((elemPtr)emptyStr));
        EXPECT_TRUE(getStringTI()->setForWhere[2]((elemPtr)emptyStr));
        free(emptyStr);
        
        // NULL pointer handling
        char* nullStr = nullptr;
        EXPECT_EQ(getStringTI()->assign(&testPtr, (elemPtr)nullStr), MEMORY_ALLOCATION_ERROR);
        
        // Long string
        char* longStr = (char*)malloc(1000);
        memset(longStr, 'A', 999);
        longStr[999] = '\0';
        Exception result = getStringTI()->setForMap[1]((elemPtr)longStr);
        EXPECT_EQ(result, SUCCESSFUL_EXECUTION);
        free(longStr);
    }