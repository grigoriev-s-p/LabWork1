/*
 * Sergey Grigoriev
 * st142081@student.spbu.ru
 * LabWork1
*/
#include "BMPImg.h"
#include <iostream>

int main()
{
    BMPImg image;
    image.readFromFile("input.bmp");

    // ѕоворот на 90 по часовой стрелке
    image.rotate90Cw();
    image.writeToFile("output_clockwise.bmp");
    image.rotate90CCw();//возвращаем в исходное состо€ние

    // ѕоворот на 90 против часовой стрелки 
    image.rotate90CCw();
    image.writeToFile("output_counterclockwise.bmp");
    image.rotate90Cw();//возвращаем в исходное состо€ние

    // ѕрименение гауссовского фильтра
    image.Gauss();
    image.writeToFile("output_gaussian.bmp");

    std::cout << "Complete"<<std::endl;

    return 0;
}
