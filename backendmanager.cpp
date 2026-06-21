#include "backendmanager.h"
#include <QDebug>

// 🎯 SINGLETON INSTANCE SETUP
BackendManager& BackendManager::getInstance() {
    static BackendManager instance;
    return instance;
}

// CONSTRUCTOR
BackendManager::BackendManager(QObject *parent) : QObject(parent) {
    pythonProcess = new QProcess(this);

    // ⚠️ Tumhari Python file ka exact RASTA
    scriptPath = "D:/NED_OOP_SEM2_PROJECT/GroqRAG_Project/backend/rag_engine.py";

    // --- MAGIC HAPPENS HERE: Reading Python Output ---
    connect(pythonProcess, &QProcess::readyReadStandardOutput, this, [this]() {
        QString output = pythonProcess->readAllStandardOutput();
        qDebug() << "Python Terminal Output:\n" << output;

        // Agar output mein humara AI ka jawab hai
        if(output.contains("--- AI ANSWER ---")) {
            QStringList parts = output.split("--- AI ANSWER ---");
            if(parts.size() > 1) {
                QString answerPart = parts[1].remove("-----------------").trimmed();
                emit answerReceived(answerPart);
            }
        }
        // Agar upload successful ho gaya
        else if (output.contains("SUCCESS!")) {
            emit answerReceived("System: Document processed and saved successfully! You can ask questions now.");
        }
    });

    // 🎯 THE FIX: Python ki background loading bars ko ignore karo
    connect(pythonProcess, &QProcess::readyReadStandardError, this, [this]() {
        QString err = pythonProcess->readAllStandardError();
        // Ab hum GUI par error emit nahi kar rahe, sirf background mein chup chap print kar rahe hain
        qDebug() << "Python Background Process: " << err;
    });
}

// DESTRUCTOR
BackendManager::~BackendManager() {
    if(pythonProcess->state() == QProcess::Running) {
        pythonProcess->kill();
        pythonProcess->waitForFinished();
    }
}

// --- CORE FUNCTIONS ---
void BackendManager::uploadDocument(const QString& filePath) {
    runPythonCommand(QStringList() << "--upload" << filePath);
}

void BackendManager::askQuestion(const QString& query) {
    runPythonCommand(QStringList() << "--ask" << query);
}

void BackendManager::runPythonCommand(const QStringList& arguments) {
    // Agar AI pehle se soch raha hai, toh doosra sawal rok do
    if(pythonProcess->state() == QProcess::Running) {
        emit errorOccurred("Please wait, the AI is already processing a request...");
        return;
    }

    QStringList fullArguments;
    fullArguments << scriptPath << arguments;

    emit processStarted("Connecting to AI Brain...");
    pythonProcess->start("python", fullArguments);
}