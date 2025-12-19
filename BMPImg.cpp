/*
 * Sergey Grigoriev
 * st142081@student.spbu.ru
 * LabWork1
*/
#include "BMPImg.h"
#include "Constants.h"
#include <fstream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <cstring>


// Загрузка BMP файла
bool BMPImg::readFromFile(const std::string& path)
{
    std::ifstream in(path, std::ios::binary);
    if (!in) return false;

    bmpHeader.resize(BMP_HEADER_SIZE);
    in.read(reinterpret_cast<char*>(bmpHeader.data()), BMP_HEADER_SIZE);
    if (bmpHeader[0] != 'B' || bmpHeader[1] != 'M') return false;

    uint32_t dibLen;
    in.read(reinterpret_cast<char*>(&dibLen), sizeof(dibLen));
    in.seekg(BMP_HEADER_SIZE, std::ios::beg);

    dibHeader.resize(dibLen);
    in.read(reinterpret_cast<char*>(dibHeader.data()), dibLen);

    width = read_s32_le(dibHeader.data() + DIB_WIDTH_OFFSET);
    int32_t hRaw = read_s32_le(dibHeader.data() + DIB_HEIGHT_OFFSET);
    originalTopDown = hRaw < 0;
    height = std::abs(hRaw);

    uint16_t planesVal = read_u16_le(dibHeader.data() + DIB_PLANES_OFFSET);
    uint16_t bppVal = read_u16_le(dibHeader.data() + DIB_BPP_OFFSET);
    uint32_t compress = read_u32_le(dibHeader.data() + DIB_COMPRESSION_OFFSET);

    if (planesVal != 1 || bppVal != BITS_PER_PIXEL || compress != 0)
        throw std::runtime_error("Unsupported BMP format");

    uint32_t pixelStart = read_u32_le(bmpHeader.data() + BMP_DATA_OFFSET);
    int stride = calculateRowSize(width);

    pixels.assign(stride * height, 0);
    in.seekg(pixelStart, std::ios::beg);

    for (int rowIndex = 0; rowIndex < height; ++rowIndex)
    {
        int targetRow = originalTopDown ? rowIndex : (height - 1 - rowIndex);
        in.read(reinterpret_cast<char*>(&pixels[targetRow * stride]), stride);
    }

    return true;
}


// Сохранение BMP файла

bool BMPImg::writeToFile(const std::string& path)
{
    std::ofstream out(path, std::ios::binary);
    if (!out) return false;

    int stride = calculateRowSize(width);
    uint32_t imgSize = stride * height;
    uint32_t totalSize = bmpHeader.size() + dibHeader.size() + imgSize;

    write_u32_le(bmpHeader.data() + BMP_FILESIZE_OFFSET, totalSize);
    write_u32_le(bmpHeader.data() + BMP_DATA_OFFSET, bmpHeader.size() + dibHeader.size());
    write_s32_le(dibHeader.data() + DIB_WIDTH_OFFSET, width);
    write_s32_le(dibHeader.data() + DIB_HEIGHT_OFFSET, originalTopDown ? -height : height);
    write_u32_le(dibHeader.data() + DIB_IMAGE_SIZE_OFFSET, imgSize);

    out.write(reinterpret_cast<char*>(bmpHeader.data()), bmpHeader.size());
    out.write(reinterpret_cast<char*>(dibHeader.data()), dibHeader.size());

    for (int rowIdx = 0; rowIdx < height; ++rowIdx)
    {
        int targetRow = originalTopDown ? rowIdx : (height - 1 - rowIdx);
        out.write(reinterpret_cast<char*>(&pixels[targetRow * stride]), stride);
    }

    return true;
}


// Поворот по часовой стрелке

void BMPImg::rotate90Cw()
{
    int oldStride = calculateRowSize(width);
    int newWidth = height;
    int newHeight = width;
    int newStride = calculateRowSize(newWidth);

    std::vector<uint8_t> buffer(newStride * newHeight, 0);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            for (int ch = 0; ch < BYTES_PER_PIXEL; ++ch)
            {
                int src = y * oldStride + x * BYTES_PER_PIXEL + ch;
                int dstX = height - 1 - y;
                int dstY = x;
                int dst = dstY * newStride + dstX * BYTES_PER_PIXEL + ch;
                buffer[dst] = pixels[src];
            }
        }
    }

    pixels = std::move(buffer);
    width = newWidth;
    height = newHeight;
    originalTopDown = false;
}


// Поворот против часовой стрелки

void BMPImg::rotate90CCw()
{
    int oldStride = calculateRowSize(width);
    int newW = height;
    int newH = width;
    int newStride = calculateRowSize(newW);

    std::vector<uint8_t> tmp(newStride * newH, 0);

    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            for (int ch = 0; ch < BYTES_PER_PIXEL; ++ch)
            {
                int src = y * oldStride + x * BYTES_PER_PIXEL + ch;
                int dstX = y;
                int dstY = width - 1 - x;
                int dst = dstY * newStride + dstX * BYTES_PER_PIXEL + ch;
                tmp[dst] = pixels[src];
            }

    pixels = std::move(tmp);
    width = newW;
    height = newH;
    originalTopDown = false;
}


// Применение фильтра Гаусса 

void BMPImg::Gauss()
{
    int stride = calculateRowSize(width);
    std::vector<uint8_t> newPixels(pixels.size(), 0);

    constexpr int k[3][3] = { {1, 2, 1}, {2, 4, 2}, {1, 2, 1} };

    for (int y = 1; y < height - 1; ++y)
        for (int x = 1; x < width - 1; ++x)
            for (int ch = 0; ch < BYTES_PER_PIXEL; ++ch)
            {
                int sum = 0;
                for (int ky = -1; ky <= 1; ++ky)
                    for (int kx = -1; kx <= 1; ++kx)
                        sum += pixels[(y + ky) * stride + (x + kx) * BYTES_PER_PIXEL + ch] * k[ky + 1][kx + 1];

                newPixels[y * stride + x * BYTES_PER_PIXEL + ch] = sum / GAUSS_KERNEL_SUM;
            }

    pixels = std::move(newPixels);
}
