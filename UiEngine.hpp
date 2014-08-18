#ifndef FOOTPAL_UIENGINE_HPP
#define FOOTPAL_UIENGINE_HPP


#include <memory>
#include <QtQml/QQmlApplicationEngine>

namespace Footpal {
class UiEngine;
}

class Footpal::UiEngine : public QQmlApplicationEngine
{
    Q_OBJECT

    Q_ENUMS( ScreenType )

    Q_PROPERTY ( QString EmptyLangString READ GetEmptyLangString NOTIFY signal_LanguageChanged )
    Q_PROPERTY ( ScreenType ScreenType READ GetScreenType NOTIFY signal_ScreenTypeChangedChanged )

public:
    enum ScreenType {
        ScreenType_PC,
        ScreenType_Phone,
        ScreenType_Tablet7,
        ScreenType_Tablet10
    };

private:
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

public:
    explicit UiEngine(QObject *parent = 0);
    UiEngine(const QUrl &url, QObject *parent = 0);
    UiEngine(const QString &filePath, QObject *parent = 0);
    virtual ~UiEngine();

signals:
    void signal_LanguageChanged();
    void signal_ScreenTypeChangedChanged(ScreenType);

public:
    QString GetEmptyLangString() const;
    ScreenType GetScreenType();

public:
    Q_INVOKABLE bool notify(const QString &title, const QString &text, const int id = 0) const;
#if defined ( Q_OS_ANDROID )
    Q_INVOKABLE bool showToast(const QString &text, const int duration = 8000) const;
#else
    Q_INVOKABLE bool showToast(const QString &text, const int duration = 8000);
#endif // defined ( Q_OS_ANDROID )
};


#endif /* FOOTPAL_UIENGINE_HPP */

