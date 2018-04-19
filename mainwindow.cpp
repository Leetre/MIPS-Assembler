#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QToolBar *fileToolBar = addToolBar(tr("File"));
    ui->setupUi(this);
    assembler = new Assembler();
    disassembler = new Disassembler();
    setWindowIcon(QIcon(":/images/mips.jpg"));

//    ui->textEdit->setFontPointSize(15);
//    ui->textEdit->setFontFamily(tr("Consolas"));
//    QList<QStringList> p;
//    QStringList t;
//    t << "A" << "01" << "02" << "03" << "04";
//    p << t;
//    model = new TableModel(p);
//    ui->tableView->setModel(model);
//    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open"), this);
    openAct->setStatusTip(tr("Open the exist file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::openActionCallback);
    fileToolBar->addAction(openAct);
    ui->actionOpen->setShortcut(QKeySequence::Open);
    ui->actionOpen->setStatusTip(tr("Open an existing file"));
    connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(openActionCallback()));

    QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    QAction *saveAct = new QAction(saveIcon, tr("&Save"), this);
    saveAct->setStatusTip(tr("Save the current file"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveActionCallback);
    fileToolBar->addAction(saveAct);
    ui->actionSave->setShortcut(QKeySequence::Save);
    ui->actionSave->setStatusTip(tr("Save the current file"));
    connect(ui->actionSave, SIGNAL(triggered(bool)), this, SLOT(saveActionCallback()));

    QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *newAct = new QAction(newIcon, tr("&New"), this);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::CreateNewActionCallback);
    fileToolBar->addAction(newAct);
    ui->actionNew->setShortcut(QKeySequence::New);
    ui->actionNew->setStatusTip(tr("Create a new file"));
    connect(ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(CreateNewActionCallback()));

    ui->actionFont_Size->setStatusTip(tr("Change the size of font"));
    connect(ui->actionFont_Size, SIGNAL(triggered(bool)), this, SLOT(FontSizeSetActionCallback()));

    ui->actionFont_Color->setStatusTip(tr("Change the color of font"));
    connect(ui->actionFont_Color, SIGNAL(triggered(bool)), this, SLOT(FontColorSetActionCallback()));

    ui->actionChange_Color->setStatusTip(tr("Change the color of back ground"));
    connect(ui->actionChange_Color, SIGNAL(triggered(bool)), this, SLOT(BackGroudActionCallback()));

    QIcon copyIcon = QIcon::fromTheme("document-copy", QIcon(":/images/copy.png"));
    QAction *copyAct = new QAction(copyIcon, tr("&Copy"), this);
    copyAct->setStatusTip(tr("Copy the text"));
    connect(copyAct, &QAction::triggered, this, &MainWindow::CopyTextActionCallback);
    fileToolBar->addAction(copyAct);
    ui->actionCopy->setShortcut(QKeySequence::Copy);
    ui->actionCopy->setStatusTip(tr("Copy the text"));
    connect(ui->actionCopy, SIGNAL(triggered(bool)), this, SLOT(CopyTextActionCallback()));

    QIcon pasteIcon = QIcon::fromTheme("document-paste", QIcon(":/images/paste.png"));
    QAction *pasteAct = new QAction(pasteIcon, tr("&Paste"), this);
    pasteAct->setStatusTip(tr("Paste the text"));
    connect(pasteAct, &QAction::triggered, this, &MainWindow::PasteTextActionCallback);
    fileToolBar->addAction(pasteAct);
    ui->actionPaste->setShortcut(QKeySequence::Paste);
    ui->actionPaste->setStatusTip(tr("Paste the text"));
    connect(ui->actionPaste, SIGNAL(triggered(bool)), this, SLOT(PasteTextActionCallback()));

    ui->actionClose->setShortcut(QKeySequence::Close);
    ui->actionClose->setStatusTip(tr("Close the window"));
    connect(ui->actionClose, SIGNAL(triggered(bool)), this, SLOT(CloseWinActionCallback()));

    const QIcon compileIcon = QIcon::fromTheme("compile-file", QIcon(":/images/compfile.png"));
    QAction *compileAct = new QAction(compileIcon, tr("&Compile"), this);
    compileAct->setStatusTip(tr("Compile file"));
    connect(compileAct, &QAction::triggered, this, &MainWindow::compile);
    fileToolBar->addAction(compileAct);
    ui->actionCompile->setShortcut(tr("CTRL+D"));
    ui->actionCompile->setStatusTip(tr("Compile file"));
    connect(ui->actionCompile, SIGNAL(triggered(bool)), this, SLOT(compile()));

    const QIcon discompileIcon = QIcon::fromTheme("discompile-file", QIcon(":/images/discompile.png"));
    QAction *discompileAct = new QAction(discompileIcon, tr("&Compile"), this);
    discompileAct->setStatusTip(tr("Discompile file"));
    connect(discompileAct, &QAction::triggered, this, &MainWindow::discompile);
    fileToolBar->addAction(discompileAct);
    ui->actionDiscompile->setShortcut(tr("CTRL+G"));
    ui->actionDiscompile->setStatusTip(tr("Discompile file"));
    connect(ui->actionDiscompile, SIGNAL(triggered(bool)), this, SLOT(discompile()));

    QIcon exitIcon = QIcon::fromTheme("document-exit", QIcon(":/images/exit.jpg"));
    QAction *exitAct = fileToolBar->addAction(exitIcon, tr("&Exit"), this, &QWidget::close);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::openActionCallback()
{
    QString fileName = QFileDialog::getOpenFileName();
    if(fileName.length() != 0){
        QFile file(fileName);
        if(!file.open(QFile::ReadOnly | QFile::Text)) {
            throw "could not open file.";
            return false;
        }
        ui->textEdit->setText(QString(file.readAll()));
        ui->textEdit->setVisible(true);
        ui->messageEdit->document()->setPlainText("Open the file:" + fileName);
        ui->messageEdit->setVisible(true);
    }
    else{
        ui->messageEdit->document()->setPlainText("You didn't open any file");
    }


    return true;
}


bool MainWindow::saveActionCallback()
{
    QString text = ui->textEdit->toPlainText();
    QString filename = QFileDialog::getSaveFileName();
    if (filename.length() != 0){
        QFile file(filename);
        if(!file.open(QFile::WriteOnly | QFile::Text)) {
            throw "could not write file.";
            return false;
        }
        QTextStream out(&file);
        out << text;
        ui->messageEdit->document()->setPlainText("Save the file successfully!");
        ui->messageEdit->setVisible(true);
    }
    else{
        ui->messageEdit->document()->setPlainText("You didn't do any thing");
    }


    return true;
}

bool MainWindow::maybeSave()
{
    if (!ui->textEdit->document()->isModified())
        return true;
    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("Application"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        return saveActionCallback();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

bool MainWindow::CreateNewActionCallback()
{
    if (maybeSave()) {
        ui->textEdit->clear();
        ui->messageEdit->document()->setPlainText("Clear the information successfully!");
        ui->messageEdit->setVisible(true);
    }
    QString curFile = tr("Untiled.txt");
    setWindowTitle(curFile);
    ui->textEdit->setVisible(true);

    return true;
}

void MainWindow::FontSizeSetActionCallback()
{
    bool ok;
    QFont uiFont = QFontDialog::getFont(&ok,ui->textEdit->font(),this,tr("字体设置"));
    if(ok)
    {
        ui->textEdit->setFont(uiFont);
    }
}

void MainWindow::FontColorSetActionCallback()
{
    QPalette uiPalette = ui->textEdit->palette();
    QColor uiColor = QColorDialog::getColor(Qt::blue,this,"设置字体颜色");
    if(uiColor.isValid())
    {
        uiPalette.setColor(QPalette::Text,uiColor);
        ui->textEdit->setPalette(uiPalette);
    }
}

void MainWindow::BackGroudActionCallback()
{
    QPalette uiBackPalette = ui->textEdit->palette();
    QColor uiColor = QColorDialog::getColor(Qt::blue,this,tr("选择背景颜色"));
    if(uiColor.isValid())
    {
        uiBackPalette.setColor(QPalette::Base,uiColor);
        ui->textEdit->setPalette(uiBackPalette);
    }
}

void MainWindow::CopyTextActionCallback()
{
    ui->textEdit->copy();
}

void MainWindow::PasteTextActionCallback()
{
    ui->textEdit->paste();
}

void MainWindow::CloseWinActionCallback()
{
    if(maybeSave()) {
        ui->textEdit->setVisible(false);
    }
}

void MainWindow::compile()
{
    QString textEditPlainText = ui->textEdit->document()->toPlainText();
    QString result = assembler->Compile(textEditPlainText);
    ui->assemblyText->document()->setPlainText(result);
    ui->messageEdit->document()->setPlainText("Compile successfully!");
}

void MainWindow::discompile()
{
    QString textEditPlainText = ui->textEdit->document()->toPlainText();
    QString result = disassembler->Discompile(textEditPlainText);
    ui->assemblyText->document()->setPlainText(result);
    ui->messageEdit->document()->setPlainText("Discompile successfully!");
}
