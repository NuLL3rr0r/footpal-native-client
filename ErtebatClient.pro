# @author  Mohammad S. Babaei <info@babaei.net>
# @author  Morteza Sabetraftar <morteza.sabetraftar@gmail.com>


TEMPLATE = app


QT += core gui network qml quick websockets widgets
QT -= multimedia multimediawidgets sql testlib webkit
android {
    QT += androidextras
}


android {
    DEFINES += STATIC_LINK_DEPENDENCIES
}
win32 {
    DEFINES += SHARED_LINK_DEPENDENCIES
}
DEFINES += "BUFFERSIZE=16777216"
DEFINES += REST_BASE_URL=\\\"http://localhost:4000\\\"


QMAKE_CXXFLAGS += -std=c++1y
QMAKE_CXXFLAGS += -Wall -Wextra -pedantic
INCLUDEPATH += $$PWD
#INCLUDEPATH += $$PWD/dependencies/include
QMAKE_CFLAGS += -isystem $$PWD/dependencies/include
QMAKE_CXXFLAGS += -isystem $$PWD/dependencies/include


android {
    debug {
        equals ( ANDROID_TARGET_ARCH, armeabi ) {
            LIBS += -L$$PWD/dependencies/lib/android_armv5/debug
        }
        equals ( ANDROID_TARGET_ARCH, armeabi-v7a ) {
            LIBS += -L$$PWD/dependencies/lib/android_armv7/debug
        }
        equals ( ANDROID_TARGET_ARCH, x86 ) {
            LIBS += -L$$PWD/dependencies/lib/android_x86/debug
        }
    }
    release {
        equals ( ANDROID_TARGET_ARCH, armeabi ) {
            LIBS += -L$$PWD/dependencies/lib/android_armv5/release
        }
        equals ( ANDROID_TARGET_ARCH, armeabi-v7a ) {
            LIBS += -L$$PWD/dependencies/lib/android_armv7/release
        }
        equals ( ANDROID_TARGET_ARCH, x86 ) {
            LIBS += -L$$PWD/dependencies/lib/android_x86/release
        }
    }
}
win32 {
    debug {
        LIBS += -L$$PWD/dependencies/lib/mingw482_32/debug
    }
    release {
        LIBS += -L$$PWD/dependencies/lib/mingw482_32/release
    }
}
LIBS += -lb64 -lboost_date_time -lboost_filesystem -lboost_system -lcppdb -lcppdb_sqlite3 -lcryptopp -lsqlite3
win32 {
    LIBS += -lvmime
}


HEADERS += \
    make_unique.hpp \
    Mail/BlindCertificateVerifier.hpp \
    Mail/ImapClient.hpp \
    Mail/Mail.hpp \
    Mail/Mailbox.hpp \
    Mail/Message.hpp \
    Mail/Pop3Client.hpp \
    Mail/SmtpClient.hpp \
    Application.hpp \
    Crypto.hpp \
    Database.hpp \
    Exception.hpp \
    Http.hpp \
    HttpStatus.hpp \
    Log.hpp \
    Pool.hpp \
    RestApi.hpp \
    UiEngine.hpp \
    Mail/Client.hpp \
    Mail/MailDatabase.hpp \
    Mail/MailFolder.hpp \
    Mail/MailView.hpp \
    Mail/Android/MailProfile.hpp
SOURCES += \
    main.cpp \
    Mail/BlindCertificateVerifier.cpp \
    Mail/ImapClient.cpp \
    Mail/Mailbox.cpp \
    Mail/Message.cpp \
    Mail/Pop3Client.cpp \
    Mail/SmtpClient.cpp \
    Application.cpp \
    Crypto.cpp \
    Database.cpp \
    Exception.cpp \
    Http.cpp \
    HttpStatus.cpp \
    Log.cpp \
    Pool.cpp \
    RestApi.cpp \
    UiEngine.cpp \
    Mail/MailDatabase.cpp \
    Mail/MailView.cpp \
    Mail/Client.cpp
