// Picking up and using items
//
// Good to see you again! Today, I'll show you how to have your player pick up and use items which are
// on the ground. Sounds fun? Let's get started!


#include <QApplication>

#include "qge/MapGrid.h"
#include "qge/Map.h"
#include "qge/Game.h"
#include "qge/Entity.h"
#include "qge/Utilities.h"
#include "qge/EntitySprite.h"

#include "qge/ECKeyboardMoverPerspective.h"
#include "qge/ECMouseFacer.h"
#include "qge/PathingMap.h"
#include "qge/ECCameraFollower.h"
#include "qge/ECMapMover.h"
#include "qge/ECCurrentMapGrabber.h"

#include "qge/SpriteSheet.h"
#include "qge/AngledSprite.h"
#include "qge/ECBodyThruster.h"

#include "qge/MeleeWeaponSlot.h"
#include "qge/AnimationAttack.h"

#include "qge/FogWeather.h"
#include "qge/RainWeather.h"
#include "qge/SnowWeather.h"

#include "qge/ItemTeleport.h"
#include "qge/ItemShardsOfFire.h"
#include "qge/ItemRainOfSpears.h"
#include "qge/InventoryUser.h"
#include "qge/ECItemPickerUpper.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // create map
    qge::Map* map = new qge::Map();

    // create map grid
    qge::MapGrid* mapGrid = new qge::MapGrid(1,2);
    mapGrid->setMapAtPos(map,0,1); // add map to map grid

    // create game
    qge::Game* game = new qge::Game(mapGrid,0,1);

    // create minitaur looking entity
    qge::Entity* entity = qge::getMinitaurEntity();

    // add the entity into the map at position (300,300)
    entity->setPos(QPointF(300,300));
    map->addEntity(entity);

    // play the stand animation of the entity
    entity->sprite()->play("stand" ,-1 ,10 ,0);

    // make entity move relative to its own facing angle in reponse to the arrow keys
    qge::ECKeyboardMoverPerspective* keyboardMoverController = new qge::ECKeyboardMoverPerspective(entity);

    // make entity always face the mouse
    qge::ECMouseFacer* mouseFacerController = new qge::ECMouseFacer(entity);

    // Let's add another map
    qge::Map* map2 = new qge::Map(50,50,32);
    mapGrid->setMapAtPos(map2,0,0);

    // let's make the camera follow the entity
    qge::ECCameraFollower* camFollowerCont = new qge::ECCameraFollower(entity);

    // let's make the the entity move to the next map when close enough to the border of the current map
    qge::ECMapMover* mapMoverCont = new qge::ECMapMover(entity);

    // let's set game's current map as the map that the entity is in
    qge::ECCurrentMapGrabber* mapGrabber = new qge::ECCurrentMapGrabber(entity);

    // let's create a new entity that will look like a spider
    // NOTE: we encapsulate all that code in a function called qge::getSpiderEntity()
    qge::Entity* spiderEntity = qge::getSpiderEntity();

    spiderEntity->setPos(QPointF(600,600));
    map->addEntity(spiderEntity);

    // let's make the entity enemies with the minitaur
    entity->setGroup(0);
    spiderEntity->setGroup(1);
    spiderEntity->addEnemyGroup(0);

    // let's make the entity body thrust (or body slam - w.e. you wanna call it) enemies that come to inside it's field of view (i.e. in front of it)
    qge::ECBodyThruster* bodyThrusterC = new qge::ECBodyThruster(spiderEntity);

    // let's show the field of view of the body thruster
    bodyThrusterC->setShowFOV(true);

    // let's create a call back function for when the mouse is clicked.
    auto mousePressedCallback = [entity](QPoint pos, Qt::MouseButton button){
        // if left click
        if (button == Qt::MouseButton::LeftButton){
            entity->slot("weapon")->use(); // use whatever item is equipped in the slot named "weapon"
        }
    };
    
    // let's give the player a weapon slot
    entity->addSlot(new qge::MeleeWeaponSlot(),"weapon");

    // let's create a weapon to equip in the slot
    // AnimationAttack is a MeleeWeapon that will play an animation and then damage enemies in an arc in front.
    qge::AnimationAttack* animationWeapon = new qge::AnimationAttack("attack","qrc:/resources/sounds/axe.wav",10,60,90);

    // equip the weapon in the slot
    entity->slot("weapon")->equip(animationWeapon);

    // when the mouse is pressed, make the weapon attack.
    QObject::connect(game,&qge::Game::mousePressed,mousePressedCallback);

    qge::FogWeather* fogWeather = new qge::FogWeather();
    qge::RainWeather* rainWeather = new qge::RainWeather();
    map->addWeatherEffect(*fogWeather);
    map->addWeatherEffect(*rainWeather);

    // == NEW STUFF ==

    // let's place some items on the ground
    qge::ItemRainOfSpears* itemROS = new qge::ItemRainOfSpears();
    qge::ItemShardsOfFire* itemSOF = new qge::ItemShardsOfFire();
    qge::ItemTeleport* itemTeleport = new qge::ItemTeleport();

    map->addEntity(itemROS);
    map->addEntity(itemSOF);
    map->addEntity(itemTeleport);

    itemROS->setPos(QPoint(100,100));
    itemSOF->setPos(QPoint(200,100));
    itemTeleport->setPos(QPoint(300,100));

    // have the player pick up items as he collides with them
    qge::ECItemPickerUpper* itemPicker = new qge::ECItemPickerUpper(entity);

    // create a GUI that allows using items in an inventory
    qge::InventoryUser* inventoryUser = new qge::InventoryUser(game,entity->inventory());
    game->addGui(inventoryUser);

    game->launch(); // launch game
    return a.exec();
}
