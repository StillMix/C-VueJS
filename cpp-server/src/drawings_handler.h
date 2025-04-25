// Добавьте в новый файл cpp-server/src/drawings_handler.h
#ifndef DRAWINGS_HANDLER_H
#define DRAWINGS_HANDLER_H

#include <QWebEngineUrlSchemeHandler>
#include <QWebEngineUrlRequestJob>
#include <QFile>
#include <QMimeDatabase>

class DrawingsSchemeHandler : public QWebEngineUrlSchemeHandler {
    Q_OBJECT
public:
    DrawingsSchemeHandler(QObject *parent = nullptr) : QWebEngineUrlSchemeHandler(parent) {}

    void requestStarted(QWebEngineUrlRequestJob *job) override {
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
};

#endif // DRAWINGS_HANDLER_H