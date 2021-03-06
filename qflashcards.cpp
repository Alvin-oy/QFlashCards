#include "qflashcards.h"
#include "ui_qflashcards.h"
#include "cardgroup.h"
#include "cardmanager.h"
#include "cardentry.h"
#include "centrallabel.h"
#include <QLabel>
#include <QSizePolicy>
#include <QFileDialog>
#include <QTextBrowser>
#include <QDesktopServices>
#include <QUrl>
#include <QFileInfo>
#include <QDebug>
#include "filehandler.h"


const QString GPL("https://www.gnu.org/licenses/gpl-3.0.en.html");
const QUrl GPLURL(GPL);

QFlashCards::QFlashCards(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QFlashCards)
{
    ui->setupUi(this);
    show();
}

QFlashCards::~QFlashCards()
{
    delete ui;
}


void QFlashCards::setWorkingDir(QString dir) {
    this->dir = dir;
}


void QFlashCards::showSpecialText(MessageType msg, CardType caller) {
    CentralLabel* label = new CentralLabel(this);
    label->setMessage(msg);
    label->setCardType(caller);
    connect(label, &CentralLabel::signalTryAgain, this, &QFlashCards::gotoCard);
    connect(label, &CentralLabel::signalNextCard, this, &QFlashCards::gotoNextCard);
    connect(label, &CentralLabel::signalShowAnswer, this, &QFlashCards::gotoShowAnswer);
    setCentralWidget(label);
}

/**
 * @brief QFlashCards::on_actionExit_triggered
 *
 * Well, exit the application when the exit menu is selected
 */
void QFlashCards::on_actionExit_triggered()
{
    exit(0);
}

void QFlashCards::on_actionReview_triggered()
{
    reviewCard = new ReviewCard(this);
    CardManager::getCardManager()->shuffle();
    CardManager::getCardManager()->reset();
    reviewCard->setData(CardManager::getCardManager()->getCard());
    setCentralWidget(reviewCard);
    centralWidget()->showNormal();
}

void QFlashCards::on_actionFree_Responce_triggered()
{
    freeResponceCard = new InputCard(this);
    CardManager::getCardManager()->shuffle();
    CardManager::getCardManager()->reset();
    freeResponceCard->setData(CardManager::getCardManager()->getCard());
    setCentralWidget(freeResponceCard);
    connect(freeResponceCard, &InputCard::writeToStatus,
            this, &QFlashCards::showSpecialText);
    centralWidget()->showNormal();
}

void QFlashCards::on_actionNewCards_triggered()
{
    createNewCards = new CreateCard(this);
    CardManager::getCardManager()->sort();
    CardManager::getCardManager()->reset();
    createNewCards->setData(CardManager::getCardManager()->getNewCard());
    statusBar()->showMessage(tr("New Deck Started"), 2000);
    setWindowTitle(tr("Flash Cards"));
    setCentralWidget(createNewCards);
    centralWidget()->showNormal();
}

void QFlashCards::on_actionEdit_Cards_triggered()
{
    editCards = new EditingCard(this);
    CardManager::getCardManager()->sort();
    CardManager::getCardManager()->preset();
    CardEntry* aCard = CardManager::getCardManager()->getCard();
    editCards->setData(aCard);
    setCentralWidget(editCards);    
    CardManager::getCardManager()->sort();
    centralWidget()->showNormal();
}


void QFlashCards::gotoCard(CardType card) {
    switch(card) {
        case CardType::INPUT:
            gotoFreeResponce();
            break;
        default:
            break;
    }
}



void QFlashCards::gotoNextCard(CardType card) {
    switch(card) {
        case CardType::INPUT:            
			freeResponceCard = new InputCard(this);			
			freeResponceCard->setData(CardManager::getCardManager()->getCard());
			setCentralWidget(freeResponceCard);
			connect(freeResponceCard, &InputCard::writeToStatus,
					this, &QFlashCards::showSpecialText);
			centralWidget()->showNormal();
            break;
        default:
            break;
    }
}



