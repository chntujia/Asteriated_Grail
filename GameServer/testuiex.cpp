#include "testuiex.h"
#include "Server.h"
#include "main.h"
#include <QInputDialog>

void TestUIex::toDisplay(QString content)
{
    this->ui->inforDisplay->append(content);
}

/*
void TestUIex::actSLOT(QList<CardEntity*> hand,int currentID)
{
    int n;
    BatInfor infor;
    char temp[100];
    sprintf(temp,"you are player%d ,chose a card:",currentID);
    this->ui->inforDisplay->append(QString(temp));
    n = QInputDialog::getInt(NULL,"action","chose a card:",0,0,hand.size()-1);

    infor.CardID = hand.at(n)->getID();
    if(hand.at(n)->getType() == "attack")
    {
        infor.reply = ATTACK;
        infor.srcID = currentID;
        n = QInputDialog::getInt(NULL,"action","chose the target:",0,0,5);
        infor.dstID = n;

    }
    else if(hand.at(n)->getType() == "magic")
    {
        infor.reply = MAGIC;
        infor.srcID = currentID;
        //待完成
    }

    batBufferFree.acquire(1);
    batBuffer[batBufferTop++] = infor;
    if(batBufferTop == BUFFERSIZE)
        batBufferTop = 0;
    batBufferUsed.release(1);
}

void TestUIex::replySLOT(QList<CardEntity*> hand,QString element,int userID)
{
    int n;
    BatInfor infor;
    CardEntity* temp;
    char tempContent[100];
    sprintf(tempContent,"you are player%d ,and you are under %s attack",userID,element.toStdString().c_str());
    this->ui->inforDisplay->append(QString(tempContent));
    do
    {
    n = QInputDialog::getInt(NULL,"action","chose a card or input -1 if you don't reply:",0,-1,hand.size()-1);
    if(n == FINISH)
    {
        infor.reply = HIT;
        break;
    }
    else
    {
        temp = hand.at(n);
        if(temp->getElement() == "light")
        {
            infor.CardID = temp->getID();
            infor.reply = BLOCKED;
            infor.srcID = userID;
            break;
        }
        else if(element == "darkness")
            continue;
        else if(temp->getElement() == element || temp->getElement() == "darkness")
        {
            infor.CardID = temp->getID();
            infor.reply = REPLYBATTLE;

            n = QInputDialog::getInt(NULL,"action","chose the target:",0,0,5);
            infor.dstID = n;
            infor.srcID = userID;
            break;
        }
    }
    }while(1);
    batBufferFree.acquire(1);
    batBuffer[batBufferTop++] = infor;
    if(batBufferTop == BUFFERSIZE)
        batBufferTop = 0;
    batBufferUsed.release(1);
}

void TestUIex::healSLOT(int cross,int harmPoint)
{
    int n;
    do
    {

    n = QInputDialog::getInt(NULL,"heal","how many crosses do you want to use?:",0,0,cross);
    if(n <0 || n > harmPoint || n > cross)
        break;
    }while(1);

    inforBufferFree.acquire(1);
    inforBuffer[inforBufferTop++] = n;
    if(inforBufferTop == BUFFERSIZE)
        inforBufferTop = 0;
    inforBufferUsed.release(1);
}

void TestUIex::overLoadSLOT(QList<CardEntity*> hand,int overNum)
{
    int n,count = 0;
    char tempContent[100];
    sprintf(tempContent,"you need to discard %d cards.",overNum);
    this->ui->inforDisplay->append(QString(tempContent));

    for(int i = 0;i < hand.size();i++)
    {
        this->ui->inforDisplay->append(hand.at(i)->getName());
    }
    cardBufferFree.acquire(overNum);
    while(count < overNum)
    {

        sprintf(tempContent,"chose %d cards to discard:",overNum - count);


        n = QInputDialog::getInt(NULL,"discard",QString(tempContent),0,0,hand.size()-1);
        cardBuffer[cardBufferTop++] = hand.at(n)->getID();
        if(cardBufferTop == BUFFERSIZE)
            cardBufferTop = 0;
        count++;
    }
    cardBufferUsed.release(overNum);
}
*/

