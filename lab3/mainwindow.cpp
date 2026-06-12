
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListView>
#include <QTreeView>
#include <QFileSystemModel>
#include <QItemSelectionModel>
#include <QTableView>
#include <QHeaderView>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_chartView(nullptr)
    , m_isChartMode(false)
    , m_splitter(nullptr)
    , m_chartWidget(nullptr)
{
    ui->setupUi(this);

    registerDependencies();

    this->setGeometry(100, 100, 1500, 500);
    this->setStatusBar(new QStatusBar(this));
    this->statusBar()->showMessage("Выбранный путь : ");
    QString homePath = QDir::homePath();

    leftPartModel =  new QFileSystemModel(this);
    leftPartModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    leftPartModel->setRootPath(homePath);

    rightPartModel = new QFileSystemModel(this);
    rightPartModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    rightPartModel->setRootPath(homePath);

    treeView = new QTreeView(this);
    treeView->setModel(leftPartModel);
    treeView->expandAll();
    treeView->header()->resizeSection(0, 200);

    tableView = new QTableView(this);
    tableView->setModel(rightPartModel);

    m_splitter = new QSplitter(this);
    m_splitter->addWidget(treeView);
    m_splitter->addWidget(tableView);
    setCentralWidget(m_splitter);

    QItemSelectionModel *selectionModel = treeView->selectionModel();
    connect(selectionModel, &QItemSelectionModel::selectionChanged, this, &MainWindow::on_selectionChangedSlot);


    QItemSelection toggleSelection;
    QModelIndex topLeft = leftPartModel->index(homePath);
    toggleSelection.select(topLeft, topLeft);
    selectionModel->select(toggleSelection, QItemSelectionModel::Toggle);

    onChartTypeChanged(0);
    onStyleChanged(0);
}

void MainWindow::registerDependencies()
{
    m_container.RegisterInstance<IChartRenderer>("BarChart", std::make_shared<BarChartRenderer>());
    m_container.RegisterInstance<IChartRenderer>("PieChart", std::make_shared<PieChartRenderer>());

    m_container.RegisterInstance<IPrintStyle>("Color", std::make_shared<ColorStyle>());
    m_container.RegisterInstance<IPrintStyle>("Gray", std::make_shared<GrayStyle>());
}

void MainWindow::onChartTypeChanged(int index)
{
    Q_UNUSED(index);
    QString chartType = m_chartTypeCombo->currentData().toString();

    if (chartType == "BarChart") {
        m_currentRenderer = m_container.GetObject<IChartRenderer>("BarChart");
    } else if (chartType == "PieChart") {
        m_currentRenderer = m_container.GetObject<IChartRenderer>("PieChart");
    }
}

void MainWindow::onStyleChanged(int index)
{
    Q_UNUSED(index);
    QString styleType = m_styleCombo->currentData().toString();

    if (styleType == "Color") {
        m_currentStyle = m_container.GetObject<IPrintStyle>("Color");
    } else {
        m_currentStyle = m_container.GetObject<IPrintStyle>("Gray");
    }
}

void MainWindow::onFileSelectedForChart(const QModelIndex &index)
{
    if (!index.isValid()) return;

    QString filePath = rightPartModel->filePath(index);
    QFileInfo fileInfo(filePath);
    QString suffix = fileInfo.suffix().toLower();

    if (suffix == "json" || suffix == "db" || suffix == "sqlite") {
        loadDataFromFile(filePath);
        statusBar()->showMessage("Выбран файл: " + fileInfo.fileName());
    } else {
        statusBar()->showMessage("Ошибка: выберите JSON или SQLite файл");
        m_printButton->setEnabled(false);
    }
}

void MainWindow::on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);

    QModelIndexList indexs = selected.indexes();

    QString filePath = "";

    if (indexs.count() >= 1) {
        QModelIndex ix =  indexs.constFirst();
        filePath = leftPartModel->filePath(ix);
        this->statusBar()->showMessage("Выбранный путь : " + leftPartModel->filePath(indexs.constFirst()));
    }
    tableView->setRootIndex(rightPartModel->setRootPath(filePath));
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadDataFromFile(const QString& filePath)
{
    m_currentFilePath = filePath;
    QFileInfo fileInfo(filePath);
    QString suffix = fileInfo.suffix().toLower();
    m_statusLabel->setText("Загрузка: " + QFileInfo(filePath).fileName());
    statusBar()->showMessage("Загрузка данных из: " + filePath);
    QCoreApplication::processEvents();

    std::shared_ptr<IDataProvider> provider;

    if (suffix == "json"){
        provider = std::make_shared<JSONDataAdapter>(filePath, "dd.MM.yyyy hh:mm");
    }
    else if (suffix == "sqlite") {
        provider = std::make_shared<SQLiteAdapter>(filePath, "measurements", "Time", "Value", "dd.MM.yyyy hh:mm");
    }
    else {
        m_statusLabel->setText("Ошибка: неподдерживаемый тип файла");
        m_printButton->setEnabled(false);
        return;
    }

    m_currentData = provider->getData();

    if (m_currentData.isEmpty()) {
        m_statusLabel->setText("Ошибка: нет данных в файле");
        m_printButton->setEnabled(false);
        statusBar()->showMessage("Нет данных в файле");
    } else {
        m_statusLabel->setText(QString("Загружено %1 точек данных").arg(m_currentData.size()));
        m_printButton->setEnabled(true);
        statusBar()->showMessage(QString("Загружено %1 точек данных").arg(m_currentData.size()));
    }
    switchToChartMode();

}

void MainWindow::switchToChartMode()
{
    if (m_isChartMode) return;

    m_savedSizeSplitter = m_splitter->sizes();

    while (m_splitter->count()) {
        m_splitter->widget(0)->setParent(nullptr);
    }

    m_splitter->addWidget(tableView);
    m_splitter->addWidget(m_chartWidget);
    m_splitter->setSizes(m_savedSizeSplitter);

    m_isChartMode = true;
    m_exitButton->setVisible(true);
    statusBar()->showMessage("Режим графика. Нажмите 'Выход' для возврата");
}

void MainWindow::onExitChartMode()
{
    if (!m_isChartMode) return;

    while (m_splitter->count()) {
        m_splitter->widget(0)->setParent(nullptr);
    }

    m_splitter->addWidget(treeView);
    m_splitter->addWidget(tableView);

    if (!m_savedSizeSplitter.isEmpty() && m_savedSizeSplitter.size() == 2) {
        m_splitter->setSizes(m_savedSizeSplitter);
    } else {
        m_splitter->setSizes({400, this->width() - 400});
    }

    m_isChartMode = false;
    m_exitButton->setVisible(false);
    statusBar()->showMessage("Режим выбора файлов");
}
