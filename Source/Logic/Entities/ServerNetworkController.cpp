#include "ServerNetworkController.h"

#include "../NetworkEntityPairs.h"
#include "../Event.h"
#include "../../Engine/Entity.h"
#include <stdio.h>
#include <cstring>

/*------------------------------------------------------------------------------
-- FUNCTION:        ServerNetworkController::ServerNetworkController
--
-- DATE:            April 1, 2015
--
-- REVISIONS:       (Date and Description)
--
-- DESIGNER:        Eric Tsang
--
-- PROGRAMMER:      Calvin Rempel, Eric Tsang
--
-- INTERFACE:       ServerNetworkController::ServerNetworkController()
--
-- RETURNS:         void
--
-- NOTES:           constructor for the {ServerGameState} class.
------------------------------------------------------------------------------*/
ServerNetworkController::ServerNetworkController()
    :Controller()
    ,NetworkEntity((int)NetworkEntityPair::SERVERCONTROLLER_NETCONTROLLER)
{
}

/*------------------------------------------------------------------------------
-- FUNCTION:        ServerNetworkController::ServerNetworkController
--
-- DATE:            April 1, 2015
--
-- REVISIONS:       (Date and Description)
--
-- DESIGNER:        Eric Tsang
--
-- PROGRAMMER:      Calvin Rempel
--
-- INTERFACE:       ServerNetworkController::ServerNetworkController(int type)
--                  type - the
--
-- RETURNS:         void
--
-- NOTES:           destructor for the {ServerGameState} class.
------------------------------------------------------------------------------*/
ServerNetworkController::ServerNetworkController(int type)
    :NetworkEntity(type)
{
}

/*------------------------------------------------------------------------------
-- FUNCTION:        ServerNetworkController::~ServerNetworkController
--
-- DATE:            April 1, 2015
--
-- REVISIONS:       (Date and Description)
--
-- DESIGNER:        Eric Tsang
--
-- PROGRAMMER:      Calvin Rempel, Eric Tsang
--
-- INTERFACE:       ServerNetworkController::~ServerNetworkController()
--
-- RETURNS:         void
--
-- NOTES:           constructor for the {ServerGameState} class.
------------------------------------------------------------------------------*/
ServerNetworkController::~ServerNetworkController()
{
}

/*------------------------------------------------------------------------------
-- FUNCTION:        void ServerNetworkController::addEvent
--
-- DATE:            April 1, 2015
--
-- REVISIONS:       (Date and Description)
--
-- DESIGNER:        Eric Tsang
--
-- PROGRAMMER:      Calvin Rempel, Eric Tsang
--
-- INTERFACE:       void ServerNetworkController::addEvent(Event *event)
--
-- RETURNS:         void
--
-- NOTES:           adds an event into the event queue, but since this is the
--                  server side, it will send the event to the clients first,
--                  so that they can enqueue it into their own event queue, and
--                  then the server enqueues the event into its event queue.
------------------------------------------------------------------------------*/
void ServerNetworkController::addEvent(Event *event)
{
    sendEventMessage(event);
    Controller::addEvent(event);
}

/*------------------------------------------------------------------------------
-- FUNCTION:        void ServerNetworkController::sendEventMessage
--
-- DATE:            April 1, 2015
--
-- REVISIONS:       (Date and Description)
--
-- DESIGNER:        Eric Tsang
--
-- PROGRAMMER:      Calvin Rempel, Eric Tsang
--                  Sanders Lee (Debugged synchronization problem across clients)
--
-- INTERFACE:       void ServerNetworkController::sendEventMessage(Event *event)
--
-- RETURNS:         void
--
-- NOTES:           converts an event into a message that can be sent over the
--                  network.
------------------------------------------------------------------------------*/
void ServerNetworkController::sendEventMessage(Event *event)
{
    // create network message from event
    switch(event->type)
    {
        case ::Marx::MOVE:
        {
            // cast event to event subclass
            MoveEvent* me = (MoveEvent*) event;

            // parse move event into move message
            MoveMessage mm;
            printf("entity server x, y: %f %f\n", getEntity()->left, getEntity()->top);
            mm.x      = getEntity()->left;
            mm.y      = getEntity()->top;
            mm.xDir   = me->getXDir();
            mm.yDir   = me->getYDir();
            mm.forced = me->forced();

            // message to be sent over the network
            Message message;
            message.data = &mm;
            message.len  = sizeof(MoveMessage);
            message.type = ::Marx::MOVE;

            // send the network event
            update(message);
            break;
    }
    case ::Marx::UPDATE:
    {
          UpdateMessage um;
          um.x      = getEntity()->left;
          um.y      = getEntity()->top;

          Message message;

          message.data = &um;
          message.len  = sizeof(UpdateMessage);
          message.type = ::Marx::UPDATE;

          // send the network event
          update(message);
          break;
    }
    default:
        printf("\r\nWARNING: NetworkController::sendEventMessage received an "
            "unknown event type. please add new case to switch statement\r\n");
        fflush(stdout);
        break;
    }
}

/*------------------------------------------------------------------------------
-- FUNCTION:        void ServerNetworkController::onUpdate
--
-- DATE:            April 1, 2015
--
-- REVISIONS:       (Date and Description)
--
-- DESIGNER:        Eric Tsang
--
-- PROGRAMMER:      Calvin Rempel, Eric Tsang
--		            Sanders Lee (Debugged synchronization problem across clients)
--
--
-- INTERFACE:       void ServerNetworkController::onUpdate(Message msg)
--                  msg - message received from the update
--
-- RETURNS:         void
--
-- NOTES:           parses the message received in the update, creates the Event
--                  equivalent, and enqueues it into its event queue.
------------------------------------------------------------------------------*/
void ServerNetworkController::onUpdate(Message msg)
{
    float x = getEntity()->left;
    float y = getEntity()->top;

    switch((PlayerCommandMsgType)msg.type)
    {
        case PlayerCommandMsgType::START_MV_LEFT_COMMAND:
        {
            MoveEvent *event = new MoveEvent(x,y,-1,0,0);
            addEvent(event);
            break;
        }
        case PlayerCommandMsgType::START_MV_RIGHT_COMMAND:
        {
            MoveEvent *event = new MoveEvent(x,y,1,0,0);
            addEvent(event);
            break;
        }
        case PlayerCommandMsgType::START_MV_UP_COMMAND:
        {
            MoveEvent *event = new MoveEvent(x,y,0,-1,0);
            addEvent(event);
            break;
        }
        case PlayerCommandMsgType::START_MV_DOWN_COMMAND:
        {
            MoveEvent *event = new MoveEvent(x,y,0,1,0);
            addEvent(event);
            break;
        }
        case PlayerCommandMsgType::STOP_MV_LEFT_COMMAND:
        {
            MoveEvent *event = new MoveEvent(x,y,1,0,0);
            addEvent(event);
            break;
        }
        case PlayerCommandMsgType::STOP_MV_RIGHT_COMMAND:
        {
            MoveEvent *event = new MoveEvent(x,y,-1,0,0);
            addEvent(event);
            break;
        }
        case PlayerCommandMsgType::STOP_MV_UP_COMMAND:
        {
            MoveEvent *event = new MoveEvent(x,y,0,1,0);
            addEvent(event);
            break;
        }
        case PlayerCommandMsgType::STOP_MV_DOWN_COMMAND:
        {
            MoveEvent *event = new MoveEvent(x,y,0,-1,0);
            addEvent(event);
            break;
        }
    }
}
