#ifndef IMAGE_MANIPULATION_H
#define IMAGE_MANIPULATION_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <memory>

class image_Manipulation : public QMainWindow
{
    Q_OBJECT

public:
    image_Manipulation(QWidget *parent = nullptr);
    ~image_Manipulation();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void increaseBrightness();
    void decreaseBrightness();
    void selectImage();
    void saveImage();

private:
    void setupUI();
    void changeBrightness(int delta);
    void updateImageDisplay();

    std::unique_ptr<QLabel> imageLabel;
    std::unique_ptr<QPixmap> image;
    std::unique_ptr<QPushButton> increaseButton;
    std::unique_ptr<QPushButton> decreaseButton;
    std::unique_ptr<QPushButton> selectImageButton;
    std::unique_ptr<QPushButton> saveImageButton;
    std::unique_ptr<QVBoxLayout> mainLayout;
    std::unique_ptr<QHBoxLayout> buttonLayout;
    std::unique_ptr<QWidget> centralWidget;
};

#endif // IMAGE_MANIPULATION_H

