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
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_chartView(nullptr)
    , m_isChartMode(false)
    , m_splitter(nullptr)
    , m_chartWidget(nullptr)
    , m_currentRenderer(nullptr)
    , m_currentStyle(nullptr)
    , m_exitButton(nullptr)
    , m_printButton(nullptr)
    , m_statusLabel(nullptr)
    , m_chartTypeCombo(nullptr)
    , m_styleCombo(nullptr)
{
    if (!ui) {
        QMessageBox::warning(this, "Ошибка", "Не удалось создать UI");
        return;
    }
    ui->setupUi(this);

    registerDependencies();
    setupChartArea();

    this->setGeometry(100, 100, 1500, 500);
    this->setStatusBar(new QStatusBar(this));
    if (this->statusBar()) {
        this->statusBar()->showMessage("Выбранный путь : ");
    }
    QString homePath = QDir::homePath();

    leftPartModel = new QFileSystemModel(this);
    if (leftPartModel) {
        leftPartModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
        leftPartModel->setRootPath(homePath);
    }

    rightPartModel = new QFileSystemModel(this);
    if (rightPartModel) {
        rightPartModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
        rightPartModel->setRootPath(homePath);
    }

    treeView = new QTreeView(this);
    if (treeView && leftPartModel) {
        treeView->setModel(leftPartModel);
        treeView->expandAll();
        treeView->header()->resizeSection(0, 200);
    }

    tableView = new QTableView(this);
    if (tableView && rightPartModel) {
        tableView->setModel(rightPartModel);
    }

    m_splitter = new QSplitter(this);
    if (m_splitter) {
        if (treeView) m_splitter->addWidget(treeView);
        if (tableView) m_splitter->addWidget(tableView);
        setCentralWidget(m_splitter);
    }

    if (treeView) {
        QItemSelectionModel *selectionModel = treeView->selectionModel();
        if (selectionModel) {
            connect(selectionModel, &QItemSelectionModel::selectionChanged, this, &MainWindow::on_selectionChangedSlot);
        }
    }

    if (tableView && tableView->selectionModel()) {
        connect(tableView->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainWindow::onFileSelectedForChart);
    }

    if (m_exitButton) {
        connect(m_exitButton, &QPushButton::clicked, this, &MainWindow::onExitChartMode);
    }

    if (m_printButton) {
        connect(m_printButton, &QPushButton::clicked, this, &MainWindow::onPrintButtonClicked);
    }

    if (leftPartModel && treeView && treeView->selectionModel()) {
        QItemSelection toggleSelection;
        QModelIndex topLeft = leftPartModel->index(homePath);
        if (topLeft.isValid()) {
            toggleSelection.select(topLeft, topLeft);
            treeView->selectionModel()->select(toggleSelection, QItemSelectionModel::Toggle);
        }
    }

    if (m_chartTypeCombo)
        onChartTypeChanged(0);
    if (m_styleCombo)
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
    if (!m_chartTypeCombo) {
        QMessageBox::warning(this, "Ошибка", "Комбобокс не инициализирован");
        return;
    }

    QString chartType = m_chartTypeCombo->currentData().toString();

    if (chartType == "BarChart") {
        m_currentRenderer = m_container.GetObject<IChartRenderer>("BarChart");
    } else if (chartType == "PieChart") {
        m_currentRenderer = m_container.GetObject<IChartRenderer>("PieChart");
    }

    if (!m_currentData.isEmpty() && m_currentRenderer) {
        updateChart();
    }
}

void MainWindow::onStyleChanged(int index)
{
    Q_UNUSED(index);
    if (!m_styleCombo) {
        QMessageBox::warning(this, "Ошибка", "Комбобокс не инициализирован");
        return;
    }

    QString styleType = m_styleCombo->currentData().toString();

    if (styleType == "Color") {
        m_currentStyle = m_container.GetObject<IPrintStyle>("Color");
    } else {
        m_currentStyle = m_container.GetObject<IPrintStyle>("Gray");
    }

    if (!m_currentData.isEmpty() && m_currentStyle) {
        updateChart();
    }
}

void MainWindow::onFileSelectedForChart(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    if (!rightPartModel) {
        statusBar()->showMessage("Ошибка: модель файлов не инициализирована");
        return;
    }

    QString filePath = rightPartModel->filePath(index);
    QFileInfo fileInfo(filePath);
    QString suffix = fileInfo.suffix().toLower();

    if (suffix == "json" || suffix == "db" || suffix == "sqlite") {
        loadDataFromFile(filePath);
        if (statusBar()) {
            statusBar()->showMessage("Выбран файл: " + fileInfo.fileName());
        }
    } else {
        if (statusBar()) {
            statusBar()->showMessage("Ошибка: выберите JSON или SQLite файл");
        }
        QMessageBox::warning(this, "Ошибка", "Неподдерживаемый тип файла.\nВыберите JSON или SQLite файл.");
        if (m_printButton)
            m_printButton->setEnabled(false);
    }
}

