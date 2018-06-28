/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "startrek/filestream.h"
#include "startrek/iwfile.h"
#include "startrek/room.h"
#include "startrek/startrek.h"

#include "rooms/function_map.h"

namespace StarTrek {

Room::Room(StarTrekEngine *vm, const Common::String &name) : _vm(vm) {
	SharedPtr<FileStream> rdfFile = _vm->loadFile(name + ".RDF");

	int size = rdfFile->size();
	_rdfData = new byte[size];
	rdfFile->read(_rdfData, size);

	// Find room-specific code table
	if (name == "DEMON0") {
		_roomActionList = demon0ActionList;
		_numRoomActions = sizeof(demon0ActionList) / sizeof(RoomAction);
	}
	else if (name == "DEMON1") {
		_roomActionList = demon1ActionList;
		_numRoomActions = sizeof(demon1ActionList) / sizeof(RoomAction);
	}
	else if (name == "DEMON2") {
		_roomActionList = demon2ActionList;
		_numRoomActions = sizeof(demon2ActionList) / sizeof(RoomAction);
	}
	else if (name == "DEMON3") {
		_roomActionList = demon3ActionList;
		_numRoomActions = sizeof(demon3ActionList) / sizeof(RoomAction);
	}
	else if (name == "DEMON4") {
		_roomActionList = demon4ActionList;
		_numRoomActions = sizeof(demon4ActionList) / sizeof(RoomAction);
	}
	else if (name == "DEMON5") {
		_roomActionList = demon5ActionList;
		_numRoomActions = sizeof(demon5ActionList) / sizeof(RoomAction);
	}
	else if (name == "DEMON6") {
		_roomActionList = demon6ActionList;
		_numRoomActions = sizeof(demon6ActionList) / sizeof(RoomAction);
	}
	else if (name == "TUG0") {
		_roomActionList = tug0ActionList;
		_numRoomActions = sizeof(tug0ActionList) / sizeof(RoomAction);
	}
	else if (name == "TUG1") {
		_roomActionList = tug1ActionList;
		_numRoomActions = sizeof(tug1ActionList) / sizeof(RoomAction);
	}
	else if (name == "TUG2") {
		_roomActionList = tug2ActionList;
		_numRoomActions = sizeof(tug2ActionList) / sizeof(RoomAction);
	}
	else if (name == "TUG3") {
		_roomActionList = tug3ActionList;
		_numRoomActions = sizeof(tug3ActionList) / sizeof(RoomAction);
	}
	else if (name == "LOVE0") {
		_roomActionList = love0ActionList;
		_numRoomActions = sizeof(love0ActionList) / sizeof(RoomAction);
	}
	else if (name == "LOVE1") {
		_roomActionList = love1ActionList;
		_numRoomActions = sizeof(love1ActionList) / sizeof(RoomAction);
	}
	else if (name == "LOVE2") {
		_roomActionList = love2ActionList;
		_numRoomActions = sizeof(love2ActionList) / sizeof(RoomAction);
	}
	else if (name == "LOVE3") {
		_roomActionList = love3ActionList;
		_numRoomActions = sizeof(love3ActionList) / sizeof(RoomAction);
	}
	else if (name == "LOVE4") {
		_roomActionList = love4ActionList;
		_numRoomActions = sizeof(love4ActionList) / sizeof(RoomAction);
	}
	else if (name == "LOVE5") {
		_roomActionList = love5ActionList;
		_numRoomActions = sizeof(love5ActionList) / sizeof(RoomAction);
	}
	else if (name == "MUDD0") {
		_roomActionList = mudd0ActionList;
		_numRoomActions = sizeof(mudd0ActionList) / sizeof(RoomAction);
	}
	else if (name == "MUDD1") {
		_roomActionList = mudd1ActionList;
		_numRoomActions = sizeof(mudd1ActionList) / sizeof(RoomAction);
	}
	else if (name == "MUDD2") {
		_roomActionList = mudd2ActionList;
		_numRoomActions = sizeof(mudd2ActionList) / sizeof(RoomAction);
	}
	else if (name == "MUDD3") {
		_roomActionList = mudd3ActionList;
		_numRoomActions = sizeof(mudd3ActionList) / sizeof(RoomAction);
	}
	else if (name == "MUDD4") {
		_roomActionList = mudd4ActionList;
		_numRoomActions = sizeof(mudd4ActionList) / sizeof(RoomAction);
	}
	else if (name == "MUDD5") {
		_roomActionList = mudd5ActionList;
		_numRoomActions = sizeof(mudd5ActionList) / sizeof(RoomAction);
	}
	else {
		warning("Room \"%s\" unimplemented", name.c_str());
		_numRoomActions = 0;
	}

	memset(&_roomVar, 0, sizeof(_roomVar));
}

Room::~Room() {
	delete[] _rdfData;
}

uint16 Room::readRdfWord(int offset) {
	return _rdfData[offset] | (_rdfData[offset+1]<<8);
}

bool Room::actionHasCode(const Action &action) {
	RoomAction *roomActionPtr = _roomActionList;
	int n = _numRoomActions;

	while (n-- > 0) {
		if (action == roomActionPtr->action)
			return true;
		roomActionPtr++;
	}
	return false;
}

bool Room::handleAction(const Action &action) {
	RoomAction *roomActionPtr = _roomActionList;
	int n = _numRoomActions;

	while (n-- > 0) {
		if (action == roomActionPtr->action) {
			_vm->_awayMission.rdfStillDoDefaultAction = false;
			(this->*(roomActionPtr->funcPtr))();
			if (!_vm->_awayMission.rdfStillDoDefaultAction)
				return true;
		}
		roomActionPtr++;
	}
	return false;
}

bool Room::handleActionWithBitmask(const Action &action) {
	RoomAction *roomActionPtr = _roomActionList;
	int n = _numRoomActions;

	while (n-- > 0) {
		uint32 bitmask = roomActionPtr->action.getBitmask();
		if ((action.toUint32() & bitmask) == (roomActionPtr->action.toUint32() & bitmask)) {
			_vm->_awayMission.rdfStillDoDefaultAction = false;
			(this->*(roomActionPtr->funcPtr))();
			if (!_vm->_awayMission.rdfStillDoDefaultAction)
				return true;
		}
		roomActionPtr++;
	}
	return false;
}

Common::Point Room::getBeamInPosition(int crewmanIndex) {
	int base = 0xaa + crewmanIndex * 4;
	return Common::Point(readRdfWord(base), readRdfWord(base + 2));
}


// For actions of type ACTION_FINISHED_ANIMATION or ACTION_FINISHED_WALKING, this takes
// a function pointer and returns the index corresponding to that callback.
// Creates a fatal error on failure.
int Room::findFunctionPointer(int action, void (Room::*funcPtr)()) {
	assert(action == ACTION_FINISHED_ANIMATION || action == ACTION_FINISHED_WALKING);

	for (int i = 0; i < _numRoomActions; i++) {
		if (_roomActionList[i].action.type == action && _roomActionList[i].funcPtr == funcPtr)
			return _roomActionList[i].action.b1;
	}

	if (action == ACTION_FINISHED_ANIMATION)
		error("Couldn't find FINISHED_ANIMATION function pointer");
	else
		error("Couldn't find FINISHED_WALKING function pointer");
}

// Interface for room-specific code

void Room::loadActorAnim(int actorIndex, Common::String anim, int16 x, int16 y, uint16 finishedAnimActionParam) {
	Actor *actor = &_vm->_actorList[actorIndex];

	if (x == -1 || y == -1) {
		x = actor->sprite.pos.x;
		y = actor->sprite.pos.y;
	}

	if (actorIndex >= 0 && actorIndex < SCALED_ACTORS_END)
		_vm->loadActorAnimWithRoomScaling(actorIndex, anim, x, y);
	else
		_vm->loadActorAnim(actorIndex, anim, x, y, 256);

	if (finishedAnimActionParam != 0) {
		actor->triggerActionWhenAnimFinished = true;
		actor->finishedAnimActionParam = finishedAnimActionParam;
	}
}

// Same as above, but accepts a callback for when the animation finished (instead of an
// integer for an action)
void Room::loadActorAnimC(int actorIndex, Common::String anim, int16 x, int16 y, void (Room::*funcPtr)()) {
	Actor *actor = &_vm->_actorList[actorIndex];

	if (x == -1 || y == -1) {
		x = actor->sprite.pos.x;
		y = actor->sprite.pos.y;
	}

	if (actorIndex >= 0 && actorIndex < SCALED_ACTORS_END)
		_vm->loadActorAnimWithRoomScaling(actorIndex, anim, x, y);
	else
		_vm->loadActorAnim(actorIndex, anim, x, y, 256);

	if (funcPtr != nullptr) {
		actor->triggerActionWhenAnimFinished = true;
		actor->finishedAnimActionParam = findFunctionPointer(ACTION_FINISHED_ANIMATION, funcPtr);
	}
}

void Room::loadActorStandAnim(int actorIndex) {
	if (_vm->_awayMission.redshirtDead && actorIndex == OBJECT_REDSHIRT)
		_vm->removeActorFromScreen(actorIndex);
	else {
		Actor *actor = &_vm->_actorList[actorIndex];
		if (actor->animationString[0] == '\0')
			_vm->removeActorFromScreen(actorIndex);
		else
			_vm->initStandAnim(actorIndex);
	}
}

/**
 * This is exactly the same as "loadActorAnim", but the game calls it at different times?
 */
void Room::loadActorAnim2(int actorIndex, Common::String anim, int16 x, int16 y, uint16 finishedAnimActionParam) {
	loadActorAnim(actorIndex, anim, x, y, finishedAnimActionParam);
}

int Room::showRoomSpecificText(const char **array) {
	Common::String speaker;
	byte textColor;

	if (array[0] != nullptr && array[0][0] != '\0') {
		speaker = Common::String(array[0]);
		if (speaker.equalsIgnoreCase("Capt. Kirk"))
			textColor = TEXTCOLOR_YELLOW;
		else if (speaker.equalsIgnoreCase("Mr. Spock"))
			textColor = TEXTCOLOR_BLUE;
		else if (speaker.equalsIgnoreCase("Dr. McCoy"))
			textColor = TEXTCOLOR_BLUE;
		else if (speaker.equalsIgnoreCase("Mr. Chekov"))
			textColor = TEXTCOLOR_YELLOW;
		else if (speaker.equalsIgnoreCase("Mr. Scott"))
			textColor = TEXTCOLOR_RED;
		else if (speaker.hasPrefixIgnoreCase("Lt"))
			textColor = TEXTCOLOR_RED;
		else if (speaker.hasPrefixIgnoreCase("Ensign"))
			textColor = TEXTCOLOR_RED;
		else
			textColor = TEXTCOLOR_GREY;
	}
	else
		textColor = TEXTCOLOR_YELLOW;

	return _vm->showText(&StarTrekEngine::readTextFromArrayWithChoices, (uintptr)array, 20, 20, textColor, true, false, false);
}

int Room::showText(const TextRef *textIDs) {
	int numIDs = 0;
	while (textIDs[numIDs] != TX_BLANK)
		numIDs++;

	const char **text = (const char **)malloc(sizeof(const char *) * (numIDs + 1));
	for (int i = 0; i <= numIDs; i++)
		text[i] = g_gameStrings[textIDs[i]];
	int retval = showRoomSpecificText(text);
	free(text);

	return retval;
}

int Room::showText(TextRef speaker, TextRef text) {
	int textIDs[3];
	textIDs[0] = speaker;
	textIDs[1] = text;
	textIDs[2] = TX_BLANK;
	return showText(textIDs);
}

int Room::showText(TextRef text) {
	return showText(TX_NULL, text);
}

void Room::giveItem(int item) {
	assert(item >= ITEMS_START && item < ITEMS_END);
	_vm->_itemList[item - ITEMS_START].have = true;
}

void Room::loadRoomIndex(int roomIndex, int spawnIndex) {
	if (_vm->_awayMission.crewDownBitset != 0)
		return;

	_vm->_missionToLoad = _vm->_missionName;
	_vm->_roomIndexToLoad = roomIndex;
	_vm->_spawnIndexToLoad = spawnIndex;

	// WORKAROUND: original game manipulates the stack to return directly to the start of
	// "runAwayMission". Instead, we set some variables and the room will be changed
	// later. (We wouldn't want to delete the room we're currently in...)
}

void Room::loseItem(int item) {
	assert(item >= ITEMS_START && item < ITEMS_END);
	_vm->_itemList[item - ITEMS_START].have = false;

	if (_vm->_awayMission.activeAction == ACTION_USE && _vm->_awayMission.activeObject == item) {
		_vm->_awayMission.activeAction = ACTION_WALK;
		_vm->chooseMouseBitmapForAction(ACTION_WALK, false);
		_vm->hideInventoryIcons();
	}
}

void Room::walkCrewman(int actorIndex, int16 destX, int16 destY, uint16 finishedAnimActionParam) {
	if (!(actorIndex >= OBJECT_KIRK && actorIndex <= OBJECT_REDSHIRT))
		error("Tried to walk a non PC");

	Actor *actor = &_vm->_actorList[actorIndex];
	Common::String anim = _vm->getCrewmanAnimFilename(actorIndex, "walk");
	bool success = _vm->actorWalkToPosition(actorIndex, anim, actor->pos.x, actor->pos.y, destX, destY);

	if (success && finishedAnimActionParam != 0) {
		actor->triggerActionWhenAnimFinished = true;
		actor->finishedAnimActionParam = finishedAnimActionParam;
	}
}

// Same as above, but with a function callback instead of an integer value to generate an
// action
void Room::walkCrewmanC(int actorIndex, int16 destX, int16 destY, void (Room::*funcPtr)()) {
	if (!(actorIndex >= OBJECT_KIRK && actorIndex <= OBJECT_REDSHIRT))
		error("Tried to walk a non PC");

	Actor *actor = &_vm->_actorList[actorIndex];
	Common::String anim = _vm->getCrewmanAnimFilename(actorIndex, "walk");
	bool success = _vm->actorWalkToPosition(actorIndex, anim, actor->pos.x, actor->pos.y, destX, destY);

	if (success && funcPtr != nullptr) {
		actor->triggerActionWhenAnimFinished = true;
		actor->finishedAnimActionParam = 0;
		actor->finishedAnimActionParam = findFunctionPointer(ACTION_FINISHED_WALKING, funcPtr);
	}
}

/**
 * Loads a pair of .map and .iw files to change the room's collisions and pathfinding.
 */
void Room::loadMapFile(const Common::String &name) {
	_vm->_mapFilename = name;
	_vm->_iwFile.reset();
	_vm->_mapFile.reset();
	_vm->_iwFile = SharedPtr<IWFile>(new IWFile(_vm, name + ".iw"));
	_vm->_mapFile = _vm->loadFile(name + ".map");
}

void Room::showBitmapFor5Ticks(const Common::String &bmpName, int priority) {
	if (priority < 0 || priority > 15)
		priority = 5;

	Sprite sprite;
	_vm->_gfx->addSprite(&sprite);
	sprite.setXYAndPriority(0, 0, priority);
	sprite.setBitmap(_vm->_gfx->loadBitmap(bmpName));

	_vm->_gfx->drawAllSprites();

	TrekEvent event;
	int ticks = 0;

	while (ticks < 5) {
		while (!_vm->popNextEvent(&event));

		if (event.type == TREKEVENT_TICK)
			ticks++;
	}

	sprite.dontDrawNextFrame();
	_vm->_gfx->drawAllSprites();
	_vm->_gfx->delSprite(&sprite);
}

bool Room::haveItem(int item) {
	return _vm->_itemList[item - 0x40].have;
}

Common::Point Room::getActorPos(int actorIndex) {
	return _vm->_actorList[actorIndex].pos;
}

/**
 * Returns a word in range [start, end] (that's inclusive).
 */
int16 Room::getRandomWordInRange(int start, int end) {
	return _vm->getRandomWord() % (end - start + 1) + start;
}

void Room::playSoundEffectIndex(int soundEffect) {
	_vm->playSoundEffectIndex(soundEffect);
}

void Room::playMidiMusicTracks(int startTrack, int loopTrack) {
	_vm->playMidiMusicTracks(startTrack, loopTrack);
}

void Room::endMission(int16 score, int16 arg1, int16 arg2) {
	_vm->_awayMission.disableInput = true;

	for (int i = 0; i < (_vm->_awayMission.redshirtDead ? 3 : 4); i++) {
		Actor *actor = &_vm->_actorList[i];
		Common::String anim = _vm->getCrewmanAnimFilename(i, "teled");
		_vm->loadActorAnimWithRoomScaling(i, anim, actor->sprite.pos.x, actor->sprite.pos.y);
	}

	_vm->_kirkActor->animationString[0] = '\0';
	_vm->_spockActor->animationString[0] = '\0';
	_vm->_mccoyActor->animationString[0] = '\0';
	_vm->_redshirtActor->animationString[0] = '\0';

	playSoundEffectIndex(8);

	while (_vm->_kirkActor->spriteDrawn)
		_vm->handleAwayMissionEvents();

	_vm->_awayMission.disableInput = false;

	// TODO: game mode switch
}

void Room::showGameOverMenu() {
	_vm->showGameOverMenu();
	// TODO: shouldn't do this within a room
}

void Room::playVoc(Common::String filename) {
	_vm->_sound->playVoc(filename);
}

void Room::spockScan(int direction, TextRef text, bool changeDirection) {
	const char *dirs = "nsew";
	Common::String anim = "sscan_";
	anim.setChar(dirs[direction], 5);

	if (changeDirection) // Check whether he should turn back to original direction after scanning
		_vm->_awayMission.crewDirectionsAfterWalk[OBJECT_SPOCK] = direction;

	loadActorAnim2(OBJECT_SPOCK, anim, -1, -1, 0);
	playSoundEffectIndex(SND_TRICORDER);
	showText(TX_SPEAKER_SPOCK, text);
}

void Room::mccoyScan(int direction, TextRef text, bool changeDirection) {
	const char *dirs = "nsew";
	Common::String anim = "mscan_";
	anim.setChar(dirs[direction], 5);

	if (changeDirection)
		_vm->_awayMission.crewDirectionsAfterWalk[OBJECT_MCCOY] = direction;

	loadActorAnim2(OBJECT_MCCOY, anim, -1, -1, 0);
	playSoundEffectIndex(SND_TRICORDER);
	showText(TX_SPEAKER_MCCOY, text);
}

}