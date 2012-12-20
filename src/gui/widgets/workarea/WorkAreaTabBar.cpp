#include "WorkAreaTabBar.h"
#include "QMouseEvent"
#include <QStylePainter>
#include <QStyleOptionTabV3>

using namespace Robomongo;

/**
 * @brief Creates WorkAreaTabBar, without parent widget. We are
 * assuming, that tab bar will be installed to (and owned by)
 * WorkAreaTabWidget, using QTabWidget::setTabBar().
 */
WorkAreaTabBar::WorkAreaTabBar() : QTabBar()
{
    setStyleSheet(buildStyleSheet());

    _menu = new QMenu(this);

    _newShellAction = new QAction("&New Shell", _menu);
    _newShellAction->setShortcut(Qt::CTRL + Qt::Key_T);

    _reloadShellAction = new QAction("&Reload", _menu);
    _duplicateShellAction = new QAction("&Duplicate", _menu);
    _pinShellAction = new QAction("&Pin Shell", _menu);
    _closeShellAction = new QAction("&Close Shell", _menu);
    _closeOtherShellsAction = new QAction("Close &Other Shells", _menu);
    _closeShellsToTheRightAction = new QAction("Close Shells to the R&ight", _menu);

    _menu->addAction(_newShellAction);
    _menu->addSeparator();
    _menu->addAction(_reloadShellAction);
    _menu->addAction(_duplicateShellAction);
    _menu->addSeparator();
    _menu->addAction(_closeShellAction);
    _menu->addAction(_closeOtherShellsAction);
    _menu->addAction(_closeShellsToTheRightAction);
}

/**
 * @brief Overrides QTabBar::mouseReleaseEvent() in order to support
 * middle-mouse tab close and to implement tab context menu.
 */
void WorkAreaTabBar::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MidButton)
    {
        int tabIndex = tabAt(event->pos());

        if (tabIndex < 0)
            return;

        emit tabCloseRequested(tabIndex);
    } else if (event->button() == Qt::RightButton) {
        int tabIndex = tabAt(event->pos());

        if (tabIndex < 0)
            return;

        QAction *selected = _menu->exec(QCursor::pos());

        if (!selected)
            return;

        emitSignalForContextMenuAction(tabIndex, selected);
    }

    QTabBar::mouseReleaseEvent(event);
}

/**
 * @brief Builds stylesheet for this WorkAreaTabBar widget.
 */
QString WorkAreaTabBar::buildStyleSheet()
{
    QString styles =
        "QTabBar::close-button { "
            "image: url(:/robomongo/icons/close_2_16x16.png);"
            "width: 10px;"
            "height: 10px;"
        "}"
        "QTabBar::close-button:hover { "
              "image: url(:/robomongo/icons/close_hover_16x16.png);"
              "width: 15px;"
              "height: 15px;"
        "}"
        "QTabBar::tab {"
            "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                        "stop: 0 #F0F0F0, stop: 0.4 #DEDEDE,"
                                        "stop: 0.5 #E6E6E6, stop: 1.0 #E1E1E1);"
            "border: 1px solid #C4C4C3;"
            "border-bottom-color: #C2C7CB;" // same as the pane color
            "border-top-left-radius: 6px;"
            "border-top-right-radius: 6px;"
            "min-width: 8ex;"
            "max-width: 200px;"
            "padding: 4px 0px 4px 2px;"
            "margin: 0px;"
            "margin-right: -2px;"
        "}"

        "QTabBar::tab:selected, QTabBar::tab:hover {"
            "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                        "stop: 0 #fafafa, stop: 0.4 #f4f4f4,"
                                        "stop: 0.5 #e7e7e7, stop: 1.0 #fafafa);"
        "}"

        "QTabBar::tab:selected {"
            "border-color: #9B9B9B;"
            "border-bottom-color: #fafafa;"
        "}"

        "QTabBar::tab:!selected {"
            "margin-top: 2px;" // make non-selected tabs look smaller
        "}  "
        "QTabBar::tab:only-one { margin-top: 2px; }"
    ;

    return styles;
}

/**
 * @brief Emits signal, based on specified action. Only actions
 * specified in this class are supported. If we don't know specified
 * action - no signal will be emited.
 * @param tabIndex: index of tab, for which signal will be emited.
 * @param action: context menu action.
 */
void WorkAreaTabBar::emitSignalForContextMenuAction(int tabIndex, QAction *action)
{
    if (action == _newShellAction)
        emit newTabRequested(tabIndex);
    else if (action == _reloadShellAction)
        emit reloadTabRequested(tabIndex);
    else if (action == _duplicateShellAction)
        emit duplicateTabRequested(tabIndex);
    else if (action == _pinShellAction)
        emit pinTabRequested(tabIndex);
    else if (action == _closeShellAction)
        emit tabCloseRequested(tabIndex);
    else if (action == _closeOtherShellsAction)
        emit closeOtherTabsRequested(tabIndex);
    else if (action == _closeShellsToTheRightAction)
        emit closeTabsToTheRightRequested(tabIndex);
}