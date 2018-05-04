#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <unordered_set>

struct KIntPoint3D
{
public:
    KIntPoint3D(int x_ = 0, int y_ = 0, int z_ = 0)
    {
        x = x_;
        y = y_;
        z = z_;
    }

    bool operator==(const KIntPoint3D& rhs) const
    {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }

    int x;
    int y;
    int z;
};

struct AStarNode;
typedef std::shared_ptr<AStarNode>  AStarNodePtr;
struct AStarNode
{
    AStarNodePtr        m_parent; // parent node (zero pointer represents starting node)

    KIntPoint3D         m_location; // location of node
    float               m_cost; // cost to get to this node
    float               m_heuristic; // heuristic cost to get to goal node
    float               m_total; // total cost (cost + heuristic estimate)
    bool                m_isOpen; // on Open list
    bool                m_isClosed; // on Closed list

    // must have default constructor for A* internal structure
    AStarNode(int x = 0, int y = 0, int z = 0)
    {
        m_location = KIntPoint3D(x, y, z);
        m_parent = nullptr;
        m_cost = 0.0f;
        m_heuristic = 0.0f;
        m_total = 0.0f;
        m_isOpen = false;
        m_isClosed = false;
    }

    bool operator==(const AStarNode& rhs_) const
    {
        return m_location == rhs_.m_location;
    }

    void Print() const
    {
        printf(" location = (%i, %i, %i)\n", m_location.x, m_location.y, m_location.z);
        printf(" cost = %g\n", m_cost);
        printf(" heuristic = %g\n", m_heuristic);
        printf(" total = %g\n", m_total);
        printf(" open=%d, closed=%d\n", m_isOpen, m_isClosed);
    }//Print()

    void PrintLocation() const
    {
        printf("(%i, %i, %i)", m_location.x, m_location.y, m_location.z);
    }//Print()
};

class AStarNodeHash
{
public:
    size_t operator()(const AStarNodePtr node) const
    {
        size_t h1 = std::hash<int>()(node->m_location.x);
        size_t h2 = std::hash<int>()(node->m_location.y);
        size_t h3 = std::hash<int>()(node->m_location.z);
        return h1 ^ (h2 << 8) ^ (h3 << 16);
    }
};//class AStarNodeHash

struct AStarNodeEqualTo
{
    bool operator()(const AStarNodePtr& _Left, const AStarNodePtr& _Right) const
    {
        return (*_Left == *_Right);
    }
};

void main() {
    std::unordered_set<AStarNodePtr, AStarNodeHash, AStarNodeEqualTo> uset;
    AStarNodePtr   node;
    node.reset(new AStarNode(1, 0, 0));
    uset.insert(node);
    node.reset(new AStarNode(3, 0, 0));
    uset.insert(node);
    node.reset(new AStarNode(5, 0, 0));
    uset.insert(node);

    AStarNodePtr   node2;
    node2.reset(new AStarNode(3, 0, 0));
    auto sitor = uset.find(node2);
    if (sitor != uset.end())
    {
        (*sitor)->Print();
    }//if
}
