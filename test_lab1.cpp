/*
 * Sergey Grigoriev
 * st142081@student.spbu.ru
 * LabWork1
*/
#include "gtest/gtest.h"
#include "BMPImg.h"
class BMPImgTest : public ::testing::Test {
protected:
    BMPImg testImage;
    void SetUp() override {
        ASSERT_TRUE(testImage.readFromFile("sample.bmp"));
    }
};

TEST_F(BMPImgTest, GaussianFilterModifiesPixels) {
    auto oldPixels = testImage.getPixels();
    testImage.Gauss();
    auto newPixels = testImage.getPixels();
    bool changed = false;
    for (size_t i = 0; i < oldPixels.size(); ++i)
        if (oldPixels[i] != newPixels[i]) { changed = true; break; }
    EXPECT_TRUE(changed);
}

TEST_F(BMPImgTest, Rotate90ClockwiseChangesDimensions) {
    int oldWidth = testImage.getWidth();
    int oldHeight = testImage.getHeight();
    testImage.rotate90Cw();
    EXPECT_EQ(testImage.getWidth(), oldHeight);
    EXPECT_EQ(testImage.getHeight(), oldWidth);
}

TEST_F(BMPImgTest, Rotate90CounterClockwiseTwiceRestoresDimensions) {
    int oldWidth = testImage.getWidth();
    int oldHeight = testImage.getHeight();
    testImage.rotate90CCw();
    testImage.rotate90CCw();
    EXPECT_EQ(testImage.getWidth(), oldWidth);
    EXPECT_EQ(testImage.getHeight(), oldHeight);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
