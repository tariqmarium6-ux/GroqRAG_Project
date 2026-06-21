#ifndef BACKENDMANAGER_H
#define BACKENDMANAGER_H

#include <QObject>
#include <QProcess>
#include <QString>

class BackendManager : public QObject
{
    Q_OBJECT

public:
    // 🎯 SINGLETON PATTERN: Ek hi instance get karne ka function
    static BackendManager& getInstance();

    // Copy Constructor aur Assignment Operator ko delete kar diya
    // taake koi galti se iska doosra object na bana le.
    BackendManager(BackendManager const&) = delete;
    void operator=(BackendManager const&) = delete;

    // Core Backend Functions
    void uploadDocument(const QString& filePath);
    void askQuestion(const QString& query);

signals:
    // Yeh signals GUI (MainWindow) ko update karenge
    void answerReceived(QString answer);
    void errorOccurred(QString error);
    void processStarted(QString status);

private:
    // Private constructor (Singleton ki main requirement)
    explicit BackendManager(QObject *parent = nullptr);
    ~BackendManager();

    QProcess *pythonProcess;
    QString scriptPath;

    // Helper function Python ko chalane ke liye
    void runPythonCommand(const QStringList& arguments);
};

#endif // BACKENDMANAGER_H