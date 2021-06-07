#include "Inc.h"

/// <summary>
/// Kiểm tra file có tồn tại hay không
/// </summary>
/// <param name="fileName"></param>
/// <returns></returns>
bool checkFileExist(const char* fileName) {
	struct stat buffer;
	return stat(fileName, &buffer) == 0;
}

struct Edge {
	int v;
	double weight;

	Edge(const int& _v, const double _weight) {
		v = _v;
		weight = _weight;
	}
};

struct DijkstraResult {
	int source, target;
	int* parent;
	double* distance;

	//Khởi tạo kết quả dijkstra gồm 2 mảng parent[u] với distance[u] với kích cỡ _size
	//_size là tổng số đỉnh
	//source là đỉnh xuất phát
	//target là đỉnh đến
	//parent[u]: cha của u trên cây dijkstra
	//distance[u]: giá trị đường đi ngắn nhất đến đỉnh u
	DijkstraResult(const int& _size, const int& _source, const int& _target) {
		parent = new int[_size];
		distance = new double[_size];
		source = _source;
		target = _target;

		//mặc định parent[u] = -1 với mọi u
		for (int u = 0; u < _size; ++u)
			parent[u] = -1;
		//mặc định distance[u] = oo với mọi u trừ _source
		for (int u = 0; u < _size; ++u)
			distance[u] = DOUBLE_INF;
		distance[_source] = 0.0000;
	}
};

struct Graph {
	int n, m;
	//Edge* edges = NULL;
	vector<Edge>* adj = NULL;

	//----------------------------------------------Model
	/// <summary>
	/// Đọc dữ liệu đầu vào danh sách kề, trả về true nếu đọc được và ngược lạ
	/// </summary>
	bool readFromFile(const char* fileName) {
		if (!checkFileExist(fileName)) {
			return false; //File không tồn tại
		}
		if (adj != NULL)
			delete[] adj;
		ifstream input(fileName);
		input >> n >> m;
		adj = new vector<Edge>[this->n];
		for (int i = 0; i < this->m; ++i) {
			int u, v;
			double w;
			input >> u >> v >> w;
			adj[u].emplace_back(Edge(v, w));
		}
		input.close();
		return true;
	}

	DijkstraResult runDijkstra(const int& source, const int& target) {
		DijkstraResult result(this->n, source, target);

		//Gọi S là tập đỉnh đã được tối ưu, ban đầu S là rỗng
		//inHeap[u] đánh dấu đỉnh u đã được tối ưu hay chưa? Tức là u có thuộc S hay không?
		bool* inHeap = new bool[this->n];
		for (int u = 0; u < this->n; ++u) {
			inHeap[u] = false; //Mặc định là KHÔNG
		}

		//chạy dijkstra
		while (true) {
			//Tìm một đỉnh u mà u không nằm trong S và distance[u] là nhỏ nhất
			//Nếu có nhiều u như vậy thì chọn u có chỉ số nhỏ nhất
			//Tiện cho tiện tính toán, ta giả sử u = -1 (tức là không tìm thấy)
			int u = -1;
			for (int v = 0; v < this->n; ++v) {
				if (!inHeap[v]) {
					if (u == -1) {
						u = v;
					}
					else if (result.distance[u] > result.distance[v]) {
						u = v;
					}
				}
			}
			
			//Nếu tìm thấy đỉnh u như thế
			if (u >= 0) {
				//Đánh dấu u đã được tối ưu, cho u vào S
				inHeap[u] = true;

				//Cực tiểu hóa các distance[v] mà có đỉnh v chưa nằm trong S và có cung nối từ u đến v
				for (int i = 0; i < int(adj[u].size()); ++i) {
					int v = adj[u][i].v;
					double weight = adj[u][i].weight;
					if (result.distance[v] > result.distance[u] + weight) {
						result.distance[v] = result.distance[u] + weight;
						result.parent[v] = u;
					}
				}
			}
			else {
				//Ngược lại thì thoát khỏi vòng lặp
				break;
			}
		}

		return result;
	}

	//------------------------------------------View
	void DijkstraResultView(DijkstraResult result) {
		int source = result.source, target = result.target;
		if (result.distance[target] == DOUBLE_INF) {
			printf("---> Khong ton tai duong di tu %d den %d\n", source, target);
		}
		else {
			printf("---> Ton tai duong di tu %d den %d\n", source, target);
			printf("---> Chi phi %lf\n", result.distance[target]);
			vector<int> st;
			int temp = target;
			while (temp != -1) {
				st.emplace_back(temp);
				temp = result.parent[temp];
			}
			printf("---> Duong di:\n");
			reverse(st.begin(), st.end()); //Đảo ngược lại
			for (int i = 0; i < int(st.size()) - 1; ++i) {
				printf("-----> Di tu dinh %d den %d ton chi phi: %lf\n",
					st[i],
					st[i + 1],
					result.distance[st[i + 1]] - result.distance[st[i]]);
			}
		}
	}
};

int main(int argc, char* argv[]) {
	Graph g;
	int source, target;
	if (!g.readFromFile("graph.txt")) {
		printf("Khong mo duoc file ");
		return -1;
	}
	printf("Nhap dinh xuat phat: ");
	scanf("%d", &source);
	printf("Nhap dinh ket thuc: ");
	scanf("%d", &target);
	DijkstraResult result = g.runDijkstra(source, target);
	g.DijkstraResultView(result);
	return 0;
}

