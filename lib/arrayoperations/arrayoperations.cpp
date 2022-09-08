#include <arrayoperations.h>
void reversearr(uint8_t (*arr)[LENGTHOF2NDARRAY], size_t len)
{
    for (size_t i = 0; i < len / 2; i++)
    {
        uint8_t temp[LENGTHOF2NDARRAY];

        for (size_t j = 0; j < LENGTHOF2NDARRAY; j++)
        {
            temp[j] = arr[i][j];
        }
        for (size_t j = 0; j < LENGTHOF2NDARRAY; j++)
        {
            arr[i][j] = arr[len - i - 1][j];
        }
        for (size_t j = 0; j < LENGTHOF2NDARRAY; j++)
        {
            arr[len - i - 1][j] = temp[j];
        }
    }
}

void rotatearrleft(uint8_t (*arr)[LENGTHOF2NDARRAY], size_t n, size_t len)
{
    reversearr(arr, n);
    reversearr(arr + n, len - n);
    reversearr(arr, len);
}

void rotatearrright(uint8_t (*arr)[LENGTHOF2NDARRAY], size_t n, size_t len)
{
    reversearr(arr, len);
    reversearr(arr + n, len - n);
    reversearr(arr, n);
}

void reversearr(int (*arr)[LENGTHOF2NDARRAY], size_t len)
{
    for (size_t i = 0; i < len / 2; i++)
    {
        int temp[LENGTHOF2NDARRAY];

        for (size_t j = 0; j < LENGTHOF2NDARRAY; j++)
        {
            temp[j] = arr[i][j];
        }
        for (size_t j = 0; j < LENGTHOF2NDARRAY; j++)
        {
            arr[i][j] = arr[len - i - 1][j];
        }
        for (size_t j = 0; j < LENGTHOF2NDARRAY; j++)
        {
            arr[len - i - 1][j] = temp[j];
        }
    }
}

void rotatearrleft(int (*arr)[LENGTHOF2NDARRAY], size_t n, size_t len)
{
    reversearr(arr, n);
    reversearr(arr + n, len - n);
    reversearr(arr, len);
}

void rotatearrright(int (*arr)[LENGTHOF2NDARRAY], size_t n, size_t len)
{
    reversearr(arr, len);
    reversearr(arr + n, len - n);
    reversearr(arr, n);
}
