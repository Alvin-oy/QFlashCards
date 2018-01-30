#include "editcard.h"
#include "cardmanager.h"

EditingCard::EditingCard(QWidget *parent) :
            QWidget(parent),
            answer(new QTextEdit),
            question(new QTextEdit),
            buttons(new ButtonPanel),
            layout(new QVBoxLayout) {

    buttons->button1->setEffect("&Back", 0);
    buttons->button2->setEffect("Delete", 0);
    buttons->button3->setEffect("&Save", 0);
    buttons->button4->setEffect("&Next", 0);

    layout->addWidget(question);
    layout->addWidget(answer);
    layout->addWidget(buttons);
    setLayout(layout);

    QObject::connect(buttons->button1, SIGNAL(clicked()),
                     this, SLOT(previousCard()));
    QObject::connect(buttons->button2, SIGNAL(clicked()),
                     this, SLOT(removeCard()));
    QObject::connect(buttons->button3, SIGNAL(clicked()),
                     this, SLOT(saveCard()));
    QObject::connect(buttons->button4, SIGNAL(clicked()),
                     this, SLOT(nextCard()));
}


void EditingCard::setData(const CardEntry* data) {
    question->setText(data->getQuestion());
    answer->setText(data->getAnswer());
}


void EditingCard::previousCard() {

}

void EditingCard::removeCard() {

}

void EditingCard::saveCard() {

}

void EditingCard::nextCard() {

}
