#include <iostream>
#include <algorithm>
#include <vector>

int main()
{
    std::vector<int> v{ 4, 1, 3, 2, 16, 9, 10, 14, 8, 7 };

    std::make_heap(v.begin(), v.end());

    std::cout << "v: ";
    for (auto i : v) std::cout << i << ' ';
    std::cout << '\n';

    std::pop_heap(v.begin(), v.end()); // moves the largest to the end

    std::cout << "after pop_heap: ";
    for (auto i : v) std::cout << i << ' ';
    std::cout << '\n';

    int largest = v.back();
    v.pop_back();  // actually removes the largest element
    std::cout << "largest element: " << largest << '\n';

    std::cout << "heap without largest: ";
    for (auto i : v) std::cout << i << ' ';
    std::cout << '\n';

    //Pushes the node onto the back of the vector (the heap is now unsorted)
    std::cout << "after push heap: ";
    v.push_back(12);
    //Sorts the new element into the heap
    std::push_heap(v.begin(), v.end());

    for (auto i : v) std::cout << i << ' ';
    std::cout << '\n';

    /** output:
        v: 16 14 10 8 7 9 3 2 4 1
        after pop_heap: 14 8 10 4 7 9 3 2 1 16
        largest element: 16
        heap without largest: 14 8 10 4 7 9 3 2 1
        after push heap: 14 12 10 4 8 9 3 2 1 7
    */
}
