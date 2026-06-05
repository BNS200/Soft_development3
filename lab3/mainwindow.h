#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QTableView>
#include <QLabel>
#include <QPushButton>
#include <QSplitter>
#include <memory>
#include <QComboBox>

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
    void switchToChartMode();


    Ui::MainWindow *ui;

    // MVC компоненты
    QFileSystemModel *rightPartModel;
    QFileSystemModel *leftPartModel;
    QTreeView *treeView;
    QTableView *tableView;
    QSplitter *m_splitter;

    //Данные
    DataSet m_currentData;
    QString m_currentFilePath;

    QWidget* m_chartWidget;
    QChartView* m_chartView;
    QComboBox* m_chartTypeCombo;
    QComboBox* m_styleCombo;
    QLabel* m_statusLabel;
    QPushButton* m_exitButton;
    QPushButton* m_printButton;

    bool m_isChartMode;
    QList<int> m_savedSizeSplitter;



};

#endif // MAINWINDOW_H
