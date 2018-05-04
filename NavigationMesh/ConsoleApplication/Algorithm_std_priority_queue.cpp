#include <functional>
#include <queue>
#include <vector>
#include <iostream>
 
template<typename T> void print_queue(T& q) {
    while(!q.empty()) {
        std::cout << q.top() << " ";
        q.pop();
    }
    std::cout << '\n';
}
 
int main() {
    std::priority_queue<int> q;
 
    for(int n : {1,8,5,6,3,4,0,9,7,2}) // generalized range-for
        q.push(n);
 
    print_queue(q);
 
    std::priority_queue<int, std::vector<int>, std::greater<int> > q2;
 
    for(int n : {1,8,5,6,3,4,0,9,7,2})
        q2.push(n);
 
    print_queue(q2);
 
    // Using lambda to compare elements.
    auto cmp = [](int left, int right) { return (left ^ 1) < (right ^ 1);};
    std::priority_queue<int, std::vector<int>, decltype(cmp)> q3(cmp);
 
    for(int n : {1,8,5,6,3,4,0,9,7,2})
        q3.push(n);
 
    print_queue(q3);
    /** output:
        9 8 7 6 5 4 3 2 1 0 
        0 1 2 3 4 5 6 7 8 9 
        8 9 6 7 4 5 2 3 0 1
    */
}
