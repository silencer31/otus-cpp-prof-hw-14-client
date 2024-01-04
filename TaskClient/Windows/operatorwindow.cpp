#include "operatorwindow.h"
#include "ui_operatorwindow.h"

OperatorWindow::OperatorWindow(const req_mngr_shared rm_ptr, const collector_shared cltr_ptr, const parser_shared par_ptr,
                               const message_win_shared mw_ptr, const QString& uname, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::OperatorWindow)
    , request_manager_ptr(rm_ptr)
    , collector_ptr(cltr_ptr)
    , parser_ptr(par_ptr)
    , message_window_ptr(mw_ptr)
    , user_name(uname)
    , user_id(collector_ptr->get_own_id())
    , users_table_model(new QStandardItemModel())
    , tasks_table_model(new QStandardItemModel())
    , users_table_delegate(new SimpleItemDelegate(users_table_model))
    , tasks_table_delegate(new SimpleItemDelegate(tasks_table_model))
{
    ui->setupUi(this);

#ifdef WIN32
    QFont font = QFont("Tahoma", 12, QFont::DemiBold);
#else
    QFont font = QFont("Open Sans", 12, QFont::DemiBold);
#endif

    // Настройка модели с данными о пользователях.
    users_table_model->insertRow(0);
    users_table_model->insertColumns(0,2);
    users_table_model->setData(users_table_model->index(0, 0), QString("  Номер"), Qt::DisplayRole);
    users_table_model->setData(users_table_model->index(0, 1), QString("Есть на АРМ"), Qt::DisplayRole);
    users_table_model->itemFromIndex(users_table_model->index(0, 0))->setFont(font);
    users_table_model->itemFromIndex(users_table_model->index(0, 0))->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    users_table_model->itemFromIndex(users_table_model->index(0, 1))->setFont(font);
    users_table_model->itemFromIndex(users_table_model->index(0, 1))->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    // Настройка модели с данными о задачах.
    tasks_table_model->insertRow(0);
    tasks_table_model->insertColumns(0,2);
    tasks_table_model->setData(tasks_table_model->index(0, 0), QString("  Номер"), Qt::DisplayRole);
    tasks_table_model->setData(tasks_table_model->index(0, 1), QString("Есть на АРМ"), Qt::DisplayRole);
    tasks_table_model->itemFromIndex(tasks_table_model->index(0, 0))->setFont(font);
    tasks_table_model->itemFromIndex(tasks_table_model->index(0, 0))->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    tasks_table_model->itemFromIndex(tasks_table_model->index(0, 1))->setFont(font);
    tasks_table_model->itemFromIndex(tasks_table_model->index(0, 1))->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    // Настройка отображения таблицы с данными о пользователях.
    ui->tvUsers->setModel(users_table_model);
    ui->tvUsers->setItemDelegate(users_table_delegate); // Устанавливаем делегат в представление.
    ui->tvUsers->horizontalHeader()->hide();
    ui->tvUsers->verticalHeader()->hide();
    ui->tvUsers->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tvUsers->setShowGrid(true);
    ui->tvUsers->setRowHeight(0, ROW_HEIGHT);
    ui->tvUsers->setColumnWidth(0, 230);
    ui->tvUsers->setColumnWidth(1, 120);
    ui->tvUsers->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Настройка отображения таблицы с данными о задачах.
    ui->tvTasks->setModel(tasks_table_model);
    ui->tvTasks->setItemDelegate(tasks_table_delegate); // Устанавливаем делегат в представление.
    ui->tvTasks->horizontalHeader()->hide();
    ui->tvTasks->verticalHeader()->hide();
    ui->tvTasks->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tvTasks->setShowGrid(true);
    ui->tvTasks->setRowHeight(0, ROW_HEIGHT);
    ui->tvTasks->setColumnWidth(0, 230);
    ui->tvTasks->setColumnWidth(1, 120);
    ui->tvTasks->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->pbAppointUser, SIGNAL(clicked(bool)), this, SLOT(appoint_user()) );
    connect(ui->pbSetStatus, SIGNAL(clicked(bool)), this, SLOT(set_task_status()) );
    connect(ui->pbDeleteTask, SIGNAL(clicked(bool)), this, SLOT(delete_task()) );
    connect(ui->pbGetTasks, SIGNAL(clicked(bool)), this, SLOT(get_tasks_list()) );
    connect(ui->pbSetDeadline, SIGNAL(clicked(bool)), this, SLOT(set_deadline()) );
    connect(ui->pbNewTask, SIGNAL(clicked(bool)), this, SLOT(create_task()) );

    connect(ui->pbExit, SIGNAL(clicked(bool)), this, SLOT(close()) );
}

void OperatorWindow::lock_buttons()
{
    ui->pbAppointUser->setEnabled(false);
    ui->pbSetStatus->setEnabled(false);
    ui->pbDeleteTask->setEnabled(false);
    ui->pbGetTasks->setEnabled(false);
    ui->pbSetDeadline->setEnabled(false);
    ui->pbNewTask->setEnabled(false);
    ui->pbExit->setEnabled(false);
}

void OperatorWindow::unlock_buttons()
{
    ui->pbAppointUser->setEnabled(true);
    ui->pbSetStatus->setEnabled(true);
    ui->pbDeleteTask->setEnabled(true);
    ui->pbGetTasks->setEnabled(true);
    ui->pbSetDeadline->setEnabled(true);
    ui->pbNewTask->setEnabled(true);
    ui->pbExit->setEnabled(true);
}

bool OperatorWindow::handle_request(CommandType comm_type)
{
    // Пытаемся получить ответ от сервера.
    if (!request_manager_ptr->get_server_answer(server_reply)) {
        error_text = QString("Unable to read server reply!\n%1").arg(request_manager_ptr->get_last_error());
        return false;
    }

    // Пробуем распарсить json в ответе.
    if ( !parser_ptr->handle_reply(comm_type , server_reply)) {
       error_text = QString("Unable to parse server reply\n%1").arg(parser_ptr->get_last_error());
       return false;
    }

    // Проверка наличия ошибки в запросе
    if (collector_ptr->req_error()) {
        error_text = QString("Unable to perform request!\n%1").arg(collector_ptr->get_error_text());
        return false;
    }

    // Проверка результата выполнения запроса.
    if (!collector_ptr->get_result()) {
        error_text = collector_ptr->get_details();
        return false;
    }

    return true;
}

OperatorWindow::~OperatorWindow()
{
    delete ui;
}
