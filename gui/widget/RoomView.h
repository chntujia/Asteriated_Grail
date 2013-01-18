#ifndef ROOMVIEW_H
#define ROOMVIEW_H

#include <QGraphicsView>
#include "RoomScene.h"
#include <QPixmap>

class RoomView : public QGraphicsView
{
    Q_OBJECT

public:
    RoomView();
private:
    RoomScene *scene;
};

#endif // ROOMVIEW_H
