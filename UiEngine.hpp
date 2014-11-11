/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 * @author  Mohamad mehdi Kharatizadeh <m_kharatizadeh@yahoo.com>
 */

#ifndef ERTEBAT_UIENGINE_HPP
#define ERTEBAT_UIENGINE_HPP


#include <memory>
#include <QtQml/QQmlApplicationEngine>

namespace Ertebat {
class Screen;
class UiEngine;
}

class Ertebat::Screen : public QObject {
    Q_OBJECT

    Q_ENUMS ( Type )

public:
    enum Type {
        PC,
        Phone,
        Tablet7,
        Tablet10
    };
};

class Ertebat::UiEngine : public QQmlApplicationEngine
{
    Q_OBJECT

    Q_PROPERTY ( QString EmptyLangString READ GetEmptyLangString NOTIFY signal_LanguageChanged )
    Q_PROPERTY ( Ertebat::Screen::Type TargetScreenType READ GetTargetScreenType NOTIFY signal_TargetScreenTypeChanged )

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
    void signal_TargetScreenTypeChanged(Ertebat::Screen::Type);

public:
    QString GetEmptyLangString() const;
    Ertebat::Screen::Type GetTargetScreenType();

public:
    Q_INVOKABLE bool notify(const QString &title, const QString &text, const int id = 0) const;
#if defined ( Q_OS_ANDROID )
    Q_INVOKABLE bool openFileDialog() const;
    Q_INVOKABLE bool showToast(const QString &text, const int duration = 4000) const;
#else
    Q_INVOKABLE bool openFileDialog();
    Q_INVOKABLE bool showToast(const QString &text, const int duration = 4000);
#endif // defined ( Q_OS_ANDROID )
};


#endif /* ERTEBAT_UIENGINE_HPP */

