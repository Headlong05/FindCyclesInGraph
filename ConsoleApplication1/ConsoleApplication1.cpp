#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <list>
#include <sstream>
#include <unordered_set> // Для использования std::unordered_set

class Graph {
private:
    struct Edge {
        int to;
        int weight;
    };

    std::unordered_map<int, std::string> vertices; // Список вершин (узлов)
    std::unordered_map<int, std::list<Edge>> adjacencyList; // Список дуг

public:
    void ADD_V(int id, const std::string& label) {
        vertices[id] = label;
    }

    void ADD_E(int from, int to, int weight) {
        adjacencyList[from].push_back({ to, weight });
    }

    void DEL_V(int id) {
        vertices.erase(id);
        adjacencyList.erase(id);
        for (auto& pair : adjacencyList) {
            pair.second.remove_if([id](const Edge& edge) { return edge.to == id; });
        }
    }

    void DEL_E(int from, int to) {
        if (adjacencyList.find(from) != adjacencyList.end()) {
            adjacencyList[from].remove_if([to](const Edge& edge) { return edge.to == to; });
        }
    }

    void EDIT_V(int id, const std::string& newLabel) {
        if (vertices.find(id) != vertices.end()) {
            vertices[id] = newLabel;
        }
    }

    void EDIT_E(int from, int to, int newWeight) {
        if (adjacencyList.find(from) != adjacencyList.end()) {
            for (auto& edge : adjacencyList[from]) {
                if (edge.to == to) {
                    edge.weight = newWeight;
                    break;
                }
            }
        }
    }

    int FIRST(int v) {
        if (adjacencyList.find(v) != adjacencyList.end() && !adjacencyList[v].empty()) {
            return adjacencyList[v].begin()->to;
        }
        return -1; // signifies "нулевая" вершина
    }

    void findCyclesUtil(int v, std::unordered_map<int, bool>& visited, std::vector<int>& path, int length) {
        visited[v] = true;
        path.push_back(v);

        // Проверяем, достигли ли нужной длины пути
        if (path.size() == length) {
            // Проверяем, замыкается ли цикл на начальной вершине
            if (path[path.size() - 1] == path[0]) {
                // Используем set для проверки уникальности вершин в пути
                std::unordered_set<int> uniqueVertices;

                bool hasDuplicates = false;
                for (int i = 0; i < path.size(); i++) {
                    // Если это не начальная вершина, проверяем на дубли
                    if (i != 0 && i == path.size() - 1)  // Пропускаем начальную вершину (последний элемент)
                        continue;

                    if (uniqueVertices.find(path[i]) != uniqueVertices.end()) {
                        hasDuplicates = true;
                        break;
                    }
                    uniqueVertices.insert(path[i]);
                }

                if (!hasDuplicates) {
                    std::cout << "Найденный цикл: ";
                    for (int node : path) {
                        std::string alf = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
                        std::cout << alf[node-1] << " ";
                        //Вторая строка для вывода номеров, вместо букв
                    }
                    std::cout << std::endl;
                }
            }
        }
        else {
            // Если еще не достигли нужной длины, продолжаем обход
            if (adjacencyList.find(v) != adjacencyList.end()) {
                for (const auto& edge : adjacencyList[v]) {
                    int neighbor = edge.to;
                    // Проверяем, не посещен ли сосед или если это начальная вершина и у нас еще не полный цикл
                    if ((std::find(path.begin(), path.end(), neighbor) == path.end()) ||
                        (path.size() < length && neighbor == path[0])) {
                        findCyclesUtil(neighbor, visited, path, length);
                    }
                }
            }
        }

        // Убираем вершину из пути и помечаем, что она не посещена
        path.pop_back();
        visited[v] = false;
    }



    void findCycles(int length) {

        std::unordered_map<int, bool> visited; // Обновлено на unordered_map
        std::vector<int> path;

        for (auto pair : vertices) {
            visited.clear(); // Сбрасываем состояние посещения перед каждой итерацией
            findCyclesUtil(pair.first, visited, path, length);
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    Graph graph;

    std::string input;
    std::vector<std::vector<int>> table;

    // Чтение всей таблицы целиком
    std::cout << "Введите таблицу(пустая строка для конца):" << std::endl;
    while (true) {
        std::string line;
        std::getline(std::cin, line);
        if (line.empty()) break; // Завершение ввода при пустой строке

        // Используем istringstream для разбивки строки на числа
        std::istringstream iss(line);
        std::vector<int> row;
        int num;

        while (iss >> num) {
            row.push_back(num); // Заполнение текущей строки числами
        }

        table.push_back(row); // Добавление строки в таблицу
    }
    std::string alf = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < table.size(); ++i) {
        graph.ADD_V(i + 1, std::string(1 + i, alf[i]));
    }

    // Проход по строкам и элементам таблицы
    for (size_t i = 0; i < table.size(); ++i) {
        for (size_t j = 0; j < table[i].size(); ++j) {
            if (table[i][j] != 0) {
                graph.ADD_E(i + 1, j + 1, table[i][j]);
            }
        }
    }


    int length;
    std::cout << "Введите длинну: ";
    std::cin >> length;

    graph.findCycles(length+1);

    return 0;
}