void QFlashCards::gotoShowAnswer(CardType card) {
    switch(card) {
        case CardType::INPUT:
            freeResponceCard = new InputCard(this);
            freeResponceCard->setData(CardManager::getCardManager()->getCurrentCard());
            setCentralWidget(freeResponceCard);
            connect(freeResponceCard, &InputCard::writeToStatus,
                    this, &QFlashCards::showSpecialText);
            freeResponceCard->showAnswer();
            centralWidget()->showNormal();
            break;
        default:
            break;
    }
}

void QFlashCards::gotoFreeResponce()
{
    freeResponceCard = new InputCard(this);
    freeResponceCard->setData(CardManager::getCardManager()->getCurrentCard());
    setCentralWidget(freeResponceCard);
    connect(freeResponceCard, &InputCard::writeToStatus,
            this, &QFlashCards::showSpecialText);
    centralWidget()->showNormal();
}


void QFlashCards::on_actionLoad_triggered()
{
    QString fileName
            = QFileDialog::getOpenFileName(
                this, tr("Open File"),
                dir,
                tr("Flashcards (*.qfcml)"));
    if(fileName.isEmpty()) {
        return;
    }
    statusBar()->showMessage(tr("Loaded File"), 2000);
    FileHandler handler;
    handler.readFile(fileName);
    upDateTileWName(fileName);
    on_actionReview_triggered();
}

void QFlashCards::on_actionSave_triggered()
{   QString* fileName = CardManager::getCardManager()->getFileName();
    if(fileName->isEmpty()) {
        qDebug() << "Card was empty";
        on_actionSave_As_triggered();
    } else {
        FileHandler handler;
        QFile file(*fileName);
        QFileInfo info(file);
        // This should never really occur now
        if(info.isDir()) {
            fileName->append("untitled.qfcml");
        }
        statusBar()->showMessage(tr("Saved File"), 2000);
        handler.saveFile(*fileName);
        upDateTileWName(*fileName);
    }
}


void QFlashCards::on_actionSave_As_triggered()
{
    QString fileName
            = QFileDialog::getSaveFileName(
                this, tr("Save Flashcards"),
                dir,
                tr("Flashcards (*.qfcml)"));
    if(fileName.isEmpty()) {
        return;
    }
    FileHandler handler;
    QFile file(fileName);
    QFileInfo info(file);
    // This should never really occur now
    if(info.isDir()) {
        fileName.append("untitled.qfcml");
    }
    statusBar()->showMessage(tr("Saved File"), 2000);
    handler.saveFile(fileName);
    upDateTileWName(fileName);
}


void QFlashCards::on_actionNew_triggered()
{
    CardManager::getCardManager()->newCardGroup();
    on_actionNewCards_triggered();
}

void QFlashCards::on_actionHelp_triggered()
{
    QTextBrowser* helpview = new QTextBrowser;
    helpview->setEnabled(true);
    helpview->setSource(QString("qrc:///html/help.html"));
    setCentralWidget(helpview);
}

void QFlashCards::on_actionAbout_triggered()
{
    QTextBrowser* helpview = new QTextBrowser;
    helpview->setEnabled(true);
    helpview->setSource(QString("qrc:///html/about.html"));
    setCentralWidget(helpview);
}

void QFlashCards::on_actionLicense_triggered()
{
    QDesktopServices::openUrl(GPLURL);
}

void QFlashCards::upDateTileWName(const QString& fileName) {
    QFile file(fileName);
    QFileInfo info(file);
    QString name = info.fileName();
    int dot = name.lastIndexOf(".");
    int len = name.length() - dot;
    if(dot > - 1) {
        name.remove(dot, len);
    }
    setWindowTitle(tr("Flash Cards")
                   .append(" - ")
                   .append(name));
    setWorkingDir(info.canonicalPath());
}
