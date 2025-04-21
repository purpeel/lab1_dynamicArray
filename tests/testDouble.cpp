#include <gtest/gtest.h>
extern "C" {
    #include "../inc/doubleTypeinfo.h"
}

class DoubleTypeInfoTest : public ::testing::Test {
    protected:
        double *testValue;
        ElemPtr testPtr;
    
        void SetUp() override {
            testValue = new double(0.0);
            testPtr = (ElemPtr)testValue;
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
        Exception result = getDoubleTI()->destruct((ElemPtr)value);
        EXPECT_EQ(result, SUCCESSFUL_EXECUTION);
    }
    
    // Test assignDouble()
    TEST_F(DoubleTypeInfoTest, AssignDouble) {
        double sourceVal = 3.14;
        double* destVal = nullptr;
        ElemPtr source = (ElemPtr)&sourceVal;
        ElemPtr *dest = (ElemPtr *)&destVal;
    
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
    
        EXPECT_EQ(getDoubleTI()->compare((ElemPtr)&val1, (ElemPtr)&val2), GREATER);
        EXPECT_EQ(getDoubleTI()->compare((ElemPtr)&val2, (ElemPtr)&val1), LESS);
        EXPECT_EQ(getDoubleTI()->compare((ElemPtr)&val1, (ElemPtr)&val3), EQUAL);
    }
    
    // Test printDouble()
    TEST_F(DoubleTypeInfoTest, PrintDouble) {
        double value = 3.14159;
        testing::internal::CaptureStdout();
        getDoubleTI()->print((ElemPtr)&value);
        std::string output = testing::internal::GetCapturedStdout();
        EXPECT_EQ(output, "3.141590");
    }
    
    // Test inputDouble()
    TEST_F(DoubleTypeInfoTest, InputDouble) {
        ElemPtr result = nullptr;
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
    
        EXPECT_TRUE(getDoubleTI()->setForWhere[0]((ElemPtr)&normalizedValue));
        EXPECT_FALSE(getDoubleTI()->setForWhere[0]((ElemPtr)&nonNormalizedValue));
        EXPECT_TRUE(getDoubleTI()->setForWhere[0]((ElemPtr)&negativeValue));
    }
    
    // Test isRound()
    TEST_F(DoubleTypeInfoTest, IsRound) {
        double roundValue = 5.0;
        double nonRoundValue = 5.5;
        double negativeRoundValue = -3.0;
    
        EXPECT_TRUE(getDoubleTI()->setForWhere[1]((ElemPtr)&roundValue));
        EXPECT_FALSE(getDoubleTI()->setForWhere[1]((ElemPtr)&nonRoundValue));
        EXPECT_TRUE(getDoubleTI()->setForWhere[1]((ElemPtr)&negativeRoundValue));
    }
    
    // Test isPositive()
    TEST_F(DoubleTypeInfoTest, IsPositive) {
        double positiveValue = 5.0;
        double negativeValue = -5.0;
        double zeroValue = 0.0;
    
        EXPECT_TRUE(getDoubleTI()->setForWhere[2]((ElemPtr)&positiveValue));
        EXPECT_FALSE(getDoubleTI()->setForWhere[2]((ElemPtr)&negativeValue));
        EXPECT_FALSE(getDoubleTI()->setForWhere[2]((ElemPtr)&zeroValue));
    }
    
    // Test oppose()
    TEST_F(DoubleTypeInfoTest, Oppose) {
        double value = 5.0;
        ElemPtr ptr = (ElemPtr)&value;
        
        Exception result = getDoubleTI()->setForMap[0](ptr);
        
        EXPECT_EQ(result, SUCCESSFUL_EXECUTION);
        EXPECT_DOUBLE_EQ(*(double*)ptr, -5.0);
    }
    
    // Test logarithm()
    TEST_F(DoubleTypeInfoTest, Logarithm) {
        double value = 100.0;
        ElemPtr ptr = (ElemPtr)&value;
        
        Exception result = getDoubleTI()->setForMap[1](ptr);
        
        EXPECT_EQ(result, SUCCESSFUL_EXECUTION);
        EXPECT_DOUBLE_EQ(*(double*)ptr, 2.0); // log10(100) = 2
    
        // Test special cases
        double zero = 0.0;
        result = getDoubleTI()->setForMap[1]((ElemPtr)&zero);
        EXPECT_EQ(result, SUCCESSFUL_EXECUTION);
        
        double negative = -100.0;
        result = getDoubleTI()->setForMap[1]((ElemPtr)&negative);
        EXPECT_EQ(result, SUCCESSFUL_EXECUTION);
        EXPECT_DOUBLE_EQ(*(double*)&negative, 2.0); // Should handle negative by taking abs
    }
    
    // Test sign()
    TEST_F(DoubleTypeInfoTest, Sign) {
        double positive = 5.0;
        double negative = -5.0;
        double zero = 0.0;
    
        Exception result = getDoubleTI()->setForMap[2]((ElemPtr)&positive);
        EXPECT_EQ(result, SUCCESSFUL_EXECUTION);
        EXPECT_DOUBLE_EQ(*(double*)&positive, 1.0);
    
        result = getDoubleTI()->setForMap[2]((ElemPtr)&negative);
        EXPECT_EQ(result, SUCCESSFUL_EXECUTION);
        EXPECT_DOUBLE_EQ(*(double*)&negative, -1.0);
    
        result = getDoubleTI()->setForMap[2]((ElemPtr)&zero);
        EXPECT_EQ(result, SUCCESSFUL_EXECUTION);
        EXPECT_DOUBLE_EQ(*(double*)&zero, 0.0);
    }
    
    // Test maxDouble()
    TEST_F(DoubleTypeInfoTest, MaxDouble) {
        double val1 = 5.0;
        double val2 = 3.0;
        
        ElemPtr result = getDoubleTI()->maximum((ElemPtr)&val1, (ElemPtr)&val2);
        EXPECT_EQ(result, (ElemPtr)&val1);
    
        result = getDoubleTI()->maximum((ElemPtr)&val2, (ElemPtr)&val1);
        EXPECT_EQ(result, (ElemPtr)&val1);
    }
    
    // Test minDouble()
    TEST_F(DoubleTypeInfoTest, MinDouble) {
        double val1 = 5.0;
        double val2 = 3.0;
        
        ElemPtr result = getDoubleTI()->minimum((ElemPtr)&val1, (ElemPtr)&val2);
        EXPECT_EQ(result, (ElemPtr)&val2);
    
        result = getDoubleTI()->minimum((ElemPtr)&val2, (ElemPtr)&val1);
        EXPECT_EQ(result, (ElemPtr)&val2);
    }
    
    // Test swapDouble()
    TEST_F(DoubleTypeInfoTest, SwapDouble) {
        double val1 = 5.0;
        double val2 = 3.0;
        ElemPtr ptr1 = (ElemPtr)&val1;
        ElemPtr ptr2 = (ElemPtr)&val2;
        
        Exception result = getDoubleTI()->swap(&ptr1, &ptr2);
        
        EXPECT_EQ(result, SUCCESSFUL_EXECUTION);
        EXPECT_EQ(ptr1, (ElemPtr)&val2);
        EXPECT_EQ(ptr2, (ElemPtr)&val1);
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