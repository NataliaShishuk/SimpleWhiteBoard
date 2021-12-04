#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include <QGraphicsItem>
#include <QGraphicsScene>

enum CommandType
{
    Undo,
    Redo
};

class SceneteItemCommand : public QUndoCommand
{
public:
    SceneteItemCommand(QGraphicsScene *scene, QGraphicsItem *item, QUndoCommand *parent = nullptr);

    void undo();
    void redo();

private:
    QGraphicsScene* scene;
    QGraphicsItem* item;
    void resetItem(const CommandType commandType);
};

#endif // COMMANDS_H
