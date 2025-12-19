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
    image.readFromFile("sample.bmp");

    // Rotate90 Clockwise
    image.rotate90Cw();
    image.writeToFile("output_clockwise.bmp");
    image.rotate90CCw();//back to original image

    // Rotate90 CounterClockwise
    image.rotate90CCw();
    image.writeToFile("output_counterclockwise.bmp");
    image.rotate90Cw();//back to original image

    // Applying Gauss Filter
    image.Gauss();
    image.writeToFile("output_gaussian.bmp");

    std::cout << "Complete"<<std::endl;

    return 0;
}
