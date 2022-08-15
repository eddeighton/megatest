
#include <QApplication>

#include "main_window.hpp"

#ifndef Q_MOC_RUN

#include "service/tool.hpp"

#include "boost/asio/spawn.hpp"

#include <iostream>

#endif

namespace editor
{
void MainWindow::OnIdle()
{
    /*std::ostringstream os;
    os << "Ed was here";
    QMessageBox::warning( this, tr( "Editor" ), QString::fromUtf8( os.str().c_str() ) );*/
    boost::asio::post( m_yield_context );

    QTimer::singleShot( 10, this, SLOT( OnIdle() ) );
}
} // namespace editor

int runGui( int argc, char* argv[], boost::asio::yield_context& yield_ctx )
{
    QApplication application( argc, argv );

    editor::MainWindow mainWindow( yield_ctx );
    mainWindow.show();
    mainWindow.init();

    return application.exec();
}

int main( int argc, char* argv[] )
{
    mega::service::Tool tool;

    mega::service::Tool::Functor functor
        = [ argc, argv ]( boost::asio::yield_context& yield_ctx ) -> int { return runGui( argc, argv, yield_ctx ); };

    try
    {
        tool.run( functor );
    }
    catch ( std::exception& ex )
    {
        std::cout << "Exception: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
