#include "TeamArea.h"
#include "data/DataInterface.h"
#include <QPainter>

TeamArea::TeamArea()
{
    red=dataInterface->getRedTeam();
    blue=dataInterface->getBlueTeam();
    background=QPixmap("resource/teamArea.png");

    Rgrail=QPixmap("resource/Rgrail.png");
    Bgrail=QPixmap("resource/Bgrail.png");
    Pcrystal=QPixmap("resource/crystal.png");
    Pgem=QPixmap("resource/gem.png");

    Rdice[0]=QPixmap("resource/1r.png");
    Rdice[1]=QPixmap("resource/2r.png");
    Rdice[2]=QPixmap("resource/3r.png");
    Rdice[3]=QPixmap("resource/4r.png");
    Rdice[4]=QPixmap("resource/5r.png");
    Rdice[5]=QPixmap("resource/6r.png");

    Bdice[0]=QPixmap("resource/1b.png");
    Bdice[1]=QPixmap("resource/2b.png");
    Bdice[2]=QPixmap("resource/3b.png");
    Bdice[3]=QPixmap("resource/4b.png");
    Bdice[4]=QPixmap("resource/5b.png");
    Bdice[5]=QPixmap("resource/6b.png");

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

    offset=145;
    for(i=0;i<red->getGrail();i++) //杯
        painter->drawPixmap(Rgrail.width()*i+offset,5,Rgrail);

    offset+=Rgrail.width()*5+5;
    for(i=0;i<red->getCrystal();i++) //水晶
        painter->drawPixmap(Pcrystal.width()*i+offset,3,Pcrystal);

    offset+=Pcrystal.width()*i;
    for(i=0;i<red->getGem();i++) //宝石
        painter->drawPixmap(Pgem.width()*i+offset,2,Pgem);

    offset=455;
    for(i=0;i<blue->getCrystal();i++) //水晶
        painter->drawPixmap(Pcrystal.width()*i+offset,3,Pcrystal);

    offset+=(Pcrystal.width()+4)*i;
    for(i=0;i<blue->getGem();i++) //宝石
        painter->drawPixmap(Pgem.width()*i+offset,2,Pgem);

    offset=598;
    for(i=0;i<blue->getGrail();i++) //杯
        painter->drawPixmap(Rgrail.width()*i+offset,5,Bgrail);

    offset=788;
    morale=blue->getMorale();
    for(i=0;i<morale/avg;i++) //士气
        painter->drawPixmap(i*(Bdice[1].width()+2)+offset,17,Bdice[avg-1]);
    offset+=i*(Bdice[1].width()+2);
    if(morale%avg!=0)
        painter->drawPixmap(offset,17,Bdice[morale%avg-1]);
}
