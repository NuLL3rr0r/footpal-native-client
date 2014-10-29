/**
 * @author Morteza Sabetraftar <morteza.sabetraftar@gmail.com>
 */

.pragma library

var currentMailAccount          //  Profile of the currently selected mail account
var selectedMessageId           //  ID of the most recently viewed message
var composeMode                 //  [new, reply, forward]
var newServerMode               //  [new, edit]

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
