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



// Добавим в backend.cpp

bool Backend::deleteImage(const QString &fileName) {
    qDebug() << "Получен запрос на удаление изображения:" << fileName;
    
    // Проверяем, что имя файла не пустое
    if (fileName.isEmpty()) {
        qWarning() << "Пустое имя файла для удаления";
        return false;
    }
    
    // Формируем путь к файлу
    QString imagePath = QDir::current().filePath("./frontend/dist/img/" + fileName);
    qDebug() << "Путь к удаляемому файлу:" << imagePath;
    
    // Проверяем существование файла
    QFile file(imagePath);
    if (!file.exists()) {
        qWarning() << "Файл не существует:" << imagePath;
        return false;
    }
    
    // Удаляем файл
    if (!file.remove()) {
        qWarning() << "Не удалось удалить файл:" << imagePath << "Ошибка:" << file.errorString();
        return false;
    }
    
    qDebug() << "Файл успешно удален:" << imagePath;
    
    // Отправляем сигнал о том, что список изображений изменился
    emit imagesChanged();
    
    return true;
}

bool Backend::saveSvgImage(const QString &svgData, const QString &fileName) {
    qDebug() << "Получен запрос на сохранение SVG изображения:" << fileName;
    
    // Проверка данных SVG
    if (!svgData.startsWith("<svg") && !svgData.startsWith("<?xml")) {
        qWarning() << "Неверный формат SVG данных";
        return false;
    }
    
    // Путь для сохранения в альбоме (другая директория)
    QString albumPath = QDir::current().filePath("./frontend/dist/img/");
    QDir albumDir(albumPath);
    
    // Создаем директорию, если она не существует
    if (!albumDir.exists()) {
        qDebug() << "Создаем директорию для альбома:" << albumPath;
        if (!albumDir.mkpath(".")) {
            qWarning() << "Не удалось создать директорию для альбома";
            return false;
        }
    }
    
    // Полный путь к файлу
    QString savePath = albumPath + fileName;
    qDebug() << "Путь для сохранения SVG:" << savePath;
    
    // Сохраняем SVG файл
    QFile file(savePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Не удалось открыть файл для записи:" << savePath;
        return false;
    }
    
    QTextStream stream(&file);
    stream << svgData;
    file.close();
    
    // Проверяем, что файл был создан
    if (!QFile::exists(savePath)) {
        qWarning() << "Файл не был создан:" << savePath;
        return false;
    }
    
    qDebug() << "SVG успешно сохранен:" << savePath;
    
    // Отправляем сигнал об изменении списка изображений альбома
    emit albumImagesChanged();
    
    return true;
}

QStringList Backend::getAlbumImages() {
    qDebug() << "JavaScript запросил список изображений альбома";
    
    QStringList images;
    
    // Путь к директории с изображениями альбома
    QString path = "./frontend/dist/img";
    qDebug() << "Ищем изображения в директории альбома:" << path;
    
    QDir directory(path);
    if (!directory.exists()) {
        qWarning() << "Директория альбома" << path << "не существует!";
        
        // Создаем директорию, если она не существует
        if (directory.mkpath(".")) {
            qDebug() << "Директория альбома успешно создана";
        } else {
            qWarning() << "Не удалось создать директорию альбома!";
        }
        
        return images; // Возвращаем пустой список
    }
    
    // Фильтр для поиска SVG файлов
    QStringList filters;
    filters << "*.svg";
    directory.setNameFilters(filters);
    
    // Получаем список файлов, соответствующих фильтру
    QFileInfoList fileList = directory.entryInfoList();
    qDebug() << "Найдено файлов в альбоме:" << fileList.size();
    
    // Проходим по списку файлов и добавляем их имена в результат
    for (const QFileInfo &fileInfo : fileList) {
        qDebug() << "Добавляем файл альбома:" << fileInfo.fileName();
        images.append(fileInfo.fileName());
    }
    
    return images;
}