#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include <QGraphicsItem>
#include <QGraphicsScene>

class ClearSceneCommand : public QUndoCommand
{
public:
    ClearSceneCommand(QGraphicsScene *scene, QUndoCommand *parent = nullptr);

    void undo();
    void redo();

private:
    QGraphicsScene* scene;
    QList<QGraphicsItem*> items;
};

class AddSceneItemCommand : public QUndoCommand
{
public:
    AddSceneItemCommand(QGraphicsScene *scene, QGraphicsItem *item, QUndoCommand *parent = nullptr);

    void undo();
    void redo();

private:
    QGraphicsScene* scene;
    QGraphicsItem* item;
};

class RemoveSceneItemCommand : public QUndoCommand
{
public:
    RemoveSceneItemCommand(QGraphicsScene *scene, QGraphicsItem *item, QUndoCommand *parent = nullptr);

    void undo();
    void redo();

private:
    QGraphicsScene* scene;
    QGraphicsItem* item;
};

#endif // COMMANDS_H
