#include <stdio.h>
#include <malloc.h>

int main(int argc, char *argv[]) {
    int n;
    scanf("%d", &n);
    int i, j;
    if (n < 1) return 0;
    int **arr = (int **) malloc((n + 1) * sizeof(int *));
    for (i = 0; i <= n; i++) {
        arr[i] = (int *) malloc((n + 1) * sizeof(int));
    }
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= n; j++) {
            arr[i][j] = i * j;
        }
    }
    while (1) {
        int x1, y1, x2, y2;
        scanf("%d", &x1);
        if (x1 == 0) {
            for (i = 0; i <= n; i++) {
                free(arr[i]);
            }
            free(arr);
            return 0;
        }
        scanf("%d%d%d", &y1, &x2, &y2);
        if ( x1 < 1 || x2 < 1 || y1 < 1 || y2 < 1 || x1 > x2 || y1 > y2
                || x1 > n || x2 > n || y1 > n || y2 > n){
            for (i = 0; i <= n; i++) {
                free(arr[i]);
            }
            free(arr);
            return 0;
        }
        for (i = y1; i <= y2; ++i) {
            for (j = x1; j <= x2; ++j) {
                printf("%d ", arr[i][j]);
            }
            printf("\n");
        }

    }
}
