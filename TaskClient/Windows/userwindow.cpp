#include "userwindow.h"
#include "ui_userwindow.h"

UserWindow::UserWindow(const req_mngr_shared rm_ptr, const collector_shared cltr_ptr, const parser_shared par_ptr,
                       const handler_shared hdlr_ptr, const data_keeper_shared dk_ptr,
                       const message_win_shared mw_ptr, const passwd_win_shared pwd_ptr,
                       QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UserWindow)
    , request_manager_ptr(rm_ptr)
    , collector_ptr(cltr_ptr)
    , parser_ptr(par_ptr)
    , handler_ptr(hdlr_ptr)
    , data_keeper_ptr(dk_ptr)
    , message_window_ptr(mw_ptr)
    , passwd_window_ptr(pwd_ptr)
    , tasks_table_model(new QStandardItemModel())
    , tasks_table_delegate(new SimpleItemDelegate(tasks_table_model))
{
    ui->setupUi(this);

#ifdef WIN32
    QFont font = QFont("Tahoma", 12, QFont::DemiBold);
#else
    QFont font = QFont("Open Sans", 12, QFont::DemiBold);
#endif

    // Настройка модели с данными о задачах.
    tasks_table_model->insertRow(0);
    tasks_table_model->insertColumns(0, 7);
    tasks_table_model->setData(tasks_table_model->index(0, 0), QString("Task id"), Qt::DisplayRole);
    tasks_table_model->setData(tasks_table_model->index(0, 1), QString("Status"), Qt::DisplayRole);
    tasks_table_model->setData(tasks_table_model->index(0, 2), QString("Name"), Qt::DisplayRole);
    tasks_table_model->setData(tasks_table_model->index(0, 3), QString("Deadline"), Qt::DisplayRole);
    tasks_table_model->setData(tasks_table_model->index(0, 4), QString("Description"), Qt::DisplayRole);
    tasks_table_model->setData(tasks_table_model->index(0, 5), QString("User id"), Qt::DisplayRole);
    tasks_table_model->setData(tasks_table_model->index(0, 6), QString("Login"), Qt::DisplayRole);
    for(int i = 0; i < 7; ++i) {
        tasks_table_model->itemFromIndex(tasks_table_model->index(0, i))->setFont(font);
        tasks_table_model->itemFromIndex(tasks_table_model->index(0, i))->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }

    // Настройка отображения таблицы с данными о задачах.
    ui->tvTasks->setModel(tasks_table_model);
    ui->tvTasks->setItemDelegate(tasks_table_delegate); // Устанавливаем делегат в представление.
    ui->tvTasks->horizontalHeader()->hide();
    ui->tvTasks->verticalHeader()->hide();
    ui->tvTasks->setShowGrid(true);
    ui->tvTasks->setRowHeight(0, ROW_HEIGHT);
    ui->tvTasks->setColumnWidth(0, 100); // Task id
    ui->tvTasks->setColumnWidth(1, 150); // Status
    ui->tvTasks->setColumnWidth(2, 250); // Name
    ui->tvTasks->setColumnWidth(3, 150); // Deadline
    ui->tvTasks->setColumnWidth(4, 500); // Description
    ui->tvTasks->setColumnWidth(5, 100); // User id
    ui->tvTasks->setColumnWidth(6, 200); // Login
    ui->tvTasks->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvTasks->setSelectionMode(QAbstractItemView::SingleSelection);
    //ui->tvUsers->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tvTasks->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Нажатие по строке в таблице с пользователями.
    connect(ui->tvTasks, SIGNAL(clicked(const QModelIndex &)),
            this, SLOT(task_clicked(const QModelIndex &)));

    connect(ui->pbGetTasks, SIGNAL(clicked(bool)), this, SLOT(get_tasks_list()) );
    connect(ui->pbApply, SIGNAL(clicked(bool)), this, SLOT(add_or_edit_task()) );
    connect(ui->pbClear, SIGNAL(clicked(bool)), this, SLOT(clear_fields()) );
    connect(ui->pbExit, SIGNAL(clicked(bool)), this, SLOT(close()) );

    for(auto iter = collector_ptr->task_statuses_cib(); iter != collector_ptr->task_statuses_cie(); ++iter) {
        ui->cbTaskStatus->addItem(QString("%1 : %2").arg(QString::number(iter.key()), iter.value()));
    }
}

// Обработка события закрытия окна.
void UserWindow::closeEvent(QCloseEvent*)
{
    emit closed();
}

// Вывести данные текущего пользователя.
void UserWindow::output_user_data()
{
    ui->leOwnId->setText(QString::number(data_keeper_ptr->get_own_id()));
    ui->leOwnLogin->setText(data_keeper_ptr->get_own_login());

    // Запрос ФИО.
    if (handler_ptr->get_fullname(data_keeper_ptr->get_own_id())) {
        const Fullname fn = collector_ptr->get_fullname();
        ui->leOwnFullname->setText(QString("%1 %2 %3").arg(fn.second, fn.first, fn.patronymic));
    }
}

void UserWindow::lock_buttons()
{
    ui->pbGetTasks->setEnabled(false);
    ui->pbApply->setEnabled(false);
    ui->pbClear->setEnabled(false);
    ui->pbExit->setEnabled(false);
}

void UserWindow::unlock_buttons()
{
    ui->pbGetTasks->setEnabled(true);
    ui->pbApply->setEnabled(true);
    ui->pbClear->setEnabled(true);
    ui->pbExit->setEnabled(true);
}

// Показать сообщение пользователю.
void UserWindow::show_message(const QString& message)
{
    message_window_ptr->set_message(message);
    message_window_ptr->exec();
}

UserWindow::~UserWindow()
{
    delete ui;
}
