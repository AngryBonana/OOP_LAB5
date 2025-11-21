#include <iostream>
#include "../include/FixedBlockMemoryResource.h"
#include "../include/DoublyLinkedList.h"

struct Point {
    int x = 0, y = 0;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        return os << "(" << p.x << "," << p.y << ")";
    }
};

int main() {
    FixedBlockMemoryResource mr_int{4096, sizeof(ListNode<int>)};
    FixedBlockMemoryResource mr_point{4096, sizeof(ListNode<Point>)};

    std::cout << "=== int ===" << std::endl;
    {
        DoublyLinkedList<int> list(&mr_int);
        list.push_back(1);
        list.push_back(2);
        list.push_front(0);
        for (int x : list) std::cout << x << " ";
        std::cout << std::endl << "Size: " << list.size() << std::endl << std::endl;
    }

    std::cout << "=== Point ===" << std::endl;
    {
        DoublyLinkedList<Point> list(&mr_point);
        list.push_back({1, 1});
        list.push_back({2, 2});
        list.push_front({0, 0});
        for (const auto& p : list) std::cout << p << " ";
        std::cout << std::endl << "Size: " << list.size() << std::endl;
    }

    return 0;
}