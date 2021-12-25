
#include <vector>
#include <iostream>
#include <GLFW/glfw3.h>
#include <algorithm>
#include "kNN.h"
typedef unsigned char uchar;
/// <summary>
/// 
/// </summary>
/// /// <param name='end'>флаг конца цикла для поиска центра масс</param>
/// <param name='size_points'>количество точек</param>
/// <param name='size_clusters'>количество кластеров</param>
/// <param name='scale'>коэффициент масштабируемости</param>
/// <param name='mean_x'>среднеарифметичекое точек кластера по оси X для поиска центра масс</param>
/// <param name='mean_y'>среднеарифметичекое точек кластера по оси Y для поиска центра масс</param>
/// <param name='cluster_index'>индекс кластера: к какому кластеру принадлежит каждая точка?</param>
/// <param name='count_pts'>количество точек, которые принадлежат данному кластеру</param>
/// <param name='center_mass_x'>позиция центра масс по x</param>
/// <param name='center_mass_y'>позиция центра масс по y</param>
/// <param name='center_mass_0'>определяет центр масс на нулевой итерации по номеру точки, которую мы взяли</param>
/// <param name='x'>позиция точки по x</param>
/// <param name='y'>позиция точки по y</param>
/// <param name='color'>вектор цветов по модели RGB</param>
/// <param name='Hcolor'>коэффицент,который предназначен для установки кластеру индивидуального цвета по модели HSV</param>
/// <returns></returns>
/// 

int main()
{
    system("chcp 1251>nul");
#pragma region Init
    double scale = 1.8;
    long size_clusters, size_points = 100000;
    int mode;
    bool InputСheck = false;

    std::cout << "Введите количество кластеров: \n";
    InputCheck(InputСheck, size_clusters);
    std::cout << "Если хотите увидеть конечный результат, то нажмите 1 \n";
    std::cout << "Если хотите увидеть пошаговый вывод, то нажмите 2 \n";
    InputCheck(InputСheck, mode);

    std::vector<double> cluster_index(size_points, 0);
    std::vector<double> center_mass_x(size_clusters, 0);
    std::vector<double> center_mass_y(size_clusters, 0);
    std::vector<double> center_mass_0(size_clusters, 0);
    std::vector<double> x;
    std::vector<double> y;
#pragma endregion

    InitPoints(size_points, x, y, scale);
    InitCenterMass(center_mass_0, size_points, size_clusters);
    InitCenterMassPos(size_clusters, center_mass_0, center_mass_x, center_mass_y, x, y);

    GLFWwindow* window;
    if (!glfwInit())
        return -1;
    window = glfwCreateWindow(700, 700, "KNN", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    bool result = false;
    switch (mode)
    {
    case 1:
        result = DrawEnd(window, x, y, center_mass_x, center_mass_y, cluster_index, size_clusters, size_points);
        if (result) return 0;
        else return -1;
        break;
    case 2:
        result = DrawStepByStep(window, x, y, center_mass_x, center_mass_y, cluster_index, size_clusters, size_points);
        if (result) return 0;
        else return -1;
        break;
    default: return -1;
        break;
    }  
    return 0;
}

