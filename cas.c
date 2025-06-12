#include <stdio.h>
#include <stdbool.h>

// 비원자적 모의 compare_and_swap (단일 스레드용 예시)
bool compare_and_swap_naive(int *ptr, int expected, int newval) {
    if (*ptr == expected) {
        *ptr = newval;
        return true;
    }
    return false;
}

int main() {
    int val = 10;
    int expected = 10;
    int newval = 20;

    if (compare_and_swap_naive(&val, expected, newval)) {
        printf("CAS 성공: %d\n", val);
    } else {
        printf("CAS 실패: %d\n", val);
    }

    return 0;
}
