#include "commands.h"

ClearSceneCommand::ClearSceneCommand(QGraphicsScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    this->scene = scene;

    for (QGraphicsItem* item : scene->items())
    {
        items.append(item);
    }
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
