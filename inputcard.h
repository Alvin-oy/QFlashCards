#ifndef INPUTCARD_H
#define INPUTCARD_H

#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QLabel>
#include <QVBoxLayout>

#include "cardentry.h"
#include "buttonpanel.h"

class InputCard : public QWidget
{
    Q_OBJECT
public:
    explicit InputCard(QWidget *parent = 0);
    void setData(const CardEntry* data);

signals:

public slots:

private:
    QLabel* question;
    QTextEdit* answer;
    ButtonPanel* buttons;
    QVBoxLayout* layout;
};

#endif // INPUTCARD_H
