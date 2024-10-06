#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <algorithm>
using namespace std;

/*
 * input case:
8 11 1 3
1 2
1 3
2 3
3 4
3 5
4 5
5 6
6 7
7 8
1 8
2 7
 */

/*
 * unordered_map<int, int> similarity; 用于存储用户的好友关系。
 * Key : Value
 * 5 : 1
 * 4 : 1
 * 7 : 2
 */


/**
 * User 结构体，用于存储用户编号和相似度
 * 
 * id                           表示用户的编号（ID），即社交网络中用户的唯一标识。
 * similarity                   表示用户与用户K的相似度，即用户K的好友中有多少个是该用户的好友；
 *                              相似度越高，表示他们有越多的共同好友。
 * User(int id, int similarity) 含参构造函数，创建User对象时直接赋值给 id 和 similarity 这两个成员变量。
 *                              User user(3, 5); 表示用户编号为3，与用户K的相似度为5。
 * bool operator<(const User& other) const;               
 *                              重载小于号运算符，用于自定义排序规则，相似度高优先，相似度相同时编号小优先。
 *                              用于实现两个用户对象之间的比较，用于排序推荐好友列表。
 *                              (const User& other) 表示传入的参数是一个 User 类型的对象。
 *                              const         修饰函数，表示该函数不会修改成员变量的值。
 *                              bool          返回值类型。
 *                              return true   表示当前对象小于传入的对象。
 *                              return false  表示当前对象大于传入的对象。
 */
struct User {
    int id, similarity;
	User(int id, int similarity) : id(id), similarity(similarity) {}
	// 自定义排序规则：相似度高优先，相似度相同时编号小优先
	bool operator<(const User& other) const {
		if (similarity == other.similarity)
		    // 如果两个用户的相似度相同，选择 id 值较小的用户排在前面。
            return id < other.id;
		return similarity > other.similarity;
	}
};

int main() {
	int N, M, K, L;
	cin >> N >> M >> K >> L;
	
	vector<set<int>> friends(N + 1); // 邻接表，存储每个用户的好友列表
	
	// 输入好友关系
	for (int i = 0; i < M; ++i) {
		int X, Y;
		cin >> X >> Y;
		friends[X].insert(Y);
		friends[Y].insert(X);
	}
	
	unordered_map<int, int> similarity; // 用于存储与用户K的相似度
	
	// 遍历K的所有好友，找出与K有共同好友的用户
	if (friends[K].size() > 0) { // 特殊情况2: 如果K没有好友，跳过这一步
		for (int friendOfK : friends[K]) {
			// 计算K的每个好友的好友
			for (int potentialFriend : friends[friendOfK]) {
				// 如果这个用户不是K本身且不是K的好友，则增加相似度
				if (potentialFriend != K && !friends[K].count(potentialFriend)) {
					similarity[potentialFriend]++;
				}
			}
		}
	}

	// 将结果放入一个用户结构体数组中
	vector<User> potentialFriends;
	for (int i = 1; i <= N; ++i) {
		if (i != K && !friends[K].count(i)) { // 不是自己且不是好友
            // 如果 similarity 中存在 i，则取出 i 的相似度，否则相似度为 0
			int sim = similarity.count(i) ? similarity[i] : 0; 
			// 将用户编号和相似度放入数组中
            potentialFriends.push_back(User(i, sim));
		}
	}
	
	// 对所有潜在好友进行排序
	sort(potentialFriends.begin(), potentialFriends.end());
	
	vector<int> result;
	int recommended = 0; // 记录已推荐的好友数量
	
	// 按照相似度高低输出潜在好友
	for (const User& user : potentialFriends) {
		if (recommended < L) {
			result.push_back(user.id);
			recommended++;
		} else {
			break;
		}
	}
	
	// 如果推荐不足L个，补充陌生人（没有与用户K共同好友的非好友）
	for (int i = 1; i <= N && recommended < L; ++i) {
		if (i != K && !friends[K].count(i) && similarity.count(i) == 0) {
			if (find(result.begin(), result.end(), i) == result.end()) { // 避免重复推荐
				result.push_back(i);
				recommended++;
			}
		}
	}
	
	// 如果仍不足L个，补充0
	while (recommended < L) {
		result.push_back(0);
		recommended++;
	}
	
	// 输出结果
	for (auto i = 0u; i < result.size(); ++i) {
		if (i > 0) cout << " ";
		cout << result[i];
	}
	cout << endl;
	
	return 0;
}