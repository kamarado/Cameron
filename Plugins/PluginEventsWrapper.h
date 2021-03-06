#ifndef PLUGINEVENTSWRAPPER_H
#define PLUGINEVENTSWRAPPER_H

#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QVariantList>


class PluginEventsBase :  public QObject
{
    Q_OBJECT

public:
    explicit PluginEventsBase(QObject * parent): QObject(parent) {}

signals:
    void doAction(QString, QVariantList);
    void addAction(QString, QObject *, const char *);
};



class PluginEvents : public PluginEventsBase
{
    Q_OBJECT

public:
    PluginEvents(QObject * parent =0): PluginEventsBase(parent) {
        qDebug()<<"Plugin Events constructor";
        _sInst = this;
        connect(this, SIGNAL(doAction(QString, QVariantList)), parent, SIGNAL(doAction(QString, QVariantList)));
        connect(this, SIGNAL(addAction(QString, QObject *, const char *)), parent, SIGNAL(addAction(QString, QObject *, const char *)) );
    }

    QVariant ARG(void * p) { return qVariantFromValue((void *)p); }

    static PluginEvents * Instance() {
        if(!_sInst) {
            qDebug()<<"PluginEvents not created!!!";
        }
        return _sInst;
    }

private:
    static PluginEvents * _sInst;
};


#define Events PluginEvents::Instance()


#endif // PLUGINEVENTSWRAPPER_H

