#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define ROW 10
#define COL 10

typedef struct {
    int parentRow, parentCol;
    double f, g, h;
    bool isClosed;
} Cell;

typedef struct {
    int row, col;
} Point;

// 방향 벡터 (상, 하, 좌, 우, 대각선 포함)
int dRow[8] = {-1, 1, 0, 0, -1, -1, 1, 1};
int dCol[8] = {0, 0, -1, 1, -1, 1, -1, 1};

// 맨해튼 거리 휴리스틱
double heuristic(int row, int col, int destRow, int destCol) {
    return abs(row - destRow) + abs(col - destCol);
}

// 경로 출력 함수
void tracePath(Cell cellDetails[ROW][COL], Point dest) {
    printf("최단 경로: \n");
    int row = dest.row;
    int col = dest.col;

    while (!(cellDetails[row][col].parentRow == row && cellDetails[row][col].parentCol == col)) {
        printf("-> (%d,%d) ", row, col);
        int tempRow = cellDetails[row][col].parentRow;
        int tempCol = cellDetails[row][col].parentCol;
        row = tempRow;
        col = tempCol;
    }
    printf("-> (%d,%d)\n", row, col);
}

// 유효한 좌표인지 확인
bool isValid(int row, int col, int grid[ROW][COL]) {
    return (row >= 0) && (row < ROW) && (col >= 0) && (col < COL) && (grid[row][col] == 0);
}

void aStar(int grid[ROW][COL], Point start, Point dest) {
    if (!isValid(start.row, start.col, grid) || !isValid(dest.row, dest.col, grid)) {
        printf("시작 또는 도착 위치가 유효하지 않습니다.\n");
        return;
    }

    bool closedList[ROW][COL] = { false };
    Cell cellDetails[ROW][COL];

    // 초기화
    for (int i = 0; i < ROW; i++)
        for (int j = 0; j < COL; j++) {
            cellDetails[i][j].f = cellDetails[i][j].g = cellDetails[i][j].h = INFINITY;
            cellDetails[i][j].parentRow = i;
            cellDetails[i][j].parentCol = j;
        }

    int i = start.row;
    int j = start.col;
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;

    Point openList[ROW * COL];
    int openSize = 0;

    openList[openSize++] = start;

    while (openSize > 0) {
        // 가장 낮은 f 값을 가지는 노드 선택
        int lowestIdx = 0;
        for (int k = 1; k < openSize; k++) {
            int r = openList[k].row, c = openList[k].col;
            if (cellDetails[r][c].f < cellDetails[openList[lowestIdx].row][openList[lowestIdx].col].f) {
                lowestIdx = k;
            }
        }

        Point current = openList[lowestIdx];

        // openList에서 제거
        openList[lowestIdx] = openList[--openSize];
        closedList[current.row][current.col] = true;

        // 목적지에 도달했는지 확인
        if (current.row == dest.row && current.col == dest.col) {
            tracePath(cellDetails, dest);
            return;
        }

        // 8방향 이웃 확인
        for (int dir = 0; dir < 8; dir++) {
            int newRow = current.row + dRow[dir];
            int newCol = current.col + dCol[dir];

            if (isValid(newRow, newCol, grid) && !closedList[newRow][newCol]) {
                double gNew = cellDetails[current.row][current.col].g + 1.0;
                double hNew = heuristic(newRow, newCol, dest.row, dest.col);
                double fNew = gNew + hNew;

                if (cellDetails[newRow][newCol].f == INFINITY || fNew < cellDetails[newRow][newCol].f) {
                    cellDetails[newRow][newCol].f = fNew;
                    cellDetails[newRow][newCol].g = gNew;
                    cellDetails[newRow][newCol].h = hNew;
                    cellDetails[newRow][newCol].parentRow = current.row;
                    cellDetails[newRow][newCol].parentCol = current.col;

                    openList[openSize++] = (Point){newRow, newCol};
                }
            }
        }
    }

    printf("경로를 찾을 수 없습니다.\n");
}

int main() {
    // 0: 빈 공간, 1: 장애물
    int grid[ROW][COL] = {
        { 0, 1, 0, 0, 0, 0, 0, 1, 0, 0 },
        { 0, 1, 0, 1, 1, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 1, 0, 1, 0, 0, 0 },
        { 0, 1, 1, 0, 0, 0, 0, 0, 1, 0 },
        { 0, 1, 0, 0, 1, 1, 1, 0, 1, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
        { 1, 1, 1, 1, 1, 1, 0, 1, 1, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    };

    Point start = {0, 0};
    Point dest = {9, 9};

    aStar(grid, start, dest);

    return 0;
}
