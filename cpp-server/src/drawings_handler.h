// drawings_handler.h
#ifndef DRAWINGS_HANDLER_H
#define DRAWINGS_HANDLER_H

#include <QWebEngineUrlSchemeHandler>
#include <QWebEngineUrlRequestJob>
#include <QFile>
#include <QMimeDatabase>
#include <QDebug>

class DrawingsSchemeHandler : public QWebEngineUrlSchemeHandler {
    Q_OBJECT
public:
    explicit DrawingsSchemeHandler(QObject *parent = nullptr) : QWebEngineUrlSchemeHandler(parent) {
        qDebug() << "DrawingsSchemeHandler создан";
    }
    
    void requestStarted(QWebEngineUrlRequestJob *job) override {
        QUrl url = job->requestUrl();
        QString fileName = url.path();
        
        qDebug() << "Запрос к drawings://" << fileName;
        
        // Удаляем начальный слеш
        if (fileName.startsWith("/")) {
            fileName = fileName.mid(1);
        }
        
        // Формируем путь к файлу
        QString filePath = QDir::current().filePath("drawings/" + fileName);
        qDebug() << "Полный путь к файлу:" << filePath;
        
        QFile *file = new QFile(filePath, job);
        if (!file->open(QIODevice::ReadOnly)) {
            qWarning() << "Ошибка открытия файла:" << file->errorString();
            job->fail(QWebEngineUrlRequestJob::UrlNotFound);
            return;
        }
        
        // Определяем MIME-тип файла
        QMimeDatabase db;
        QMimeType mime = db.mimeTypeForFile(filePath);
        qDebug() << "Отправка файла с MIME-типом:" << mime.name();
        
        // Отправляем файл
        job->reply(mime.name().toLatin1(), file);
    }
};

#endif // DRAWINGS_HANDLER_H