android {
    HEADERS += \
        Android.hpp
    SOURCES += \
        Android.cpp
}
lupdate_only {
    SOURCES += \
        deployment/resources/ui/Main.qml \
        deployment/resources/ui/AddMailServer.qml \
        deployment/resources/ui/ChangePassword.qml \
        deployment/resources/ui/Chat.qml \
        deployment/resources/ui/ChatLog.qml \
        deployment/resources/ui/ComposeMail.qml \
        deployment/resources/ui/ContactList.qml \
        deployment/resources/ui/ContactProfile.qml \
        deployment/resources/ui/ContactSelection.qml \
        deployment/resources/ui/CreateRoom.qml \
        deployment/resources/ui/ForgotPassword.qml \
        deployment/resources/ui/Home.qml \
        deployment/resources/ui/Mailbox.qml \
        deployment/resources/ui/MailDetails.qml \
        deployment/resources/ui/MailServers.qml \
        deployment/resources/ui/SignIn.qml \
        deployment/resources/ui/SignUp.qml \
        deployment/resources/ui/Splash.qml \
        deployment/resources/ui/UserProfile.qml
}


RESOURCES += \
    deployment/resources/db.qrc \
    deployment/resources/fnt.qrc \
    deployment/resources/img.qrc \
    deployment/resources/js.qrc \
    deployment/resources/ui.qrc


android {
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/deployment/android

    OTHER_FILES += \
        deployment/android/AndroidManifest.xml \
        deployment/android/libs/activation.jar \
        deployment/android/libs/additionnal.jar \
        deployment/android/libs/mail.jar \
        deployment/android/res/drawable-hdpi/icon.png \
        deployment/android/res/drawable-ldpi/icon.png \
        deployment/android/res/drawable-mdpi/icon.png \
        deployment/android/res/values/screen.xml \
        deployment/android/res/values/strings.xml \
        deployment/android/res/values-sw600dp/screen.xml \
        deployment/android/res/values-sw720dp/screen.xml \
        deployment/android/src/com/ertebat/client/Android.java
}


#################
# Automatically generating .qm language files and adding them to resources
#################

# var, prepend, append
defineReplace(prependAll) {
    for(a,$$1):result += $$2$${a}$$3
    return($$result)
}

# Supported languages
LANGUAGES = en fa

# Available translations
TRANSLATIONS = $$prependAll(LANGUAGES, $$PWD/translations/, .ts)

# run LUPDATE to generate or update the ts files
qtPrepareTool(LUPDATE, lupdate)
command = $$LUPDATE ErtebatClient.pro
system($$command)|error("Failed to run: $$command")

# Used to embed the qm files in resources
TRANSLATIONS_FILES =

# run LRELEASE to generate the qm files
qtPrepareTool(LRELEASE, lrelease)
for(tsfile, TRANSLATIONS) {
    qmfile = $$shadowed($$tsfile)
    qmfile ~= s,\\.ts$,.qm,
    qmdir = $$dirname(qmfile)
    !exists($$qmdir) {
        mkpath($$qmdir)|error("Aborting.")
    }
    command = $$LRELEASE -removeidentical $$tsfile -qm  $$qmfile
    system($$command)|error("Failed to run: $$command")
    TRANSLATIONS_FILES += $$qmfile
}

# Create the resource file
GENERATED_RESOURCE_FILE = $$OUT_PWD/translations.qrc

RESOURCE_CONTENT = \
    "<RCC>" \
    "<qresource>"

for(translationfile, TRANSLATIONS_FILES) {
    relativepath_out = $$relative_path($$translationfile, $$OUT_PWD)
    RESOURCE_CONTENT += "<file alias=\"$$relativepath_out\">$$relativepath_out</file>"
}

RESOURCE_CONTENT += \
    "</qresource>" \
    "</RCC>"

write_file($$GENERATED_RESOURCE_FILE, RESOURCE_CONTENT)|error("Aborting.")

RESOURCES += $$GENERATED_RESOURCE_FILE

#################
### End of  # Automatically generating .qm language files and adding them to resources
#################


# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

OTHER_FILES += \
    deployment/android/src/top/social/mailapplication/MailProfile.java \
    deployment/android/src/top/social/mailapplication/MailProtocol.java \
    deployment/android/src/top/social/mailapplication/MainActivity.java \
    deployment/android/src/top/social/mailapplication/MessageAdapter.java \
    deployment/android/src/top/social/mailapplication/SettingsActivity.java

