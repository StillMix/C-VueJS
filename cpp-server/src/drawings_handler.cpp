// drawings_handler.cpp
#include "drawings_handler.h"

// Реализация перемещена в .h файл для улучшения отладки
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
    QString albumPath = QDir::current().filePath("./frontend/dist/album/");
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
    QString path = "./frontend/dist/album";
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