void MainWindow::on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);

    if (!treeView || !leftPartModel || !tableView || !rightPartModel) {
        if (statusBar()) {
            statusBar()->showMessage("Компоненты не инициализированы");
        }
        return;
    }

    QModelIndexList indexs = selected.indexes();

    QString filePath = "";

    if (indexs.count() >= 1) {
        QModelIndex ix = indexs.constFirst();
        filePath = leftPartModel->filePath(ix);
        if (statusBar()) {
            this->statusBar()->showMessage("Выбранный путь : " + leftPartModel->filePath(indexs.constFirst()));
        }
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
    if (m_statusLabel) {
        m_statusLabel->setText("Загрузка: " + QFileInfo(filePath).fileName());
    }
    if (statusBar()) {
        statusBar()->showMessage("Загрузка данных из: " + filePath);
    }
    QCoreApplication::processEvents();

    std::shared_ptr<IDataProvider> provider = nullptr;

    if (suffix == "json"){
        provider = std::make_shared<JSONDataAdapter>(filePath, "dd.MM.yyyy hh:mm");
    }
    else if (suffix == "sqlite") {
        QStringList tables = SQLiteAdapter::getTableList(filePath);
        if (tables.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "В базе данных нет таблиц");
            return;
        }

        QString tableName;
        if (tables.size() == 1) {
            tableName = tables.first();
        } else {
            bool ok;
            tableName = QInputDialog::getItem(this, "Выбор таблицы", "Выберите таблицу с данными:", tables, 0, false, &ok);
            if (!ok)
                return;
        }

        provider = std::make_shared<SQLiteAdapter>(filePath, tableName, "Time", "Value", "dd.MM.yyyy hh:mm");

    } else {
        QMessageBox::warning(this, "Ошибка", "Неподдерживаемый тип файла");
        if (m_statusLabel) {
            m_statusLabel->setText("Ошибка: неподдерживаемый тип файла");
        }
        if (m_printButton)
            m_printButton->setEnabled(false);
        return;
    }

    if (!provider) {
        QMessageBox::warning(this, "Ошибка", "Не удалось создать провайдер данных");
        return;
    }

    try {
        m_currentData = provider->getData();

        if (m_currentData.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "В файле нет данных или файл имеет неверный формат.\n" + filePath);
            if (m_statusLabel) {
                m_statusLabel->setText("Ошибка: нет данных в файле");
            }
            if (m_printButton)
                m_printButton->setEnabled(false);
            if (statusBar()) {
                statusBar()->showMessage("Нет данных в файле");
            }
            return;

        } else {
            if (m_statusLabel) {
                m_statusLabel->setText(QString("Загружено %1 точек данных").arg(m_currentData.size()));
            }
            if (m_printButton)
                m_printButton->setEnabled(true);
            if (statusBar()) {
                statusBar()->showMessage(QString("Загружено %1 точек данных").arg(m_currentData.size()));
            }
        }
        switchToChartMode();
        updateChart();

    } catch (const std::exception& e){
        QMessageBox::warning(this, "Ошибка загрузки данных", e.what());
        if (m_statusLabel) {
            m_statusLabel->setText("Ошибка: неверный формат данных");
        }
        if (m_printButton) m_printButton->setEnabled(false);
        m_currentData.clear();
    }
}

void MainWindow::switchToChartMode()
{
    if (m_isChartMode)
        return;

    if (!m_splitter) {
        if (statusBar()) {
            statusBar()->showMessage("Ошибка: сплиттер не создан");
        }
        return;
    }

    if (!tableView || !m_chartWidget) {
        if (statusBar()) {
            statusBar()->showMessage("Ошибка: компоненты не созданы");
        }
        return;
    }

    m_savedSizeSplitter = m_splitter->sizes();

    while (m_splitter->count()) {
        QWidget* w = m_splitter->widget(0);
        if (w)
            w->setParent(nullptr);
    }

    m_splitter->addWidget(tableView);
    m_splitter->addWidget(m_chartWidget);
    m_splitter->setSizes(m_savedSizeSplitter);

    m_isChartMode = true;
    if (m_exitButton)
        m_exitButton->setVisible(true);
    if (statusBar()) {
        statusBar()->showMessage("Режим графика. Нажмите 'Выход' для возврата");
    }

}

void MainWindow::onExitChartMode()
{
    if (!m_isChartMode)
        return;

    if (!m_splitter) {
        if (statusBar()) {
            statusBar()->showMessage("Ошибка: сплиттер не создан");
        }
        return;
    }

    while (m_splitter->count()) {
        QWidget* w = m_splitter->widget(0);
        if (w)
             w->setParent(nullptr);
    }

    if (treeView)
        m_splitter->addWidget(treeView);
    if (tableView)
        m_splitter->addWidget(tableView);

    if (!m_savedSizeSplitter.isEmpty() && m_savedSizeSplitter.size() == 2) {
        m_splitter->setSizes(m_savedSizeSplitter);
    } else {
        m_splitter->setSizes({400, this->width() - 400});
    }

    m_isChartMode = false;
    if (m_exitButton) m_exitButton->setVisible(false);
    if (statusBar()) {
        statusBar()->showMessage("Режим выбора файлов");
    }

}

