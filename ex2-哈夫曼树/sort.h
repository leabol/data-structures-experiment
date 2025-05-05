int partition(int **arr, int lo, int hi)
{
    int i = lo, j = hi + 1;
    int v = arr[lo][1];
    while (1){
        while (arr[++i][1] < v) if (i == hi) break;
        while (arr[--j][1] > v) if (j == lo) break;
        if (i >= j) break;

        int a = arr[i][0];
        int b = arr[i][1];
        arr[i][0] = arr[j][0];
        arr[i][1] = arr[j][1];
        arr[j][0] = a;
        arr[j][1] = b;
    }
        int a = arr[lo][0];
        int b = arr[lo][1];
        arr[lo][0] = arr[j][0];
        arr[lo][1] = arr[j][1];
        arr[j][0] = a;
        arr[j][1] = b;

        return j;
}
void sort(int **arr, int lo, int hi)
{
    if (hi <= lo) return;
    int j = partition(arr, lo, hi);

    sort(arr, lo, j-1);
    sort(arr, j+1, hi);

}