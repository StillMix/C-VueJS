// main.cpp - Главный файл приложения
#include <QApplication>
#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QWebChannel>
#include <QFile>
#include <QDir>
#include <QDebug>
#include "backend.h"
#include <QWebEngineProfile>
#include "drawings_handler.h"
#include <QWebEngineUrlScheme>

int main(int argc, char *argv[]) {
    qDebug() << "Запуск приложения...";
    
    // Для работы с Qt WebEngine необходимы эти параметры
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    
    qDebug() << "Регистрация обработчика схемы drawings://...";

    QWebEngineUrlScheme drawingsScheme("drawings");
drawingsScheme.setFlags(QWebEngineUrlScheme::SecureScheme | 
                       QWebEngineUrlScheme::LocalScheme |
                       QWebEngineUrlScheme::LocalAccessAllowed);
QWebEngineUrlScheme::registerScheme(drawingsScheme);

QWebEngineProfile::defaultProfile()->installUrlSchemeHandler(
    "drawings", new DrawingsSchemeHandler()
);
    
    qDebug() << "Инициализация приложения Qt...";
    // Инициализация приложения Qt
    QApplication app(argc, argv);
    
    // Проверка существования директории для рисунков
    QDir drawingsDir("./drawings");
    if (!drawingsDir.exists()) {
        qDebug() << "Директория drawings не существует, создаём...";
        if (drawingsDir.mkpath(".")) {
            qDebug() << "Директория drawings успешно создана";
        } else {
            qWarning() << "Не удалось создать директорию drawings!";
        }
    } else {
        qDebug() << "Директория drawings существует.";
    }
    
    // Создаем экземпляр WebEngineView - компонент для отображения веб-содержимого
    qDebug() << "Создание WebEngineView...";
    QWebEngineView view;
    
    // Создаем экземпляр нашего бэкенда
    qDebug() << "Создание бэкенда...";
    Backend *backend = new Backend();
    
    // Создаем канал для общения между C++ и JavaScript
    qDebug() << "Создание WebChannel...";
    QWebChannel *channel = new QWebChannel();
    
    // Регистрируем объект backend для доступа из JavaScript
    channel->registerObject("backend", backend);
    
    // Устанавливаем канал для веб-страницы
    view.page()->setWebChannel(channel);
    
    // Определяем путь до нашего Vue.js приложения
    QString path;
    
#ifdef QT_DEBUG
    path = "http://localhost:8080";
    qDebug() << "Режим отладки: загрузка из" << path;
#else
    // В релизном режиме используем собранные файлы Vue.js из директории dist
    path = QDir::current().filePath("frontend/dist/index.html");
    QFile file(path);
    if (!file.exists()) {
        qWarning() << "Файл не найден:" << path;
        path = "qrc:/vue-client/dist/index.html";
        qDebug() << "Пробуем загрузить из ресурсов:" << path;
    } else {
        path = QUrl::fromLocalFile(path).toString();
        qDebug() << "Загрузка из локального файла:" << path;
    }
#endif
    
    // Загружаем Vue.js приложение
    qDebug() << "Загрузка приложения Vue.js...";
    view.load(QUrl(path));
    
    // Устанавливаем размеры окна
    view.resize(1024, 768);
    qDebug() << "Отображение окна...";
    view.show();
    
    qDebug() << "Запуск главного цикла событий...";
    return app.exec();
}