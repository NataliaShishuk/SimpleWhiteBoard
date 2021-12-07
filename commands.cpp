#include "commands.h"

ClearSceneCommand::ClearSceneCommand(QGraphicsScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    this->scene = scene;
    this->items.append(scene->items());
}

void ClearSceneCommand::undo()
{
    for (QGraphicsItem* item : items)
    {
        this->scene->addItem(item);
    }
}

void ClearSceneCommand::redo()
{
    for (QGraphicsItem* item : items)
    {
        this->scene->removeItem(item);
    }
}

AddSceneItemCommand::AddSceneItemCommand(QGraphicsScene *scene, QGraphicsItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    this->scene = scene;
    this->item = item;
}

void AddSceneItemCommand::undo()
{
    this->scene->removeItem(item);
}

void AddSceneItemCommand::redo()
{
    this->scene->addItem(item);
}

RemoveSceneItemCommand::RemoveSceneItemCommand(QGraphicsScene *scene, QGraphicsItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    this->scene = scene;
    this->item = item;
}

void RemoveSceneItemCommand::undo()
{
    this->scene->addItem(item);
}

void RemoveSceneItemCommand::redo()
{
    this->scene->removeItem(item);
}

PasteSceneCommand::PasteSceneCommand(QGraphicsScene *scene, QList<QGraphicsItem*> items, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    this->scene = scene;
    this->items.append(items);
}

void PasteSceneCommand::undo()
{
    for (QGraphicsItem* item : this->items)
    {
        this->scene->removeItem(item);
    }
}

void PasteSceneCommand::redo()
{
    for (QGraphicsItem* item : this->items)
    {
        this->scene->addItem(item);
    }
}
