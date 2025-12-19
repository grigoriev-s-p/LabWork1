/*
 * Sergey Grigoriev
 * st142081@student.spbu.ru
 * LabWork1
*/
#include "gtest/gtest.h"
#include "BMPImg.h"

class BMPImgTest : public ::testing::Test
{
protected:
    BMPImg img;
    void SetUp() override
    {
        img.load("test.bmp");
    }
};

TEST_F(BMPImgTest, RotateClockwise)
{
    int oldWidth = img.getWidth();
    int oldHeight = img.getHeight();
    img.rotateCw();
    EXPECT_EQ(img.getWidth(), oldHeight);
    EXPECT_EQ(img.getHeight(), oldWidth);
}

TEST_F(BMPImgTest, ApplyGaussianFilterChangesPixels)
{
    auto originalPixels = img.getRawPixels();
    img.Gauss();
    auto filteredPixels = img.getRawPixels();
    bool hasChanged = false;
    for (size_t i = 0; i < originalPixels.size(); ++i)
    {
        if (originalPixels[i] != filteredPixels[i])
        {
            hasChanged = true;
            break;
        }
    }
    EXPECT_TRUE(hasChanged);
}

TEST_F(BMPImgTest, LoadImageSetsCorrectDimensions)
{
    EXPECT_GT(img.getWidth(), 0);
    EXPECT_GT(img.getHeight(), 0);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}