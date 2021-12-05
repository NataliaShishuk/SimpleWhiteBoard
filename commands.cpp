#include "commands.h"

ClearSceneteCommand::ClearSceneteCommand(QGraphicsScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    this->scene = scene;

    for (QGraphicsItem* item : scene->items())
    {
        items.append(item);
    }
}

void ClearSceneteCommand::undo()
{
    for (QGraphicsItem* item : items)
    {
        this->scene->addItem(item);
    }
}

void ClearSceneteCommand::redo()
{
    for (QGraphicsItem* item : items)
    {
        this->scene->removeItem(item);
    }
}

SceneteItemCommand::SceneteItemCommand(QGraphicsScene *scene, QGraphicsItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    this->scene = scene;
    this->item = item;
}

void SceneteItemCommand::undo()
{
    this->scene->removeItem(item);
}

void SceneteItemCommand::redo()
{
    this->scene->addItem(item);
}
