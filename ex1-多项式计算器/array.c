#include "array.h"

array *array_new(size_t capacity)
{
    array *newarray = malloc(sizeof(array));
    if (newarray == NULL) return NULL;

    valueType *data = malloc(sizeof(valueType) * capacity);
    if (data == NULL){
        free(newarray);
        return NULL;
    }

    newarray -> data  = data;
    newarray -> size = 0;
    newarray -> capacity = capacity;

    return newarray;
}

array *array_enlarge(array *arr)
{
    size_t new_capacity = arr -> capacity * 2;
    valueType *new_data = realloc(arr -> data, sizeof(valueType) * new_capacity);
    if (new_data == NULL) return NULL;
    arr -> data = new_data;
    arr -> capacity = new_capacity;
    return arr;
}

status array_insert(array *arr, size_t index, valueType value)
{
    if (arr == NULL) return ARRAY_NULL;
    if (index > arr -> size) return ARRAY_OUT_OF_BOUNDS;
    if (arr -> size == arr ->capacity){
        if (array_enlarge(arr) == NULL) return ENLARGE_FAIL;
    }
    for (size_t i = arr -> size; i > index; i--){
        arr -> data[i] = arr -> data[i - 1];
    }
    arr -> data[index] = value;
    arr -> size++;
    return OK;
}

status array_append(array *arr, valueType value)
{
    return array_insert(arr, arr -> size, value);
}

void array_print(const array *arr)
{
    if (arr == NULL) return;
    if (arr -> size == 0){
        printf("###arr is enpty\n");
        return;
    }
    
    printf("***: ");
    for (size_t i = 0; i < arr -> size; i++){
        printf("%d ", arr -> data[i]);
    }
    printf("\n");
    return;
}

status array_remove(array *arr, size_t index)
{
    if (arr == NULL) return ARRAY_NULL;
    //删除的index必须在数组范围内, 这与插入不同
    if (index >= arr -> size){
        printf("###index is invalid, index must in(0 , %lu]\n", arr -> size);
        return ARRAY_OUT_OF_BOUNDS;
    }
    for (size_t i = index; i < arr -> size - 1; i++){
        arr -> data[i] = arr -> data [i + 1];
    }
    arr -> size--;
    return OK;
}

status array_get(array *arr, size_t index, valueType *value)
{
    if (arr == NULL){
        printf("###arr is NULL\n");
        return ARRAY_NULL;
    }
    if (index >= arr -> size){
        printf("###index is invalid, index must in(0 , %lu]\n", arr -> size);
        return ARRAY_OUT_OF_BOUNDS;
    }
    *value = arr -> data[index];
    return OK;
}

status array_set(array *arr, size_t index, valueType value)
{
    if (arr == NULL){
        printf("###arr is NULL\n");
        return ARRAY_NULL;
    }
    if (index >= arr -> size){
        printf("###index is invalid, index must in(0 , %lu]\n", arr -> size);
        return ARRAY_OUT_OF_BOUNDS;
    }
    arr -> data[index] = value;
    return OK;
}

status array_destroy(array *arr)
{
    if (arr == NULL){
        printf("###arr is NULL\n");
        return ARRAY_NULL;
    }
    free(arr -> data);
    free(arr);
    return OK;
}

status array_clear(array *arr)
{
    if (arr == NULL){
        printf("###arr is NULL\n");
        return ARRAY_NULL;
    }

    arr -> size = 0;
    return OK;
}