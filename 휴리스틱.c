#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>

#define MAX 100

// 도시 개수
int n;

// 좌표 구조체
typedef struct {
    double x, y;
} Point;

Point cities[MAX];
bool visited[MAX];

// 두 도시 간 거리 계산
double distance(Point a, Point b) {
    return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

// 최근접 이웃 휴리스틱
void nearest_neighbor(int start) {
    int current = start;
    visited[current] = true;
    printf("경로: %d -> ", current);

    double total_distance = 0.0;

    for (int i = 1; i < n; i++) {
        double min_dist = INT_MAX;
        int next_city = -1;

        for (int j = 0; j < n; j++) {
            if (!visited[j]) {
                double d = distance(cities[current], cities[j]);
                if (d < min_dist) {
                    min_dist = d;
                    next_city = j;
                }
            }
        }

        visited[next_city] = true;
        total_distance += min_dist;
        current = next_city;
        printf("%d -> ", current);
    }

    // 시작점으로 돌아감
    total_distance += distance(cities[current], cities[start]);
    printf("%d\n", start);
    printf("총 거리: %.2f\n", total_distance);
}

int main() {
    printf("도시 개수를 입력하세요: ");
    scanf("%d", &n);

    printf("각 도시의 좌표 (x y):\n");
    for (int i = 0; i < n; i++) {
        scanf("%lf %lf", &cities[i].x, &cities[i].y);
        visited[i] = false;
    }

    nearest_neighbor(0); // 0번 도시에서 시작

    return 0;
}
