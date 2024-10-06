#include <iostream>
#include <vector>
#include <deque>
#include <unordered_map>
#include <algorithm>

using namespace std;

int main() { 
    int n, m;
    cin >> n;

    vector<int> nums1(n);
    for (int i = 0; i < n; ++i) { 
        cin >> nums1[i];
    }

    cin >> m;
    vector<int> nums2(m);
    for (int i = 0; i < m; ++i) { 
        cin >> nums2[i];
    }

    // 定义一个二维哈希表，用于统计参照二叉树每层结点的出现次数
    vector<unordered_map<int, int>> nums2_cnt(1001);
    deque<int> q; // 用于层序遍历的队列

    q.push_back(0); // 根结点的层号为 0
    int depth = 0; // 当前结点的深度

    // 统计参照二叉树每层结点的出现次数
    while (!q.empty()) { 
        int length = q.size(); // 当前层的结点个数
        for (int i = 0; i < length; ++i) { 
            int node = q.front(); // 取队首元素
            q.pop_front(); // 出队
            nums2_cnt[depth][nums2[node]]++; // 当前结点的出现次数加 1
            if (node * 2 + 1 < m) { 
                q.push_back(node * 2 + 1); // 左孩子入队
                q.push_back(node * 2 + 2); // 右孩子入队
            }
        }
        depth++;
    }

    // 使用哈希表统计原始二叉树的结点出现次数
    unordered_map<int, int> nums1_cnt;
    for (int num : nums1) {
        nums1_cnt[num]++; 
    }

    // 遍历原始二叉树，逐层处理并减去参照树对应层的结点
    q.push_back(0); // 根结点的层号为 0
    depth = 0; // 当前结点的深度

    while (!q.empty()) {
        int length = q.size();
        unordered_map<int, int> cur; // 临时哈希表，用于存储当前层结点的出现次数

        for (int i = 0; i < length; ++i) { 
            int node = q.front(); // 取队首元素
            q.pop_front(); // 出队
            cur[nums1[node]]++; // 当前结点的出现次数加 1
            if (node * 2 + 1 < n) { 
                q.push_back(node * 2 + 1); // 左孩子入队
                q.push_back(node * 2 + 2); // 右孩子入队
            }
        }

        for (auto &kv : cur) { 
            int k = kv.first;
            int v = kv.second;
            if (nums2_cnt[depth][k] >= 0) { 
                nums1_cnt[k] = max(nums1_cnt[k] - nums2_cnt[depth][k], 0); // 减去参照树对应层的结点
            }
        }
        depth++;
    }

    vector<pair<int, int>> sorted_dic(nums1_cnt.begin(), nums1_cnt.end());
    sort(sorted_dic.begin(), sorted_dic.end(), [](const pair<int, int> &a, const pair<int, int> &b) { 
        if (a.second != b.second)
            return a.second > b.second;
        return a.first > b.first;
    });

    // 输出结果
    for (auto &p : sorted_dic) {
        if (p.second > 0)
            cout << p.first;
    }
    cout << endl;

    return 0;
}