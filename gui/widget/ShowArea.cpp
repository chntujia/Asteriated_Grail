#include "ShowArea.h"
#include <QPainter>

static QRectF ShowAreaRect(0, 0, 550, 150);
ShowArea::ShowArea()
{

}
QRectF ShowArea::boundingRect() const
{
    return ShowAreaRect;
}

void ShowArea::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    int n = cards.size();
    int i,j;
    int card_skip;
    int offset=0;
    if(n > 5)
        card_skip = 450/(n-1);        
    else
    {
        card_skip = 100;
        offset=275-n*0.5*100;
    }
    Card*card;
    QPixmap pixmap;
    int width;
    int height;
    for(i=0;i<n;i++)
    {
        card=cards[i];
        pixmap=QPixmap(card->getSource());
        width=pixmap.width();
        height=pixmap.height();
        painter->drawPixmap(offset+card_skip*i,0,pixmap);
        painter->drawText(offset+width*0.8+i*card_skip,height*0.68,card->getProperty());
        for(j=0;j<card->getHasSpeciality();j++)
            painter->drawText(offset+width*0.15+i*card_skip,height*(0.76+j*0.13),card->getSpecialityList().at(j));
    }

}

void ShowArea::showCards(QList<Card *> cards)
{
    this->cards.clear();
    foreach(Card* ptr,cards)
        this->cards<<ptr;
    update();
}


