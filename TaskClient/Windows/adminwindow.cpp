#include "adminwindow.h"
#include "ui_adminwindow.h"

AdminWindow::AdminWindow(const req_mngr_shared rm_ptr, const collector_shared cltr_ptr,
                         const parser_shared par_ptr, const data_keeper_shared dk_ptr,
                         const message_win_shared mw_ptr, const QString& uname, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdminWindow)
    , request_manager_ptr(rm_ptr)
    , collector_ptr(cltr_ptr)
    , parser_ptr(par_ptr)
    , data_keeper_ptr(dk_ptr)
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
    users_table_model->insertColumns(0, 6);
    users_table_model->setData(users_table_model->index(0, 0), QString("User id"), Qt::DisplayRole);
    users_table_model->setData(users_table_model->index(0, 1), QString("Login"), Qt::DisplayRole);
    users_table_model->setData(users_table_model->index(0, 2), QString("Type"), Qt::DisplayRole);
    users_table_model->setData(users_table_model->index(0, 3), QString("Surename"), Qt::DisplayRole);
    users_table_model->setData(users_table_model->index(0, 4), QString("Name"), Qt::DisplayRole);
    users_table_model->setData(users_table_model->index(0, 5), QString("Patronymic"), Qt::DisplayRole);
    for(int i = 0; i < 6; ++i) {
        users_table_model->itemFromIndex(users_table_model->index(0, i))->setFont(font);
        users_table_model->itemFromIndex(users_table_model->index(0, i))->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }

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

    // Настройка отображения таблицы с данными о пользователях.
    ui->tvUsers->setModel(users_table_model);
    ui->tvUsers->setItemDelegate(users_table_delegate); // Устанавливаем делегат в представление.
    ui->tvUsers->horizontalHeader()->hide();
    ui->tvUsers->verticalHeader()->hide();
    //ui->tvUsers->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tvUsers->setShowGrid(true);
    ui->tvUsers->setRowHeight(0, ROW_HEIGHT);
    ui->tvUsers->setColumnWidth(0, 100); // User id
    ui->tvUsers->setColumnWidth(1, 200); // Login
    ui->tvUsers->setColumnWidth(2, 200); // Type
    ui->tvUsers->setColumnWidth(3, 300); // Surename
    ui->tvUsers->setColumnWidth(4, 200); // Name
    ui->tvUsers->setColumnWidth(5, 300); // Patronymic
    ui->tvUsers->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tvUsers->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tvUsers->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Настройка отображения таблицы с данными о задачах.
    ui->tvTasks->setModel(tasks_table_model);
    ui->tvTasks->setItemDelegate(tasks_table_delegate); // Устанавливаем делегат в представление.
    ui->tvTasks->horizontalHeader()->hide();
    ui->tvTasks->verticalHeader()->hide();
    ui->tvTasks->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tvTasks->setShowGrid(true);
    ui->tvTasks->setRowHeight(0, ROW_HEIGHT);
    ui->tvTasks->setColumnWidth(0, 100); // Task id
    ui->tvTasks->setColumnWidth(1, 150); // Status
    ui->tvTasks->setColumnWidth(2, 250); // Name
    ui->tvTasks->setColumnWidth(3, 150); // Deadline
    ui->tvTasks->setColumnWidth(4, 400); // Description
    ui->tvTasks->setColumnWidth(5, 100); // User id
    ui->tvTasks->setColumnWidth(6, 200); // Login
    ui->tvTasks->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Нажатие по элементу представления.
    connect(ui->tvUsers, SIGNAL(clicked(const QModelIndex &)),
            this, SLOT(user_clicked(const QModelIndex &)));
    connect(ui->pbGetUsers, SIGNAL(clicked(bool)), this, SLOT(get_users_list()) );
    connect(ui->pbGetTasks, SIGNAL(clicked(bool)), this, SLOT(get_tasks_list()) );
    connect(ui->pbApply,    SIGNAL(clicked(bool)), this, SLOT(add_or_edit_user()) );
    connect(ui->pbClear,    SIGNAL(clicked(bool)), this, SLOT(clear_fields()) );
    connect(ui->pbShutdown, SIGNAL(clicked(bool)), this, SLOT(shutdown_server()) );
    connect(ui->pbExit,     SIGNAL(clicked(bool)), this, SLOT(close()) );

    ui->leUserId->setText(QString::number(user_id));
    ui->leUserName->setText(user_name);

    ui->pbApply->setEnabled(false);
    ui->pbClear->setEnabled(false);
}

void AdminWindow::lock_buttons()
{
    ui->pbGetUsers->setEnabled(false);
    ui->pbGetTasks->setEnabled(false);
    ui->pbApply->setEnabled(false);
    ui->pbClear->setEnabled(false);
    ui->pbShutdown->setEnabled(false);
    ui->pbExit->setEnabled(false);
}

void AdminWindow::unlock_buttons()
{
    ui->pbGetUsers->setEnabled(true);
    ui->pbGetTasks->setEnabled(true);
    ui->pbApply->setEnabled(true);
    ui->pbClear->setEnabled(true);
    ui->pbShutdown->setEnabled(true);
    ui->pbExit->setEnabled(true);
}

void AdminWindow::check_server()
{
    // Есть ли связь с сервером.
    if (!request_manager_ptr->connected_to_server()) {
        message_window_ptr->set_message(QString("No connection with server!"));
        message_window_ptr->exec();
        return;
    }

    // Отправляем тестовый запрос, чтобы узнать, готов ли сервер обрабатывать запросы.
    if (!request_manager_ptr->send_test_request()) {
        message_window_ptr->set_message(QString("Unable to send test request!\n%1")
                                        .arg(request_manager_ptr->get_last_error()));
        message_window_ptr->exec();
        return;
    }

    // Контроль выполнения запроса.
    if ( !handle_request(CommandType::Test)) {
        message_window_ptr->set_message(QString("Server is unable to handle requests!\n%1").arg(error_text));
        message_window_ptr->exec();
    }

    message_window_ptr->set_message(QString("Server shutdown completed.\nFurther communication impossible"));
    message_window_ptr->exec();
}

bool AdminWindow::handle_request(CommandType comm_type)
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

AdminWindow::~AdminWindow()
{
    delete ui;
}

