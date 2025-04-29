// backend.h - Заголовочный файл для класса Backend
#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>
#include <QStringList>

// Класс, который будет доступен из JavaScript
class Backend : public QObject {
    Q_OBJECT
    
public:
    explicit Backend(QObject *parent = nullptr);
    
public slots:
    // Методы, которые будут вызываться из JavaScript
    QString getMessage();
    void processData(const QString &data);
    int calculate(int a, int b);
    void notifyImagesChanged();
    // Новый метод для получения списка изображений
    QStringList getDrawingImages();
    bool saveImage(const QString &imageData, const QString &fileName);
    // Добавим в backend.h новый метод
    bool deleteImage(const QString &fileName);
// Добавим в backend.h
    bool saveSvgImage(const QString &svgData, const QString &fileName);
signals:
    void albumImagesChanged();
    // Сигналы, которые будут отправляться в JavaScript
    void dataChanged(const QString &newData);
    void progressUpdated(int progress);
    void imagesChanged();
    
private:
    QStringList getAlbumImages();
    QString m_message;
};

#endif // BACKEND_H