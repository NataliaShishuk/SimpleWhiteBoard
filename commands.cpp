#include "commands.h"

SceneteItemCommand::SceneteItemCommand(QGraphicsScene *scene, QGraphicsItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    this->scene = scene;
    this->item = item;
}

void SceneteItemCommand::undo()
{
    resetItem(CommandType::Undo);
}

void SceneteItemCommand::redo()
{
    resetItem(CommandType::Redo);
}

void SceneteItemCommand::resetItem(const CommandType commandType)
{
    switch (commandType)
    {

    case Undo:
        this->scene->removeItem(item);
        break;

    case Redo:
        this->scene->addItem(item);
        break;

    }
}
