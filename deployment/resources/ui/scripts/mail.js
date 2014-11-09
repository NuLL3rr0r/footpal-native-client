/**
 * @author Morteza Sabetraftar <morteza.sabetraftar@gmail.com>
 * @author  Mohamad mehdi Kharatizadeh <m_kharatizadeh@yahoo.com>
 */

.pragma library

var currentMailAccount          //  Profile of the currently selected mail account
var selectedMessageId           //  ID of the most recently viewed message
var composeMode                 //  [new, reply, forward]
var newServerMode               //  [new, edit]
var text;                       //  HTML body of message
var from;                       //  Sender contact
var date;                       //  Date of sent message
var time;                       //  Time of sent message
var subject;                    //  Subject of message


var isMailboxCached = false;            // Determines whether storage of cache is enabled for Mailbox.qml(default is false)
var cache_isInitialzed;         // Used to cache Mailbox.qml private date to discourage re-fetching mails after navigating to view individual mails.
var cache_initializedTo;        // Used to cache Mailbox.qml private date to discourage re-fetching mails after navigating to view individual mails.
var cache_maxSummary;           // Used to cache Mailbox.qml private date to discourage re-fetching mails after navigating to view individual mails.
var cache_itemHeight;           // Used to cache Mailbox.qml private date to discourage re-fetching mails after navigating to view individual mails.
var cache_itemSpacing;          // Used to cache Mailbox.qml private date to discourage re-fetching mails after navigating to view individual mails.
var cache_loadSize;             // Used to cache Mailbox.qml private date to discourage re-fetching mails after navigating to view individual mails.
var cache_loadIndex;            // Used to cache Mailbox.qml private date to discourage re-fetching mails after navigating to view individual mails.
var cache_mailCount;            // Used to cache Mailbox.qml private date to discourage re-fetching mails after navigating to view individual mails.
var cache_idToIndex;            // Used to cache Mailbox.qml private date to discourage re-fetching mails after navigating to view individual mails.
var cache_mailJSON;             // Used to cache Mailbox.qml private date to discourage re-fetching mails after navigating to view individual mails.
var cache_targetClient;         // Used to cache Mailbox.qml private date to discourage re-fetching mails after navigating to view individual mails.

function strToSecurityType(type) {
    if (type == "plain")
        return 0;
    else if (type == "ssl")
        return 2;
    else if (type == "starttls")
        return 1;
    else
        return -1;
}

function strToSecurityComboIndex(type) {
    if (type == "plain")
        return 0;
    else if (type == "ssl")
        return 2;
    else if (type == "starttls")
        return 4;
    else
        return -1;
}
