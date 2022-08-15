
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMessageBox>

#ifndef Q_MOC_RUN

#include "boost/asio/spawn.hpp"

#include <iostream>
#include <sstream>

#endif

namespace Ui
{
class MainWindow;
}

namespace editor
{

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow( boost::asio::yield_context& yield_context, QWidget* pParent = 0 )
        : QMainWindow( pParent )
        , m_yield_context( yield_context )
    {
    }
    virtual ~MainWindow() {}

    void init() { QTimer::singleShot( 0, this, SLOT( OnIdle() ) ); }

public slots:
    void OnIdle();

protected:
    boost::asio::yield_context& m_yield_context;
    // Ui::MainWindow* m_pMainWindowImpl;
};
} // namespace editor

#endif // MAINWINDOW_H
