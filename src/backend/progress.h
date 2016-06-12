#pragma once

#include <QObject>
#include <QDebug>

class Progress : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool busy READ busy WRITE setBusy NOTIFY busyChanged)
    Q_PROPERTY(int progress READ progress WRITE setProgress NOTIFY progressChanged)
    Q_PROPERTY(QString status READ status WRITE setStatus NOTIFY statusChanged)

public:
    bool busy() const { return m_busy; }
    int progress() const { return m_progress; }
    QString status() const { return m_status; }

    static Progress *begin(const QString &status) {
        return new Progress(true, status);
    }

    static Progress *done(const QString &status) {
        return new Progress(false, status);
    }

    static Progress *done() {
        return done(QString());
    }

public slots:
    void setBusy(bool busy) {
        if (busy != m_busy) {
            m_busy = busy;
            qDebug() << "Busy changed to " << busy;
            emit busyChanged();
        }
    }

    void setProgress(int progress) {
        if (progress != m_progress) {
            m_progress = progress;
            emit progressChanged();
        }
    }

    void setStatus(const QString &status) {
        qDebug() << status;
        if (status != m_status) {
            m_status = status;
            emit statusChanged();
        }
    }

    void update(const QString &status, int progress) {
        setStatus(status);
        setProgress(progress);
    }

    void update(const QString &status) {
        setStatus(status);
    }

    void update(int progress) {
        setProgress(progress);
    }

    void finish(const QString &status) {
        setStatus(status);
        finish();
    }

    void finish() {
        if (progress() != -1)
            setProgress(100);
        setBusy(false);
    }

signals:
    void busyChanged();
    void progressChanged();
    QString statusChanged();

private:
    explicit Progress(bool busy, const QString &status) : m_busy(busy), m_status(status) {}

    bool m_busy = false;
    int m_progress = -1;
    QString m_status;
};
