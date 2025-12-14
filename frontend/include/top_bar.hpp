#ifndef TOP_BAR_HPP
#define TOP_BAR_HPP

#include "qt_common.hpp"
#include "globals.hpp"
#include <functional>

class TopBar : public QWidget {
    Q_OBJECT
public:
    TopBar(QWidget *parent = nullptr);

    void refreshBar();

    void setAuthChangedCallback(const std::function<void(const QString &)> &cb) { authChangedCallback = cb; }

signals:
    void accountButtonClicked();
    void bookButtonClicked();
    void logButtonClicked();
    void authChanged(const QString &message);

private slots:
    void onLoginButtonClicked();
    void onRegisterButtonClicked();
    void onLogoutButtonClicked();
    void onProfileButtonClicked();
    void onAccountButtonClicked();
    void onBookButtonClicked();
    void onLogButtonClicked();

private:
    QLabel *titleLabel;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QPushButton *accountButton;
    QPushButton *bookButton;
    QPushButton *logButton;
    QPushButton *userButton;

    std::function<void(const QString &)> authChangedCallback;

    void applyStyle();
};

#endif