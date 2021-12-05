#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include <QGraphicsItem>
#include <QGraphicsScene>

class ClearSceneteCommand : public QUndoCommand
{
public:
    ClearSceneteCommand(QGraphicsScene *scene, QUndoCommand *parent = nullptr);

    void undo();
    void redo();

private:
    QGraphicsScene* scene;
    QList<QGraphicsItem*> items;
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
};

#endif // COMMANDS_H
