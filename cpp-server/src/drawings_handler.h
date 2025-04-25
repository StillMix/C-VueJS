// drawings_handler.h
#ifndef DRAWINGS_HANDLER_H
#define DRAWINGS_HANDLER_H

#include <QWebEngineUrlSchemeHandler>
#include <QWebEngineUrlRequestJob>
#include <QFile>
#include <QMimeDatabase>

class DrawingsSchemeHandler : public QWebEngineUrlSchemeHandler {
    Q_OBJECT
public:
    explicit DrawingsSchemeHandler(QObject *parent = nullptr) : QWebEngineUrlSchemeHandler(parent) {}
    
    void requestStarted(QWebEngineUrlRequestJob *job) override;
};

#endif // DRAWINGS_HANDLER_H