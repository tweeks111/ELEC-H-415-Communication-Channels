#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include <QDialog>
#include <QMap>

namespace Ui {
class SettingWindow;
}

class SettingWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingWindow(QWidget *parent = nullptr);
    ~SettingWindow();
    QMap<QString, qreal> getSettings();
signals:
    void accept();

protected slots:
    void updateLabels();

private:
    Ui::SettingWindow *ui;
};

#endif // SETTINGWINDOW_H
