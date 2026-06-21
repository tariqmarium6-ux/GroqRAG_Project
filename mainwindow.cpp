#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>

// --- CCA REQUIREMENT: STL Container ---
#include <vector>
#include <stdexcept>

std::vector<QString> chatHistoryVector;

// --- CCA REQUIREMENT: Template Function ---
template <typename T>
int getHistoryCount(const std::vector<T>& historyList) {
    return historyList.size();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // --- MAKE GUI PRETTY (Modern Dark Theme) ---
    this->setStyleSheet(
        "QMainWindow { background-color: #343541; }"
        "QTextEdit, QLineEdit { background-color: #40414f; color: white; border: 1px solid #565869; border-radius: 8px; padding: 10px; font-size: 14px; }"
        "QPushButton { background-color: #10a37f; color: white; border-radius: 6px; padding: 10px; font-weight: bold; font-size: 13px; }"
        "QPushButton:hover { background-color: #0e906f; }"
        "QPushButton#uploadbutton, QPushButton#btnExport { background-color: #565869; }"
        "QPushButton#uploadbutton:hover, QPushButton#btnExport:hover { background-color: #40414f; }"
        );

    // Backend setup
    BackendManager& backend = BackendManager::getInstance();
    connect(&backend, &BackendManager::answerReceived, this, &MainWindow::displayAnswer);
    connect(&backend, &BackendManager::errorOccurred, this, &MainWindow::displayError);
    connect(&backend, &BackendManager::processStarted, this, &MainWindow::displayStatus);

    // --- NEW: THE AUTOMATIC BACKGROUND SAVE MAGIC ---
    connect(ui->chatdisplay, &QTextEdit::textChanged, this, [this]() {
        // Yeh file automatically tumhare project ke 'build' folder (jahan .exe banti hai) mein save hoti jayegi
        QFile file("AutoSaved_Chat.txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << "--- GROQ AI RAG CHAT HISTORY (AUTO-SAVED) ---\n\n";
            out << ui->chatdisplay->toPlainText();
            file.close();
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

// --- BUTTON CLICKS ---

void MainWindow::on_uploadbutton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select PDF", "", "PDF Files (*.pdf)");

    if(!filePath.isEmpty()) {
        ui->chatdisplay->append("<b>You:</b> Uploaded document -> " + filePath);
        BackendManager::getInstance().uploadDocument(filePath);
    }
}

void MainWindow::on_askbutton_clicked()
{
    QString query = ui->queryinput->text().trimmed();

    // Exception Handling (try/catch/throw)
    try {
        if(query.isEmpty()) {
            throw std::invalid_argument("You cannot send an empty message to the AI!");
        }

        chatHistoryVector.push_back(query);
        int totalAsks = getHistoryCount(chatHistoryVector);
        qDebug() << "Total questions asked so far:" << totalAsks;

        ui->chatdisplay->append("<b>You:</b> " + query);
        ui->queryinput->clear();

        BackendManager::getInstance().askQuestion(query);

    } catch (const std::invalid_argument& e) {
        QMessageBox::warning(this, "Input Error", e.what());
    }
}

// Manual Save ka code maine rehne diya hai in case tum button click karo,
// lekin ab background mein khud bhi save ho raha hoga!
void MainWindow::on_btnExport_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save Chat History", "Manual_Chat_History.txt", "Text Files (*.txt)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not create file!");
        return;
    }

    QTextStream out(&file);
    out << "--- GROQ AI RAG CHAT HISTORY ---\n\n";
    out << ui->chatdisplay->toPlainText();
    file.close();

    QMessageBox::information(this, "Success", "Chat successfully saved to your computer!");
}

// --- DISPLAY FUNCTIONS ---

void MainWindow::displayAnswer(QString answer)
{
    ui->chatdisplay->append("<br><b>AI:</b> " + answer + "<br>");
}

void MainWindow::displayError(QString error)
{
    ui->chatdisplay->append("<br><font color='red'><b>Error:</b> " + error + "</font><br>");
}

void MainWindow::displayStatus(QString status)
{
    ui->chatdisplay->append("<i>" + status + "</i>");
}