void MainWindow::updateChart()
{
    if (m_currentData.isEmpty() || !m_currentRenderer) {
        QMessageBox::warning(this, "Ошибка", "Нет данных или не выбран тип графика");
        return;
    }

    if (!m_chartView) {
        QMessageBox::warning(this, "Ошибка", "Виджет графика не инициализирован");
        return;
    }

    if (!m_currentStyle) {
        QMessageBox::warning(this, "Ошибка", "Стиль не выбран");
        return;
    }

    QChart* chart = m_currentRenderer->createChart(m_currentData);

    if (!chart) {
        QMessageBox::warning(this, "Ошибка", "Не удалось создать график");
        return;
    }

    m_currentStyle->customizeChart(chart);
    m_chartView->setChart(chart);

}

void MainWindow::setupChartArea()
{
    m_chartWidget = new QWidget();
    if (!m_chartWidget)
        return;

    QVBoxLayout* layout = new QVBoxLayout(m_chartWidget);
    if (!layout)
        return;

    QHBoxLayout* controlLayout = new QHBoxLayout();
    if (!controlLayout)
        return;

    QLabel* chartTypeLabel = new QLabel("Тип графика:");
    if (chartTypeLabel)
        controlLayout->addWidget(chartTypeLabel);

    m_chartTypeCombo = new QComboBox();
    if (m_chartTypeCombo) {
        m_chartTypeCombo->addItem("Столбчатая диаграмма", "BarChart");
        m_chartTypeCombo->addItem("Круговая диаграмма", "PieChart");
        controlLayout->addWidget(m_chartTypeCombo);
    }

    QLabel* styleLabel = new QLabel("Стиль печати:");
    if (styleLabel)
        controlLayout->addWidget(styleLabel);

    m_styleCombo = new QComboBox();
    if (m_styleCombo) {
        m_styleCombo->addItem("Цветной", "Color");
        m_styleCombo->addItem("Черно-белый", "Gray");
        controlLayout->addWidget(m_styleCombo);
    }

    m_printButton = new QPushButton("Печать в PDF");
    if (m_printButton) {
        m_printButton->setEnabled(false);
        controlLayout->addWidget(m_printButton);
    }

    m_exitButton = new QPushButton("Выход");
    if (m_exitButton) {
        m_exitButton->setVisible(false);
        controlLayout->addWidget(m_exitButton);
    }

    m_chartView = new QChartView();
    if (m_chartView) {
        m_chartView->setRenderHint(QPainter::Antialiasing);
    }

    m_statusLabel = new QLabel("Выберите JSON или SQLite файл");
    if (m_statusLabel) {
        m_statusLabel->setAlignment(Qt::AlignCenter);
    }

    if (layout) {
        if (controlLayout)
            layout->addLayout(controlLayout);
        if (m_chartView)
            layout->addWidget(m_chartView);
        if (m_statusLabel)
            layout->addWidget(m_statusLabel);
    }

    if (m_chartTypeCombo) {
        connect(m_chartTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onChartTypeChanged);
    }
    if (m_styleCombo) {
        connect(m_styleCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onStyleChanged);
    }

}

void MainWindow::onPrintButtonClicked()
{
    if (m_currentData.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Нет данных для печати");
        return;
    }

    if (!m_currentRenderer) {
        QMessageBox::warning(this, "Ошибка", "Рендерер графика не выбран");
        return;
    }

    if (!m_currentStyle) {
        QMessageBox::warning(this, "Ошибка", "Стиль печати не выбран");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить PDF", QDir::homePath() + "/chart.pdf", "PDF файлы (*.pdf)");
    if (fileName.isEmpty())
        return;

    QChart* chart = m_currentRenderer->createChart(m_currentData);

    if (!chart) {
        QMessageBox::warning(this, "Ошибка", "Не удалось создать график для печати");
        return;
    }

    m_currentStyle->customizeChart(chart);

    QChartView tempView;
    tempView.setChart(chart);
    tempView.resize(1200, 800);
    tempView.show();
    QCoreApplication::processEvents();

    QPrinter printer;
    m_currentStyle->configurePrinter(printer);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    QPainter painter(&printer);
    tempView.render(&painter);

    delete chart;

    if (statusBar()) {
        statusBar()->showMessage("PDF сохранен: " + QFileInfo(fileName).fileName());
    }
    QMessageBox::information(this, "Успех", "График сохранен в PDF");

}
