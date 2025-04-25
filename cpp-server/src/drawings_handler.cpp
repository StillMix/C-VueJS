// drawings_handler.cpp
#include "drawings_handler.h"

void DrawingsSchemeHandler::requestStarted(QWebEngineUrlRequestJob *job) {
    QUrl url = job->requestUrl();
    QString fileName = url.path();
    
    // Удаляем начальный слеш
    if (fileName.startsWith("/")) {
        fileName = fileName.mid(1);
    }
    
    // Формируем путь к файлу
    QString filePath = "./drawings/" + fileName;
    
    QFile *file = new QFile(filePath, job);
    if (!file->open(QIODevice::ReadOnly)) {
        job->fail(QWebEngineUrlRequestJob::UrlNotFound);
        return;
    }
    
    // Определяем MIME-тип файла
    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(filePath);
    
    // Отправляем файл
    job->reply(mime.name().toLatin1(), file);
}