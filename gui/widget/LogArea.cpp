#include "LogArea.h"

LogArea::LogArea(QWidget *parent) :
    QTextEdit(parent)
{
    setReadOnly(true);
    setStyleSheet("background-color:transparent");
    setFixedSize(240,618);
}
void LogArea::append(const QString &text)
{
    QTextEdit::append(QString("<p style=\"margin:3px p2x; line-height:120%;\"><font color=\'white\'>%1</font></p>").arg(text));
}
