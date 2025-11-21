#include <gtest/gtest.h>
#include "../include/FixedBlockMemoryResource.h"
#include "../include/DoublyLinkedList.h"

// Тестовая структура
struct Point {
    int x = 0, y = 0;
    Point() = default;
    Point(int x, int y) : x(x), y(y) {}
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

// Проверка работы с int
TEST(DoublyLinkedListTest, PushBackAndFrontInt) {
    FixedBlockMemoryResource mr{4096, sizeof(ListNode<int>)};
    DoublyLinkedList<int> list(&mr);

    list.push_back(10);
    list.push_back(20);
    list.push_front(5);

    std::vector<int> expected = {5, 10, 20};
    std::vector<int> actual;
    for (const auto& val : list) {
        actual.push_back(val);
    }

    ASSERT_EQ(actual, expected);
    ASSERT_EQ(list.size(), 3);
}

// Проверка работы с Point
TEST(DoublyLinkedListTest, PushBackPoint) {
    FixedBlockMemoryResource mr{4096, sizeof(ListNode<int>)};
    DoublyLinkedList<Point> list(&mr);

    list.push_back({1, 2});
    list.push_back({3, 4});
    list.push_front({0, 0});

    std::vector<Point> expected = {{0, 0}, {1, 2}, {3, 4}};
    std::vector<Point> actual;
    for (const auto& p : list) {
        actual.push_back(p);
    }

    ASSERT_EQ(actual, expected);
    ASSERT_EQ(list.size(), 3);
}

// Проверка очистки
TEST(DoublyLinkedListTest, Clear) {
    FixedBlockMemoryResource mr{4096, sizeof(ListNode<int>)};
    DoublyLinkedList<int> list(&mr);

    list.push_back(1);
    list.push_back(2);
    ASSERT_EQ(list.size(), 2);

    list.clear();
    ASSERT_EQ(list.size(), 0);
    ASSERT_TRUE(list.begin() == list.end());
}

// Проверка итератора: begin == end для пустого списка
TEST(DoublyLinkedListTest, EmptyListIterator) {
    FixedBlockMemoryResource mr{4096, sizeof(ListNode<int>)};
    DoublyLinkedList<int> list(&mr);
    ASSERT_TRUE(list.begin() == list.end());
}

// Проверка корректного освобождения памяти 
TEST(DoublyLinkedListTest, MemoryReusedAfterClear) {
    FixedBlockMemoryResource mr{4096, sizeof(ListNode<int>)};

    {
        DoublyLinkedList<int> list(&mr);
        for (int i = 0; i < 100; ++i) {
            list.push_back(i);
        }
        
    }

    {
        DoublyLinkedList<int> list2(&mr);
        for (int i = 0; i < 100; ++i) {
            list2.push_back(i * 10);
        }
        ASSERT_EQ(list2.size(), 100);
    }
}

// Проверка соответствия итератора forward_iterator
TEST(DoublyLinkedListTest, IteratorCategory) {
    using It = DoublyLinkedList<int>::iterator;
    static_assert(std::is_same_v<
        std::iterator_traits<It>::iterator_category,
        std::forward_iterator_tag
    >);
}