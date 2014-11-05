/**
 * @author  Majid Sadeghi Alavijeh <majid.sadeghi.alavijeh@gmail.com>
 */

.pragma library

Qt.include("chat.js")

var url = "ws://192.168.10.75:1337";

var socketJs;
var timer;

function registerSocket(socketIns){
    socketJs = socketIns;
}

function registerTimer(ti){
    //console.log("socket registered");
    timer = ti;
}

function openSocket(){
    //console.log("try to open socket");
    socketJs.active = true;
}

function parseTextMessage(message){
    //console.log("new message : " + message);
    var encoded = JSON.parse(message);
    //console.log(encoded + " : " + encoded.code);
    if(encoded.code === 0){
        authorizeToWs(Context.token);
    }
    else if(encoded.code === 1){
        //console.log("Send GetIndividualRooms cmd");
        getIndividualRooms();
        getMyProfile();
        getFriendList();
    }
    else if(encoded.code === 5){
        //console.log("IndividualContacts result");
        for(var i = 0 ; i < encoded.rooms.length ; i++){
            //console.log(encoded.rooms[i]._id);
            Context.addNewRoom("room", encoded.rooms[i]._id, "", "", "I");
            for(var j = 0 ; j < encoded.rooms[i].Members.length; j++){
                //console.log(encoded.rooms[i].Members[j]);
                Context.addMemberToRoom(encoded.rooms[i]._id, encoded.rooms[i].Members[j], encoded.rooms[i].Members[j]);
            }
            Context.getRoomById(encoded.rooms[i]._id).log();
        }
    }
    else if(encoded.code === 6){
        //console.log("currentProfile");
        Context.currentProfile = encoded.profile;
        //console.log(Context.currentProfile.id + " : " + Context.currentProfile.username);
    }
    else if(encoded.code === 3){
        //console.log(encoded.message);
        //console.log(encoded.value.roomId);
        var room = Context.getRoomById(encoded.value.roomId);
        var self = 'self';
        if(encoded.value.from === Context.currentProfile.username){
            //console.log('self message');
        }
        else{
            self = encoded.value.from;
        }

        if(room){
            room.addNewMessage(new MessageSchema(self, encoded.value.roomId, new Date().toDateString(), new Date().toTimeString().substring(0, 5), encoded.value.content));
        }
    }
    else if(encoded.code === 8){
        //console.log("user added to database successfully : " + encoded.friend.friendId);
        Context.addFriend(encoded.friend.friendId, encoded.friend.friendUsername, encoded.friend.state   );
    }
    else if(encoded.code === 9 ){
        for(var i = 0 ; i <encoded.friends.length; i++){
            //console.log(encoded.friends[i].friendId + " : " + encoded.friends[i].friendUsername);
            Context.addFriend(encoded.friends[i].friendId, encoded.friends[i].friendUsername, encoded.friends[i].state   );
        }
    }
}

function getMyProfile(){
    var cmd = "{\"requestCode\" : \"4\", \"message\": \"GetCurrentProfile\"}";
    //console.log(cmd)
    socketJs.sendTextMessage(cmd);
}

function TryToConnectToWS(){
    //console.log(socketJs.status);
    if(socketJs.status != 0 || socketJs.status != 1)
    {
        socketJs.active = false;
        openSocket();
    }
}

function getUserByUserId(userId){
    var cmd = "{\"requestCode\" : \"5\", \"message\": \"GetUsernameViaUserId\"}";
    //console.log(cmd);
    socketJs.sendTextMessage(cmd);
}

function websocketOpened(){
    //console.log("Socket has been opened");
    timer.stop();
}

function webSocketClosed(){
    //console.log("Socket has been closed");
    //console.log("try to open about 5 seconds later");
    timer.start();
}

function webSocketError(err){
    //console.log("Error: " + err);
    timer.start();
}

function authorizeToWs(token){
    var cmd = "{\"token\":\"" + token + "\"}";
    //console.log(cmd)
    socketJs.sendTextMessage(cmd);
}

function getIndividualRooms(){
    var cmd = "{\"requestCode\" : \"3\", \"message\": \"GetIndividualRooms\"}";
    //console.log(cmd);
    socketJs.sendTextMessage(cmd);
}

function addFriendToList(username){
    var cmd = "{\"requestCode\" : \"6\", \"message\": \"AddUserToFriend\", \"username\" : \"" + username + "\"}";
    //console.log(cmd);
    socketJs.sendTextMessage(cmd);
}

function getFriendList(){
    var cmd = "{\"requestCode\" : \"7\", \"message\": \"GetFriendList\"}";
    //console.log(cmd);
    socketJs.sendTextMessage(cmd);
}

function sendTextMessageToRoom(message, to){
    try
    {
        var cmd = "{\"requestCode\" : \"1\", \"message\":\"SendTextMessage\", \"messageContent\":\"" + message + "\", \"publishType\":\"Now\", \"roomId\":\"" + to + "\"}";
        socketJs.sendTextMessage(cmd);

//        var room = Context.getRoomById(to);
//        if(room){
//            room.addNewMessage(new MessageSchema('self', to, new Date().toDateString(), new Date().toTimeString().substring(0, 5), message));
//        }
    }
    catch(ex){
        //console.log(ex);
    }
}
