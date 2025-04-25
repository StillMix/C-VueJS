// backend.cpp - Реализация класса Backend
#include "backend.h"
#include <QDebug>
#include <QThread>
#include <QDir>
#include <QFileInfo>

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
        return images;
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