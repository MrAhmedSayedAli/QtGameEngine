#ifndef ENTITY_H
#define ENTITY_H

#include "PathingMap.h"
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <typeinfo> // run time type identification (RTTI)
#include <typeindex>// same ^
#include <set>
#include <QPointer>

class Map;
class Inventory;
class EquipableItem;
class Item;
class Sprite;
class QPointF;
class Slot;
class Inventory;
class AsyncShortestPathFinder;
class PlayerControlledMoveBehavior;
class QTimer;

/// An Entity is the base class for anything that can go inside a Map.
/// @author Abdullah Aghazadah
/// @date 5-20-15
///
/// When an Entity is first added to a Map, the map will use the Entitie's
/// PathingMap and position to update its own PathingMap. When an Entity is
/// moved, it will notify the Map so that it can once again update the
/// PathingMap.
///
/// Entities support parent/child relationships. When a parent Entity moves or
/// rotates, so do all of its children. When a parent Entity is deleted, so
/// are all of its children. When a parent Entity is removed from a Map,
/// so are all of its children.
///
/// Entities can be controlled by entity controllers (TODO: link to doc).
class Entity: public QObject
{
    Q_OBJECT
    friend class Map; // Map needs to be able to set the map_
                      // pointer of the Entity (we cannot use a public setter
                      // because we don't want ANYONE else to be able to set this)
public:
    // constructor
    Entity();

    // destructor
    virtual ~Entity();

    // pathing map
    PathingMap pathingMap() const;  // TODO do not return by value, too expensive to copy
    void setPathingMap(const PathingMap& pathingMap, const QPointF &pos=QPointF(0,0));

    // map
    Map* map() const;
    void setMap(Map* toMap);
    QPointF mapToMap(const QPointF& point) const;

    // pos/height/facing angle
    QPointF pointPos() const;
    QPointF pathingMapPos() const;
    double pointX() const;
    double pointY() const;
    double pointZ() const;
    double height() const;
    void setPointPos(const QPointF &pos);
    void setPointPos(std::string namedPos, const QPointF& pos);
    void setPointPos(const QPointF& moveThisPt, const QPointF& toThisPoint);
    void setPointX(double x);
    void setPointY(double y);
    void setPointZ(double z);
    void setHeight(double height);
    Node cellPos();
    void setCellPos(const Node& cell);
    int facingAngle();
    void setFacingAngle(double angle);

    // sprite
    void setSprite(Sprite* sprite);
    Sprite* sprite() const;
    void setRotationPoint(QPointF point);

    // parent/child relationship
    std::unordered_set<Entity*> children();
    void setParentEntity(Entity* parent);
    Entity* parent();

    void addNamedPoint(const QPointF& point, std::string name);
    QPointF namedPoint(std::string name);

    void setSpeed(double speed);
    double speed();
    void setRotationSpeed(double rotationSpeed);
    double rotationSpeed();

    void setHealth(double health);
    double health();
    void damage(Entity* entity, double amount);

    void setCanOnlyBeDamagedBy(bool tf);
    void addCanBeDamagedBy(std::type_index typeOfEntity);
    void addCannotBeDamagedBy(std::type_index typeOfEntity);
    bool canBeDamagedBy(Entity* entity);
    bool isInvulnerable();
    void setInvulnerable(bool tf);

    // group/enemy groups
    void setGroup(int groupNumber_);
    int group();
    void addEnemyGroup(int groupNumber);
    std::unordered_set<int> enemyGroups();
    bool isAnEnemyGroup(int groupNumber);

    // slot/equipment/inventory
    void addSlot(Slot* slot);
    Slot* slot(std::string name);
    std::unordered_set<Slot*> getSlots();
    bool equipItem(EquipableItem* item, std::string slot);
    bool equipItem(EquipableItem* item, Slot* slot);
    void setInventory(Inventory* inv);
    Inventory* inventory();

    // misc
    bool canFit(const QPointF& atPos);
    QRectF boundingRect();

signals:
    /// Emitted whenever the Entity moves from a certain position to another
    /// position.
    void moved(Entity* entity, QPointF fromPos, QPointF toPos);

    /// Emitted whenever the Entity collides with another Entity.
    void collided(Entity* sender, Entity* collidedWith);

private:
    // main attributes
    PathingMap pathingMap_;
    QPointF pathingMapPos_;
    QPointF currentPos_;
    QPointF lastPos_;
    Map* map_;
    Sprite* sprite_;
    std::unordered_set<Entity*> children_;
    Entity* parent_;
    std::map<std::string,QPointF> namedPoints_;
    double health_;
    std::set<std::type_index> canOnlyBeDamagedBy_;
    std::set<std::type_index> canBeDamagedByAllExcept_;
    bool canOnlyBeDamagedByMode_;
    int groupNumber_;
    std::unordered_set<int> enemyGroups_;
    bool invulnerable_;
    double zPos_;
    double height_;
    Inventory* inventory_;
    std::unordered_map<std::string,Slot*> stringToSlot_;
    double speed_; // pixels per second
    double rotationSpeed_; // degrees per second

    // helper functions
    void scaleSprite_();
};

// make QPointer<Entity> hashable (simply "delegate" to hasing a regular Entity*)
namespace std {
template <> struct hash<QPointer<Entity>>
{
    size_t operator()(const QPointer<Entity>& node) const{
        return hash<Entity*>()(node.data());
    }
};
}

#endif // ENTITY_H
