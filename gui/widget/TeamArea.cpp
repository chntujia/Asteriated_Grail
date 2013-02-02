#include "TeamArea.h"
#include "data/DataInterface.h"
#include <QPainter>
#include <QFont>

TeamArea::TeamArea()
{
    round=0;
    droppedCardNum=0;
    leftCardNum=150;
    red=dataInterface->getRedTeam();
    blue=dataInterface->getBlueTeam();
    background=QPixmap("resource/teamArea.png");

    Rgrail=QPixmap("resource/Rgrail.png");
    Bgrail=QPixmap("resource/Bgrail.png");
    Pcrystal=QPixmap("resource/crystal.png");
    Pgem=QPixmap("resource/gem.png");
//    Rdice[0]=QPixmap("resource/1r.png");
//    Rdice[1]=QPixmap("resource/2r.png");
//    Rdice[2]=QPixmap("resource/3r.png");
//    Rdice[3]=QPixmap("resource/4r.png");
//    Rdice[4]=QPixmap("resource/5r.png");
//    Rdice[5]=QPixmap("resource/6r.png");

//    Bdice[0]=QPixmap("resource/1b.png");
//    Bdice[1]=QPixmap("resource/2b.png");
//    Bdice[2]=QPixmap("resource/3b.png");
//    Bdice[3]=QPixmap("resource/4b.png");
//    Bdice[4]=QPixmap("resource/5b.png");
//    Bdice[5]=QPixmap("resource/6b.png");

    Rdice[0]=QPixmap("resource/S1.png");
    Rdice[1]=QPixmap("resource/S2.png");
    Rdice[2]=QPixmap("resource/S3.png");
    Rdice[3]=QPixmap("resource/S4.png");
    Rdice[4]=QPixmap("resource/S5.png");
    Rdice[5]=QPixmap("resource/S6.png");

    Bdice[0]=QPixmap("resource/S1.png");
    Bdice[1]=QPixmap("resource/S2.png");
    Bdice[2]=QPixmap("resource/S3.png");
    Bdice[3]=QPixmap("resource/S4.png");
    Bdice[4]=QPixmap("resource/S5.png");
    Bdice[5]=QPixmap("resource/S6.png");

    Num[0]=QPixmap("resource/0.png");
    Num[1]=QPixmap("resource/1.png");
    Num[2]=QPixmap("resource/2.png");
    Num[3]=QPixmap("resource/3.png");
    Num[4]=QPixmap("resource/4.png");
    Num[5]=QPixmap("resource/5.png");
    Num[6]=QPixmap("resource/6.png");
    Num[7]=QPixmap("resource/7.png");
    Num[8]=QPixmap("resource/8.png");
    Num[9]=QPixmap("resource/9.png");

    if(red->getMoraleMax()==15)
        avg=5;
    else
        avg=6;
    width=background.width();
    height=background.height();
}

QRectF TeamArea::boundingRect() const
{
    return QRectF(0, 0, width, height);
}

void TeamArea::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    int i;

    int offset=13;
    int morale=red->getMorale();
    painter->drawPixmap(0, 0, background);
    for(i=0;i<morale/avg;i++) //士气
        painter->drawPixmap(i*(Rdice[1].width()+2)+offset,17,Rdice[avg-1]);
    offset+=i*(Rdice[1].width()+2);
    if(morale%avg!=0)
        painter->drawPixmap(offset,17,Rdice[morale%avg-1]);

    offset=100;
    painter->drawPixmap(offset,4,Num[morale/10]);
    painter->drawPixmap(offset+Num[0].width(),4,Num[morale%10]);

    offset=145;
    for(i=0;i<red->getGrail();i++) //杯
        painter->drawPixmap(Rgrail.width()*i+offset,5,Rgrail);

    offset+=Rgrail.width()*5+9;
    for(i=0;i<red->getCrystal();i++) //水晶
        painter->drawPixmap((Pcrystal.width()+4)*i+offset,4,Pcrystal);

    offset+=(Pcrystal.width()+4)*i;
    for(i=0;i<red->getGem();i++) //宝石
        painter->drawPixmap((Pgem.width()+4)*i+offset,4,Pgem);

    offset=420;
    painter->drawPixmap(offset,4,Num[round/10]);
    painter->drawPixmap(offset+Num[0].width(),2,Num[round%10]);

    offset=459;
    for(i=0;i<blue->getCrystal();i++) //水晶
        painter->drawPixmap((Pcrystal.width()+4)*i+offset,4,Pcrystal);

    offset+=(Pcrystal.width()+4)*i;
    for(i=0;i<blue->getGem();i++) //宝石
        painter->drawPixmap((Pgem.width()+4)*i+offset,4,Pgem);

    offset=598;
    for(i=0;i<blue->getGrail();i++) //杯
        painter->drawPixmap(Rgrail.width()*i+offset,5,Bgrail);

    offset=737;
    painter->drawPixmap(offset,4,Num[morale/10]);
    painter->drawPixmap(offset+Num[0].width(),4,Num[morale%10]);

    offset=788;
    morale=blue->getMorale();
    for(i=0;i<morale/avg;i++) //士气
        painter->drawPixmap(i*(Bdice[1].width()+2)+offset,17,Bdice[avg-1]);
    offset+=i*(Bdice[1].width()+2);
    if(morale%avg!=0)
        painter->drawPixmap(offset,17,Bdice[morale%avg-1]);

    QFont font;
    font.setBold(1);
    painter->setFont(font);
    painter->drawText(width+90,20,QString::number(leftCardNum));
    painter->drawText(width+230,20,QString::number(droppedCardNum));
}
