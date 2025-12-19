/*
 * Sergey Grigoriev
 * st142081@student.spbu.ru
 * LabWork1
*/
#pragma once
#include <string>

class ImageInterface
{
public:
    virtual bool readFromFile(const std::string& path) = 0;
    virtual bool writeToFile(const std::string& path) = 0;

    virtual void rotate90Cw() = 0;
    virtual void rotate90CCw() = 0;

    virtual void Gauss() = 0;

    virtual ~ImageInterface() = default;
};
