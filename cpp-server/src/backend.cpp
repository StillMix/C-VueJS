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

QStringList Backend::getDrawingImages() {
    qDebug() << "JavaScript запросил список изображений";
    
    QStringList images;
    
    // Путь к директории с изображениями
    QString path = "./drawings";
    
    QDir directory(path);
    if (!directory.exists()) {
        qWarning() << "Директория" << path << "не существует!";
        // Создаем директорию, если она не существует
        directory.mkpath(".");
    }
    
    // Фильтр для поиска только изображений
    QStringList filters;
    filters << "*.jpg" << "*.jpeg" << "*.png" << "*.gif" << "*.bmp";
    directory.setNameFilters(filters);
    
    // Получаем список файлов, соответствующих фильтру
    QFileInfoList fileList = directory.entryInfoList();
    
    // Проходим по списку файлов и добавляем их имена в результат
    for (const QFileInfo &fileInfo : fileList) {
        images.append(fileInfo.fileName());
    }
    
    qDebug() << "Найдено" << images.size() << "изображений";
    return images;
}

bool Backend::saveImage(const QString &imageData, const QString &fileName) {
    qDebug() << "Получен запрос на сохранение изображения";
    
    // Проверяем, что изображение в формате Data URL (base64)
    if (!imageData.startsWith("data:image/")) {
        qWarning() << "Неверный формат данных изображения";
        return false;
    }
    
    // Извлекаем тип изображения и данные base64
    QString dataType = imageData.mid(5, imageData.indexOf(";") - 5);
    QString base64Data = imageData.mid(imageData.indexOf(",") + 1);
    
    // Конвертируем base64 в бинарные данные
    QByteArray imageBytes = QByteArray::fromBase64(base64Data.toUtf8());
    
    // Создаем имя файла с текущей датой и временем
    QDateTime now = QDateTime::currentDateTime();
    QString dateTime = now.toString("ddMMyy-HHmmss");
    
    // Определяем расширение из типа данных
    QString extension;
    if (dataType == "image/png") {
        extension = ".png";
    } else if (dataType == "image/jpeg") {
        extension = ".jpg";
    } else if (dataType == "image/gif") {
        extension = ".gif";
    } else {
        extension = ".png"; // По умолчанию
    }
    
    // Составляем полное имя файла
    QString saveFileName;
    if (fileName.isEmpty()) {
        saveFileName = QString("userfoto-%1%2").arg(dateTime).arg(extension);
    } else {
        saveFileName = fileName;
    }
    
    // Путь к директории для сохранения
    QString savePath = "./drawings/" + saveFileName;
    
    // Проверяем наличие директории
    QDir dir("./drawings");
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    
    // Сохраняем изображение
    QFile file(savePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Не удалось открыть файл для записи:" << savePath;
        return false;
    }
    
    file.write(imageBytes);
    file.close();
    
    qDebug() << "Изображение успешно сохранено:" << savePath;
    return true;
}