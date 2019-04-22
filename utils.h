//
// Created by rkindela on 27-05-18.
//

#ifndef PMTOOL_UTILS_H
#define PMTOOL_UTILS_H

#include <limits>
#include <iostream>
#include <typeinfo>
#include <cmath>
#include <ctime>

#include <cstdio>
#include <unistd.h>
#include <cerrno>


# define M_PI           3.14159265358979323846
# define RAD            0.01745329251   // M_PI/180
#define INV_RAD         57.2957795131   // 180/M_PI
#define PRECISION       3
#define MAX_VALUE               100

using namespace std;

template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
almost_equal(T x, T y, int ulp = PRECISION)
{

    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)

//    auto e = std::numeric_limits<T>::epsilon();
//    return abs(x-y) <= e * abs(x+y) * ulp
//            unless the result is subnormal
//           || abs(x-y) < std::numeric_limits<T>::min();

    try{
        float result = pow(10.0, ulp);

        bool resp1 = (abs(x-y) <= 1.0f/result);

        return resp1;
    }
    catch (exception& e)
    {
        cout<<endl<<"Error de precision";
        cout.flush();
    }
    return false;
}

template<class T>
double toRadians(T angle)
{
    return angle*RAD;
}

template<class T>
double toDegree(T angle)
{
    return angle*INV_RAD;
}

inline float clock2milisec(clock_t old)
{
    return (clock() - old)/(CLOCKS_PER_SEC/1000);
};


inline std::string currentDirectory()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == nullptr)
        return "";
    string currentDir = cwd;
    int idx = currentDir.rfind("/");
    if (idx == -1)
        return "";

    return currentDir;
}

/**
 * MERGE SORT
 * */

template<typename RandomIter1, typename RandomIter2, typename CompareFunctionType>
void merge(RandomIter1 begin,
           RandomIter1 middle,
           RandomIter1 end,
           RandomIter2 aux, CompareFunctionType compareFunc) {
    RandomIter1 left = begin;
    RandomIter1 right = middle;
    RandomIter1 left_bound = middle;
    RandomIter1 right_bound = end;

    while (left != left_bound and right != right_bound) {
        if (compareFunc(&(*left), &(*right))) {
            *aux = *right;
            ++right;
        } else {
            *aux = *left;
            ++left;
        }

        ++aux;
    }

    std::copy(left, left_bound, aux);
    std::copy(right, right_bound, aux);
}

template<typename RandomIter1, typename RandomIter2, typename CompareFunctionType>
void merge_sort(RandomIter1 source_begin,
                RandomIter1 source_end,
                RandomIter2 target_begin,
                RandomIter2 target_end, CompareFunctionType compareFunc) {
    auto distance = std::distance(source_begin, source_end);

    if (distance < 2) {
        return;
    }

    RandomIter1 source_middle = source_begin;
    RandomIter2 target_middle = target_begin;
    std::advance(source_middle, distance >> 1); // distance >> 1 is the same as distance / 2
    std::advance(target_middle, distance >> 1);

    merge_sort(target_begin,
               target_middle,
               source_begin,
               source_middle, compareFunc);

    merge_sort(target_middle,
               target_end,
               source_middle,
               source_end, compareFunc);

    merge(source_begin,
          source_middle,
          source_end,
          target_begin, compareFunc);
}

template<typename RandomIter, typename CompareFunctionType>
void merge_sort(RandomIter begin, RandomIter end, CompareFunctionType compareFunction) {
    auto distance = std::distance(begin, end);

    if (distance < 2) {
        return;
    }

    using value_type = typename std::iterator_traits<RandomIter>::value_type;
    value_type* aux = new value_type[distance];
    std::copy(begin, end, aux);
    merge_sort(aux, aux + distance, begin, end, compareFunction);
    delete [] aux;
}


#endif //PMTOOL_UTILS_H
