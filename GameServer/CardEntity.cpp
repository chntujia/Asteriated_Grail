#include "CardEntity.h"
CardEntity::CardEntity(QStringList* cardEntry):QObject(NULL)
{
    this->id=cardEntry->at(0).toInt();
    this->type=cardEntry->at(1);
    this->element=cardEntry->at(2);
    this->property=cardEntry->at(3);
    this->name=cardEntry->at(4);
    this->source=cardEntry->at(5);
    this->hasSpeceiality=cardEntry->at(6).toInt();
    this->srcUser = -1;
    this->magicName = this->checkBasicMagic(this->name);
    int i;
    this->specialityList.clear();
    for(i=0;i<this->hasSpeceiality;i++)
        this->specialityList.append(cardEntry->at(i+7));
    this->owner = 0;
    this->place = PILE;

}

int CardEntity::checkBasicMagic(QString cardName)
{
    if(cardName == tr("圣盾"))
        return SHIELDCARD;
    else if(cardName == tr("中毒"))
        return POISONCARD;
    else if(cardName == tr("魔弹"))
        return MISSILECARD;
    else if(cardName == tr("虚弱"))
        return WEAKCARD;
    /*
    for(int i = 0;i < shieldCardID.size();i++)
    {
        if(shieldCardID.at(i) == cardID)
            return SHIELDCARD;
    }
    for(int i = 0;i < poisonCardID.size();i++)
    {
        if(poisonCardID.at(i) == cardID)
            return POISONCARD;
    }
    for(int i = 0;i < missileCardID.size();i++)
    {
        if(missileCardID.at(i) == cardID)
            return MISSILECARD;
    }
    for(int i = 0;i < weakCardID.size();i++)
    {
        if(weakCardID.at(i) == cardID)
            return WEAKCARD;
    }
    */
    return -1;
}

int CardEntity::getID()
{
    return this->id;
}
int CardEntity::getHasSpeciality()
{
    return this->hasSpeceiality;
}

QString CardEntity::getType()
{
    return this->type;
}

QString CardEntity::getElement()
{
    return this->element;
}

QString CardEntity::getProperty()
{
    return this->property;
}

QString CardEntity::getName()
{
    return this->name;
}

QString CardEntity::getSource()
{
    return this->source;
}

QStringList CardEntity::getSpecialityList()
{
    return this->specialityList;
}
