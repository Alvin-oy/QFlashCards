#include "reviewcard.h"
#include <QVBoxLayout>

#include "cardentry.h"
#include "cardmanager.h"

ReviewCard::ReviewCard(QWidget *parent) : QWidget(parent)
{    
    question = new QTextBrowser(this);
    answer  = new QTextBrowser(this);
    buttons = new ButtonPanel(this);

    layout = new QVBoxLayout;
    layout->addWidget(question);
    layout->addWidget(answer);
    layout->addWidget(buttons);
    this->setLayout(layout);

    buttons->button1->setEffect(tr("Back"), 0);
    buttons->button2->hide();
    buttons->button3->hide();
    buttons->button4->setEffect(tr("Next"), 0);

    QObject::connect(buttons->button1, SIGNAL(clicked()),
                     this, SLOT(lastCard()));
    QObject::connect(buttons->button4, SIGNAL(clicked()),
                     this, SLOT(nextCard()));

    this->show();
}


void ReviewCard::setData(const CardEntry* data) {
    question->setText(data->getQuestion());
    answer->setText(data->getAnswer());
}


void ReviewCard::nextCard() {
    CardEntry* card = CardManager::getCardManager()->getNextCard();
    setData(card);
}


void ReviewCard::lastCard() {
    CardEntry* card = CardManager::getCardManager()->getPreviousCard();
    setData(card);
}
