#include "image_manipulation.h"
#include <QFileDialog>
#include <QHBoxLayout>
#include <QDebug>
#include <memory>

image_Manipulation::image_Manipulation(QWidget *parent)
    : QMainWindow(parent),
    imageLabel(std::make_unique<QLabel>(this)),
    image(std::make_unique<QPixmap>()),
    increaseButton(std::make_unique<QPushButton>("+", this)),
    decreaseButton(std::make_unique<QPushButton>("-", this)),
    selectImageButton(std::make_unique<QPushButton>("Выбрать изображение", this)),
    saveImageButton(std::make_unique<QPushButton>("Сохранить изображение", this)),
    mainLayout(std::make_unique<QVBoxLayout>()),
    buttonLayout(std::make_unique<QHBoxLayout>()),
    centralWidget(std::make_unique<QWidget>(this))
{
    setupUI();
    resize(300, 400);
}

void image_Manipulation::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    if (!image->isNull()) {
        updateImageDisplay();
    }
}

image_Manipulation::~image_Manipulation() {}

void image_Manipulation::setupUI()
{
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setText("Изображение не выбрано");

    connect(increaseButton.get(), &QPushButton::clicked, this, &image_Manipulation::increaseBrightness);
    connect(decreaseButton.get(), &QPushButton::clicked, this, &image_Manipulation::decreaseBrightness);
    connect(selectImageButton.get(), &QPushButton::clicked, this, &image_Manipulation::selectImage);
    connect(saveImageButton.get(), &QPushButton::clicked, this, &image_Manipulation::saveImage);

    buttonLayout->addWidget(increaseButton.get());
    buttonLayout->addWidget(decreaseButton.get());
    buttonLayout->addWidget(selectImageButton.get());

    mainLayout->addWidget(imageLabel.get());
    mainLayout->addLayout(buttonLayout.get());
    mainLayout->addWidget(saveImageButton.get());

    centralWidget->setLayout(mainLayout.get());
    setCentralWidget(centralWidget.get());
}

void image_Manipulation::increaseBrightness()
{
    changeBrightness(10);
}

void image_Manipulation::decreaseBrightness()
{
    changeBrightness(-10);
}

void image_Manipulation::selectImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Выбрать изображение", "", "Изображения (*.png *.jpg *.jpeg *.bmp)");
    if (!fileName.isEmpty()) {
        if (!image->load(fileName)) {
            qWarning() << "Failed to load image:" << fileName;
            return;
        }
        updateImageDisplay();
    }
}

void image_Manipulation::saveImage()
{
    if (image->isNull()) {
        qWarning() << "Image not loaded or invalid.";
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить изображение", "", "Изображения (*.png *.jpg *.jpeg *.bmp)");
    if (!fileName.isEmpty()) {
        if (!image->save(fileName)) {
            qWarning() << "Failed to save image:" << fileName;
        }
    }
}

void image_Manipulation::changeBrightness(int delta)
{
    if (image->isNull()) {
        qWarning() << "Image not loaded or invalid.";
        return;
    }

    QImage tempImage = image->toImage();
    tempImage = tempImage.convertToFormat(QImage::Format_ARGB32);

    int width = tempImage.width();
    int height = tempImage.height();

    QRgb *pixels = reinterpret_cast<QRgb *>(tempImage.bits());
    int pixelCount = width * height;
    int r,g,b;
    for (int i = 0; i < pixelCount; ++i) {
        r = qBound(0, qRed(pixels[i]) + delta, 255);
        g = qBound(0, qGreen(pixels[i]) + delta, 255);
        b = qBound(0, qBlue(pixels[i]) + delta, 255);
        pixels[i] = qRgb(r, g, b);
    }

    *image = QPixmap::fromImage(tempImage);
    updateImageDisplay();
}

void image_Manipulation::updateImageDisplay()
{
    QSize windowSize = size();
    QSize imageSize = image->size();
    imageSize.scale(windowSize, Qt::KeepAspectRatio);
    imageLabel->setPixmap(image->scaled(imageSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
