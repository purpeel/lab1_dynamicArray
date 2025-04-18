#include <gtest/gtest.h>
extern "C" {
    #include "../inc/doubleTypeinfo.h"
}

class DoubleTypeInfoTest : public ::testing::Test {
    protected:
        double *testValue;
        elemPtr testPtr;
    
        void SetUp() override {
            testValue = new double(0.0);
            testPtr = (elemPtr)testValue;
        }
    
        void TearDown() override {
            delete testValue;
        }
    };
    
    // Test doubleGetSize()
    TEST_F(DoubleTypeInfoTest, GetSize) {
        size_t size = getDoubleTI()->getSize();
        EXPECT_EQ(size, sizeof(double));
    }
    
    // Test deleteDouble()
    TEST_F(DoubleTypeInfoTest, DeleteDouble) {
        double* value = new double(5.0);
        Exception result = getDoubleTI()->destruct((elemPtr)value);
        EXPECT_EQ(result, SUCCESSFUL_EXECUTION);
    }
    
    // Test assignDouble()
    TEST_F(DoubleTypeInfoTest, AssignDouble) {
        double sourceVal = 3.14;
        double* destVal = nullptr;
        elemPtr source = (elemPtr)&sourceVal;
        elemPtr *dest = (elemPtr *)&destVal;
    
        Exception result = getDoubleTI()->assign(dest, source);
        
        EXPECT_EQ(result, SUCCESSFUL_EXECUTION);
        EXPECT_NE(destVal, nullptr);
        EXPECT_DOUBLE_EQ(*destVal, 3.14);
    
        free(destVal);
    }
    
    // Test compareDouble()
    TEST_F(DoubleTypeInfoTest, CompareDouble) {
        double val1 = 3.14;
        double val2 = 2.71;
        double val3 = 3.14;
    
        EXPECT_EQ(getDoubleTI()->compare((elemPtr)&val1, (elemPtr)&val2), GREATER);
        EXPECT_EQ(getDoubleTI()->compare((elemPtr)&val2, (elemPtr)&val1), LESS);
        EXPECT_EQ(getDoubleTI()->compare((elemPtr)&val1, (elemPtr)&val3), EQUAL);
    }
    
    // Test printDouble()
    TEST_F(DoubleTypeInfoTest, PrintDouble) {
        double value = 3.14159;
        testing::internal::CaptureStdout();
        getDoubleTI()->print((elemPtr)&value);
        std::string output = testing::internal::GetCapturedStdout();
        EXPECT_EQ(output, "3.141590");
    }
    
    // Test inputDouble()
    TEST_F(DoubleTypeInfoTest, InputDouble) {
        elemPtr result = nullptr;
        const char* input = "3.14";
        
        Exception status = getDoubleTI()->input(&result, input);
        
        EXPECT_EQ(status, SUCCESSFUL_EXECUTION);
        EXPECT_DOUBLE_EQ(*(double*)result, 3.14);
        
        free(result);
    }
    
    // Test isNormalized()
    TEST_F(DoubleTypeInfoTest, IsNormalized) {
        double normalizedValue = 0.5;
        double nonNormalizedValue = 1.5;
        double negativeValue = -0.5;
    
        EXPECT_TRUE(getDoubleTI()->setForWhere[0]((elemPtr)&normalizedValue));
        EXPECT_FALSE(getDoubleTI()->setForWhere[0]((elemPtr)&nonNormalizedValue));
        EXPECT_TRUE(getDoubleTI()->setForWhere[0]((elemPtr)&negativeValue));
    }
    
    // Test isRound()
    TEST_F(DoubleTypeInfoTest, IsRound) {
        double roundValue = 5.0;
        double nonRoundValue = 5.5;
        double negativeRoundValue = -3.0;
    
        EXPECT_TRUE(getDoubleTI()->setForWhere[1]((elemPtr)&roundValue));
        EXPECT_FALSE(getDoubleTI()->setForWhere[1]((elemPtr)&nonRoundValue));
        EXPECT_TRUE(getDoubleTI()->setForWhere[1]((elemPtr)&negativeRoundValue));
    }
    
    // Test isPositive()
    TEST_F(DoubleTypeInfoTest, IsPositive) {
        double positiveValue = 5.0;
        double negativeValue = -5.0;
        double zeroValue = 0.0;
    
        EXPECT_TRUE(getDoubleTI()->setForWhere[2]((elemPtr)&positiveValue));
        EXPECT_FALSE(getDoubleTI()->setForWhere[2]((elemPtr)&negativeValue));
        EXPECT_FALSE(getDoubleTI()->setForWhere[2]((elemPtr)&zeroValue));
    }
    
    // Test oppose()
    TEST_F(DoubleTypeInfoTest, Oppose) {
        double value = 5.0;
        elemPtr ptr = (elemPtr)&value;
        
        Exception result = getDoubleTI()->setForMap[0](ptr);
        
        EXPECT_EQ(result, SUCCESSFUL_EXECUTION);
        EXPECT_DOUBLE_EQ(*(double*)ptr, -5.0);
    }
    
    // Test logarithm()
    TEST_F(DoubleTypeInfoTest, Logarithm) {
        double value = 100.0;
        elemPtr ptr = (elemPtr)&value;
        
        Exception result = getDoubleTI()->setForMap[1](ptr);
        
        EXPECT_EQ(result, SUCCESSFUL_EXECUTION);
        EXPECT_DOUBLE_EQ(*(double*)ptr, 2.0); // log10(100) = 2
    
        // Test special cases
        double zero = 0.0;
        result = getDoubleTI()->setForMap[1]((elemPtr)&zero);
        EXPECT_EQ(result, SUCCESSFUL_EXECUTION);
        
        double negative = -100.0;
        result = getDoubleTI()->setForMap[1]((elemPtr)&negative);
        EXPECT_EQ(result, SUCCESSFUL_EXECUTION);
        EXPECT_DOUBLE_EQ(*(double*)&negative, 2.0); // Should handle negative by taking abs
    }
    
    // Test sign()
    TEST_F(DoubleTypeInfoTest, Sign) {
        double positive = 5.0;
        double negative = -5.0;
        double zero = 0.0;
    
        Exception result = getDoubleTI()->setForMap[2]((elemPtr)&positive);
        EXPECT_EQ(result, SUCCESSFUL_EXECUTION);
        EXPECT_DOUBLE_EQ(*(double*)&positive, 1.0);
    
        result = getDoubleTI()->setForMap[2]((elemPtr)&negative);
        EXPECT_EQ(result, SUCCESSFUL_EXECUTION);
        EXPECT_DOUBLE_EQ(*(double*)&negative, -1.0);
    
        result = getDoubleTI()->setForMap[2]((elemPtr)&zero);
        EXPECT_EQ(result, SUCCESSFUL_EXECUTION);
        EXPECT_DOUBLE_EQ(*(double*)&zero, 0.0);
    }
    
    // Test maxDouble()
    TEST_F(DoubleTypeInfoTest, MaxDouble) {
        double val1 = 5.0;
        double val2 = 3.0;
        
        elemPtr result = getDoubleTI()->maximum((elemPtr)&val1, (elemPtr)&val2);
        EXPECT_EQ(result, (elemPtr)&val1);
    
        result = getDoubleTI()->maximum((elemPtr)&val2, (elemPtr)&val1);
        EXPECT_EQ(result, (elemPtr)&val1);
    }
    
    // Test minDouble()
    TEST_F(DoubleTypeInfoTest, MinDouble) {
        double val1 = 5.0;
        double val2 = 3.0;
        
        elemPtr result = getDoubleTI()->minimum((elemPtr)&val1, (elemPtr)&val2);
        EXPECT_EQ(result, (elemPtr)&val2);
    
        result = getDoubleTI()->minimum((elemPtr)&val2, (elemPtr)&val1);
        EXPECT_EQ(result, (elemPtr)&val2);
    }
    
    // Test swapDouble()
    TEST_F(DoubleTypeInfoTest, SwapDouble) {
        double val1 = 5.0;
        double val2 = 3.0;
        elemPtr ptr1 = (elemPtr)&val1;
        elemPtr ptr2 = (elemPtr)&val2;
        
        Exception result = getDoubleTI()->swap(&ptr1, &ptr2);
        
        EXPECT_EQ(result, SUCCESSFUL_EXECUTION);
        EXPECT_EQ(ptr1, (elemPtr)&val2);
        EXPECT_EQ(ptr2, (elemPtr)&val1);
    }
    
    // Test getDoubleTI()
    TEST_F(DoubleTypeInfoTest, GetDoubleTI) {
        const TypeInfo* info = getDoubleTI();
        
        EXPECT_NE(info, nullptr);
        EXPECT_STREQ(info->typeName, "double");
        EXPECT_NE(info->swap, nullptr);
        EXPECT_NE(info->input, nullptr);
        EXPECT_NE(info->assign, nullptr);
        EXPECT_NE(info->compare, nullptr);
        EXPECT_NE(info->maximum, nullptr);
        EXPECT_NE(info->minimum, nullptr);
        EXPECT_NE(info->getSize, nullptr);
        EXPECT_NE(info->destruct, nullptr);
        EXPECT_NE(info->print, nullptr);
        
        // Verify function pointers in arrays
        EXPECT_NE(info->setForMap[0], nullptr);
        EXPECT_NE(info->setForMap[1], nullptr);
        EXPECT_NE(info->setForMap[2], nullptr);
        EXPECT_NE(info->setForWhere[0], nullptr);
        EXPECT_NE(info->setForWhere[1], nullptr);
        EXPECT_NE(info->setForWhere[2], nullptr);
    }