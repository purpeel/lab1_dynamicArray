#include <gtest/gtest.h>
extern "C" {
    #include "../inc/collection.h"
}

class CollectionTest : public ::testing::Test {
protected:
    DynamicArray* array;
    const TypeInfo* doubleInfo;

    void SetUp() override {
        doubleInfo = getDoubleTI();
        array = nullptr;
    }

    void TearDown() override {
        if (array != nullptr) {
            delete(array);
        }
        deleteStorage(getStorage());
    }
};

// Storage Tests
TEST_F(CollectionTest, GetStorageInitialization) {
    ArrayStorage* storage = getStorage();
    ASSERT_NE(storage, nullptr);
    EXPECT_EQ(storage->count, 0);
    EXPECT_EQ(storage->arrayPtrs, nullptr);
}

TEST_F(CollectionTest, DeleteEmptyStorage) {
    ArrayStorage* storage = getStorage();
    EXPECT_EQ(deleteStorage(storage), SUCCESSFUL_EXECUTION);
}

// Array Initialization Tests
TEST_F(CollectionTest, InitializeArray) {
    EXPECT_EQ(init(&array, doubleInfo, 5), SUCCESSFUL_EXECUTION);
    ASSERT_NE(array, nullptr);
    EXPECT_EQ(array->size, 0);
    EXPECT_EQ(array->capacity, 5);
    EXPECT_NE(array->begin, nullptr);
}

TEST_F(CollectionTest, InitializeWithInvalidCapacity) {
    EXPECT_EQ(init(&array, doubleInfo, -1), SUCCESSFUL_EXECUTION);
    ASSERT_NE(array, nullptr);
}

TEST_F(CollectionTest, InitializeWithNullTypeInfo) {
    EXPECT_EQ(init(&array, nullptr, 5), NULL_TYPEINFO_ERROR);
    ASSERT_NE(array, nullptr);
}

// Array Storage Management Tests
TEST_F(CollectionTest, AddArrayToStorage) {
    ASSERT_EQ(init(&array, doubleInfo, 5), SUCCESSFUL_EXECUTION);
    ArrayStorage* storage = getStorage();
    EXPECT_EQ(storage->count, 1);
    EXPECT_EQ(storage->arrayPtrs[0], array);
}

TEST_F(CollectionTest, RemoveArrayFromStorage) {
    ASSERT_EQ(init(&array, doubleInfo, 5), SUCCESSFUL_EXECUTION);
    ArrayStorage* storage = getStorage();
    int initialCount = storage->count;
    
    EXPECT_EQ(removeArrayFromStorage(array, storage), SUCCESSFUL_EXECUTION);
    EXPECT_EQ(storage->count, initialCount - 1);
}

// Array Operations Tests
TEST_F(CollectionTest, AppendElement) {
    ASSERT_EQ(init(&array, doubleInfo, 5), SUCCESSFUL_EXECUTION);
    double value = 3.14;
    elemPtr elem = &value;
    
    EXPECT_EQ(append(array, &elem), SUCCESSFUL_EXECUTION);
    EXPECT_EQ(array->size, 1);
}

TEST_F(CollectionTest, PrependElement) {
    ASSERT_EQ(init(&array, doubleInfo, 5), SUCCESSFUL_EXECUTION);
    double value = 3.14;
    elemPtr elem = &value;
    
    EXPECT_EQ(prepend(array, &elem), SUCCESSFUL_EXECUTION);
    EXPECT_EQ(array->size, 1);
}

TEST_F(CollectionTest, PushIndexElement) {
    ASSERT_EQ(init(&array, doubleInfo, 5), SUCCESSFUL_EXECUTION);
    double value = 3.14;
    elemPtr elem = &value;
    
    EXPECT_EQ(pushIndex(array, &elem, 0), SUCCESSFUL_EXECUTION);
    EXPECT_EQ(array->size, 1);
}

// Array Resizing Tests
TEST_F(CollectionTest, ResizeArrayExtend) {
    ASSERT_EQ(init(&array, doubleInfo, 4), SUCCESSFUL_EXECUTION);
    
    // Fill array to trigger resize
    double value = 1.0;
    elemPtr elem = &value;
    for(int i = 0; i < 4; i++) {
        EXPECT_EQ(append(array, &elem), SUCCESSFUL_EXECUTION);
    }
    
    EXPECT_EQ(resize(array, EXTEND), SUCCESSFUL_EXECUTION);
    EXPECT_GT(array->capacity, 4);
}

TEST_F(CollectionTest, ResizeArrayShrink) {
    ASSERT_EQ(init(&array, doubleInfo, 16), SUCCESSFUL_EXECUTION);
    
    // Add few elements
    double value = 1.0;
    elemPtr elem = &value;
    for(int i = 0; i < 4; i++) {
        EXPECT_EQ(append(array, &elem), SUCCESSFUL_EXECUTION);
    }
    
    EXPECT_EQ(resize(array, SHRINK), SUCCESSFUL_EXECUTION);
    EXPECT_LE(array->capacity, 16);
}

