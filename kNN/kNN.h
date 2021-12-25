#pragma once

//инициализация позиции центров масс
void InitCenterMassPos(int size_clusters, std::vector<double>& center_mass_0, std::vector<double>& center_mass_x, std::vector<double>& center_mass_y, std::vector<double>& x, std::vector<double>& y)
{
    for (int i = 0; i < size_clusters; i++) {
        center_mass_x[i] = x[center_mass_0[i]];
        center_mass_y[i] = y[center_mass_0[i]];
    }
}
//инициализация центров масс
void InitCenterMass(std::vector<double>& center_mass_0, int size_points, int size_clusters)
{
    center_mass_0[0]= rand() % size_points;//присваивает номер точки, к которой прикрепится центр масс
    for (int i = 1; i < center_mass_0.size();i++) {
        center_mass_0[i] = rand() % size_points;
        while(center_mass_0[i] == center_mass_0[i-1]) {;
            center_mass_0[i] = rand() % size_points;
        }
    }
}
//инициализация точек
void InitPoints(int size_points, std::vector<double>& x, std::vector<double>& y, double scale = 1.0)
{
    for (int i = 0; i < size_points; i++)
    {
        x.emplace_back((double)rand() / (double)RAND_MAX * scale);
        y.emplace_back((double)rand() / (double)RAND_MAX * scale);
    }
}
//проверяет позицию центров масс. Если они больше не меняются(сошлись), то цикл останавливается
bool CheckPosition(int size_clusters, std::vector<double>& center_mass_x, std::vector<double>& center_mass_y, double* mean_x, double* mean_y)
{
    bool end = false;
    int count = 0;
    for (int i = 0; i < size_clusters; i++)
    {
        double x_check = center_mass_x[i];
        double y_check = center_mass_y[i];
        center_mass_x[i] = mean_x[i];
        center_mass_y[i] = mean_y[i];
        if (center_mass_x[i] == x_check && center_mass_y[i] == y_check) {
            count++;
        }
        if (count == size_clusters) {
            end = true;
        }
    }
    count = 0;
    return end;
}
//Присваивает центру масс новую позицию
void CenterMassNewPos(int size_clusters, int size_points, std::vector<double>& cluster_index, double* mean_x, std::vector<double>& x, std::vector<int>& count_pts, double* mean_y, std::vector<double>& y)
{
    for (int i = 0; i < size_clusters; i++)
    {
        for (int j = 0; j < size_points; j++)
        {
            if (cluster_index[j] == i) {
                mean_x[i] += x[j] / count_pts[i];
                mean_y[i] += y[j] / count_pts[i];
            }
        }
    }
}
//показывает центры масс и количество точек, принадлежащих данному центру масс
void ShowCentMass_and_NumPoints(int size_clusters, std::vector<double>& center_mass_0, std::vector<double>& center_mass_x, std::vector<double>& center_mass_y, std::vector<int>& count_pts)
{
    for (int i = 0; i < size_clusters; i++)
    {
        std::cout << "Кластер " << center_mass_0[i] << "\n";
        std::cout << center_mass_x[i] - 0.9 << " " << center_mass_y[i] - 0.9 << " " << count_pts[i] << "\n";
    }
}
//вычисление расстояние от точек до центров масс и присваивает индексы кластеров точкам
void CalculateMinDist(int size_points, std::vector<double>& x, std::vector<double>& center_mass_x, std::vector<double>& y, std::vector<double>& center_mass_y, int size_clusters, std::vector<double>& cluster_index, std::vector<int>& count_pts)
{
    for (int i = 0; i < size_points; i++) {
        double min = sqrt(pow(x[i] - center_mass_x[0], 2) + pow(y[i] - center_mass_y[0], 2));
        int m = 0;
        for (int j = 0; j < size_clusters - 1; j++) {
            if (min >= sqrt(pow(x[i] - center_mass_x[j + 1], 2) + pow(y[i] - center_mass_y[j + 1], 2))) {
                min = sqrt(pow(x[i] - center_mass_x[j + 1], 2) + pow(y[i] - center_mass_y[j + 1], 2));
                m = j + 1;
            }
        }
        cluster_index[i] = m;
        count_pts[m]++;
    }
}
//Переводит RGB  в HSV
std::vector<float> RGBtoHSV(double r, double g, double b) {
    r /= 255, g /= 255, b /= 255;
    int max = std::max(std::max(r, g), b), min = std::min(std::max(r, g), b);
    float h = 0, s = 0, v = max;

    s = max == 0 ? 0 : (1 - (double)min / (double)max);

    if (max > r && g >= b) {
        h = 60 * (g - b) / (max - min);
    }
    else if (max = r && g < b) {
        h = 60 * (g - b) / (max - min) + 240;
    }
    else if (max = g) {
        h = 60 * (b - r) / (max - min) + 120;
    }
    else if (max = b) {
        h = 60 * (b - g) / (max - min) + 240;
    }
    std::vector<float> hsv{ h, s, v };
    return hsv;
}
//Переводит HSV в RGB
std::vector<float> HSVtoRGB(int inH, int inS, int inV) {
    float r = 0, g = 0, b = 0;
    double H = inH%360, S = inS, V = inV;
    int Hi = floor(H / 60);
    double Vmin = (100 - S) * V / 100;
    double a = (V - Vmin) * (inH % 360 % 60) / 60;
    double Vinc = Vmin + a;
    double Vdec = V - a;

    switch (Hi % 6) {
    case 0: r = V, g = Vinc, b = Vmin; break;
    case 1: r = Vdec, g = V, b = Vmin; break;
    case 2: r = Vmin, g = V, b = Vinc; break;
    case 3: r = Vmin, g = Vdec, b = V; break;
    case 4: r = Vinc, g = Vmin, b = V; break;
    case 5: r = V, g = Vmin, b = Vdec; break;
    }
    std::vector<float> rgb{ r * 255 / 100,g * 255 / 100,b * 255 / 100 };
    return rgb;
}
//Проверка на входные данные по количеству кластеров
void InputCheck(bool& InputСheck, long& n)
{
    while (!InputСheck) {

        std::cin >> n;
        if (std::cin.fail()) // если предыдущее извлечение было неудачным,
        {
            std::cout << "Ошибка ввода! Вы ввели некорректный символ, попробуйте снова: " << std::endl;
            std::cin.clear(); // то возвращаем cin в 'обычный' режим работы
            std::cin.ignore(32767, '\n'); // и удаляем значения предыдущего ввода из входного буфера
        }
        else if (n > 0) { InputСheck = true; }
        else { std::cout << "Ошибка ввода! Количество кластеров не может быть отрицательным или равняться нулю, попробуйте снова: " << std::endl; }
    }
    InputСheck = false;
}
// проверка на входные данные по режиму
void InputCheck(bool& InputСheck, int& mode)
{
    while (!InputСheck) {

        std::cin >> mode;
        if (std::cin.fail()) // если предыдущее извлечение было неудачным,
        {
            std::cout << "Ошибка ввода! Вы ввели некорректный символ, попробуйте снова: " << std::endl;
            std::cin.clear(); // то возвращаем cin в 'обычный' режим работы
            std::cin.ignore(32767, '\n'); // и удаляем значения предыдущего ввода из входного буфера
        }
        else if (mode==1 || mode==2) { InputСheck = true; }
        else { std::cout << "Ошибка ввода! Количество кластеров не может быть отрицательным или равняться нулю, попробуйте снова: " << std::endl; }
    }
    InputСheck = false;
}
//Кластеризует методом kNN
bool CalculateKNN(std::vector<double>& x, std::vector<double>& y, std::vector<double>& center_mass_x, std::vector<double>& center_mass_y, std::vector<double>& cluster_index, int size_clusters, long size_points) {
    bool end = false;
    int Hcolor = round(360.0 / size_clusters);
    for (; !end;) {
        std::vector<int> count_pts(size_clusters, 0);
        CalculateMinDist(size_points, x, center_mass_x, y, center_mass_y, size_clusters, cluster_index, count_pts);
        double* mean_x = new double[size_clusters]();
        double* mean_y = new double[size_clusters]();

        CenterMassNewPos(size_clusters, size_points, cluster_index, mean_x, x, count_pts, mean_y, y);
        count_pts.clear();
        end = CheckPosition(size_clusters, center_mass_x, center_mass_y, mean_x, mean_y);

        delete[] mean_x;
        delete[] mean_y;
    }
    return true;
}
//рисует конечный результат
bool DrawEnd(GLFWwindow* window, std::vector<double>& x, std::vector<double>& y, std::vector<double>& center_mass_x, std::vector<double>& center_mass_y, std::vector<double>& cluster_index, int size_clusters, long size_points) {
    bool result = false;
    int Hcolor = round(360.0 / size_clusters);
    std::vector<float> color(3, 0);
    result = CalculateKNN(x, y, center_mass_x, center_mass_y, cluster_index, size_clusters, size_points);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glPointSize(10.0);
        glBegin(GL_POINTS);
        for (int j = 0; j < size_clusters; j++)
        {
            color = HSVtoRGB(Hcolor * j, 100, 100);
            glColor3f(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);
            glVertex2d(center_mass_x[j] - 0.9, center_mass_y[j] - 0.9);
        }
        glEnd();
        glPointSize(1.0);
        glBegin(GL_POINTS);
        for (int i = 0; i < size_points; i++) {
            for (int j = 0; j < size_clusters; j++) {
                if (cluster_index[i] == j) {
                    color = HSVtoRGB(Hcolor * j, 100, 100);
                    glColor3f(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);
                }
                glVertex2d(x[i] - 0.9, y[i] - 0.9);
            }
        }
        glEnd();
        glClearColor(0.2, 0.2, 0.2, 1.0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return result;
}
//рисует пошагово
bool DrawStepByStep(GLFWwindow* window, std::vector<double>& x, std::vector<double>& y, std::vector<double>& center_mass_x, std::vector<double>& center_mass_y, std::vector<double>& cluster_index, int size_clusters, long size_points) {
    bool end = false;
    int Hcolor = round(360.0 / size_clusters);
    std::vector<float> color(3, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glfwMakeContextCurrent(window);
    while (!glfwWindowShouldClose(window))
    {
        for (; !end;) {
            std::vector<int> count_pts(size_clusters, 0);
            CalculateMinDist(size_points, x, center_mass_x, y, center_mass_y, size_clusters, cluster_index, count_pts);
            double* mean_x = new double[size_clusters]();
            double* mean_y = new double[size_clusters]();

            CenterMassNewPos(size_clusters, size_points, cluster_index, mean_x, x, count_pts, mean_y, y);
            count_pts.clear();
            end = CheckPosition(size_clusters, center_mass_x, center_mass_y, mean_x, mean_y);

            delete[] mean_x;
            delete[] mean_y;

            glClear(GL_COLOR_BUFFER_BIT);
            glPushMatrix();
            glPointSize(10.0);
            glBegin(GL_POINTS);
            for (int j = 0; j < size_clusters; j++)
            {
                color = HSVtoRGB(Hcolor * j, 100, 100);
                glColor3f(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);
                glVertex2d(center_mass_x[j] - 0.9, center_mass_y[j] - 0.9);
            }

            glEnd();
            glPointSize(1.0);
            glBegin(GL_POINTS);
            for (int i = 0; i < size_points; i++) {
                for (int j = 0; j < size_clusters; j++) {
                    if (cluster_index[i] == j) {
                        color = HSVtoRGB(Hcolor * j, 100, 100);
                        glColor3f(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);
                    }
                    glVertex2d(x[i] - 0.9, y[i] - 0.9);
                }
            }
            glEnd();
            glPopMatrix();
            glClearColor(0.2, 0.2, 0.2, 1.0);
            glfwSwapBuffers(window);
            glfwSwapInterval(0);
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return end;
}
