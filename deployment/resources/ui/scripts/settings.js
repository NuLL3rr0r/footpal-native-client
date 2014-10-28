/**
 * @author Morteza Sabetraftar <morteza.sabetraftar@gmail.com>
 */

.pragma library

var globalBgColor = "#203070";// "#00AEFF";

function getDatabase() {
    return openDatabaseSync("SettingsDB", "1.0", "StorageDatabase", 100000);
}

// At the start of the application, we can initialize the tables we need if they haven't been created yet
function initialize() {
    var db = getDatabase();
    db.transaction(
                function(tx) {
                    // Mail Accounts Table
                    tx.executeSql('CREATE TABLE IF NOT EXISTS MailServers(name TEXT PRIMARY KEY,' +
                                  'protocol TEXT, username TEXT, password TEXT, readHost TEXT, readPort INTEGER,' +
                                  'readSecurity TEXT, sendHost TEXT, sendPort INTEGER, sendScurity TEXT)');
                });
}

function addMailAccount(name, protocol, username, password, readHost, readPort, readSecurity,
                        sendHost, sendPort, sendSecurity) {
    var db = getDatabase();
    var res = "";
    db.transaction(function(tx) {
        var rs = tx.executeSql('INSERT OR REPLACE INTO settings VALUES (?,?,?,?,?,?,?,?,?,?);',
                               [name, protocol, username, password, readHost, readPort, readSecurity,
                                sendHost, sendPort, sendSecurity]);
        if (rs.rowsAffected > 0) {
            res = "OK";
        } else {
            res = "Error";
        }
    });
    return res;
}

function getMailAccount(name) {
    var db = getDatabase();
    var res="";
    db.transaction(function(tx) {
        var rs = tx.executeSql('SELECT value FROM MailServers WHERE name=?;', [name]);
        if (rs.rows.length > 0) {
            res = rs.rows.item(0).value;
        } else {
            res = "Unknown";
        }
    });
    return res
}


function setSetting(setting, value) {
    // setting: string representing the setting name (eg: “username”)
    // value: string representing the value of the setting (eg: “myUsername”)
    var db = getDatabase();
    var res = "";
    db.transaction(function(tx) {
        var rs = tx.executeSql('INSERT OR REPLACE INTO settings VALUES (?,?);', [setting,value]);
        //console.log(rs.rowsAffected)
        if (rs.rowsAffected > 0) {
            res = "OK";
        } else {
            res = "Error";
        }
    }
    );
    // The function returns “OK” if it was successful, or “Error” if it wasn't
    return res;
}

function getSetting(setting) {
    var db = getDatabase();
    var res="";
    db.transaction(function(tx) {
        var rs = tx.executeSql('SELECT value FROM settings WHERE setting=?;', [setting]);
        if (rs.rows.length > 0) {
            res = rs.rows.item(0).value;
        } else {
            res = "Unknown";
        }
    });
    // The function returns “Unknown” if the setting was not found in the database
    // For more advanced projects, this should probably be handled through error codes
    return res
}
