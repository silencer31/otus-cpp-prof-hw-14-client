#include "userwindow.h"
#include "ui_userwindow.h"

UserWindow::UserWindow(const req_mngr_shared rm_ptr, const collector_shared cltr_ptr,
                       const parser_shared par_ptr, const data_keeper_shared dk_ptr,
                       const message_win_shared mw_ptr, const passwd_win_shared pwd_ptr,
                       const QString& uname, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UserWindow)
    , request_manager_ptr(rm_ptr)
    , collector_ptr(cltr_ptr)
    , parser_ptr(par_ptr)
    , data_keeper_ptr(dk_ptr)
    , message_window_ptr(mw_ptr)
    , passwd_window_ptr(pwd_ptr)
    , own_name(uname)
    , own_id(collector_ptr->get_own_id())
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

    connect(ui->pbGetTasks, SIGNAL(clicked(bool)), this, SLOT(get_tasks_list()) );
    connect(ui->pbApply, SIGNAL(clicked(bool)), this, SLOT(add_or_edit_task()) );
    connect(ui->pbClear, SIGNAL(clicked(bool)), this, SLOT(clear_fields()) );
    connect(ui->pbExit, SIGNAL(clicked(bool)), this, SLOT(close()) );

    ui->leOwnId->setText(QString::number(own_id));
    ui->leOwnLogin->setText(own_name);
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

bool UserWindow::handle_request(CommandType comm_type)
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

UserWindow::~UserWindow()
{
    delete ui;
}
