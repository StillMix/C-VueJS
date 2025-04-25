// backend.cpp - Реализация класса Backend
#include "backend.h"
#include <QDebug>
#include <QThread>
#include <QDir>
#include <QFileInfo>
#include <QByteArray>
#include <QFile>
#include <QDateTime>
#include <QBuffer>
#include <QImage>

Backend::Backend(QObject *parent) : QObject(parent) {
    m_message = "Привет из C++!";
}

QString Backend::getMessage() {
    qDebug() << "JavaScript запросил сообщение";
    return m_message;
}

void Backend::notifyImagesChanged() {
    emit imagesChanged();
}
void Backend::processData(const QString &data) {
    qDebug() << "Получены данные от JavaScript:" << data;
    
    // Обрабатываем данные
    QString processedData = "Обработано: " + data;
    
    // Отправляем сигнал в JavaScript
    emit dataChanged(processedData);
    
    // Эмулируем процесс с прогрессом
    for (int i = 0; i <= 100; i += 10) {
        emit progressUpdated(i);
        QThread::msleep(200); // Небольшая задержка для демонстрации
    }
}

int Backend::calculate(int a, int b) {
    qDebug() << "Выполняется вычисление:" << a << "+" << b;
    return a + b;
}


// в backend.cpp добавляем вывод подробной отладочной информации в метод saveImage

bool Backend::saveImage(const QString &imageData, const QString &fileName) {
    qDebug() << "Получен запрос на сохранение изображения с именем:" << fileName;
    
    // Проверяем, что изображение в формате Data URL (base64)
    if (!imageData.startsWith("data:image/")) {
        qWarning() << "Неверный формат данных изображения";
        return false;
    }
    
    // Извлекаем тип изображения и данные base64
    QString dataType = imageData.mid(5, imageData.indexOf(";") - 5);
    QString base64Data = imageData.mid(imageData.indexOf(",") + 1);
    qDebug() << "Тип данных изображения:" << dataType;
    
    // Конвертируем base64 в бинарные данные
    QByteArray imageBytes = QByteArray::fromBase64(base64Data.toUtf8());
    qDebug() << "Размер полученных данных:" << imageBytes.size() << "байт";
    
    // Загружаем изображение
    QImage image;
    if (!image.loadFromData(imageBytes)) {
        qWarning() << "Не удалось загрузить изображение из данных";
        return false;
    }
    qDebug() << "Изображение успешно загружено, размер:" << image.width() << "x" << image.height();
    
    // Составляем имя файла
    QString saveFileName = fileName;
    
    // Путь к директории для сохранения
    QString savePath = "./frontend/dist/img/" + saveFileName;
    qDebug() << "Путь для сохранения:" << savePath;
    
    // Проверяем наличие директории
    QDir dir("./frontend/dist/img");
    if (!dir.exists()) {
        qDebug() << "Создаем директорию drawings";
        if (!dir.mkpath(".")) {
            qWarning() << "Не удалось создать директорию drawings";
            return false;
        }
    }
    
    // Сохраняем изображение в формате JPG с высоким качеством
    if (!image.save(savePath, "JPG", 95)) {
        qWarning() << "Не удалось сохранить изображение:" << savePath;
        return false;
    }
    
    qDebug() << "Изображение успешно сохранено:" << savePath;
    
    // Проверяем, действительно ли файл был создан
    QFileInfo checkFile(savePath);
    if (checkFile.exists() && checkFile.isFile()) {
        qDebug() << "Файл существует, размер:" << checkFile.size() << "байт";
    } else {
        qWarning() << "Файл не был создан!";
        return false;
    }
    emit imagesChanged();
    return true;
}

// Также улучшим метод получения списка изображений

QStringList Backend::getDrawingImages() {
    qDebug() << "JavaScript запросил список изображений";
    
    QStringList images;
    
    // Путь к директории с изображениями
    QString path = "./frontend/dist/img";
    qDebug() << "Ищем изображения в директории:" << path;
    
    QDir directory(path);
    if (!directory.exists()) {
        qWarning() << "Директория" << path << "не существует!";
        qDebug() << "Текущая директория:" << QDir::currentPath();
        
        // Создаем директорию, если она не существует
        if (directory.mkpath(".")) {
            qDebug() << "Директория drawings успешно создана";
        } else {
            qWarning() << "Не удалось создать директорию drawings!";
        }
    }
    
    // Фильтр для поиска только изображений
    QStringList filters;
    filters << "*.jpg" << "*.jpeg" << "*.png" << "*.gif" << "*.bmp";
    directory.setNameFilters(filters);
    
    // Получаем список файлов, соответствующих фильтру
    QFileInfoList fileList = directory.entryInfoList();
    qDebug() << "Найдено файлов:" << fileList.size();
    
    // Проходим по списку файлов и добавляем их имена в результат
    for (const QFileInfo &fileInfo : fileList) {
        qDebug() << "Добавляем файл:" << fileInfo.fileName();
        images.append(fileInfo.fileName());
    }
    
    qDebug() << "Всего отправляем" << images.size() << "изображений";
    return images;
}