// Array Concatenation Tests
TEST_F(CollectionTest, ConcatenateArrays) {
    DynamicArray *array1 = nullptr, *array2 = nullptr, *result = nullptr;
    ASSERT_EQ(init(&array1, doubleInfo, 5), SUCCESSFUL_EXECUTION);
    ASSERT_EQ(init(&array2, doubleInfo, 5), SUCCESSFUL_EXECUTION);
    
    double value = 1.0;
    elemPtr elem = &value;
    EXPECT_EQ(append(array1, &elem), SUCCESSFUL_EXECUTION);
    EXPECT_EQ(append(array2, &elem), SUCCESSFUL_EXECUTION);
    
    EXPECT_EQ(concatenate(&result, array1, array2), SUCCESSFUL_EXECUTION);
    EXPECT_EQ(result->size, 2);
}

// Array Copy Tests
TEST_F(CollectionTest, CopyArray) {
    DynamicArray *source = nullptr, *destination = nullptr;
    ASSERT_EQ(init(&source, doubleInfo, 5), SUCCESSFUL_EXECUTION);
    ASSERT_EQ(init(&destination, doubleInfo, 5), SUCCESSFUL_EXECUTION);
    
    double value = 1.0;
    elemPtr elem = &value;
    EXPECT_EQ(append(source, &elem), SUCCESSFUL_EXECUTION);
    
    EXPECT_EQ(copyArray(destination, source), SUCCESSFUL_EXECUTION);
    EXPECT_EQ(destination->size, source->size);
}

// Map and Where Tests
TEST_F(CollectionTest, MapOperation) {
    ASSERT_EQ(init(&array, doubleInfo, 5), SUCCESSFUL_EXECUTION);
    double value = 1.0;
    elemPtr elem = &value;
    EXPECT_EQ(append(array, &elem), SUCCESSFUL_EXECUTION);
    
    auto mapFunc = [](elemPtr elem) -> Exception { 
        *(double*)elem *= 2; 
        return SUCCESSFUL_EXECUTION; 
    };
    
    EXPECT_EQ(map(array, mapFunc), SUCCESSFUL_EXECUTION);
}

TEST_F(CollectionTest, WhereOperation) {
    ASSERT_EQ(init(&array, doubleInfo, 5), SUCCESSFUL_EXECUTION);
    double value = 1.0;
    elemPtr elem = &value;
    EXPECT_EQ(append(array, &elem), SUCCESSFUL_EXECUTION);
    
    auto predFunc = [](elemPtr elem) -> bool { 
        return *(double*)elem > 0; 
    };
    
    EXPECT_EQ(where(array, predFunc), SUCCESSFUL_EXECUTION);
}

// Input/Output Tests
TEST_F(CollectionTest, ReadValidInput) {
    ASSERT_EQ(init(&array, doubleInfo, 5), SUCCESSFUL_EXECUTION);
    const char* input = "1.0 2.0 3.0\n";
    
    EXPECT_EQ(readFromInput(array, input, strlen(input) + 1), SUCCESSFUL_EXECUTION);
    EXPECT_EQ(array->size, 3);
}


TEST_F(CollectionTest, ReadInvalidInput) {
    ASSERT_EQ(init(&array, doubleInfo, 5), SUCCESSFUL_EXECUTION);
    const char* input = "1.0 dhsa 3.0\n";
    
    EXPECT_EQ(readFromInput(array, input, strlen(input) + 1), UNEXPECTED_ALPHA_ERROR);
}


// Error Cases Tests
TEST_F(CollectionTest, NullArrayOperations) {
    double value = 1.0;
    elemPtr elem = &value;
    
    EXPECT_EQ(append(nullptr, &elem), MEMORY_ALLOCATION_ERROR);
    EXPECT_EQ(prepend(nullptr, &elem), MEMORY_ALLOCATION_ERROR);
    EXPECT_EQ(pushIndex(nullptr, &elem, 0), MEMORY_ALLOCATION_ERROR);
}

TEST_F(CollectionTest, InvalidIndexOperations) {
    ASSERT_EQ(init(&array, doubleInfo, 5), SUCCESSFUL_EXECUTION);
    double value = 1.0;
    elemPtr elem = &value;
    
    EXPECT_EQ(pushIndex(array, &elem, -1), SUCCESSFUL_EXECUTION);
    EXPECT_EQ(pushIndex(array, &elem, 100), SUCCESSFUL_EXECUTION);
}

// Memory Management Tests
TEST_F(CollectionTest, MemoryLeakCheck) {
    ASSERT_EQ(init(&array, doubleInfo, 5), SUCCESSFUL_EXECUTION);
    double value = 1.0;
    elemPtr elem = &value;
    EXPECT_EQ(append(array, &elem), SUCCESSFUL_EXECUTION);
    
    // Delete should free all allocated memory
    EXPECT_EQ(deleteArray(array), SUCCESSFUL_EXECUTION);
    array = nullptr; // Prevent double-free in TearDown
}