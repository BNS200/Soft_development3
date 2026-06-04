#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QTableView>
#include <QLabel>
#include <QPushButton>
#include <memory>
#include "ioccontainer.h"
#include "idataprovider.h"
#include "ichartrenderer.h"
#include "iprintstyle.h"
#include "jsonadapter.h"
#include "sqliteadapter.h"




QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected);

private:
    void loadDataFromFile(const QString& filePath);



    Ui::MainWindow *ui;

    QFileSystemModel *rightPartModel;
    QFileSystemModel *leftPartModel;
    QTreeView *treeView;
    QTableView *tableView;

    DataSet m_currentData;
    QString m_currentFilePath;

    QLabel* m_statusLabel;
    QPushButton* m_printButton;


};

#endif // MAINWINDOW_H
