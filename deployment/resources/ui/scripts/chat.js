/**
 * @author  Majid Sadeghi Alavijeh <majid.sadeghi.alavijeh@gmail.com>
 */

.pragma library

var Context;
if(!Context)
{
    console.log("create context");
    Context = new ContextSchema();
}
else{
    console.log("context exist");
}

function ContextSchema(){
    this.friends = [];
    this.rooms = [];
    this.token = "Nan";
    this.username = 'Nan';
    this.currentProfile = null;
    this.currentRoomId = null;
    this.currentContactId = null;
    this.currentLanguage = "EN";
}

ContextSchema.prototype.cleanStorage = function(){
    this.friends = [];
    this.rooms = [];
}

ContextSchema.prototype.addFriend = function(friendId, friendUsername, state){
    if(!this.isFriendExist(friendUsername)){
        var newFriend = new FriendSchema(friendId, friendUsername, state);
        this.friends.push(newFriend);
        if(this.newFriendCallback)
            this.newFriendCallback(newFriend);
    }
}

ContextSchema.prototype.registerNewFriendCallback = function(cb){
    this.newFriendCallback = cb;
}

ContextSchema.prototype.unregisterNewFriendCallback = function(){
    this.newFriendCallback = null;
}

ContextSchema.prototype.isFriendExist = function(friendUsername){
    for(var i = 0 ;  i < this.friends.length ; i++){
        if(this.friends[i].friendUsername === friendUsername)
            return i;
    }
    return null;
}

ContextSchema.prototype.addNewRoom = function(roomName, roomId, roomDesc, roomLogo, roomType){
    if(!this.getRoomById(roomId)){
        var newRoom = new RoomSchema(roomName, roomId, roomDesc, roomDesc, roomType);
        this.rooms.push(newRoom);
        if(this.newRoomCallback)
            this.newRoomCallback(newRoom);
    }
}

ContextSchema.prototype.isRoomExist = function(roomId){
    for(var i = 0 ;  i < this.rooms.length ; i++){
        if(this.rooms[i].id === roomId)
            return i;
    }
    return null;
}

ContextSchema.prototype.getRoomById = function(roomId){
    for(var i = 0 ;  i < this.rooms.length ; i++){
        console.log("inspect : " + this.rooms[i].id);
        if(this.rooms[i].id === roomId)
            return this.rooms[i];
    }
    return null;
}

ContextSchema.prototype.registerNewRoomCallback = function(cb){
    this.newRoomCallback = cb;
}

ContextSchema.prototype.unregisterNewRoomCallback = function(){
    this.newRoomCallback = null;
}

ContextSchema.prototype.regitserRoomAnnouncerCallback = function(cb){
    this.roomAnnouncerCallback = cb;
    for(var i = 0 ; i < this.rooms.length ; i++){
        this.rooms[i].registerMessageAnnouncerCallback(cb);
    }
}

ContextSchema.prototype.regitserRoomMessageReaderCallback = function(roomId, cb){
    this.roomMessageReaderCallback = cb;
    var room = this.getRoomById(roomId);
    if(room)
        room.registerMessageReaderCallback(cb);
}

ContextSchema.prototype.unRegitserRoomMessageReaderCallback = function(roomId){
    this.roomMessageReaderCallback = null;
    var room = this.getRoomById(roomId);
    if(room)
        room.unregisterMessageReaderCallback();
}

ContextSchema.prototype.addMemberToRoom = function(roomId, memberId, memberUsername){
    var room = this.getRoomById(roomId);
    if(room){
        console.log(room.id + " : " + memberId + "," + memberUsername);
        room.addMember(memberId, memberUsername);
    }
}

ContextSchema.prototype.testFunction = function(){
    console.log("context menu testFunction");
}

ContextSchema.prototype.testSetVariable = function(){
    this.test = "majid";
}

function RoomSchema(name, id, desc, logo, type){
    this.name = name;
    this.id = id;
    this.desc = desc;
    this.logo = logo;
    this.type = type;
}

RoomSchema.prototype.addNewMessage = function(message){
    if(!this.unreadMessages)
        this.unreadMessages = [];
    if(this.messageReaderCallback){
        console.log("MessageReaderRegistered");
        if(!this.messages)
            this.messages = [];
        this.messages.push(message);
        this.messageReaderCallback(this.id, message);
    }
    else{
        console.log("MessageAnnouncerRegistered");
        this.unreadMessages.push(message);
        if(this.messageAnnouncerCallback)
            this.messageAnnouncerCallback(this.id, message);
    }
}

RoomSchema.prototype.getMessages = function(){
    if(!this.messages){
        this.messages = [];
    }
    if(this.unreadMessages){
        var tmp = this.unreadMessages.shift();
        while(tmp){
            this.messages.push(tmp);
            tmp = this.unreadMessages.shift();
        }
    }

    return this.messages;
}

RoomSchema.prototype.registerMessageReaderCallback = function(callback){
    this.messageReaderCallback = callback;
}

RoomSchema.prototype.unregisterMessageReaderCallback = function(){
    this.messageReaderCallback = null;
}

RoomSchema.prototype.registerMessageAnnouncerCallback = function(callback){
    this.messageAnnouncerCallback = callback;
}

RoomSchema.prototype.unregisterMessageAnnouncerCallback = function(){
    this.messageAnnouncerCallback = null;
}

RoomSchema.prototype.addMember = function(userId, userName){
    if(!this.members)
        this.members = [];
    var newMember = {userId : userId, userName: userName};
    this.members.push(newMember);

    if(this.newMemberCallback)
        this.newMemberCallBack(this, newMember);
}

RoomSchema.prototype.registerNewMemberCallback = function(cb){
    this.newMemberCallback = cb;
}

RoomSchema.prototype.unregisterNewMemberCallbacj = function(){
    this.newMemberCallback = null;
}

RoomSchema.prototype.removeMember = function(userId){
    if(this.members){
        for(var i = 0 ; i < this.members.length ; i++){
            if(this.members[i].userId == userId){
                this.members.splice(i, 1);
                break;
            }
        }
    }
}

RoomSchema.prototype.testFunction = function(testMessage){
    console.log(testMessage);
}

RoomSchema.prototype.log = function(){
    console.log('##################### room log ####################');
    console.log('room name : ' + this.name);
    console.log('room id : ' + this.id);
    console.log('room desc : ' + this.desc);
    console.log('room logo : ' + this.logo);
    console.log('room type : ' + this.type);
    for(var i = 0 ; i < this.members.length ; i++){
        console.log("user name : " + this.members[i].userName + " user id : " + this.members[i].userId);
    }
    console.log('##################### end of room log ####################');
}

function MessageSchema(from, to, date, time, body){
    this.from = from;
    this.to = to;
    this.date = date;
    this.time = time;
    this.body = body;
}


function FriendSchema(friendId, friendUsername, state){
    this.friendId = friendId;
    this.friendUsername = friendUsername;
    this.state = state;
}
