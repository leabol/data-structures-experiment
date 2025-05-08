void sort(int **arr, int len)
{
    for (int i = 0; i < len; i++){
        for (int j = 0; j < len - i- 1; j++){
            if (arr[j][1] > arr[j+1][1]){
               int a = arr[j][0]; 
               int b = arr[j][1]; 
               arr[j][0] = arr[j+1][0];
               arr[j][1] = arr[j+1][1];
               arr[j+1][0] = a;
               arr[j+1][1] = b;
            }
        }
    }
}