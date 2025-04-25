// main.cpp - Главный файл приложения с расширенным логированием
#include <QApplication>
#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QWebChannel>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QStandardPaths>
#include "backend.h"
#include <QWebEngineProfile>
#include "drawings_handler.h"
#include <QWebEngineUrlScheme>

int main(int argc, char *argv[]) {
    // Настраиваем логирование в файл
    QString logFilePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/vueqt_app_log.txt";
    qDebug() << "Лог будет записан в:" << logFilePath;
    
    QFile logFile(logFilePath);
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Лог-файл успешно открыт";
    } else {
        qDebug() << "Не удалось открыть лог-файл:" << logFile.errorString();
    }
    
    qDebug() << "Запуск приложения...";
    
    // Для работы с Qt WebEngine необходимы эти параметры
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    
    qDebug() << "Регистрация обработчика схемы drawings://...";

    QWebEngineUrlScheme drawingsScheme("drawings");
    drawingsScheme.setFlags(QWebEngineUrlScheme::SecureScheme | 
                          QWebEngineUrlScheme::LocalScheme |
                          QWebEngineUrlScheme::LocalAccessAllowed);
    QWebEngineUrlScheme::registerScheme(drawingsScheme);

    QApplication app(argc, argv);
    
    qDebug() << "Инициализация приложения Qt...";
    
    // Проверка путей
    QString currentDir = QDir::currentPath();
    qDebug() << "Текущая директория:" << currentDir;
    
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    qDebug() << "Новая текущая директория:" << QDir::currentPath();
    
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
        qDebug() << "Содержимое директории drawings:";
        QStringList entries = drawingsDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QString &entry : entries) {
            qDebug() << " - " << entry;
        }
    }
    
    // Проверка frontend директории
    QDir frontendDir("./frontend");
    if (!frontendDir.exists()) {
        qWarning() << "Директория frontend не существует!";
    } else {
        qDebug() << "Директория frontend существует.";
        qDebug() << "Содержимое директории frontend:";
        QStringList entries = frontendDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QString &entry : entries) {
            qDebug() << " - " << entry;
        }
    }
    
    QWebEngineProfile::defaultProfile()->installUrlSchemeHandler(
        "drawings", new DrawingsSchemeHandler()
    );
    
    // Создаем экземпляр WebEngineView - компонент для отображения веб-содержимого
    qDebug() << "Создание WebEngineView...";
    QWebEngineView *view = new QWebEngineView();
    
    // Включаем отладочную информацию
    view->page()->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    view->page()->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    view->page()->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    view->page()->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    
    // Создаем экземпляр нашего бэкенда
    qDebug() << "Создание бэкенда...";
    Backend *backend = new Backend();
    
    // Создаем канал для общения между C++ и JavaScript
    qDebug() << "Создание WebChannel...";
    QWebChannel *channel = new QWebChannel();
    
    // Регистрируем объект backend для доступа из JavaScript
    channel->registerObject("backend", backend);
    
    // Устанавливаем канал для веб-страницы
    view->page()->setWebChannel(channel);
    
    // Подключаем обработчик загрузки для отслеживания ошибок
    QObject::connect(view->page(), &QWebEnginePage::loadFinished, 
                     [=](bool success) {
                         if (success) {
                             qDebug() << "Страница успешно загружена!";
                         } else {
                             qDebug() << "Ошибка загрузки страницы!";
                         }
                     });
    
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
        
        QFile resourceFile(":vue-client/dist/index.html");
        if (resourceFile.exists()) {
            qDebug() << "Файл существует в ресурсах";
        } else {
            qWarning() << "Файл не найден и в ресурсах!";
        }
    } else {
        path = QUrl::fromLocalFile(path).toString();
        qDebug() << "Загрузка из локального файла:" << path;
    }
#endif
    
    // Загружаем Vue.js приложение
    qDebug() << "Загрузка приложения Vue.js из:" << path;
    view->load(QUrl(path));
    
    // Устанавливаем размеры окна
    view->resize(1024, 768);
    qDebug() << "Отображение окна...";
    view->show();
    
    qDebug() << "Запуск главного цикла событий...";
    return app.exec();
}